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
 *         File: pm.c
 *      Created: 2018-03-21
 *       Author: Geoffrey Ottoy
 *      Version: 1.0
 *
 *  Description: TODO
 */

#include <em_gpio.h>

#include "pm.h"
#include "pin_mapping.h"

void PM_Init(void){
	GPIO_PinModeSet(PM_SENSORS_PORT, PM_SENSORS_PIN, gpioModePushPull, 0);
	GPIO_PinModeSet(PM_RN2483_PORT, PM_RN2483_PIN, gpioModePushPull, 0);
}

void PM_Enable(PM_SubSystem_t pmss){
	switch(pmss){
		case PM_SENSORS: {
			GPIO_PinOutSet(PM_SENSORS_PORT, PM_SENSORS_PIN);
		} break;
		case PM_RN2483: {
			GPIO_PinOutSet(PM_RN2483_PORT, PM_RN2483_PIN);
		} break;
		case PM_ALL: {
			GPIO_PinOutSet(PM_SENSORS_PORT, PM_SENSORS_PIN);
			GPIO_PinOutSet(PM_RN2483_PORT, PM_RN2483_PIN);
		} break;
		default: {
		} break;
	}
}

void PM_Disable(PM_SubSystem_t pmss){
	switch(pmss){
		case PM_SENSORS: {
			GPIO_PinOutClear(PM_SENSORS_PORT, PM_SENSORS_PIN);
		} break;
		case PM_RN2483: {
			GPIO_PinOutClear(PM_RN2483_PORT, PM_RN2483_PIN);
		} break;
		case PM_ALL: {
			GPIO_PinOutClear(PM_SENSORS_PORT, PM_SENSORS_PIN);
			GPIO_PinOutClear(PM_RN2483_PORT, PM_RN2483_PIN);
		} break;
		default: {
		} break;
	}
}
