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
void setupDmaRx(void);
void setupDmaTx(void);
void setupLeuart(void);
void sendLeuartData(char * buffer, uint8_t bufferLength);
void Leuart_BreakCondition();
bool Leuart_ResponseAvailable();

#endif /* INC_LEUART_H_ */
