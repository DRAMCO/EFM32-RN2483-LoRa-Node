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
 *         File: buttons.h
 *      Created: 2018-03-20
 *       Author: Geoffrey Ottoy
 *      Version: 1.0
 *
 *  Description: TODO
 */

#ifndef INC_BUTTONS_H_
#define INC_BUTTONS_H_

typedef enum buttons{
	BUTTON_PB0,
	BUTTON_PB1
} button_t;

void Buttons_Init(void);

void Buttons_AttachInterrupt(void * cb, button_t b);

#endif /* INC_BUTTONS_H_ */
