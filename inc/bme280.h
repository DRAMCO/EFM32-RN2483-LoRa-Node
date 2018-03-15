/*
 * bme280.h
 *
 *  Created on: 14-dec.-2017
 *      Author: Guus Leenders
 */

#ifndef BME280_H_
#define BME280_H_

#include "em_device.h"
#include <stdbool.h>

#ifdef __cplusplus
extern "C" {
#endif

#define BME280_ADDR     0x76

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


int32_t Bme280_Read8(I2C_TypeDef *i2c, uint8_t addr, uint8_t *data);
int32_t Bme280_Read16(I2C_TypeDef *i2c, uint8_t addr, uint16_t *data);
int32_t Bme280_Read24(I2C_TypeDef *i2c, uint8_t addr, uint32_t *data);

int32_t Bme280_Write8(I2C_TypeDef *i2c, uint8_t reg_addr, uint8_t data);

bool Bme280_IsReadingCalibration(I2C_TypeDef *i2c);
int32_t Bme280_ReadCoefficients(I2C_TypeDef *i2c);
int32_t Bme280_SetWeatherSampling(I2C_TypeDef *i2c);
int32_t Bme280_TakeForcedMeasurement(I2C_TypeDef *i2c);
int32_t Bme280_ReadTemperature(I2C_TypeDef *i2c, int32_t *temp);
int32_t Bme280_ReadTemperatureFine(I2C_TypeDef *i2c, int32_t *t_fine);
int32_t Bme280_ReadPressure(I2C_TypeDef *i2c, int32_t *pres);
int32_t Bme280_ReadHumidity(I2C_TypeDef *i2c, int32_t *hum);

bool Bme280_Init(I2C_TypeDef *i2c);

#endif /* BME280_H_ */
