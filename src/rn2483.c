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

#include "rn2483.h"

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

void USART0_WriteByte(uint8_t value)
{
	while( !(USART0->STATUS & (1 << 6)) ); // wait for TX buffer to empty
	USART0->TXDATA = value;
}

void USART0_WriteString(char *string){
	uint8_t i;
	for(i=0; i<strlen(string); i++) {
	   while( !(USART0->STATUS & (1 << 6)) ); // wait for TX buffer to empty
	   USART0->TXDATA = string[i];       // print each character of the test string
    }
}

bool USART0_Available(){
	return USART0->STATUS & (1 << 7);
}


