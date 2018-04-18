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
 *         File: irq.c
 *      Created: 2018-03-20
 *       Author: Geoffrey Ottoy
 *
 *  Description: GPIO pin interrupt handling.
 *  	Every interrupt is caught and, when assigned, the corresponding
 *  	callback is called.
 */

#include <em_gpio.h>

#include "irq.h"
#include "pin_mapping.h"

IO_Interrupt_t *irq[NR_IO_INTERRUPTS];

void IRQ_Init(void){
	int i;
	for(i=0; i<NR_IO_INTERRUPTS; i++){
		irq[i] = NULL;
	}
}

void IRQ_AttachInterrupt(void * cb, uint8_t pin){
	irq[pin] = cb;
}

void GPIO_ODD_IRQHandler(void){
	uint32_t irq_pins = GPIO_IntGet();
  	GPIO_IntClear(0xAAAA);

  	// TODO: add other pins that can generate interrupts
  	if(irq_pins & (1 << PB0_PIN)){
  		if (irq[PB0_PIN] != NULL)
  			irq[PB0_PIN]();
  	}
}

void GPIO_EVEN_IRQHandler(void){
	uint32_t irq_pins = GPIO_IntGet();
  	GPIO_IntClear(0x5555);

  	// TODO: add other pins that can generate interrupts
  	if(irq_pins & (1 << PB1_PIN)){
  		if (irq[PB1_PIN] != NULL)
  			irq[PB1_PIN]();
  	}

  	if(irq_pins & (1 << EXT_INT_PIN)){
  		if (irq[EXT_INT_PIN] != NULL)
  			irq[EXT_INT_PIN]();
  	}
}
