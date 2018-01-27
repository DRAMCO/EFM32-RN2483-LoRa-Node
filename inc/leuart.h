/*
 * leuart.h
 *
 *  Created on: 26-jan.-2018
 *      Author: Guus Leenders
 */

#ifndef INC_LEUART_H_
#define INC_LEUART_H_

static void basicTxComplete(unsigned int channel, bool primary, void *user);
static void basicRxComplete(unsigned int channel, bool primary, void *user);
void setupDmaRx(void);
void setupDmaTx(void);
void setupLeuart(void);
void sendLeuartData(char * buffer, uint8_t bufferLength);


#endif /* INC_LEUART_H_ */
