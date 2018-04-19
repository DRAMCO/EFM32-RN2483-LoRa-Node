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
 *         File: main.c
 *      Created: 2018-01-18
 *
 *  Description: Main application.
 */

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
#include "bme280.h"



/* LET SENSORS GO TO SLEEP:
 * 	- set to 1 to let sensors sleep
 * 	- set to 0 to not let sensors go to sleep
 */
#define APP_LET_SENSORS_SLEEP 0

// DO NOT TOUCH
// This functionality is adjusted through the above APP_LET_SENSORS_SLEEP parameter
#if APP_LET_SENSORS_SLEEP == 1
	#define SENSORS_OUT_SLEEP() PM_Enable(PM_SENS_GECKO)
	#define SENSORS_TO_SLEEP() 	PM_Disable(PM_SENS_GECKO)
#else
	#define SENSORS_OUT_SLEEP() PM_Enable(PM_SENS_GECKO)
	#define SENSORS_TO_SLEEP() ((void)0)
#endif

#define A_MINUTE 60000

volatile uint8_t errorNr = 0;
volatile bool wakeUp;

typedef enum app_states{
	INIT,
	JOIN,
	MEASURE,
	SEND_MEASUREMENTS,
	SLEEP,
	DEEP_SLEEP,
	WAKE_UP
} APP_State_t;

static volatile APP_State_t appState;

/* Interrupt callback functions */
// Button 1 callback
void PB1_Pressed(void){
	if(appState == SLEEP){
		LED_On();
		wakeUp = true;
	}
}

// Button 0 callback
void PB0_Pressed(void){
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
				Buttons_AttachInterrupt(&PB1_Pressed, BUTTON_PB1);
					// Read the battery level
				ADC_Measure(BATTERY_LEVEL, &batteryLevel);
					// Wait (in EM2)
				DelayMs(500);

				/* Initialize sensors */
				// 1. Temperature and relative humidity
				SENSORS_OUT_SLEEP();
				if(!Si7021_Detect()){
					LED_ERROR(1);
				}
				SENSORS_TO_SLEEP();

				appState = JOIN;
			} break;
			case JOIN:{
				/* Initialize LoRa communication */
				loraStatus = LoRa_Init(loraSettings);
				if(loraStatus != JOINED){
					LED_ERROR(2);
				}
				appState = MEASURE;
			} break;
			case MEASURE:{
				/* Perform measurements */
				// Measure temperature and relative humidity
				SENSORS_OUT_SLEEP();
				Si7021_MeasureRHAndTemp(&rhData, &tData);
				SENSORS_TO_SLEEP();
				// Measure battery level
				ADC_Measure(BATTERY_LEVEL, &batteryLevel);

				// No send these measurements
				appState = SEND_MEASUREMENTS;
			} break;
			case SEND_MEASUREMENTS:{
				/* Send measurement data to "the cloud" */
				// Convert sensor readings to LPP format
				int16_t tempLPP = (int16_t)(round((float)tData/100));
				uint8_t humidityLPP = (uint8_t)(rhData/500);
				int16_t batteryLPP = (int16_t)(round((float)batteryLevel/10));

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
				LoRa_Sleep(5*A_MINUTE, &wakeUp);
				// wake up because of button pressed
				if(wakeUp){ // get out of bed early
					appState = WAKE_UP;
				}
				else{ // sleep time has passed
					appState = MEASURE;
				}
			} break;
			case DEEP_SLEEP:{
				// save lora settings
				Lis3dh_DisableInterruptPin();
				//System_DeepSleep(SENS_EXT_ON);
				System_DeepSleep(NONE_ON);
			} break;
			case WAKE_UP:{
				LoRa_WakeUp();
				LED_Off();
				appState = DEEP_SLEEP;
			} break;
			default:{
				LED_HALTED();
			} break;
		}
	}
}
