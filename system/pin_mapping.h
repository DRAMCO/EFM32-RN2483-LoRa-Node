
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
 *         File: pinmapping.h
 *      Created: 2018-03-21
 *       Author: Geoffrey Ottoy
 *      Version: 1.0
 *
 *  Description: TODO
 */

#ifndef PINMAPPING_H_
#define PINMAPPING_H_

#include <em_gpio.h>

// User LED
#define LED_PORT			gpioPortA
#define LED_PIN				2

// Push buttons
#define PB0_PORT			gpioPortC
#define PB0_PIN				9
#define PB1_PORT			gpioPortC
#define PB1_PIN				10

// RN2483
#define RN2483_RESET_PORT	gpioPortA
#define RN2483_RESET_PIN	10
#define RN2483_RX_PORT		gpioPortD
#define RN2483_RX_PIN		5
#define RN2483_TX_PORT		gpioPortD
#define RN2483_TX_PIN		4
#define RN2483_UART			LEUART0
#define RN2483_UART_LOC		0

// LIS2DH external accelerometer
#define LIS3DH_INT_PORT		gpioPortF
#define LIS3DH_INT_PIN		2

// Power supply enable
#define PM_RN2483_PORT		gpioPortA
#define PM_RN2483_PIN		8
#define PM_SENS_GECKO_PORT	gpioPortC
#define PM_SENS_GECKO_PIN	8
#define PM_SENS_EXT_PORT	gpioPortA
#define PM_SENS_EXT_PIN		9

#endif /* SYSTEM_PINMAPPING_H_ */
