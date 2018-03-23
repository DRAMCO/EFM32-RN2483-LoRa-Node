/*  ____  ____      _    __  __  ____ ___
 * |  _ \|  _ \    / \  |  \/  |/ ___/ _ \
 * | | | | |_) |  / _ \ | |\/| | |  | | | |
 * | |_| |  _ <  / ___ \| |  | | |__| |_| |
 * |____/|_| \_\/_/   \_\_|  |_|\____\___/
 *                           research group
 *                             dramco.be/
 *
 *  KU Leuven - Technology Campus Gent,
 *  Gebroeders De Smetstraat 1,
 *  B-9000 Gent, Belgium
 *
 *         File: adc.c
 *      Created: 2018-03-22
 *       Author: Geoffrey Ottoy
 *      Version: 1.0
 *
 *  Description: TODO expand
 *  	Contains wrapper for EM-lib ADC functionality.
 *  	Currently only a single-channel single conversion
 */

#include <em_device.h>
#include <em_cmu.h>
#include <em_emu.h>
#include <em_adc.h>

#include "adc.h"

volatile bool adcConversionComplete = false;

void ADC_Init_All(void){
	ADC_Init_Measurement(BATTERY_LEVEL);
	// add other
}

ADC_Status_t ADC_Init_Measurement(ADC_Measurement_t measurement){
	// For now, we only support battery level measurements
	if(measurement != BATTERY_LEVEL){
		return ADC_WRONG_CHANNEL;
	}

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

	return ADC_DONE;
}

ADC_Status_t ADC_Get_Measurement(ADC_Measurement_t measurement, uint32_t * value){
	// For now, we only support battery level measurements
	if(measurement != BATTERY_LEVEL){
		return ADC_WRONG_CHANNEL;
	}

	/* Sample ADC */
	adcConversionComplete = false;
	ADC_Start(ADC0, adcStartSingle);
	while (!adcConversionComplete) EMU_EnterEM1();
	* value = ADC_DataSingleGet(ADC0);
	return ADC_DONE;
}

void ADC0_IRQHandler(void){
  uint32_t flags;

  /* Clear interrupt flags */
  flags = ADC_IntGet(ADC0);
  ADC_IntClear(ADC0, flags);

  adcConversionComplete = true;
}
