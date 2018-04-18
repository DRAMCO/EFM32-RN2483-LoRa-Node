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
 *
 *  Description: EFM32 ADC functionality for measuring (internal) temperature
 *  	and battery level.
 */

#include <em_device.h>
#include <em_cmu.h>
#include <em_emu.h>
#include <em_adc.h>

#include "adc.h"

volatile bool adcConversionComplete = false;

ADC_Init_TypeDef       init       = ADC_INIT_DEFAULT;
ADC_InitSingle_TypeDef initSingle = ADC_INITSINGLE_DEFAULT;

static float32_t ConvertToCelsius(int32_t adcSample);


void ADC_InitChannels(void){
	/* Enable ADC clock */
	CMU_ClockEnable(cmuClock_ADC0, true);
}

static ADC_Status_t ADC_StartMeasurement(ADC_Measurement_t measurement){
	//initSingle.acqTime = adcAcqTime16;

	ADC_Reset(ADC0);

	init.timebase = ADC_TimebaseCalc(0);
	init.prescale = ADC_PrescaleCalc(400000, 0);

	/* Initiate ADC peripheral */
	ADC_Init(ADC0, &init);

	switch(measurement){
		case BATTERY_LEVEL: {
			initSingle.input = adcSingleInpVDDDiv3; // vbat/3
		} break;
		case INTERNAL_TEMPERATURE: {
			initSingle.input = adcSingleInpTemp;
		} break;
		default: {
			return ADC_WRONG_CHANNEL;
		}
	}

	ADC_InitSingle(ADC0, &initSingle);

	/* Manually set some calibration values */
	//ADC0->CAL = (0x7C << _ADC_CAL_SINGLEOFFSET_SHIFT) | (0x1F << _ADC_CAL_SINGLEGAIN_SHIFT);

	/* Enable interrupt on completed conversion */
	ADC_IntEnable(ADC0, ADC_IEN_SINGLE);
	NVIC_ClearPendingIRQ(ADC0_IRQn);
	NVIC_EnableIRQ(ADC0_IRQn);

	return ADC_DONE;
}

static ADC_Status_t ADC_GetMeasurement(ADC_Measurement_t measurement, uint32_t * value){
	/* Sample ADC */
	adcConversionComplete = false;
	ADC_Start(ADC0, adcStartSingle);

	while (!adcConversionComplete) EMU_EnterEM1();
	uint32_t val = ADC_DataSingleGet(ADC0);

	if(measurement == BATTERY_LEVEL){
		float32_t fv = val * 3.75 / 4.096;
		*value = (uint32_t) fv;
	}
	if(measurement == INTERNAL_TEMPERATURE){
		float32_t ft = ConvertToCelsius(val)*1000;
		*value = (uint32_t) ft;
	}

	return ADC_DONE;
}

ADC_Status_t ADC_Measure(ADC_Measurement_t measurement, uint32_t * value){
	if(ADC_StartMeasurement(measurement) != ADC_DONE){
		return ADC_ERROR;
	}
	if(ADC_GetMeasurement(measurement, value) != ADC_DONE){
		return ADC_ERROR;
	}
	return ADC_DONE;
}

void ADC0_IRQHandler(void){
  uint32_t flags;

  /* Clear interrupt flags */
  flags = ADC_IntGet(ADC0);
  ADC_IntClear(ADC0, flags);

  adcConversionComplete = true;
}

static float32_t ConvertToCelsius(int32_t adcSample)
{
  float32_t temp;

  /* Factory calibration temperature from device information page. */
  int32_t cal_temp_0 = ((DEVINFO->CAL & _DEVINFO_CAL_TEMP_MASK)
                        >> _DEVINFO_CAL_TEMP_SHIFT);
  /* Factory calibration value from device information page. */
  int32_t cal_value_0 = ((DEVINFO->ADC0CAL2 & _DEVINFO_ADC0CAL2_TEMP1V25_MASK)
                         >> _DEVINFO_ADC0CAL2_TEMP1V25_SHIFT);

  /* Temperature gradient (from datasheet) */
  float32_t t_grad = -6.27;

  temp = (cal_temp_0 - ((cal_value_0 - adcSample) / t_grad));

  return temp;
}
