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
#include "delay.h"

#define BUFFERSIZE 50

int main(void){

	I2CSPM_Init_TypeDef i2cInit = I2CSPM_INIT_DEFAULT;

	char transmitBuffer[] = "sys get ver\r\n";
	char receiveBuffer[BUFFERSIZE];
	memset(receiveBuffer, '\0', BUFFERSIZE);

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


	GPIO_PinOutSet(gpioPortC, 2);

	while(1){
		/*char test[] = "sys get ver\r\n";
		USART0_WriteString(test);
		char response [] = "dit is een test dit is een test";
		DelayMs(5);
		USART0_ReadString(response);
		DelayMs(1000);*/

		USART0_SendBuffer(transmitBuffer, 13);
		USART0_ReceiveBuffer(receiveBuffer, BUFFERSIZE);
		DelayMs(50);

		DelayMs(1000);
	}
    /* Infinite loop */
    /*while (1) {
    	Bme280_TakeForcedMeasurement(i2cInit.port);

    	int32_t temperature;
    	Bme280_ReadTemperature(i2cInit.port, &temperature);

    	int32_t pressure;
    	Bme280_ReadPressure(i2cInit.port, &pressure);

    	int32_t humidity;
    	Bme280_ReadHumidity(i2cInit.port, &humidity);

    	DelayMs(500);
    	//Bme280_ReadTemperature(&i2cInit, &temperature);

    }*/
}
