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
 *         File: si7021.c
 *      Created: 2018-03-22
 *       Author: Geoffrey Ottoy
 *      Version: 1.0
 *
 *  Description: TODO
 */

#include <stddef.h>
#include <em_device.h>

#include "iic.h"
#include "si7021.h"

/* Si7021 I2C Address en Device Identifier */
#define SI7021_IIC_ADDRESS		0x40<<1
#define SI7021_DEVICE_ID		0x15
/* Si7021 Read Temperature Command */
#define SI7021_READ_TEMP		0xE0
/* Si7021 Read RH Command */
#define SI7021_READ_RH			0xE5
/* Si7021 Read RH (no hold) Command */
#define SI7021_READ_RH_NH		0xF5
/* Si7021 Read Thermistor Command */
#define SI7021_READ_VIN			0xEE
/* Si7021 Read ID */
#define SI7021_READ_ID1_1		0xFA
#define SI7021_READ_ID1_2		0x0F
#define SI7021_READ_ID2_1		0xFC
#define SI7021_READ_ID2_2		0xC9
/* Si7021 Read Firmware Revision */
#define SI7021_READ_FWREV_1		0x84
#define SI7021_READ_FWREV_2		0xB8

/* Internal function to request measurements from the Si7021
 */
static int32_t Si7021_Measure(uint8_t command, uint32_t *data){
	uint8_t rBuffer[2];
	uint8_t wBuffer[1];

	wBuffer[0] = command;

	if(IIC_WriteReadBuffer(SI7021_IIC_ADDRESS, wBuffer, 1, rBuffer, 2) != true){
		return false;
	}
	/*if(IIC_ReadBuffer(SI7021_IIC_ADDRESS, command, rBuffer, 2)!= true){
		return false;
	}*/

	*data = ((uint32_t) rBuffer[0] << 8) + (rBuffer[1] & 0xFC);

	return true;
}


/* Detect if an Si7021 sensor is present on the I2C bus
 * Returns true, if Si702 is present, else returns false.
 */
bool Si7021_Detect(void){

	uint8_t wBuffer[2] = {SI7021_READ_ID2_1, SI7021_READ_ID2_2};
	uint8_t rBuffer[8];

	if(IIC_WriteReadBuffer(SI7021_IIC_ADDRESS, wBuffer, 2 , rBuffer, 8) != true){
		return false;
	}

	if(SI7021_DEVICE_ID != rBuffer[0]) {
		return false;
	}
	return true;
}


/* Measure temperature and relative humidity using the Si7021.
 * 	temperature is in °C multiplied by 1000
 * 	relative humidity is in % multiplied by 1000
 */
bool Si7021_MeasureRHAndTemp(uint32_t *rhData, int32_t *tData){
	if(Si7021_Measure(SI7021_READ_RH, rhData)){
		*rhData = (((*rhData) * 15625L) >> 13) - 6000;	// convert to milli-percent
	}
	else{
		return false;
	}

	if(Si7021_Measure(SI7021_READ_TEMP, (uint32_t *)tData)){
		*tData = (((*tData) * 21965L) >> 13) - 46850;	// convert to milli-°C
	}
	else{
		return false;
	}

	return true;
}
