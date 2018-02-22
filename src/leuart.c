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

#include "leuart.h"

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

char commandBuffer[COMMAND_BUFFER_SIZE];
char receiveBuffer[RECEIVE_BUFFER_SIZE];
volatile uint8_t bufferPointer = 0;
volatile bool receiveComplete = false;

static void basicTxComplete(unsigned int channel, bool primary, void *user){
  (void) user;
  /* Refresh DMA basic transaction cycle */
  DMA_ActivateBasic(DMA_CHANNEL_RX,
                    primary,
                    false,
                    (void *)&receiveBuffer,
					(void *)&LEUART0->RXDATA,
                    0);
}

static void basicRxComplete(unsigned int channel, bool primary, void *user){
	(void) user;

	/* Refresh DMA basic transaction cycle */
	char c = LEUART0->RXDATA;
	if(c != '\n' && bufferPointer < RECEIVE_BUFFER_SIZE - 1){
		bufferPointer ++;
		DMA_ActivateBasic(DMA_CHANNEL_RX,
						  primary,
						  false,
						  &receiveBuffer[bufferPointer],
						  NULL,
						  0);

	}else{
		receiveBuffer[bufferPointer+1] = '\0';
		receiveComplete = true;
		bufferPointer = 0;
	}
}
void setupDma(void){
	/* DMA configuration structs */
	DMA_Init_TypeDef       dmaInit;
	DMA_CfgChannel_TypeDef rxChnlCfg;
	DMA_CfgChannel_TypeDef txChnlCfg;
	DMA_CfgDescr_TypeDef   rxDescrCfg;
	DMA_CfgDescr_TypeDef   txDescrCfg;

	/* Initializing the DMA */
	dmaInit.hprot        = 0;
	dmaInit.controlBlock = dmaControlBlock;
	DMA_Init(&dmaInit);

	/* RX DMA setup*/
	/* Set the interrupt callback routine */
	dmaCallBack[DMA_CHANNEL_RX].cbFunc = basicRxComplete;
	/* Callback doesn't need userpointer */
	dmaCallBack[DMA_CHANNEL_RX].userPtr = NULL;

	/* Setting up channel for TX*/
	rxChnlCfg.highPri   = false; /* Can't use with peripherals */
	rxChnlCfg.enableInt = true;  /* Enabling interrupt to refresh DMA cycle*/
	/*Setting up DMA transfer trigger request*/
	rxChnlCfg.select = DMAREQ_LEUART0_RXDATAV; //DMAREQ_LEUART0_RXDATAV;
	/* Setting up callback function to refresh descriptors*/
	rxChnlCfg.cb     = &(dmaCallBack[DMA_CHANNEL_RX]);
	DMA_CfgChannel(DMA_CHANNEL_RX, &rxChnlCfg);

	/* Setting up channel descriptor */
	/* Destination is buffer, increment ourselves */
	rxDescrCfg.dstInc = dmaDataIncNone;
	/* Source is LEUART_RX register and transfers 8 bits each time */
	rxDescrCfg.srcInc = dmaDataIncNone;
	rxDescrCfg.size   = dmaDataSize1;
	/* Default setting of DMA arbitration*/
	rxDescrCfg.arbRate = dmaArbitrate1;
	rxDescrCfg.hprot   = 0;
	/* Configure primary descriptor */
	DMA_CfgDescr(DMA_CHANNEL_RX, true, &rxDescrCfg);
	DMA_CfgDescr(DMA_CHANNEL_RX, false, &rxDescrCfg);

	/* TX DMA setup*/
	/* Set the interrupt callback routine */
	dmaCallBack[DMA_CHANNEL_TX].cbFunc = basicTxComplete;
	/* Callback doesn't need userpointer */
	dmaCallBack[DMA_CHANNEL_TX].userPtr = NULL;

	/* Setting up channel for TX*/
	txChnlCfg.highPri   = false; /* Can't use with peripherals */
	txChnlCfg.enableInt = true;  /* Enabling interrupt to refresh DMA cycle*/
	/*Setting up DMA transfer trigger request*/
	txChnlCfg.select = DMAREQ_LEUART0_TXBL; //DMAREQ_LEUART0_RXDATAV;
	/* Setting up callback function to refresh descriptors*/
	txChnlCfg.cb     = &(dmaCallBack[DMA_CHANNEL_TX]);
	DMA_CfgChannel(DMA_CHANNEL_TX, &txChnlCfg);

	/* Setting up channel descriptor */
	/* Destination is LEUART_Tx register and doesn't move */
	txDescrCfg.dstInc = dmaDataIncNone;
	/* Source is LEUART_TX register and transfers 8 bits each time */
	txDescrCfg.srcInc = dmaDataInc1;
	txDescrCfg.size   = dmaDataSize1;
	/* Default setting of DMA arbitration*/
	txDescrCfg.arbRate = dmaArbitrate1;
	txDescrCfg.hprot   = 0;
	/* Configure primary descriptor */
	DMA_CfgDescr(DMA_CHANNEL_TX, true, &txDescrCfg);
}
void sendLeuartData(char * buffer, uint8_t bufferLength){
	// Wait for sync
	while (LEUART0->SYNCBUSY);

	DMA_ActivateBasic(DMA_CHANNEL_TX,
	                  true,
	                  false,
	                  (void *)&LEUART0->TXDATA,
	                  buffer,
	                  (unsigned int)(bufferLength - 1));

	 while(DMA_ChannelEnabled(DMA_CHANNEL_TX)); // EnterEM
}
void setupLeuart(void){
  /* Enable peripheral clocks */
  CMU_ClockEnable(cmuClock_HFPER, true);
  /* Configure GPIO pins */
  CMU_ClockEnable(cmuClock_GPIO, true);
  /* To avoid false start, configure output as high */
  GPIO_PinModeSet(LEUART_TXPORT, LEUART_TXPIN, gpioModePushPull, 1);
  GPIO_PinModeSet(LEUART_RXPORT, LEUART_RXPIN, gpioModeInput, 0);

  LEUART_Init_TypeDef init = LEUART_INIT_DEFAULT; // Default config is fine

  /* Enable CORE LE clock in order to access LE modules */
  CMU_ClockEnable(cmuClock_CORELE, true);

  /* Select LFRCO for LEUARTs (and wait for it to stabilize) */
  CMU_ClockSelectSet(cmuClock_LFB, cmuSelect_LFXO);
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
void Leuart_BreakCondition(){
	GPIO_PinModeSet(LEUART_TXPORT, LEUART_TXPIN, gpioModePushPull, 1);
	DelayMs(20);
	GPIO_PinModeSet(LEUART_TXPORT, LEUART_TXPIN, gpioModePushPull, 0);
	DelayMs(20);
	GPIO_PinOutSet(LEUART_TXPORT, LEUART_TXPIN);
}
bool Leuart_ResponseAvailable(){
	return receiveComplete;
}
void Leuart_ReadResponse(char * buffer, uint8_t bufferLength){
	sprintf(buffer, "%s", receiveBuffer);
	receiveComplete = false;
	bufferPointer = 0;
}
void Leuart_SendCommand(char * cb, uint8_t cbl, char * rb, uint8_t rbl){
	sendLeuartData(cb, cbl);
	while(!Leuart_ResponseAvailable()){
		EMU_EnterEM2(true);
	}
	Leuart_ReadResponse(rb, rbl);
}
void Leuart_WaitForResponse(char * rb, uint8_t rbl){
	DMA_ActivateBasic(	DMA_CHANNEL_RX,
						true,
						false,
						(void *)&receiveBuffer,
						(void *)&LEUART0->RXDATA,
						0);
	while(!Leuart_ResponseAvailable()){
		EMU_EnterEM2(true);
	}
	Leuart_ReadResponse(rb, rbl);
}
