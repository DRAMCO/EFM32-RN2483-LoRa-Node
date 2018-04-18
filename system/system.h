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
 *
 *  Description: Header file for system.c
 */

#ifndef _SYSTEM_H_
#define _SYSTEM_H_

#include "delay.h"
#include "irq.h"
#include "led.h"
#include "buttons.h"
#include "pm.h"
#include "adc.h"
#include "iic.h"

typedef enum deepsleep_states{
	NONE_ON	= 0x00,
	RN2483_ON = 0x01,
	SENS_GECKO_ON = 0x02,
	SENS_EXT_ON = 0x04
} DeepSleep_State_t;

void System_Init(void);
void System_DeepSleep(DeepSleep_State_t PM_ON);
bool System_EM4WakeUp();

#endif /* _SYSTEM_H_ */
