/**************************************************************************//**
 * @file
 * @brief Empty Project
 * @author Energy Micro AS
 * @version 3.20.2
 ******************************************************************************
 * @section License
 * <b>(C) Copyright 2014 Silicon Labs, http://www.silabs.com</b>
 *******************************************************************************
 *
 * This file is licensed under the Silicon Labs Software License Agreement. See 
 * "http://developer.silabs.com/legal/version/v11/Silicon_Labs_Software_License_Agreement.txt"  
 * for details. Before using this software for any purpose, you must agree to the 
 * terms of that agreement.
 *
 ******************************************************************************/
#include <stdlib.h>
#include "em_device.h"
#include "em_chip.h"
#include "em_cmu.h"
#include "em_emu.h"
#include "em_gpio.h"
#include "em_i2c.h"
#include "em_adc.h"
#include "leuart.h"

#include "rtcdriver.h"

#include "i2cspm.h"

#include "bme280.h"
#include "rn2483.h"
#include "battery.h"
#include "delay.h"

#define BUFFERSIZE 50

int main(void){

	I2CSPM_Init_TypeDef i2cInit = I2CSPM_INIT_DEFAULT;

	char receiveBuffer[BUFFERSIZE];
	memset(receiveBuffer, '\0', BUFFERSIZE);

	char deviceEUI[] = "00E6457CE8B52C56";
	char applicationEUI[] = "70B3D57ED00096E2";
	char applicationKey[] = "B01931C73C3BEA2CC754343AFF3B1962";

	uint8_t messageCounter = 0;

	/*
	char deviceAddress[] = "26011DB5";
	char networkSessionKey[] = "9305CC461CA931F4DA4E16C9339622D0";
	char applicationSessionKey[] = "0A95C8F1FE8BE97ADB71B799B546269A";
	 */

	/* Chip errata */
	CHIP_Init();
	CMU_ClockEnable(cmuClock_GPIO, true);

	InitDelay();
	I2CSPM_Init(&i2cInit);
	Bme280_Init(i2cInit.port);

	adcInit();

	RN2483_Init(receiveBuffer, BUFFERSIZE);

	bool joined = RN2483_SetupOTAA(applicationEUI, applicationKey, deviceEUI, receiveBuffer, BUFFERSIZE);
	//bool joined = RN2483_SetupABP(deviceAddress, applicationSessionKey, networkSessionKey, receiveBuffer, BUFFERSIZE);

	while(1){
		Bme280_TakeForcedMeasurement(i2cInit.port);

		int32_t temperature;
		Bme280_ReadTemperature(i2cInit.port, &temperature);
		int16_t tempLPP = (int16_t) round((float)temperature/10);

		int32_t pressure;
		Bme280_ReadPressure(i2cInit.port, &pressure);
		int16_t pressureLPP = (int16_t) round((float)pressure/10);

		int32_t humidity;
		Bme280_ReadHumidity(i2cInit.port, &humidity);
		uint8_t humidityLPP = (int8_t) (humidity*2/10);

		uint32_t battery = checkBattery()*0.09765625; // 1.25/4096*100/0.3125

		if(joined){
			char payload[13];

			// Temperature
			payload[0] = 0x01;
			payload[1] = 0x67;
			payload[2] = (uint8_t)((tempLPP & 0x0000FF00)>>8);
			payload[3] = (uint8_t)(tempLPP & 0x000000FF);

			//Humidity
			payload[4] = 0x02;
			payload[5] = 0x68;
			payload[6] = humidityLPP ;

			//Pressure
			payload[7] = 0x03;
			payload[8] = 0x73;
			payload[9] = (uint8_t)((pressureLPP & 0x0000FF00)>>8);
			payload[10] = (uint8_t)(pressureLPP & 0x000000FF);

			payload[11] = '\0';

			uint8_t payloadSize = 22;
			if(messageCounter == 0){
				payload[11] = 0x04;
				payload[12] = 0x02;
				payload[13] = (uint8_t)((battery & 0x0000FF00)>>8);
				payload[14] = (uint8_t)(battery & 0x000000FF);
				payload[15] = '\0';
				payloadSize = 30;
			}

			RN2483_TransmitUnconfirmed(payload, payloadSize, receiveBuffer, BUFFERSIZE);

			messageCounter = (messageCounter + 1)%10;
		}
		RN2483_Sleep(1800000, receiveBuffer, BUFFERSIZE);
	}

}
