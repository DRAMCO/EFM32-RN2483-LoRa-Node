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
 *         File: bme280.h
 *      Created: 2018-02-26
 *       Author: Guus Leenders
 *
 *  Description: Header file for bme280.c
 */
#ifndef _BME280_H_
#define _BME280_H_

#include "em_device.h"
#include <stdbool.h>

#define BME280_ADDRESS     0x76<<1

#define BME280_REG_ID			0xD0
#define BME280_REG_SOFTRESET	0xE0
#define BME280_REG_STATUS		0xF3
#define BME280_REG_VERSION		0xD1
#define BME280_REG_DIGT1		0x88
#define BME280_REG_DIGH2		0xE1
#define BME280_REG_CTRL_HUM		0xF2
#define BME280_REG_CTRL_MEAS	0xF4
#define BME280_REG_CONFIG		0xF5
#define BME280_REG_TEMPDATA		0xFA
#define BME280_REG_PRESDATA 	0xF7
#define BME280_REG_HUMDATA 		0xFD

#define BME280_ID		0x60
#define BME280_OVERSAMPLING1	0x01
#define BME280_OVERSAMPLING4	0x02
#define BME280_FORCEDMODE		0x01
#define BME280_STANDBY_1000		0x05
#define BME280_FILTEROFF		0x00

typedef struct{
	uint16_t dig_T1;
    int16_t  dig_T2;
    int16_t  dig_T3;

    uint16_t dig_P1;
	int16_t  dig_P2;
	int16_t  dig_P3;
	int16_t  dig_P4;
	int16_t  dig_P5;
	int16_t  dig_P6;
	int16_t  dig_P7;
	int16_t  dig_P8;
	int16_t  dig_P9;

	uint8_t  dig_H1;
	int16_t  dig_H2;
	uint8_t  dig_H3;
	int16_t  dig_H4;
	int16_t  dig_H5;
	int8_t   dig_H6;
} Bme280_CalibrationData_TypeDef;

bool Bme280_Init(void);
bool Bme280_IsReadingCalibration(void);
bool Bme280_ReadCoefficients(void);
bool Bme280_SetWeatherSampling(void);
bool Bme280_TakeForcedMeasurement(void);
bool Bme280_ReadTemperature(int32_t *temp);
bool Bme280_ReadPressure(int32_t *pres);
bool Bme280_ReadHumidity(int32_t *hum);


#endif /* _BME280_H_ */
