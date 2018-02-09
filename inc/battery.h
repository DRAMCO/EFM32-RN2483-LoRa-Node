/*
 * battery.h
 *
 *  Created on: 7-feb.-2018
 *      Author: Guus Leenders
 */

#ifndef INC_BATTERY_H_
#define INC_BATTERY_H_

uint32_t checkBattery(void);
void ADC0_IRQHandler(void);
void adcInit(void);

#endif /* INC_BATTERY_H_ */
