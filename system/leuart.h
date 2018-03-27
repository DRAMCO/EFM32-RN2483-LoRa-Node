/*
 * leuart.h
 *
 *  Created on: 26-jan.-2018
 *      Author: Guus Leenders
 */

#ifndef INC_LEUART_H_
#define INC_LEUART_H_

#define RECEIVE_BUFFER_SIZE 50
#define COMMAND_BUFFER_SIZE 50

typedef enum statuses{
	RX_TIMEOUT,
	TX_TIMEOUT,
	DATA_RECEIVED,
	DATA_SENT
} Leuart_Status_t;

void setupLeuart(void);

void Leuart_Init(void);
void Leuart_Reinit(void);
void Leuart_BreakCondition(void);

Leuart_Status_t Leuart_SendCommand(char * cb, uint8_t cbl, volatile bool * wakeUp);
//   ||
// \ || /
//  \||/
//   \/
void Leuart_ReadData(char * buffer, uint8_t bufferLength);
//   ||
// \ || /
//  \||/
//   \/
Leuart_Status_t Leuart_WaitForResponse();

#endif /* INC_LEUART_H_ */
