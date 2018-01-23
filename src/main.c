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
#include <usart.h>
#include "em_device.h"
#include "em_chip.h"
#include "em_cmu.h"
#include "em_gpio.h"
#include "em_i2c.h"
#include "em_usart.h"

#include "i2cspm.h"

#include "bme280.h"
#include "rn2483.h"
#include "delay.h"

#define BUFFERSIZE 50

int main(void){

	I2CSPM_Init_TypeDef i2cInit = I2CSPM_INIT_DEFAULT;

	char receiveBuffer[BUFFERSIZE];
	memset(receiveBuffer, '\0', BUFFERSIZE);

	char deviceEUI[] = "0053AC923819B937";
	char applicationEUI[] = "70B3D57ED00096E2";
	char applicationKey[] = "7C8E68C713058981EDA0AB6A27739D68";

	/*
	char deviceAddress[] = "26011DB5";
	char networkSessionKey[] = "9305CC461CA931F4DA4E16C9339622D0";
	char applicationSessionKey[] = "0A95C8F1FE8BE97ADB71B799B546269A";
	 */

	/* Chip errata */
	CHIP_Init();
	InitDelay();

	//CMU_ClockDivSet(cmuClock_HF, cmuClkDiv_2);       // Set HF clock divider to /2 to keep core frequency < 32MHz
	//CMU_OscillatorEnable(cmuOsc_HFXO, true, true);   // Enable XTAL Osc and wait to stabilize
	//CMU_ClockSelectSet(cmuClock_HF, cmuSelect_HFXO); // Select HF XTAL osc as system clock source. 48MHz XTAL, but we divided the system clock by 2, therefore our HF clock will be 24MHz

	CMU_ClockEnable(cmuClock_GPIO, true);
	GPIO_PinModeSet(gpioPortC, 2, gpioModePushPull, 0);

	I2CSPM_Init(&i2cInit);
	Bme280_Init(i2cInit.port);

	USART0_Setup();

	RN2483_Init();


	GPIO_PinOutSet(gpioPortC, 2);

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
		uint8_t humidityLPP = (int8_t) humidity*2;

		if(joined){
			char payload[13];

			// Temperature
			payload[0] = 0x01;
			payload[1] = 0x67;
			payload[2] = (uint8_t)((tempLPP & 0x0000FF00)>>8);
			payload[3] = (uint8_t)(tempLPP & 0x000000FF);

			//Pressure
			payload[4] = 0x02;
			payload[5] = 0x68;
			payload[6] = humidityLPP ;

			//Pressure
			payload[7] = 0x03;
			payload[8] = 0x73;
			payload[9] = (uint8_t)((pressureLPP & 0x0000FF00)>>8);
			payload[10] = (uint8_t)(pressureLPP & 0x000000FF);

			payload[11] = '\0';
			RN2483_TransmitUnconfirmed(payload, 22, receiveBuffer, BUFFERSIZE);
		}
		RN2483_Sleep(10000, receiveBuffer, BUFFERSIZE);
		DelayMs(10020);
	}
    /* Infinite loop */
    /*while (1) {


    	int32_t pressure;
    	Bme280_ReadPressure(i2cInit.port, &pressure);

    	int32_t humidity;
    	Bme280_ReadHumidity(i2cInit.port, &humidity);

    	DelayMs(500);
    	//Bme280_ReadTemperature(&i2cInit, &temperature);

    }*/
}
