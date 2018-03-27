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
 *         File: user_led.h
 *      Created: 2018-03-16
 *       Author: Geoffrey Ottoy
 *      Version: 1.0
 *
 *  Description: TODO
 */


#ifndef INC_LED_H_
#define INC_LED_H_

typedef enum led_states{
	LED_ON = 1,
	LED_OFF = 0
} LED_state_t;

void LED_Init(LED_state_t initState);
void LED_On(void);
void LED_Off(void);
void LED_Toggle(void);

#endif /* INC_LED_H_ */
