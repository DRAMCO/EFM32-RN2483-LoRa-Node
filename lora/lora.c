/*  ____  ____      _    __  __  ____ ___
 * |  _ \|  _ \    / \  |  \/  |/ ___/ _ \
 * | | | | |_) |  / _ \ | |\/| | |  | | | |
 * | |_| |  _ <  / ___ \| |  | | |__| |_| |
 * |____/|_| \_\/_/   \_\_|  |_|\____\___/
 *                           research group
 *                             dramco.be/
 *
 *  KU Leuven - Technology Campus Gent,
 *  Gebroeders De Smetstraat 1,
 *  B-9000 Gent, Belgium
 *
 *         File: lora.c
 *      Created: 2018-03-21
 *       Author: Geoffrey Ottoy
 *      Version: 1.0
 *
 *  Description: TODO
 */

#include "system.h"
#include "lora.h"
#include "rn2483.h"

#define LORA_BUFFERSIZE	64

char loraReceiveBuffer[LORA_BUFFERSIZE];

LoRaStatus_t LoRa_Init(LoRaSettings_t init){
	PM_Enable(PM_RN2483);
	RN2483_Init(loraReceiveBuffer, LORA_BUFFERSIZE);

	switch(init.activationMethod){
		case OTAA: {
			if(RN2483_SetupOTAA(init.applicationEUI, init.applicationKey, init.deviceEUI, loraReceiveBuffer, LORA_BUFFERSIZE)){
				return JOINED;
			}
			else{
				return ERROR;
			}
		} break;
		case ABP: {
			if(RN2483_SetupABP(init.deviceAddress, init.applicationSessionKey, init.networkSessionKey, loraReceiveBuffer, LORA_BUFFERSIZE)){
				return JOINED;
			}
			else{
				return ERROR;
			}
		} break;
		default:{
			return ERROR;
		}
	}
}

void LoRa_Sleep(uint32_t durationMs){
	RN2483_Sleep(9900, loraReceiveBuffer, LORA_BUFFERSIZE);
}

void LoRa_DeepSleep(void){
	// power supply stuff
}
