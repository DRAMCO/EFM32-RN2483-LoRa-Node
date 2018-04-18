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
 *
 *  Description: Header file for adc.c
 */

#ifndef _ADC_H_
#define _ADC_H_

#include <em_device.h>

typedef enum adc_measurements{
	BATTERY_LEVEL,
	INTERNAL_TEMPERATURE
} ADC_Measurement_t;

typedef enum adc_statuses{
	ADC_DONE,
	ADC_BUSY,
	ADC_TIMEOUT,
	ADC_ERROR,
	ADC_WRONG_CHANNEL
} ADC_Status_t;

// Quick way to initialize all necessary channels/conversions
void ADC_InitChannels(void);

ADC_Status_t ADC_Measure(ADC_Measurement_t measurement, uint32_t * value);

#endif /* _ADC_H_ */
