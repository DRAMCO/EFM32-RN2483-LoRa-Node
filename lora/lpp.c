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
 *         File: lpp.c
 *      Created: 2018-03-23
 *       Author: Geoffrey Ottoy
 *
 *  Description: Basic Low Power Payload (LPP) functionality.
 */

#include <stdlib.h>
#include <stdbool.h>

#include "lpp.h"

// lpp types
#define LPP_DIGITAL_INPUT			0x00
#define LPP_ANALOG_INPUT			0x02
#define LPP_TEMPERATURE				0x67
#define LPP_HUMIDITY				0x68
#define LPP_ACCELEROMETER			0x71
#define LPP_PRESSURE				0x73

// lpp data sizes
#define LPP_DIGITAL_INPUT_SIZE		0x03
#define LPP_ANALOG_INPUT_SIZE		0x04
#define LPP_TEMPERATURE_SIZE		0x04
#define LPP_HUMIDITY_SIZE			0x03
#define LPP_ACCELEROMETER_SIZE		0x08
#define LPP_PRESSURE_SIZE			0x04

// lpp channel ids
#define LPP_DIGITAL_INPUT_CHANNEL	0x01
#define LPP_ANALOG_INPUT_CHANNEL	0x02
#define LPP_TEMPERATURE_CHANNEL		0x03
#define LPP_HUMIDITY_CHANNEL		0x04
#define LPP_ACCELEROMETER_CHANNEL	0x05
#define LPP_PRESSURE_CHANNEL		0x06

bool LPP_InitBuffer(LPP_Buffer_t *b, uint8_t size){
	LPP_ClearBuffer(b);

	b->buffer = (uint8_t *) malloc(sizeof(uint8_t) * size);

	if(b->buffer != NULL){
		b->fill = 0;
		b->length = size;
		return true;
	}

	return false;
}

void LPP_ClearBuffer(LPP_Buffer_t *b){
	if(b->buffer != NULL){
		free(b->buffer);
	}
}

bool LPP_AddDigital(LPP_Buffer_t *b, uint8_t data){
	uint8_t space = b->length - b->fill;
	if(space < LPP_DIGITAL_INPUT_SIZE){
		return false;
	}

	b->buffer[b->fill++] = LPP_DIGITAL_INPUT_CHANNEL;
	b->buffer[b->fill++] = LPP_DIGITAL_INPUT;
	b->buffer[b->fill++] = data;

	return true;
}

bool LPP_AddAnalog(LPP_Buffer_t *b, int16_t data){
	uint8_t space = b->length - b->fill;
	if(space < LPP_ANALOG_INPUT_SIZE){
		return false;
	}

	b->buffer[b->fill++] = LPP_ANALOG_INPUT_CHANNEL;
	b->buffer[b->fill++] = LPP_ANALOG_INPUT;
	b->buffer[b->fill++] = (uint8_t)((0xFF00 & data) >> 8);
	b->buffer[b->fill++] = (uint8_t)(0x00FF & data);

	return true;
}

bool LPP_AddTemperature(LPP_Buffer_t *b, int16_t data){
	uint8_t space = b->length - b->fill;
	if(space < LPP_TEMPERATURE_SIZE){
		return false;
	}

	b->buffer[b->fill++] = LPP_TEMPERATURE_CHANNEL;
	b->buffer[b->fill++] = LPP_TEMPERATURE;
	b->buffer[b->fill++] = (uint8_t)((0xFF00 & data) >> 8);
	b->buffer[b->fill++] = (uint8_t)(0x00FF & data);

	return true;
}

bool LPP_AddHumidity(LPP_Buffer_t *b, uint8_t data){
	uint8_t space = b->length - b->fill;
	if(space < LPP_HUMIDITY_SIZE){
		return false;
	}

	b->buffer[b->fill++] = LPP_HUMIDITY_CHANNEL;
	b->buffer[b->fill++] = LPP_HUMIDITY;
	b->buffer[b->fill++] = data;

	return true;
}

bool LPP_AddAccelerometer(LPP_Buffer_t *b, int16_t x, int16_t y, int16_t z){
	uint8_t space = b->length - b->fill;
	if(space < LPP_ACCELEROMETER_SIZE){
		return false;
	}

	b->buffer[b->fill++] = LPP_ACCELEROMETER_CHANNEL;
	b->buffer[b->fill++] = LPP_ACCELEROMETER;
	b->buffer[b->fill++] = (uint8_t)((0xFF00 & x) >> 8);
	b->buffer[b->fill++] = (uint8_t)(0x00FF & x);
	b->buffer[b->fill++] = (uint8_t)((0xFF00 & y) >> 8);
	b->buffer[b->fill++] = (uint8_t)(0x00FF & y);
	b->buffer[b->fill++] = (uint8_t)((0xFF00 & z) >> 8);
	b->buffer[b->fill++] = (uint8_t)(0x00FF & z);

	return true;
}

bool LPP_AddPressure(LPP_Buffer_t *b, uint16_t data){
	uint8_t space = b->length - b->fill;
	if(space < LPP_PRESSURE_SIZE){
		return false;
	}

	b->buffer[b->fill++] = LPP_PRESSURE_CHANNEL;
	b->buffer[b->fill++] = LPP_PRESSURE;
	b->buffer[b->fill++] = (uint8_t)((0xFF00 & data) >> 8);
	b->buffer[b->fill++] = (uint8_t)(0x00FF & data);

	return true;
}
