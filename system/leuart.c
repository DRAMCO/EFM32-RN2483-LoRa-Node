/*
 * rn2483.c
 *
 *  Created on: 19-dec.-2017
 *      Author: Guus Leenders
 */
#include <stdint.h>
#include <stdbool.h>

#include <em_device.h>
#include <em_chip.h>
#include <em_gpio.h>
#include <em_emu.h>
#include <em_leuart.h>
#include <em_dma.h>
#include <em_cmu.h>

#include <dmactrl.h>
#include <rtcdriver.h>

#include "delay.h"
#include "util.h"
#include "leuart.h"

#define TX_TIMEOUT_DURATION		2000	// ms
#define RX_TIMEOUT_DURATION		10000	// ms

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

//char commandBuffer[COMMAND_BUFFER_SIZE];
char receiveBuffer[RECEIVE_BUFFER_SIZE];
volatile uint8_t bufferPointer = 0;
volatile bool receiveComplete = false;

RTCDRV_TimerID_t xTimerForTimeout;
static volatile bool timeout = false;

void Leuart_ClearBuffers(void){
	memset(receiveBuffer, '\0', RECEIVE_BUFFER_SIZE);
	receiveComplete = false;
}

void timeoutCb(RTCDRV_TimerID_t id, void *user){
	timeout = true;
}

/** Static (internal) functions */
static void basicTxComplete(unsigned int channel, bool primary, void *user){
	(void) user;
	/* Refresh DMA basic transaction cycle */
	DMA_ActivateBasic(DMA_CHANNEL_RX,
					primary,
					false,
					(void *)&receiveBuffer[0],
					(void *)&LEUART0->RXDATA,
					0);
	bufferPointer = 0;
}

static void basicRxComplete(unsigned int channel, bool primary, void *user){
	(void) user;

	/* Refresh DMA basic transaction cycle */
	char c = LEUART0->RXDATA;
	if(bufferPointer < RECEIVE_BUFFER_SIZE - 1){
		if(c != '\n'){
			bufferPointer++;
			DMA_ActivateBasic(DMA_CHANNEL_RX,
								  primary,
								  false,
								  &receiveBuffer[bufferPointer],
								  NULL,
								  0);
		}
		else{
			receiveComplete = true;
			receiveBuffer[bufferPointer+1] = '\0';
		}
	}
}

static bool Leuart_ResponseAvailable(void){
	return receiveComplete;
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

static void sendLeuartData(char * buffer, uint8_t bufferLength){
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

static void setupLeuart(void){
	/* Allocate timer for Timeout detection*/
	RTCDRV_AllocateTimer(&xTimerForTimeout);

	/* Enable peripheral clocks */
	CMU_ClockEnable(cmuClock_HFPER, true);
	/* Configure GPIO pins */
	CMU_ClockEnable(cmuClock_GPIO, true);
	/* To avoid false start, configure output as high */
	GPIO_PinModeSet(LEUART_TXPORT, LEUART_TXPIN, gpioModePushPull, 1);
	GPIO_PinModeSet(LEUART_RXPORT, LEUART_RXPIN, gpioModeInput, 0);

	LEUART_Init_TypeDef init = LEUART_INIT_DEFAULT; // Default config is fine
	init.baudrate = 4800;

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

void Leuart_Init(void){
	setupDma();
	Leuart_BreakCondition();
	setupLeuart();

	// Auto baud setting
	char b[] = "U";
	sendLeuartData(b, 1);
	DelayMs(500);
}

void Leuart_Reinit(void){
	LEUART_Reset(LEUART0);
	Leuart_BreakCondition();
	setupLeuart();

	// Auto baud setting
	char b[] = "U";
	sendLeuartData(b, 1);
	Leuart_WaitForResponse();
	DelayMs(20);
	Leuart_WaitForResponse();
}

void Leuart_BreakCondition(void){
	GPIO_PinModeSet(LEUART_TXPORT, LEUART_TXPIN, gpioModePushPull, 1);
	DelayMs(40);
	GPIO_PinModeSet(LEUART_TXPORT, LEUART_TXPIN, gpioModePushPull, 0);
	DelayMs(20);
	GPIO_PinOutSet(LEUART_TXPORT, LEUART_TXPIN);
}

void Leuart_ReadResponse(char * buffer, uint8_t bufferLength){
	sprintf(buffer, "%s", receiveBuffer);
	receiveComplete = false;
	bufferPointer = 0;
}

void Leuart_SendData(char * buffer, uint8_t bufferLength){
	// Send data over LEUART
	sendLeuartData(buffer, bufferLength);

	// Start Timeout-timer if needed
	timeout = false;
	RTCDRV_StartTimer(xTimerForTimeout, rtcdrvTimerTypeOneshot, 10, (RTCDRV_Callback_t) &timeoutCb, NULL);
	// Wait for response or timeout
	while(!Leuart_ResponseAvailable() && !timeout){
		EMU_EnterEM2(true);
	}

	receiveComplete = true;
}

// Send a command string over the LEUART. Specifying a waitTime > 0 will result in a timeout when no command is received in time.
Leuart_Status_t Leuart_SendCommand(char * cb, uint8_t cbl, volatile bool * wakeUp){
	// Send data over LEUART
	sendLeuartData(cb, cbl);

	// Start Timeout-timer if needed
	timeout = false;
	//RTCDRV_StartTimer(xTimerForTimeout, rtcdrvTimerTypeOneshot, waitTime, (RTCDRV_Callback_t) &timeoutCb, NULL);
	// Wait for response or timeout
	while(!Leuart_ResponseAvailable() && !timeout && !(*wakeUp)){
		EMU_EnterEM2(true);
	}

	// Check for timeout
	if(timeout){ // if running is false, we got here because of the timeout.
		return TX_TIMEOUT;
	}

	// Stop Timeout-timer when command is received
	//RTCDRV_StopTimer(xTimerForTimeout); // stop timer

	return DATA_SENT;
}


Leuart_Status_t Leuart_WaitForResponse(){
	// Activate DMA
	DMA_ActivateBasic(	DMA_CHANNEL_RX,
						true,
						false,
						(void *)&receiveBuffer[0],
						(void *)&LEUART0->RXDATA,
						0);

	// Start Timeout-timer
	timeout = false;
	//RTCDRV_StartTimer(xTimerForTimeout, rtcdrvTimerTypeOneshot, RX_TIMEOUT_DURATION, (RTCDRV_Callback_t) &timeoutCb, NULL);
	// Wait for response or timeout
	while(!Leuart_ResponseAvailable() && !timeout){
		EMU_EnterEM2(true);
	}
	//RTCDRV_StopTimer(xTimerForTimeout); // stop timer

	// Check for timeout
	if(timeout){ // if running is false, we got here because of the timeout.
		return RX_TIMEOUT;
	}

	// OK
	return DATA_RECEIVED;
}
