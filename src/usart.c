/*
 * rn2483.c
 *
 *  Created on: 19-dec.-2017
 *      Author: Guus Leenders
 */


#include <stdint.h>
#include <stdbool.h>
#include "em_device.h"
#include "em_chip.h"
#include "em_cmu.h"
#include "em_gpio.h"
#include "em_usart.h"

#include "usart.h"
#include "delay.h"

/* Buffer pointers and indexes */
char* txBuffer;
char* rxBuffer;
int txBufferSize;
int rxBufferSize;
volatile int txBufferIndex;
volatile int rxBufferIndex;


/**************************************************************************//**
 * @brief  Setup USART0
 * @param[in]
 *****************************************************************************/
void USART0_Setup()
{
	CMU_ClockEnable(cmuClock_HFPER, true);
	CMU_ClockEnable(cmuClock_USART0, true);          // Enable USART0 peripheral clock

	GPIO_PinModeSet(COM_PORT, UART_TX_pin, gpioModePushPull, 1); // Configure UART TX pin as digital output, initialize high since UART TX idles high (otherwise glitches can occur)
	GPIO_PinModeSet(COM_PORT, UART_RX_pin, gpioModeInput, 0);    // Configure UART RX pin as input (no filter)

	USART_InitAsync_TypeDef uartInit =
	{
	  .enable       = usartDisable,   // Wait to enable the transmitter and receiver
	  .refFreq      = 0,              // Setting refFreq to 0 will invoke the CMU_ClockFreqGet() function and measure the HFPER clock
	  .baudrate     = 57600,         // Desired baud rate
	  .oversampling = usartOVS16,     // Set oversampling value to x16
	  .databits     = usartDatabits8, // 8 data bits
	  .parity       = usartNoParity,  // No parity bits
	  .stopbits     = usartStopbits1, // 1 stop bit
	  .mvdis        = false,          // Use majority voting
	  .prsRxEnable  = false,          // Not using PRS input
	  .prsRxCh      = usartPrsRxCh0,  // Doesn't matter which channel we select
	};

	USART_InitAsync(USART0, &uartInit);                                              // Apply configuration struct to USART0
	USART0->ROUTE = USART_ROUTE_RXPEN | USART_ROUTE_TXPEN | USART_ROUTE_LOCATION_LOC0; // Clear RX/TX buffers and shift regs, enable transmitter and receiver pins

	USART_IntClear(USART0, _USART_IF_MASK); // Clear any USART interrupt flags
	NVIC_ClearPendingIRQ(USART0_RX_IRQn);   // Clear pending RX interrupt flag in NVIC
	NVIC_ClearPendingIRQ(USART0_TX_IRQn);   // Clear pending TX interrupt flag in NVIC

	USART_Enable(USART0, usartEnable);     // Enable transmitter and receiver
}


void USART0_SetupRXInt(char* receiveBuffer, int receiveBufferSize){
  USART_TypeDef *uart = USART0;

  /* Setting up pointer and indexes */
  rxBuffer      = receiveBuffer;
  rxBufferSize  = receiveBufferSize;
  rxBufferIndex = 0;

  /* Clear previous interrupts */
  uart->IFC = _USART_IFC_MASK;

  /* Enable interrupts */
  NVIC_ClearPendingIRQ(USART0_RX_IRQn);
  NVIC_EnableIRQ(USART0_RX_IRQn);
  uart->IEN = USART_IEN_RXDATAV;
}

void USART0_RX_IRQHandler(void){
  USART_TypeDef *uart = USART0;
  uint8_t       rxdata;

  /* Checking that RX-flag is set*/
  if (uart->STATUS & USART_STATUS_RXDATAV)
  {
    rxdata = uart->RXDATA;
    /* Store Data */
    rxBuffer[rxBufferIndex] = rxdata;
    rxBufferIndex++;

    /* Checking if the buffer is full */
    if (rxBufferIndex == rxBufferSize)
    {
      /* Disable interrupt */
      uart->IEN &= ~USART_IEN_RXDATAV;
      NVIC_ClearPendingIRQ(USART0_RX_IRQn);
      NVIC_DisableIRQ(USART0_RX_IRQn);
    }
  }
}

/******************************************************************************
 * @brief sets up and enables USART1 TX interrupt
 * @param transmitBuffer is pointing to the data to send.
 * @param bytesToSend is the number of bytes to send.
 *****************************************************************************/
void USART0_SetupTXInt(char* transmitBuffer, int bytesToSend){
  USART_TypeDef *uart = USART0;

  /* Setting up pointer and indexes */
  txBuffer      = transmitBuffer;
  txBufferSize  = bytesToSend;
  txBufferIndex = 0;

  /* Clear previous interrupts */
  uart->IFC = _USART_IFC_MASK;
  /* Enable interrupts */
  uart->IEN = USART_IEN_TXBL;

  NVIC_ClearPendingIRQ(USART0_TX_IRQn);
  NVIC_EnableIRQ(USART0_TX_IRQn);
}



/**************************************************************************//**
 * @brief USART1 TX IRQ Handler
 * Sending until all data has been sent.
 *****************************************************************************/
void USART0_TX_IRQHandler(void){
  USART_TypeDef *uart = USART0;

  /* Checking that the USART is waiting for data */
  if (uart->STATUS & USART_STATUS_TXBL)
  {
    /* Transmitting the next byte */
    uart->TXDATA = txBuffer[txBufferIndex];
    txBufferIndex++;

    /* Disabling the interrupt when the buffer has been transmitted */
    if (txBufferIndex == txBufferSize)
    {
      uart->IEN &= ~USART_IEN_TXBL;
      NVIC_DisableIRQ(USART0_TX_IRQn);
      NVIC_ClearPendingIRQ(USART0_TX_IRQn);
    }
  }
}



/**************************************************************************//**
 * @brief Receiving data from USART1
 * @param receiveBuffer is pointing to where data is to be stored.
 * @param bytesToReceive is the number of bytes to receive
 *****************************************************************************/
void USART0_ReceiveBuffer(char* receiveBuffer, int bufferSize){
  USART_TypeDef *uart = USART0;
  int           ii =0;
  char a = 0;

  memset(receiveBuffer, '\0', bufferSize);

  /* Receiving data */
  while((a != '\n') && (ii <bufferSize))
  {
    /* Waiting for the usart to be ready */
    while (!(uart->STATUS & USART_STATUS_RXDATAV)) ;

    /* Writing next byte to USART */
    a = uart->RXDATA;
    *receiveBuffer = a;

    receiveBuffer++;

    ii++;
  }
}

void USART0_SendBuffer(char* txBuffer, int bytesToSend){
  USART_TypeDef *uart = USART0;
  int           ii;

  /* Sending the data */
  for (ii = 0; ii < bytesToSend;  ii++)
  {
    /* Waiting for the usart to be ready */
    while (!(uart->STATUS & USART_STATUS_TXBL)) ;

    if (txBuffer != 0)
    {
      /* Writing next byte to USART */
      uart->TXDATA = *txBuffer;
      txBuffer++;
    }
    else
    {
      uart->TXDATA = 0;
    }
  }

  /*Waiting for transmission of last byte */
  while (!(uart->STATUS & USART_STATUS_TXC)) ;
}

