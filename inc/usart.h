/*
 * rn2483.h
 *
 *  Created on: 19-dec.-2017
 *      Author: Guus Leenders
 */

#ifndef RN2483_H_
#define RN2483_H_

#include "em_device.h"
#include "em_chip.h"
#include "em_cmu.h"
#include "em_emu.h"
#include "em_usart.h"

#define COM_PORT gpioPortE // USART location #1: PE10 and PE11
#define UART_TX_pin 10      // PE10
#define UART_RX_pin 11      // PE11

void USART0_Setup();
void USART0_ClearCondition();
void USART0_SetupRXInt(char* receiveBuffer, int receiveBufferSize);
void USART0_RX_IRQHandler(void);
void USART0_SetupTXInt(char* transmitBuffer, int bytesToSend);
void USART0_TX_IRQHandler(void);
void USART0_ReceiveBuffer(char* receiveBuffer, int bufferSize);
void USART0_SendBuffer(char* txBuffer, int bytesToSend);


#endif /* RN2483_H_ */
