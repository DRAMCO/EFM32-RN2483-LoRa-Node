/*
 * rn2483.c
 *
 *  Created on: 18-jan.-2018
 *      Author: Guus Leenders
 */



#include <stdint.h>
#include <stdbool.h>
#include <stdio.h>
#include "em_device.h"
#include "em_chip.h"
#include "em_cmu.h"
#include "em_gpio.h"
#include "em_usart.h"

#include "usart.h"
#include "delay.h"
#include "util.h"

#include "rn2483.h"

char commandBuffer[RN2483_COMMANDBUFFER_SIZE];
char deviceEUI[17]; 	// Al arrays size + 1 because of \0 terminator in string
char applicationEUI[17];
char applicationKey[33];
char networkSessionKey[17];
char deviceAddress[9];

void RN2483_Init(){
	memset(commandBuffer, '\0', RN2483_COMMANDBUFFER_SIZE);
}
void RN2483_GetHardwareEUI(char * receiveBuffer, uint8_t bufferSize){
	sprintf(commandBuffer, "mac get deveui\r\n");
	USART0_SendBuffer(commandBuffer, strlen(commandBuffer));
	USART0_ReceiveBuffer(receiveBuffer, bufferSize);
}
void RN2483_GetSystemVersion(char * receiveBuffer, uint8_t bufferSize){
	sprintf(commandBuffer, "sys get ver\r\n");
	USART0_SendBuffer(commandBuffer, strlen(commandBuffer));
	USART0_ReceiveBuffer(receiveBuffer, bufferSize);
}
void RN2483_GetApplicationEUI(char * receiveBuffer, uint8_t bufferSize){
	sprintf(commandBuffer, "sys get appeui\r\n");
	USART0_SendBuffer(commandBuffer, strlen(commandBuffer));
	USART0_ReceiveBuffer(receiveBuffer, bufferSize);
}
void RN2483_MacReset(char * receiveBuffer, uint8_t bufferSize){
	sprintf(commandBuffer, "mac reset 868\r\n");
	USART0_SendBuffer(commandBuffer, strlen(commandBuffer));
	USART0_ReceiveBuffer(receiveBuffer, bufferSize);
}
void RN2483_SetDeviceEUI(char * eui, char * receiveBuffer, uint8_t bufferSize){
	if(strlen(eui) == 16){
		strcpy(deviceEUI, eui);
		deviceEUI[16] = '\0';
	}
	sprintf(commandBuffer, "mac set deveui %s\r\n", deviceEUI);
	USART0_SendBuffer(commandBuffer, strlen(commandBuffer));
	USART0_ReceiveBuffer(receiveBuffer, bufferSize);
}
void RN2483_SetApplicationEUI(char * eui, char * receiveBuffer, uint8_t bufferSize){
	if(strlen(eui) == 16){
		strcpy(applicationEUI, eui);
		applicationEUI[16] = '\0';
	}
	sprintf(commandBuffer, "mac set appeui %s\r\n", applicationEUI);
	USART0_SendBuffer(commandBuffer, strlen(commandBuffer));
	USART0_ReceiveBuffer(receiveBuffer, bufferSize);
}
void RN2483_SetApplicationKey(char * key, char * receiveBuffer, uint8_t bufferSize){
	if(strlen(key) == 32){
		strcpy(applicationKey, key);
		applicationKey[32] = '\0';
	}
	sprintf(commandBuffer, "mac set appkey %s\r\n", applicationKey);
	USART0_SendBuffer(commandBuffer, strlen(commandBuffer));
	USART0_ReceiveBuffer(receiveBuffer, bufferSize);
}
void RN2483_SetNetworkSessionKey(char * key, char * receiveBuffer, uint8_t bufferSize){
	if(strlen(key) == 16){
		strcpy(networkSessionKey, key);
		networkSessionKey[16] = '\0';
	}
	sprintf(commandBuffer, "mac set nwkskey %s\r\n", networkSessionKey);
	USART0_SendBuffer(commandBuffer, strlen(commandBuffer));
	USART0_ReceiveBuffer(receiveBuffer, bufferSize);
}
void RN2483_SetDeviceAddress(char * address, char * receiveBuffer, uint8_t bufferSize){
	if(strlen(address) == 8){
		strcpy(deviceAddress, address);
		deviceAddress[8] = '\0';
	}
	sprintf(commandBuffer, "mac set devaddr %s\r\n", deviceAddress);
	USART0_SendBuffer(commandBuffer, strlen(commandBuffer));
	USART0_ReceiveBuffer(receiveBuffer, bufferSize);
}
void RN2483_SetOutputPower(uint8_t pwr, char * receiveBuffer, uint8_t bufferSize){
	sprintf(commandBuffer, "mac set pwridx %i\r\n", pwr);
	USART0_SendBuffer(commandBuffer, strlen(commandBuffer));
	USART0_ReceiveBuffer(receiveBuffer, bufferSize);
}
void RN2483_DisableAdaptiveDataRate(char * receiveBuffer, uint8_t bufferSize){
	sprintf(commandBuffer, "mac set adr off\r\n");
	USART0_SendBuffer(commandBuffer, strlen(commandBuffer));
	USART0_ReceiveBuffer(receiveBuffer, bufferSize);
}
void RN2483_DisableAutomaticReplies(char * receiveBuffer, uint8_t bufferSize){
	sprintf(commandBuffer, "mac set ar off\r\n");
	USART0_SendBuffer(commandBuffer, strlen(commandBuffer));
	USART0_ReceiveBuffer(receiveBuffer, bufferSize);
}
void RN2483_SaveMac(char * receiveBuffer, uint8_t bufferSize){
	sprintf(commandBuffer, "mac save\r\n");
	USART0_SendBuffer(commandBuffer, strlen(commandBuffer));
	USART0_ReceiveBuffer(receiveBuffer, bufferSize);
}
bool RN2483_JoinOTAA(char * receiveBuffer, uint8_t bufferSize){
	bool joined = false;
	for(int i=0; i<10 && !joined; i++){
		sprintf(commandBuffer, "mac join otaa\r\n");
		USART0_SendBuffer(commandBuffer, strlen(commandBuffer));
		USART0_ReceiveBuffer(receiveBuffer, bufferSize); // Receive command response
		USART0_ReceiveBuffer(receiveBuffer, bufferSize); // Receive accepted
		if(StringStartsWith(receiveBuffer, "accepted")){
			joined = true;
			DelayMs(1000);
		}else{
			DelayMs(1000);
		}
	}
	return joined;
}
bool RN2483_SetupOTAA(char * appEUI, char * appKey, char * devEUI, char * receiveBuffer, uint8_t bufferSize){
	RN2483_MacReset(receiveBuffer, bufferSize);
	if(devEUI == NULL){
		RN2483_GetHardwareEUI(receiveBuffer, bufferSize);
		char address[16];
		sprintf(address, "%s", receiveBuffer);
		RN2483_SetDeviceEUI(address, receiveBuffer, bufferSize);
	}else{
		RN2483_SetDeviceEUI(devEUI, receiveBuffer, bufferSize);
	}
	RN2483_SetApplicationEUI(appEUI, receiveBuffer, bufferSize);
	RN2483_SetApplicationKey(appKey, receiveBuffer, bufferSize);
	RN2483_SetOutputPower(RN2483_POWER_14DBM, receiveBuffer, bufferSize);
	RN2483_DisableAdaptiveDataRate(receiveBuffer, bufferSize);
	RN2483_DisableAutomaticReplies(receiveBuffer, bufferSize);
	RN2483_SaveMac(receiveBuffer, bufferSize);
	bool joined = RN2483_JoinOTAA(receiveBuffer, bufferSize);
	return joined;
}


