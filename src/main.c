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

// LoRa Communication
#include "lora.h"
#include "lpp.h"
#include "my_lora_device.h"
// Micro-controller system
#include "system.h"
// Sensors
#include "lis3dh.h"
#include "si7021.h"

bool sleeping;

// Interrupt callback functions
void PB0_Pressed(void){
	if(sleeping){
		//LoRa_WakeUp();
		sleeping = false;
	}
}

void LED_ERROR(void){
	while(1){
		DelayMs(200);
		LED_Toggle();
	}
}

void LED_HALTED(void){
	while(1){
		DelayMs(2000);
		LED_Toggle();
	}
}

int main(void){
	/* Variables */
	LPP_Buffer_t appData;
	uint32_t rhData;
	int32_t tData;
	uint32_t batteryLevel;

	/* Initialize the system hardware */
	System_Init();

	// Now we can do stuff like this:
		// Respond to button press (interrupt-based)
	Buttons_AttachInterrupt(&PB0_Pressed, BUTTON_PB0);
		// Read the battery level
	ADC_Get_Measurement(BATTERY_LEVEL, &batteryLevel); // TODO: check connections & switch setting for battery level measurement
		// Wait (in EM2)
	DelayMs(500);


	/* Initialize sensors */
	// 1. Temperature and relative humidity
	PM_Enable(PM_SENS_GECKO);
	DelayMs(20);
	if(!Si7021_Detect()){
		LED_ERROR();
	}
	//PM_Disble(PM_SENS_GECKO);

	// 2. Accelerometer
	/*PM_Enable(PM_SENS_EXT);
	if(!Lis3dh_Init()){
		LED_ERROR();
	}*/

	/* Initialize LoRa communication */
	LoRaSettings_t loraSettings = LORA_INIT_MY_DEVICE;
	LoRaStatus_t loraStatus =  LoRa_Init(loraSettings);
	if(loraStatus != JOINED){
		LED_ERROR();
	}

	int16_t tempLPP;
	uint8_t humidityLPP;
	int16_t batteryLPP;
	while(1){
		// Measure temperature and relative humidity
		PM_Enable(PM_SENS_GECKO);
		DelayMs(20);
		Si7021_MeasureRHAndTemp(&rhData, &tData);
		PM_Disable(PM_SENS_GECKO);
		// Change readings to LPP format
		tempLPP = (int16_t)(round((float)tData/100));
		humidityLPP = (uint8_t)(rhData/500);

		// Measure battery level
		ADC_Get_Measurement(BATTERY_LEVEL, &batteryLevel);
		// Change reading to LPP format
		batteryLPP = (int16_t)(round((float)batteryLevel*0.09765625));

		if(!LPP_InitBuffer(&appData, 16)){
			LED_ERROR();
		}
		if(!LPP_AddTemperature(&appData, tempLPP)){
			LED_ERROR();
		}
		if(!LPP_AddHumidity(&appData, humidityLPP)){
			LED_ERROR();
		}
		if(!LPP_AddAnalog(&appData, batteryLPP)){
			LED_ERROR();
		}

		if(LoRa_SendLppBuffer(appData, LORA_UNCONFIMED) != SUCCESS){
			LED_ERROR();
		}

		sleeping = true;
		LoRa_Sleep(60000);
	}

}
