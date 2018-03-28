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

typedef enum leuart_statuses{
	TX_TIMEOUT,
	RX_TIMEOUT,
	DATA_SENT,
	DATA_RECEIVED
} Leuart_Status_t;

void Leuart_Init(void);
void Leuart_Reinit(void);
void Leuart_BreakCondition(void);

void Leuart_ReadResponse(char * buffer, uint8_t bufferLength);
Leuart_Status_t Leuart_SendCommand(char * cb, uint8_t cbl, volatile bool * wakeUp);
Leuart_Status_t Leuart_WaitForResponse();

#endif /* INC_LEUART_H_ */
