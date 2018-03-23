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
 *         File: adc.h
 *      Created: 2018-03-22
 *       Author: Geoffrey Ottoy
 *      Version: 1.0
 *
 *  Description: TODO
 */

#ifndef SYSTEM_ADC_H_
#define SYSTEM_ADC_H_

#include <em_device.h>

typedef enum adc_measurements{
	BATTERY_LEVEL
} ADC_Measurement_t;

typedef enum adc_statuses{
	ADC_DONE,
	ADC_BUSY,
	ADC_TIMEOUT,
	ADC_WRONG_CHANNEL
} ADC_Status_t;

// Quick way to initialize all necessary channels/conversions
void ADC_Init_All(void);

ADC_Status_t ADC_Init_Measurement(ADC_Measurement_t measurement);

ADC_Status_t ADC_Get_Measurement(ADC_Measurement_t measurement, uint32_t * value);

#endif /* SYSTEM_ADC_H_ */
