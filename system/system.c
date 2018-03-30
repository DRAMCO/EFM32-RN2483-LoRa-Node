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
 *         File: system.c
 *      Created: 2018-03-21
 *       Author: Geoffrey Ottoy
 *      Version: 1.0
 *
 *  Description: TODO
 */

#include <stdbool.h>
#include <em_device.h>
#include <em_chip.h>
#include <em_cmu.h>
#include <em_rmu.h>

#include "system.h"

static bool em4WakeUp;

static void System_GetResetSource(void){
	// Get reset cause(s)
	uint32_t rflags = RMU_ResetCauseGet();
	if (rflags & RMU_RSTCAUSE_EM4WURST){
		em4WakeUp = true;
	}
	RMU_ResetCauseClear();
}

void System_Init(void){
	em4WakeUp = false;

	/* Chip errata */
	CHIP_Init();
	CMU_ClockEnable(cmuClock_GPIO, true);

	// Initialize RTC for use in delay functions
	Delay_Init();

	// Initialize GPIO interrupts (call-backs can be linked later)
	IRQ_Init();

	// Initialize user LED
	LED_Init(LED_OFF);

	// Initialize push buttons
	Buttons_Init();

	// Initialize power supply management
	PM_Init();

	// Initialize Analog-to-Digital Conversions
	ADC_Init_All();

	// Initialize I2C bus (to interface with sensors)
	IIC_Init();
}

bool System_EM4WakeUp(){
	return em4WakeUp;
}
