/*
 * battery.c
 *
 *  Created on: 7-feb.-2018
 *      Author: Guus Leenders
 */

#include "em_device.h"
#include "em_cmu.h"
#include "em_emu.h"
#include "em_adc.h"
#include "em_gpio.h"

#include "battery.h"

static volatile bool adcConversionComplete = false;

uint32_t Battery_Read(void){
	GPIO_PinOutSet(gpioPortE, 10);

	uint32_t vData;
	/* Sample ADC */
	adcConversionComplete = false;
	ADC_Start(ADC0, adcStartSingle);
	while (!adcConversionComplete) EMU_EnterEM1();
	vData = ADC_DataSingleGet(ADC0);

	GPIO_PinOutClear(gpioPortE, 10);

	return vData;
}

void ADC0_IRQHandler(void){
	uint32_t flags;

	/* Clear interrupt flags */
	flags = ADC_IntGet(ADC0);
	ADC_IntClear(ADC0, flags);

	adcConversionComplete = true;
}

void Battery_Init(void){
	GPIO_PinModeSet(gpioPortE, 10, gpioModePushPull, 0);

	ADC_Init_TypeDef       init       = ADC_INIT_DEFAULT;
	ADC_InitSingle_TypeDef initSingle = ADC_INITSINGLE_DEFAULT;

	/* Enable ADC clock */
	CMU_ClockEnable(cmuClock_ADC0, true);

	/* Initiate ADC peripheral */
	ADC_Init(ADC0, &init);

	/* Setup single conversions for internal VDD/3 */
	initSingle.acqTime = adcAcqTime16;
	initSingle.input   = adcSingleInputCh1;
	ADC_InitSingle(ADC0, &initSingle);

	/* Manually set some calibration values */
	ADC0->CAL = (0x7C << _ADC_CAL_SINGLEOFFSET_SHIFT) | (0x1F << _ADC_CAL_SINGLEGAIN_SHIFT);

	/* Enable interrupt on completed conversion */
	ADC_IntEnable(ADC0, ADC_IEN_SINGLE);
	NVIC_ClearPendingIRQ(ADC0_IRQn);
	NVIC_EnableIRQ(ADC0_IRQn);
}
