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
 *         File: irq.h
 *      Created: 2018-03-20
 *       Author: Geoffrey Ottoy
 *
 *  Description: Header file for irq.c
 */

#ifndef INC_IRQ_H_
#define INC_IRQ_H_

#include "em_device.h"

#define NR_IO_INTERRUPTS 16

typedef void( IO_Interrupt_t )( void );

void IRQ_Init(void);
void IRQ_AttachInterrupt(void * cb, uint8_t pin);

#endif /* INC_IRQ_H_ */
