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
 *         File: buttons.c
 *      Created: 2018-03-20
 *       Author: Geoffrey Ottoy
 *
 *  Description: EFM32 Happy Gecko button functions.
 */

#include <stdbool.h>
#include <em_gpio.h>

#include "irq.h"
#include "pin_mapping.h"
#include "buttons.h"

/* Initialize buttons as input and enable interrupts.
 */
void Buttons_Init(void){
	GPIO_PinModeSet(PB0_PORT, PB0_PIN, gpioModeInput, 0);
	GPIO_PinModeSet(PB1_PORT, PB1_PIN, gpioModeInput, 0);

	//TODO: move this to irq.c
	GPIO_ExtIntConfig(PB0_PORT, PB0_PIN, PB0_PIN, false, true, true);
	GPIO_ExtIntConfig(PB1_PORT, PB1_PIN, PB1_PIN, false, true, true);
	GPIO_IntClear(0xFFFF);  // Clear pending interrupts
	NVIC_EnableIRQ(GPIO_ODD_IRQn);
	NVIC_EnableIRQ(GPIO_EVEN_IRQn);

}

/* Set callback function for a button press event.
 */
void Buttons_AttachInterrupt(void * cb, button_t b){
	switch(b){
		case BUTTON_PB0:{
			IRQ_AttachInterrupt(cb, PB0_PIN);
		} break;
		case BUTTON_PB1:{
			IRQ_AttachInterrupt(cb, PB1_PIN);
		} break;
		default: return;
	}
}

