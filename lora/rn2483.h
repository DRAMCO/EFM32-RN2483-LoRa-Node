/*
 * rn2483.h
 *
 *  Created on: 18-jan.-2018
 *      Author: Guus Leenders
 */

#ifndef INC_RN2483_H_
#define INC_RN2483_H_

#include "em_device.h"
#include "em_chip.h"
#include "em_cmu.h"
#include "em_emu.h"
#include "em_usart.h"

#define RN2483_COMMANDBUFFER_SIZE 80
#define RN2483_POWER_14DBM 1
#define RN2483_POWER_11DBM 2
#define RN2483_POWER_8DBM 3
#define RN2483_POWER_4DBM 4
#define RN2483_POWER_2DBM 5

#define TX_FAIL 0
#define TX_SUCCESS 1
#define TX_SUCCESS_CONFIRMED 2

void RN2483_Init(char * receiveBuffer, uint8_t bufferSize);
void RN2483_Sleep(uint32_t sleepTime, char * receiveBuffer, uint8_t bufferSize);
void RN2483_GetHardwareEUI(char * receiveBuffer, uint8_t bufferSize);
void RN2483_GetSystemVersion(char * receiveBuffer, uint8_t bufferSize);
void RN2483_GetApplicationEUI(char * receiveBuffer, uint8_t bufferSize);
void RN2483_MacReset(char * receiveBuffer, uint8_t bufferSize);
void RN2483_SetDeviceEUI(char * eui, char * receiveBuffer, uint8_t bufferSize);
void RN2483_SetApplicationEUI(char * eui, char * receiveBuffer, uint8_t bufferSize);
void RN2483_SetApplicationKey(char * key, char * receiveBuffer, uint8_t bufferSize);
void RN2483_SetApplicationSessionKey(char * skey, char * receiveBuffer, uint8_t bufferSize);
void RN2483_SetNetworkSessionKey(char * address, char * receiveBuffer, uint8_t bufferSize);
void RN2483_SetDeviceAddress(char * address, char * receiveBuffer, uint8_t bufferSize);
void RN2483_SetOutputPower(uint8_t pwr, char * receiveBuffer, uint8_t bufferSize);
void RN2483_DisableAdaptiveDataRate(char * receiveBuffer, uint8_t bufferSize);
void RN2483_DisableAutomaticReplies(char * receiveBuffer, uint8_t bufferSize);
void RN2483_SetDataRate(uint8_t dr, char * receiveBuffer, uint8_t bufferSize);
void RN2483_SaveMac(char * receiveBuffer, uint8_t bufferSize);
bool RN2483_JoinOTAA(char * receiveBuffer, uint8_t bufferSize);
bool RN2483_JoinABP(char * receiveBuffer, uint8_t bufferSize);
bool RN2483_SetupOTAA(char * appEUI, char * appKey, char * devEUI, char * receiveBuffer, uint8_t bufferSize);
bool RN2483_SetupABP(char * devAddr, char * appKey, char * NwkSKey, char * receiveBuffer, uint8_t bufferSize);
uint8_t RN2483_Transmit(char * data, uint8_t payloadSize, char * receiveBuffer, uint8_t bufferSize);
uint8_t RN2483_TransmitUnconfirmed(uint8_t * data, uint8_t payloadSize, char * receiveBuffer, uint8_t bufferSize);
uint8_t RN2483_TransmitConfirmed(uint8_t * data, uint8_t payloadSize, char * receiveBuffer, uint8_t bufferSize);
uint8_t RN2483_TransmitProcessCommand(uint8_t commandSize, char * receiveBuffer, uint8_t bufferSize);
void RN2483_Wake(char * receiveBuffer, uint8_t bufferSize);

#endif /* INC_RN2483_H_ */
