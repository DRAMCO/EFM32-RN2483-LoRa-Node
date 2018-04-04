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
#include "rn2483.h"
#include "lpp.h"
#include "lora.h"



#define LORA_BUFFERSIZE	64

char loraReceiveBuffer[LORA_BUFFERSIZE];

LoRaStatus_t LoRa_Init(LoRaSettings_t init){
	PM_Enable(PM_RN2483);
	RN2483_Init();

	if(RN2483_Setup(init, loraReceiveBuffer, LORA_BUFFERSIZE) != JOIN_ACCEPTED){
		return ERROR;
	}
	return JOINED;
}

LoRaStatus_t LoRa_SendLppBuffer(LPP_Buffer_t b, bool ackNoAck){
	if(ackNoAck == LORA_CONFIRMED){ // not tested yet !!
		if(RN2483_TransmitConfirmed(b.buffer, b.fill, loraReceiveBuffer, LORA_BUFFERSIZE) != MAC_RX){
			return ERROR;
		}
		return SUCCESS;
	}
	else{
		if(RN2483_TransmitUnconfirmed(b.buffer, b.fill, loraReceiveBuffer, LORA_BUFFERSIZE) != MAC_TX_OK){
			return ERROR;
		}
		return SUCCESS;
	}
}

void LoRa_Sleep(uint32_t durationMs, volatile bool * wakeUp){
	RN2483_Sleep(durationMs, wakeUp, loraReceiveBuffer, LORA_BUFFERSIZE);
}

LoRaStatus_t LoRa_WakeUp(void){
	if(RN2483_Wake(loraReceiveBuffer, LORA_BUFFERSIZE) != MAC_OK){
		return ERROR;
	}
	return SUCCESS;
}


