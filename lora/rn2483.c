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

static RN2483_Status_t RN2483_TransmitMacCommand(char * receiveBuffer, uint8_t bufferSize, bool secondResponse){
	bool wakeUp = false;
	if(Leuart_SendCommand(commandBuffer, strlen(commandBuffer), &wakeUp) == TX_TIMEOUT){
		return RN_TX_TIMEOUT;
	}

	Leuart_ReadData(receiveBuffer, bufferSize);
	if(StringStartsWith(receiveBuffer, "ok")){ // OK received
		if(secondResponse){ // second response expected
			// now wait for response
			if(Leuart_WaitForResponse() == RX_TIMEOUT){
				return RN_RX_TIMEOUT;
			}
			Leuart_ReadData(receiveBuffer, bufferSize);
			// analyze response
			if(StringStartsWith(receiveBuffer, "accepted")){
				return JOIN_ACCEPTED;
			}
			else if(StringStartsWith(receiveBuffer, "denied")){
				return JOIN_DENIED;
			}
			else if(StringStartsWith(receiveBuffer, "mac_tx_ok")){
				return MAC_TX_OK;
			}
			else if(StringStartsWith(receiveBuffer, "mac_rx")){ // TODO: get confirmation data
				return MAC_RX;
			}
			else if(StringStartsWith(receiveBuffer, "mac_err")){
				return MAC_ERR;
			}
			else if(StringStartsWith(receiveBuffer, "invalid_data_len")){
				return INVALID_DATA_LEN;
			}
			else{
				return UNKNOWN_ERR;
			}
		}
		else{
			return MAC_OK;
		}
	}
	else{ // check what status we got
		if(StringStartsWith(receiveBuffer, "invalid_param")){
			return INVALID_PARAM;
		}
		else if(StringStartsWith(receiveBuffer, "keys_not_init")){
			return KEYS_NOT_INIT;
		}
		else if(StringStartsWith(receiveBuffer, "not_joined")){
			return NOT_JOINED;
		}
		else if(StringStartsWith(receiveBuffer, "no_free_ch")){
			return NO_FREE_CH;
		}
		else if(StringStartsWith(receiveBuffer, "silent")){
			return SILENT;
		}
		else if(StringStartsWith(receiveBuffer, "frame_counter_err_rejoin_needed")){
			return FRAME_COUNTER_ERR_REJOIN_NEEDED;
		}
		else if(StringStartsWith(receiveBuffer, "busy")){
			return BUSY;
		}
		else if(StringStartsWith(receiveBuffer, "mac_paused")){
			return MAC_PAUSED;
		}
		else if(StringStartsWith(receiveBuffer, "invalid_data_len")){
			return INVALID_DATA_LEN;
		}
		else{
			return DATA_RETURNED;
		}
	}
}

static RN2483_Status_t RN2483_TransmitMacSleepCommand(volatile bool *wakeUp, char * receiveBuffer, uint8_t bufferSize){
	if(Leuart_SendCommand(commandBuffer, strlen(commandBuffer), wakeUp) == TX_TIMEOUT){
		return RN_TX_TIMEOUT;
	}

	Leuart_ReadData(receiveBuffer, bufferSize);
	if(StringStartsWith(receiveBuffer, "ok")){ // OK received
		return MAC_OK;
	}
	else{ // check what status we got
		if(StringStartsWith(receiveBuffer, "invalid_param")){
			return INVALID_PARAM;
		}
		else if(StringStartsWith(receiveBuffer, "keys_not_init")){
			return KEYS_NOT_INIT;
		}
		else if(StringStartsWith(receiveBuffer, "not_joined")){
			return NOT_JOINED;
		}
		else if(StringStartsWith(receiveBuffer, "no_free_ch")){
			return NO_FREE_CH;
		}
		else if(StringStartsWith(receiveBuffer, "silent")){
			return SILENT;
		}
		else if(StringStartsWith(receiveBuffer, "frame_counter_err_rejoin_needed")){
			return FRAME_COUNTER_ERR_REJOIN_NEEDED;
		}
		else if(StringStartsWith(receiveBuffer, "busy")){
			return BUSY;
		}
		else if(StringStartsWith(receiveBuffer, "mac_paused")){
			return MAC_PAUSED;
		}
		else if(StringStartsWith(receiveBuffer, "invalid_data_len")){
			return INVALID_DATA_LEN;
		}
		else{
			return DATA_RETURNED;
		}
	}
}

void RN2483_Init(char * receiveBuffer, uint8_t bufferSize){ // Setup with autobaud
	// Reset RN2483
	GPIO_PinModeSet(gpioPortA, 10, gpioModePushPull, 0);
	DelayMs(20);
	GPIO_PinModeSet(gpioPortA, 10, gpioModePushPull, 1);
	DelayMs(250);

	// Clear command buffer
	memset(commandBuffer, '\0', RN2483_COMMANDBUFFER_SIZE);

	Leuart_Init();
	DelayMs(500);
}

RN2483_Status_t RN2483_Sleep(uint32_t sleepTime, volatile bool * wakeUp, char * receiveBuffer, uint8_t bufferSize){
	sprintf(commandBuffer, "sys sleep %lu\r\n", (unsigned long) sleepTime);
	return RN2483_TransmitMacSleepCommand(wakeUp, receiveBuffer, bufferSize);
}

RN2483_Status_t RN2483_GetSystemVersion(char * receiveBuffer, uint8_t bufferSize){
	sprintf(commandBuffer, "sys get ver\r\n");
	return RN2483_TransmitMacCommand(receiveBuffer, bufferSize, false);
}

RN2483_Status_t RN2483_MacReset(char * receiveBuffer, uint8_t bufferSize){
	sprintf(commandBuffer, "mac reset 868\r\n");
	return RN2483_TransmitMacCommand(receiveBuffer, bufferSize, false);
}

RN2483_Status_t RN2483_GetHardwareEUI(char * receiveBuffer, uint8_t bufferSize){
	sprintf(commandBuffer, "mac get deveui\r\n");
	return RN2483_TransmitMacCommand(receiveBuffer, bufferSize, false);
}

RN2483_Status_t RN2483_GetApplicationEUI(char * receiveBuffer, uint8_t bufferSize){
	sprintf(commandBuffer, "mac get appeui\r\n");
	return RN2483_TransmitMacCommand(receiveBuffer, bufferSize, false);
}

RN2483_Status_t RN2483_SetDeviceEUI(char * eui, char * receiveBuffer, uint8_t bufferSize){
	sprintf(commandBuffer, "mac set deveui %s\r\n", eui);
	return RN2483_TransmitMacCommand(receiveBuffer, bufferSize, false);
}

RN2483_Status_t RN2483_SetApplicationEUI(char * eui, char * receiveBuffer, uint8_t bufferSize){
	sprintf(commandBuffer, "mac set appeui %s\r\n", eui);
	return RN2483_TransmitMacCommand(receiveBuffer, bufferSize, false);
}

RN2483_Status_t RN2483_SetApplicationKey(char * key, char * receiveBuffer, uint8_t bufferSize){
	sprintf(commandBuffer, "mac set appkey %s\r\n", key);
	return RN2483_TransmitMacCommand(receiveBuffer, bufferSize, false);
}

RN2483_Status_t RN2483_SetApplicationSessionKey(char * key, char * receiveBuffer, uint8_t bufferSize){
	sprintf(commandBuffer, "mac set appskey %s\r\n", key);
	return RN2483_TransmitMacCommand(receiveBuffer, bufferSize, false);
}

RN2483_Status_t RN2483_SetNetworkSessionKey(char * key, char * receiveBuffer, uint8_t bufferSize){
	sprintf(commandBuffer, "mac set nwkskey %s\r\n", key);
	return RN2483_TransmitMacCommand(receiveBuffer, bufferSize, false);
}

RN2483_Status_t RN2483_SetDeviceAddress(char * address, char * receiveBuffer, uint8_t bufferSize){
	sprintf(commandBuffer, "mac set devaddr %s\r\n", address);
	return RN2483_TransmitMacCommand(receiveBuffer, bufferSize, false);
}

RN2483_Status_t RN2483_SetOutputPower(uint8_t pwr, char * receiveBuffer, uint8_t bufferSize){
	sprintf(commandBuffer, "mac set pwridx %i\r\n", pwr);
	return RN2483_TransmitMacCommand(receiveBuffer, bufferSize, false);
}

RN2483_Status_t RN2483_SetBatteryLevel(uint8_t battery, char * receiveBuffer, uint8_t bufferSize){
	sprintf(commandBuffer, "mac set bat %i\r\n", battery);
	return RN2483_TransmitMacCommand(receiveBuffer, bufferSize, false);
}

RN2483_Status_t RN2483_DisableAutomaticReplies(char * receiveBuffer, uint8_t bufferSize){
	sprintf(commandBuffer, "mac set ar off\r\n");
	return RN2483_TransmitMacCommand(receiveBuffer, bufferSize, false);
}

RN2483_Status_t RN2483_DisableAdaptiveDataRate(char * receiveBuffer, uint8_t bufferSize){
	sprintf(commandBuffer, "mac set adr off\r\n");
	return RN2483_TransmitMacCommand(receiveBuffer, bufferSize, false);
}

RN2483_Status_t RN2483_EnableAdaptiveDataRate(char * receiveBuffer, uint8_t bufferSize){
	sprintf(commandBuffer, "mac set adr on\r\n");
	return RN2483_TransmitMacCommand(receiveBuffer, bufferSize, false);
}

RN2483_Status_t RN2483_SetDataRate(uint8_t dr, char * receiveBuffer, uint8_t bufferSize){
	sprintf(commandBuffer, "mac set dr %i\r\n", dr);
	return RN2483_TransmitMacCommand(receiveBuffer, bufferSize, false);
}

RN2483_Status_t RN2483_GetDataRate(int8_t * dr, char * receiveBuffer, uint8_t bufferSize){
	sprintf(commandBuffer, "mac get dr\r\n");
	RN2483_Status_t status = RN2483_TransmitMacCommand(receiveBuffer, bufferSize, true);
	// check status of command
	if(status != DATA_RETURNED){
		return MAC_ERR;
	}
	// get dr (ascii to unsigned)
	*dr = receiveBuffer[0] - '0';
	// check dr value
	if(*dr >= 0 && *dr <= 7){
		return MAC_OK;
	}
	else{
		return MAC_ERR;
	}
}

RN2483_Status_t RN2483_SaveMac(char * receiveBuffer, uint8_t bufferSize){
	sprintf(commandBuffer, "mac save\r\n");
	return RN2483_TransmitMacCommand(receiveBuffer, bufferSize, false);
}

RN2483_Status_t RN2483_JoinOTAA(char * receiveBuffer, uint8_t bufferSize){
	sprintf(commandBuffer, "mac join otaa\r\n");
	return RN2483_TransmitMacCommand(receiveBuffer, bufferSize, true);
}

RN2483_Status_t RN2483_JoinABP(char * receiveBuffer, uint8_t bufferSize){
	sprintf(commandBuffer, "mac join abp\r\n");
	return RN2483_TransmitMacCommand(receiveBuffer, bufferSize, true);
}

static RN2483_Status_t RN2483_SetDefaults(char * receiveBuffer, uint8_t bufferSize){
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

RN2483_Status_t RN2483_SetupOTAA(LoRaSettings_t init, char * receiveBuffer, uint8_t bufferSize){
	RN2483_Status_t status = RN2483_MacReset(receiveBuffer, bufferSize);
	if(status != MAC_OK){
		return status;
	}
	if(init.deviceEUI == NULL){
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
		status = RN2483_SetDeviceEUI(init.deviceEUI, receiveBuffer, bufferSize);
		if(status != MAC_OK){
			return status;
		}
	}
	status = RN2483_SetApplicationEUI(init.applicationEUI, receiveBuffer, bufferSize);
	if(status != MAC_OK){
		return status;
	}
	status = RN2483_SetApplicationKey(init.applicationKey, receiveBuffer, bufferSize);
	if(status != MAC_OK){
		return status;
	}
	status = RN2483_SetDefaults(receiveBuffer, bufferSize);
	if(status != MAC_OK){
		return status;
	}
	return RN2483_JoinOTAA(receiveBuffer, bufferSize);
}

RN2483_Status_t RN2483_SetupABP(LoRaSettings_t init, char * receiveBuffer, uint8_t bufferSize){
	RN2483_Status_t status = RN2483_MacReset(receiveBuffer, bufferSize);
	if(status != MAC_OK){
		return status;
	}
	status = RN2483_SetNetworkSessionKey(init.networkSessionKey, receiveBuffer, bufferSize);
	if(status != MAC_OK){
		return status;
	}
	status = RN2483_SetApplicationSessionKey(init.applicationSessionKey, receiveBuffer, bufferSize);
	if(status != MAC_OK){
		return status;
	}
	status = RN2483_SetDeviceAddress(init.deviceAddress, receiveBuffer, bufferSize);
	if(status != MAC_OK){
		return status;
	}
	status = RN2483_SetDefaults(receiveBuffer, bufferSize);
	if(status != MAC_OK){
		return status;
	}
	return RN2483_JoinABP(receiveBuffer, bufferSize);;
}

RN2483_Status_t RN2483_Setup(LoRaSettings_t init, char * receiveBuffer, uint8_t bufferSize){
	if(init.activationMethod == OTAA){
		return RN2483_SetupOTAA(init, receiveBuffer, bufferSize);
	}else{
		return RN2483_SetupABP(init, receiveBuffer, bufferSize);
	}
}

RN2483_Status_t RN2483_TransmitUnconfirmed(uint8_t * data, uint8_t payloadSize, char * receiveBuffer, uint8_t bufferSize){
	char * encodedPayload;
	if(!HexToString(data, payloadSize, &encodedPayload )){
		return TX_FAIL;
	}
	sprintf(commandBuffer, "mac tx uncnf 1 %s\r\n", encodedPayload);
	free(encodedPayload);
	RN2483_Status_t s = RN2483_TransmitMacCommand(receiveBuffer, bufferSize, true);
	return s;
}

RN2483_Status_t RN2483_TransmitConfirmed(uint8_t * data, uint8_t payloadSize, char * receiveBuffer, uint8_t bufferSize){
	char * encodedPayload;
	if(!HexToString(data, payloadSize, &encodedPayload )){
		return TX_FAIL;
	}
	sprintf(commandBuffer, "mac tx cnf 1 %s\r\n", encodedPayload);
	free(encodedPayload);
	return RN2483_TransmitMacCommand(receiveBuffer, bufferSize, true);
}

RN2483_Status_t RN2483_Wake(char * receiveBuffer, uint8_t bufferSize){
	Leuart_Reinit();
	// read "ok" from sleep command
	Leuart_ReadData(receiveBuffer, bufferSize);
	return MAC_OK;
}
