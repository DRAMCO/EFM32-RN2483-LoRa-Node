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
 *         File: system.h
 *      Created: 2018-03-21
 *       Author: Geoffrey Ottoy
 *      Version: 1.0
 *
 *  Description: TODO
 */

#ifndef PERIPHERALS_H_
#define PERIPHERALS_H_

#include "delay.h"
#include "irq.h"
#include "led.h"
#include "buttons.h"
#include "pm.h"
#include "adc.h"
#include "iic.h"

#define NONE_ON			0x00
#define RN2483_ON		0x01
#define SENS_GECKO_ON	0x02
#define SENS_EXT_ON		0x04

void System_Init(void);
void System_DeepSleep(uint8_t PM_ON);
bool System_EM4WakeUp();

#endif /* PERIPHERALS_H_ */
