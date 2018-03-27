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
 *         File: lora.h
 *      Created: 2018-03-21
 *       Author: Geoffrey Ottoy
 *      Version: 1.0
 *
 *  Description: TODO
 */

#ifndef LORA_LORA_H_
#define LORA_LORA_H_

#define LORA_UNCONFIMED				false
#define LORA_CONFIRMED				true

#define LORA_EUI_LENGTH				16
#define LORA_KEY_LENGTH				32
#define LORA_DEVICE_ADDRESS_LENGTH	8

#include <em_device.h>
#include "lpp.h"

typedef enum lora_statuses{
	JOINED,
	SUCCESS,
	ERROR
} LoRaStatus_t;

typedef enum activation_methods{
	OTAA,
	ABP
} LoRaActivationMethod_t;

typedef struct{
	LoRaActivationMethod_t activationMethod;
	char deviceEUI[LORA_EUI_LENGTH+1];
	char applicationEUI[LORA_EUI_LENGTH+1];
	char applicationKey[LORA_KEY_LENGTH+1];
	char deviceAddress[LORA_DEVICE_ADDRESS_LENGTH+1];
	char networkSessionKey[LORA_KEY_LENGTH+1];
	char applicationSessionKey[LORA_KEY_LENGTH+1];
} LoRaSettings_t;

LoRaStatus_t LoRa_Init(LoRaSettings_t init);

LoRaStatus_t LoRa_SendLppBuffer(LPP_Buffer_t b, bool ackNoAck);

void LoRa_Sleep(uint32_t durationMs, volatile bool * wakeUp);
LoRaStatus_t LoRa_WakeUp(void);

void LoRa_DeepSleep(void);

#endif /* LORA_LORA_H_ */
