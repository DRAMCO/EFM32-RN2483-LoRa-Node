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
#include <stdbool.h>
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

volatile uint8_t errorNr = 0;
volatile bool wakeUp;

typedef enum app_states{
	INIT,
	JOIN,
	MEASURE,
	SEND_MEASUREMENTS,
	SLEEP,
	WAKE_UP
} APP_State_t;

static volatile APP_State_t appState;

// Interrupt callback functions
void PB0_Pressed(void){
	if(appState == SLEEP){
		LED_On();
		wakeUp = true;
	}
}

void LED_ERROR(uint8_t err){
	errorNr = err;
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
	/* Application variables */
	LoRaSettings_t loraSettings = LORA_INIT_MY_DEVICE;
	LoRaStatus_t loraStatus;
	LPP_Buffer_t appData;
	uint32_t rhData;
	int32_t tData;
	uint32_t batteryLevel;

	appState = INIT;

	memset(&appData, 0, sizeof(appData));

	while(1){
		switch(appState){
			case INIT:{
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
					LED_ERROR(1);
				}
				PM_Disable(PM_SENS_GECKO);

				// 2. Accelerometer
				PM_Enable(PM_SENS_EXT);
				if(!Lis3dh_Init()){
					LED_ERROR(8);
				}
				//PM_Disable(PM_SENS_EXT);

				appState = JOIN;
			} break;
			case JOIN:{
				/* Initialize LoRa communication */
				loraStatus =  LoRa_Init(loraSettings);
				if(loraStatus != JOINED){
					LED_ERROR(2);
				}
				appState = MEASURE;
			} break;
			case MEASURE:{
				/* Perform measurements */
				// Measure temperature and relative humidity
				PM_Enable(PM_SENS_GECKO); // turn on sensor supply voltage
				DelayMs(20); // give sensor time to power up
				Si7021_MeasureRHAndTemp(&rhData, &tData);
				PM_Disable(PM_SENS_GECKO); // turn off sensor supply voltage

				// Measure battery level
				ADC_Get_Measurement(BATTERY_LEVEL, &batteryLevel);

				// No send these measurements
				appState = SEND_MEASUREMENTS;
			} break;
			case SEND_MEASUREMENTS:{
				/* Send measurement data to "the cloud" */
				// Convert sensor readings to LPP format
				int16_t tempLPP = (int16_t)(round((float)tData/100));
				uint8_t humidityLPP = (uint8_t)(rhData/500);
				int16_t batteryLPP = (int16_t)(round((float)batteryLevel*0.09765625));

				// Initialize and fill LPP-formatted payload
				if(!LPP_InitBuffer(&appData, 16)){
					LED_ERROR(3);
				}
				if(!LPP_AddTemperature(&appData, tempLPP)){
					LED_ERROR(4);
				}
				if(!LPP_AddHumidity(&appData, humidityLPP)){
					LED_ERROR(5);
				}
				if(!LPP_AddAnalog(&appData, batteryLPP)){
					LED_ERROR(6);
				}

				// Send LPP-formatted payload
				if(LoRa_SendLppBuffer(appData, LORA_UNCONFIMED) != SUCCESS){
					LED_ERROR(7);
				}

				// Go to sleep
				appState = SLEEP;
			} break;
			case SLEEP:{
				// Sleep for a specified period of time;
				wakeUp = false;
				LoRa_Sleep(1800000, &wakeUp);
				if(wakeUp){ // get out of bed early
					appState = WAKE_UP;
				}
				else{ // sleep time has passed
					appState = MEASURE;
				}
			} break;
			case WAKE_UP:{
				LoRa_WakeUp();
				LED_Off();
				appState = MEASURE;
			} break;
			default:{
				LED_HALTED();
			} break;
		}
	}
}
