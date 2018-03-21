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

//
#include "lora.h"
#include "my_lora_device.h"
//
#include "system.h"
//
#include "bme280.h"
#include "lis3dh.h"

#include "battery.h"



void DoSomething(){
	LED_Toggle();
}

int main(void){
	/* Chip errata */
	System_Init();

	// Sensors Init (don't forget power supply stuff)


	Buttons_AttachInterrupt(&DoSomething, BUTTON_PB0);



	DelayMs(500);


	/* Initialize LoRa communication */
	LoRaSettings_t loraSettings = LORA_INIT_MY_DEVICE;
	LoRaStatus_t loraStatus =  LoRa_Init(loraSettings);
	if(loraStatus == JOINED){
		while(1){
			DelayMs(200);
			LED_Toggle();
		}
	}
	else{
		while(1){
			DelayMs(2000);
			LED_Toggle();
		}
	}


	//I2CSPM_Init_TypeDef i2cInit = I2CSPM_INIT_DEFAULT;



	//uint8_t messageCounter = 0;


	//I2CSPM_Init(&i2cInit);

	//GPIO_PinModeSet(gpioPortE, 10, gpioModePushPull, 1);
	//GPIO_PinModeSet(gpioPortE, 11, gpioModePushPull, 0);

	/*Lis3dh_Init(i2cInit.port);

	uint8_t id = Lis3dh_ReadWhoAmI(i2cInit.port);


	uint16_t x = 0;
	uint16_t y = 0;
	uint16_t z = 0;

	Lis3dh_ReadValues(i2cInit.port, &x, &y, &z);
	uint8_t blabla = 0;*/
	//Bme280_Init(i2cInit.port);


	adcInit();

	//RN2483_Init(receiveBuffer, BUFFERSIZE);

	DelayMs(500);

	//bool joined = RN2483_SetupOTAA(applicationEUI, applicationKey, deviceEUI, receiveBuffer, BUFFERSIZE);
	//bool joined = RN2483_SetupABP(deviceAddress, applicationSessionKey, networkSessionKey, receiveBuffer, BUFFERSIZE);

	while(1){
		/*Bme280_TakeForcedMeasurement(i2cInit.port);

		int32_t temperature;
		Bme280_ReadTemperature(i2cInit.port, &temperature);
		int16_t tempLPP = (int16_t) round((float)temperature/10);

		int32_t pressure;
		Bme280_ReadPressure(i2cInit.port, &pressure);
		int16_t pressureLPP = (int16_t) round((float)pressure/10);

		int32_t humidity;
		Bme280_ReadHumidity(i2cInit.port, &humidity);
		uint8_t humidityLPP = (int8_t) (humidity*2/10);*/

		/*int16_t tempLPP = 1;
		int16_t pressureLPP = 2;
		uint8_t humidityLPP = 3;

		uint32_t battery = checkBattery()*0.09765625; // 1.25/4096*100/0.3125

		if(joined){
			char payload[16];

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
		RN2483_Sleep(1800000, receiveBuffer, BUFFERSIZE);*/
	}

}
