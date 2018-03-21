/*
 * inttemp.c
 *
 *  Created on: 16-mrt.-2018
 *      Author: Guus Leenders
 */

#include <stdio.h>
#include <stdlib.h>

#include "em_device.h"
#include "em_adc.h"
#include "em_chip.h"
#include "em_cmu.h"
#include "em_emu.h"
#include "em_gpio.h"

#include "inttemp.h"

/***************************************************************************//**
 * @brief Initialize ADC for temperature sensor readings in single poin
 ******************************************************************************/
void AdcSetup(void)
{
  /* Enable ADC clock */
  CMU_ClockEnable(cmuClock_ADC0, true);

  /* Base the ADC configuration on the default setup. */
  ADC_Init_TypeDef       init  = ADC_INIT_DEFAULT;
  ADC_InitSingle_TypeDef sInit = ADC_INITSINGLE_DEFAULT;

  /* Initialize timebases */
  init.timebase = ADC_TimebaseCalc(0);
  init.prescale = ADC_PrescaleCalc(400000, 0);
  ADC_Init(ADC0, &init);

  /* Set input to temperature sensor. Reference must be 1.25V */
  sInit.reference = adcRef1V25;
  sInit.input     = adcSingleInpTemp;
  ADC_InitSingle(ADC0, &sInit);
}

/***************************************************************************//**
 * @brief  Do one ADC conversion
 * @return ADC conversion result
 ******************************************************************************/
uint32_t AdcRead(void)
{
  ADC_Start(ADC0, adcStartSingle);
  while ( (ADC0->STATUS & ADC_STATUS_SINGLEDV) == 0 ) {
  }
  return ADC_DataSingleGet(ADC0);
}

/***************************************************************************//**
 * @brief Convert ADC sample values to celsius.
 * @note See section 22.3.4.2 Temperature measurement in the reference manual
 *       for details on this calculation.
 * @param adcSample Raw value from ADC to be converted to celsius
 * @return The temperature in degrees celsius.
 ******************************************************************************/
float32_t ConvertToCelsius(int32_t adcSample)
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
int16_t GetInternalTemperature(){
	AdcSetup();
	uint32_t adcValue = AdcRead();
	float32_t tempC = ConvertToCelsius(adcValue);
	return (int16_t) (tempC * 10);
}
