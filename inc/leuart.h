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

static void basicTxComplete(unsigned int channel, bool primary, void *user);
static void basicRxComplete(unsigned int channel, bool primary, void *user);
void setupDma(void);
void setupLeuart(void);
void sendLeuartData(char * buffer, uint8_t bufferLength);
void Leuart_BreakCondition();
bool Leuart_ResponseAvailable();
void Leuart_ReadResponse(char * buffer, uint8_t bufferLength);
void Leuart_SendCommand(char * cb, uint8_t cbl, char * rb, uint8_t rbl);
void Leuart_WaitForResponse(char * rb, uint8_t rbl);

#endif /* INC_LEUART_H_ */
