/*
 * inttemp.h
 *
 *  Created on: 16-mrt.-2018
 *      Author: Guus Leenders
 */

#ifndef INC_INTTEMP_H_
#define INC_INTTEMP_H_

void       AdcSetup(void);
uint32_t   AdcRead(void);
float32_t  ConvertToCelsius(int32_t adcSample);
int16_t GetInternalTemperature();

#endif /* INC_INTTEMP_H_ */
