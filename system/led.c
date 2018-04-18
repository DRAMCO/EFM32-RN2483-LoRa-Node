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
 *         File: user_led.c
 *      Created: 2018-03-16
 *       Author: Geoffrey Ottoy
 *
 *  Description: Basic functions to control the user LED.
 */

#include <em_gpio.h>
#include <led.h>

#include <pin_mapping.h>

void LED_Init(LED_state_t initState){
	GPIO_PinModeSet(LED_PORT, LED_PIN, gpioModePushPull, initState);
}

void LED_On(){
	GPIO_PinOutSet(LED_PORT, LED_PIN);
}

void LED_Off(){
	GPIO_PinOutClear(LED_PORT, LED_PIN);
}

void LED_Toggle(){
	GPIO_PinOutToggle(LED_PORT, LED_PIN);
}
