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
#include "em_emu.h"
#include "em_leuart.h"
#include "em_dma.h"
#include "dmactrl.h"
#include "delay.h"

/** LEUART Rx/Tx Port/Pin Location */
#define LEUART_LOCATION    0
#define LEUART_TXPORT      gpioPortD        /* LEUART transmission port */
#define LEUART_TXPIN       4                /* LEUART transmission pin  */
#define LEUART_RXPORT      gpioPortD        /* LEUART reception port    */
#define LEUART_RXPIN       5                /* LEUART reception pin     */

/** DMA Configurations            */
#define DMA_CHANNEL_TX       0          /* DMA channel is 0 */
#define DMA_CHANNEL_RX       1
#define DMA_CHANNELS 		 2

/** DMA callback structure */
static DMA_CB_TypeDef dmaCallBack[DMA_CHANNELS];

char commandBuffer[50];
char receiveBuffer[50];
volatile int numberOfTransfers = 0;
/***************************************************************************//**
 * @brief  DMA callback function
 *
 * @details This function is invoked once a DMA transfer cycle is completed.
 *          It then refreshes the completed DMA descriptor.
 ******************************************************************************/
static void basicTxComplete(unsigned int channel, bool primary, void *user)
{
  (void) user;
  /* Refresh DMA basic transaction cycle */
  /*DMA_ActivateBasic(DMA_CHANNEL_RX,
                    primary,
                    false,
                    &receiveBuffer,
					(void *)&LEUART0->RXDATA,
                    1);*/
  numberOfTransfers++;
}

static void basicRxComplete(unsigned int channel, bool primary, void *user)
{
	(void) user;
	/* Refresh DMA basic transaction cycle */
	  DMA_ActivateBasic(DMA_CHANNEL_RX,
	                    primary,
	                    false,
	                    &receiveBuffer,
						(void *)&LEUART0->RXDATA,
	                    1);
}


/***************************************************************************//**
 * @brief  Setup DMA
 *
 * @details
 *   This function initializes DMA controller.
 *   It configures the DMA channel to be used for LEUART0 transmit and receive.
 *   The primary descriptor for channel0 is configured for N_MINUS_1 byte
 *   transfer. For continous data reception and transmission using LEUART in
 *   basic mode DMA Callback function is configured to reactivate
 *   basic transfer cycle once it is completed.
 *
 ******************************************************************************/
void setupDmaTx(void)
{
  /* DMA configuration structs */
  DMA_Init_TypeDef       dmaInit;
  DMA_CfgChannel_TypeDef channelCfg;
  DMA_CfgDescr_TypeDef   descrCfg;

  /* Initializing the DMA */
  dmaInit.hprot        = 0;
  dmaInit.controlBlock = dmaControlBlock;
  DMA_Init(&dmaInit);

  /* Set the interrupt callback routine */
  dmaCallBack[DMA_CHANNEL_TX].cbFunc = basicTxComplete;
  /* Callback doesn't need userpointer */
  dmaCallBack[DMA_CHANNEL_TX].userPtr = NULL;

  /* Setting up channel */
  channelCfg.highPri   = false; /* Can't use with peripherals */
  channelCfg.enableInt = true;  /* Enabling interrupt to refresh DMA cycle*/

  /* Configure channel 0 */
  /*Setting up DMA transfer trigger request*/
  channelCfg.select = DMAREQ_LEUART0_TXBL; //DMAREQ_LEUART0_RXDATAV;
  /* Setting up callback function to refresh descriptors*/
  channelCfg.cb     = &(dmaCallBack[DMA_CHANNEL_TX]);
  DMA_CfgChannel(DMA_CHANNEL_TX, &channelCfg);

  /* Setting up channel descriptor */
  /* Destination is LEUART_Rx register and doesn't move */
  descrCfg.dstInc = dmaDataIncNone;

  /* Source is LEUART_TX register and transfers 8 bits each time */
  descrCfg.srcInc = dmaDataInc1;
  descrCfg.size   = dmaDataSize1;

  /* Default setting of DMA arbitration*/
  descrCfg.arbRate = dmaArbitrate1;
  descrCfg.hprot   = 0;

  /* Configure primary descriptor */
  DMA_CfgDescr(DMA_CHANNEL_TX, true, &descrCfg);

  /*sprintf(commandBuffer, "U");
  sendLeuartData(commandBuffer, (uint8_t) strlen(commandBuffer));
  DelayMs(20);
  sprintf(commandBuffer, "sys get ver\r\n");
  uint8_t length = strlen(commandBuffer);
  sendLeuartData(commandBuffer, length);*/
}
void sendLeuartData(char * buffer, uint8_t bufferLength){
	  while(DMA_ChannelEnabled(DMA_CHANNEL_TX));
	  DMA_ActivateBasic(DMA_CHANNEL_TX,
	                    true,
	                    false,
	                    (void *)&LEUART0->TXDATA,
	                    buffer,
	                    (unsigned int)(bufferLength - 1));
}
//void setupDmaRx(void)
//{
//  /* DMA configuration structs */
//  DMA_Init_TypeDef       dmaInit;
//  DMA_CfgChannel_TypeDef channelCfg;
//  DMA_CfgDescr_TypeDef   descrCfg;
//
//  /* Initializing the DMA */
//  dmaInit.hprot        = 0;
//  dmaInit.controlBlock = dmaControlBlock;
//  DMA_Init(&dmaInit);
//
//  /* Set the interrupt callback routine */
//  dmaCallBack[DMA_CHANNEL_RX].cbFunc = basicRxComplete;
//  /* Callback doesn't need userpointer */
//  dmaCallBack[DMA_CHANNEL_RX].userPtr = NULL;
//
//  /* Setting up channel */
//  channelCfg.highPri   = false; /* Can't use with peripherals */
//  channelCfg.enableInt = true;  /* Enabling interrupt to refresh DMA cycle*/
//
//  /* Configure channel 0 */
//  /*Setting up DMA transfer trigger request*/
//  channelCfg.select = DMAREQ_LEUART0_RXDATAV;
//  /* Setting up callback function to refresh descriptors*/
//  channelCfg.cb     = &dmaCallBack;
//  DMA_CfgChannel(DMA_CHANNEL_RX, &channelCfg);
//
//  /* Setting up channel descriptor */
//  /* Destination is LEUART_Rx register and doesn't move */
//  descrCfg.dstInc = dmaDataInc1;
//
//  /* Source is LEUART_TX register and transfers 8 bits each time */
//  descrCfg.srcInc = dmaDataIncNone;
//  descrCfg.size   = dmaDataSize1;
//
//  /* Default setting of DMA arbitration*/
//  descrCfg.arbRate = dmaArbitrate1;
//  descrCfg.hprot   = 0;
//
//  /* Configure primary descriptor */
//  DMA_CfgDescr(DMA_CHANNEL_RX, true, &descrCfg);
//  DMA_CfgDescr(DMA_CHANNEL_RX, false, &descrCfg);
//
//}
//(void *)&LEUART0->RXDATA

/***************************************************************************//**
 * @brief  Setting up LEUART
 ******************************************************************************/
void setupLeuart(void)
{
  /* Enable peripheral clocks */
  CMU_ClockEnable(cmuClock_HFPER, true);
  /* Configure GPIO pins */
  CMU_ClockEnable(cmuClock_GPIO, true);
  /* To avoid false start, configure output as high */
  GPIO_PinModeSet(LEUART_TXPORT, LEUART_TXPIN, gpioModePushPull, 1);
  GPIO_PinModeSet(LEUART_RXPORT, LEUART_RXPIN, gpioModeInput, 0);

  LEUART_Init_TypeDef init = LEUART_INIT_DEFAULT;

  /* Enable CORE LE clock in order to access LE modules */
  CMU_ClockEnable(cmuClock_CORELE, true);

  /* Select LFRCO for LEUARTs (and wait for it to stabilize) */
  CMU_ClockSelectSet(cmuClock_LFB, cmuSelect_LFRCO);
  CMU_ClockEnable(cmuClock_LEUART0, true);

  /* Do not prescale clock */
  CMU_ClockDivSet(cmuClock_LEUART0, cmuClkDiv_1);

  /* Configure LEUART */
  init.enable = leuartDisable;

  LEUART_Init(LEUART0, &init);

  /* Enable pins at default location */
  LEUART0->ROUTE = LEUART_ROUTE_RXPEN | LEUART_ROUTE_TXPEN | LEUART_LOCATION;

  /* Set RXDMAWU to wake up the DMA controller in EM2 */
  LEUART_RxDmaInEM2Enable(LEUART0, true);

  /* Clear previous RX interrupts */
  LEUART_IntClear(LEUART0, LEUART_IF_RXDATAV);
  NVIC_ClearPendingIRQ(LEUART0_IRQn);

  /* Finally enable it */
  LEUART_Enable(LEUART0, leuartEnable);
}

void Leuart_ClearCondition(){
	GPIO_PinModeSet(LEUART_TXPORT, LEUART_TXPIN, gpioModePushPull, 0);
	DelayMs(2);
	GPIO_PinOutSet(LEUART_TXPORT, LEUART_TXPIN);
}
