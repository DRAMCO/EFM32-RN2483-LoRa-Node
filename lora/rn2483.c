/*
 * rn2483.c
 *
 *  Created on: 18-jan.-2018
 *      Author: Guus Leenders
 */



#include <stdint.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include "em_device.h"
#include "em_chip.h"
#include "em_cmu.h"
#include "em_gpio.h"
#include "em_usart.h"
#include <em_leuart.h>

#include "leuart.h"
#include "delay.h"
#include "util.h"
#include "lora.h"

#include "rn2483.h"

char commandBuffer[RN2483_COMMANDBUFFER_SIZE];

static RN2483_Status_t RN2483_ProcessMacCommand(char * receiveBuffer, uint8_t bufferSize, bool secondResponse){
	/*bool sendDone = false;
	uint8_t busyCounter = 0;
	uint8_t retryCounter = 0;

	while(!sendDone){
		retryCounter ++;
		if(retryCounter > 30){
			return TX_FAIL;
		}
		Leuart_SendCommand(commandBuffer, strlen(commandBuffer), receiveBuffer, bufferSize);
		if(StringStartsWith(receiveBuffer, "ok")){
			Leuart_WaitForResponse(receiveBuffer, bufferSize);
			if(StringStartsWith(receiveBuffer, "mac_tx_ok")){
				sendDone = true;
				return TX_SUCCESS;
			}else if(StringStartsWith(receiveBuffer, "mac_rx")){
				// TODO: capture confirmation data
				sendDone = true;
				return TX_SUCCESS_CONFIRMED;
			}else if(StringStartsWith(receiveBuffer, "mac_err")){
				RN2483_Setup(receiveBuffer, bufferSize);
			}else if(StringStartsWith(receiveBuffer, "invalid_data_len")){
				sendDone = true;
				return TX_FAIL;
			}else if(StringStartsWith(receiveBuffer, "radio_tx_ok")){
				sendDone = true;
				return TX_SUCCESS;
			}else if(StringStartsWith(receiveBuffer, "radio_err")){
				RN2483_Setup(receiveBuffer, bufferSize);
			}
		}else if(StringStartsWith(receiveBuffer, "invalid_param")){
			sendDone = true;
			return TX_FAIL;
		}else if(StringStartsWith(receiveBuffer, "not_joined")){
			RN2483_Setup(receiveBuffer, bufferSize);
		}else if(StringStartsWith(receiveBuffer, "no_free_ch")){
			DelayMs(500);
		}else if(StringStartsWith(receiveBuffer, "silent")){
			RN2483_Setup(receiveBuffer, bufferSize);
		}else if(StringStartsWith(receiveBuffer, "frame_counter_err_rejoin_needed")){
			RN2483_Setup(receiveBuffer, bufferSize);
		}else if(StringStartsWith(receiveBuffer, "busy")){
			busyCounter ++;
			if(busyCounter >=10){
				RN2483_Setup(receiveBuffer, bufferSize);
			}else{
				DelayMs(500);
			}
		}else if(StringStartsWith(receiveBuffer, "mac_paused")){
			RN2483_Setup(receiveBuffer, bufferSize);
		}else if(StringStartsWith(receiveBuffer, "invalid_data_len")){
			sendDone = true;
			return TX_FAIL;
		}else{
			RN2483_Setup(receiveBuffer, bufferSize);
		}
	}
	return TX_FAIL;*/
}

static RN2483_Status_t RN2483_ProcessSleepCommand(char * receiveBuffer, uint8_t bufferSize, volatile bool * wakeUp){

}

void RN2483_Init(void){ // Setup with autobaud
	GPIO_PinModeSet(gpioPortA, 10, gpioModePushPull, 0);
	DelayMs(20);
	GPIO_PinModeSet(gpioPortA, 10, gpioModePushPull, 1);
	DelayMs(250);

	memset(commandBuffer, '\0', RN2483_COMMANDBUFFER_SIZE);

	Leuart_Init();

	sprintf(commandBuffer, "U");
	sendLeuartData(commandBuffer, (uint8_t) strlen(commandBuffer));
	DelayMs(20);
}

RN2483_Status_t RN2483_MacReset(char * receiveBuffer, uint8_t bufferSize){
	sprintf(commandBuffer, "mac reset 868\r\n");
	return RN2483_ProcessMacCommand(receiveBuffer, bufferSize, false);
}

RN2483_Status_t RN2483_GetHardwareEUI(char * receiveBuffer, uint8_t bufferSize){
	sprintf(commandBuffer, "mac get deveui\r\n");
	return RN2483_ProcessMacCommand(receiveBuffer, bufferSize, false);
}

RN2483_Status_t RN2483_GetSystemVersion(char * receiveBuffer, uint8_t bufferSize){
	sprintf(commandBuffer, "sys get ver\r\n");
	return RN2483_ProcessMacCommand(receiveBuffer, bufferSize, false);
}

RN2483_Status_t RN2483_GetApplicationEUI(char * receiveBuffer, uint8_t bufferSize){
	sprintf(commandBuffer, "sys get appeui\r\n");
	return RN2483_ProcessMacCommand(receiveBuffer, bufferSize, false);
}

RN2483_Status_t RN2483_SetDeviceEUI(char * eui, char * receiveBuffer, uint8_t bufferSize){
	sprintf(commandBuffer, "mac set deveui %s\r\n", eui);
	return RN2483_ProcessMacCommand(receiveBuffer, bufferSize, false);
}

RN2483_Status_t RN2483_SetApplicationEUI(char * eui, char * receiveBuffer, uint8_t bufferSize){
	sprintf(commandBuffer, "mac set appeui %s\r\n", eui);
	return RN2483_ProcessMacCommand(receiveBuffer, bufferSize, false);
}

RN2483_Status_t RN2483_SetApplicationKey(char * key, char * receiveBuffer, uint8_t bufferSize){
	sprintf(commandBuffer, "mac set appkey %s\r\n", key);
	return RN2483_ProcessMacCommand(receiveBuffer, bufferSize, false);
}

RN2483_Status_t RN2483_SetApplicationSessionKey(char * key, char * receiveBuffer, uint8_t bufferSize){
	sprintf(commandBuffer, "mac set appskey %s\r\n", key);
	return RN2483_ProcessMacCommand(receiveBuffer, bufferSize, false);
}

RN2483_Status_t RN2483_SetNetworkSessionKey(char * key, char * receiveBuffer, uint8_t bufferSize){
	sprintf(commandBuffer, "mac set nwkskey %s\r\n", key);
	return RN2483_ProcessMacCommand(receiveBuffer, bufferSize, false);
}

RN2483_Status_t RN2483_SetDeviceAddress(char * address, char * receiveBuffer, uint8_t bufferSize){
	sprintf(commandBuffer, "mac set devaddr %s\r\n", address);
	return RN2483_ProcessMacCommand(receiveBuffer, bufferSize, false);
}

RN2483_Status_t RN2483_SetOutputPower(uint8_t pwr, char * receiveBuffer, uint8_t bufferSize){
	sprintf(commandBuffer, "mac set pwridx %i\r\n", pwr);
	return RN2483_ProcessMacCommand(receiveBuffer, bufferSize, false);
}

RN2483_Status_t RN2483_DisableAdaptiveDataRate(char * receiveBuffer, uint8_t bufferSize){
	sprintf(commandBuffer, "mac set adr off\r\n");
	return RN2483_ProcessMacCommand(receiveBuffer, bufferSize, false);
}

RN2483_Status_t RN2483_DisableAutomaticReplies(char * receiveBuffer, uint8_t bufferSize){
	sprintf(commandBuffer, "mac set ar off\r\n");
	return RN2483_ProcessMacCommand(receiveBuffer, bufferSize, false);
}

RN2483_Status_t RN2483_SetDataRate(uint8_t dr, char * receiveBuffer, uint8_t bufferSize){
	sprintf(commandBuffer, "mac set dr %i\r\n", dr);
	return RN2483_ProcessMacCommand(receiveBuffer, bufferSize, false);
}

RN2483_Status_t RN2483_GetDataRate(int8_t *dr, char * receiveBuffer, uint8_t bufferSize){
	sprintf(commandBuffer, "mac get dr\r\n");
	RN2483_Status_t status = RN2483_ProcessMacCommand(receiveBuffer, bufferSize, false);
	if(status != DATA_RETURNED){
		return status;
	}

	// convert ascii to unsigned
	*dr = receiveBuffer[0] - '0';
	// final data check
	if(*dr >= 0 && *dr <= 7){
		return MAC_OK;
	}
	else{
		return MAC_ERR;
	}
}

RN2483_Status_t RN2483_EnableAdaptiveDataRate(char * receiveBuffer, uint8_t bufferSize){
	sprintf(commandBuffer, "mac set adr on\r\n");
	return RN2483_ProcessMacCommand(receiveBuffer, bufferSize, false);
}
RN2483_Status_t RN2483_SetBatteryLevel(uint8_t battery, char * receiveBuffer, uint8_t bufferSize){
	sprintf(commandBuffer, "mac set bat %i\r\n", battery);
	return RN2483_ProcessMacCommand(receiveBuffer, bufferSize, false);
}
RN2483_Status_t RN2483_SaveMac(char * receiveBuffer, uint8_t bufferSize){
	sprintf(commandBuffer, "mac save\r\n");
	return RN2483_ProcessMacCommand(receiveBuffer, bufferSize, false);
}

RN2483_Status_t RN2483_JoinOTAA(char * receiveBuffer, uint8_t bufferSize){
	sprintf(commandBuffer, "mac join otaa\r\n");
	return RN2483_ProcessMacCommand(receiveBuffer, bufferSize, true);
}

RN2483_Status_t RN2483_JoinABP(char * receiveBuffer, uint8_t bufferSize){
	sprintf(commandBuffer, "mac join abp\r\n");
	return RN2483_ProcessMacCommand(receiveBuffer, bufferSize, true);
}

static RN2483_Status_t RN2483_SetDefaultOperation(char * receiveBuffer, uint8_t bufferSize){
	RN2483_Status_t status = RN2483_SetOutputPower(RN2483_POWER_14DBM, receiveBuffer, bufferSize);
	if(status != MAC_OK){
		return status;
	}
	status = RN2483_DisableAutomaticReplies(receiveBuffer, bufferSize);
	if(status != MAC_OK){
		return status;
	}
	status = RN2483_EnableAdaptiveDataRate(receiveBuffer, bufferSize);
	if(status != MAC_OK){
		return status;
	}
	return RN2483_SetBatteryLevel(254, receiveBuffer, bufferSize);
}

RN2483_Status_t RN2483_Setup(LoRaSettings_t settings, char * receiveBuffer, uint8_t bufferSize){
	if(settings.activationMethod == OTAA){
		return RN2483_SetupOTAA(settings, receiveBuffer, bufferSize);
	}else{
		return RN2483_SetupABP(settings, receiveBuffer, bufferSize);
	}
}

RN2483_Status_t RN2483_SetupOTAA(LoRaSettings_t settings, char * receiveBuffer, uint8_t bufferSize){
	RN2483_Status_t status = RN2483_MacReset(receiveBuffer, bufferSize);
	if(status != MAC_OK){
		return status;
	}
	if(settings.deviceEUI == NULL){
		status = RN2483_GetHardwareEUI(receiveBuffer, bufferSize);
		if(status != DATA_RETURNED){
			return status;
		}
		char address[16];
		sprintf(address, "%s", receiveBuffer);
		status = RN2483_SetDeviceEUI(address, receiveBuffer, bufferSize);
		if(status != MAC_OK){
			return status;
		}
	}else{
		status = RN2483_SetDeviceEUI(settings.deviceEUI, receiveBuffer, bufferSize);
		if(status != MAC_OK){
			return status;
		}
	}
	status = RN2483_SetApplicationEUI(settings.applicationEUI, receiveBuffer, bufferSize);
	if(status != MAC_OK){
		return status;
	}
	status = RN2483_SetApplicationKey(settings.applicationKey, receiveBuffer, bufferSize);
	if(status != MAC_OK){
		return status;
	}
	status = RN2483_SetDefaultOperation(receiveBuffer, bufferSize);
	if(status != MAC_OK){
		return status;
	}
	return RN2483_JoinOTAA(receiveBuffer, bufferSize);
}

bool RN2483_SetupABP(LoRaSettings_t settings, char * receiveBuffer, uint8_t bufferSize){
	RN2483_Status_t status = RN2483_MacReset(receiveBuffer, bufferSize);
	if(status != MAC_OK){
		return status;
	}
	status = RN2483_SetDeviceAddress(settings.deviceAddress, receiveBuffer, bufferSize);
	if(status != MAC_OK){
		return status;
	}
	status = RN2483_SetNetworkSessionKey(settings.networkSessionKey, receiveBuffer, bufferSize);
	if(status != MAC_OK){
		return status;
	}
	status = RN2483_SetApplicationSessionKey(settings.applicationSessionKey, receiveBuffer, bufferSize);
	if(status != MAC_OK){
		return status;
	}
	status = RN2483_SetDefaultOperation(receiveBuffer, bufferSize);
	if(status != MAC_OK){
		return status;
	}
	return RN2483_JoinABP(receiveBuffer, bufferSize);
}

RN2483_Status_t RN2483_TransmitUnconfirmed(uint8_t * data, uint8_t payloadSize, char * receiveBuffer, uint8_t bufferSize){
	char * encodedPayload;
	if(!HexToString(data, payloadSize, &encodedPayload )){
		return MAC_ERR;
	}
	//StringToHexString(data, payloadSize/2, &decodedPayload);
	sprintf(commandBuffer, "mac tx uncnf 1 %s\r\n", encodedPayload);
	free(encodedPayload);
	return RN2483_ProcessMacCommand(receiveBuffer, bufferSize, false);
}

RN2483_Status_t RN2483_TransmitConfirmed(uint8_t * data, uint8_t payloadSize, char * receiveBuffer, uint8_t bufferSize){
	char * encodedPayload;
	if(!HexToString(data, payloadSize, &encodedPayload )){
		return MAC_ERR;
	}
	sprintf(commandBuffer, "mac tx cnf 1 %s\r\n", encodedPayload);
	free(encodedPayload);
	return RN2483_ProcessMacCommand(receiveBuffer, bufferSize, false);
}

RN2483_Status_t RN2483_Sleep(uint32_t sleepTime, volatile bool * wakeUp, char * receiveBuffer, uint8_t bufferSize){
	sprintf(commandBuffer, "sys sleep %lu\r\n", (unsigned long) sleepTime);
	return RN2483_ProcessSleepCommand(receiveBuffer, bufferSize, false);
}

RN2483_Status_t RN2483_Wake(char * receiveBuffer, uint8_t bufferSize){
	/*LEUART_Reset(LEUART0);
	GPIO_PinModeSet(gpioPortA, 10, gpioModePushPull, 1);
	Leuart_BreakCondition();
	setupLeuart();

	sprintf(commandBuffer, "U");
	sendLeuartData(commandBuffer, (uint8_t) strlen(commandBuffer));
	Leuart_WaitForResponse(receiveBuffer, bufferSize);
	DelayMs(30);*/
}
