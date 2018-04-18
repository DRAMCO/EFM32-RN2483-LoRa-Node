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
 *         File: bme280.c
 *      Created: 2018-02-26
 *       Author: Guus Leenders
 *
 *  Description: TODO
 */

#include <stddef.h>
#include <i2cspm.h>

#include "delay.h"
#include "iic.h"
#include "bme280.h"


static Bme280_CalibrationData_TypeDef bme280_calib;

static bool Bme280_ReadTemperatureFine(int32_t *t_fine);

bool Bme280_Init(void){
	uint8_t wBuffer[2] = {BME280_REG_ID,0};
	uint8_t rBuffer[1];
	if(IIC_WriteReadBuffer(BME280_ADDRESS, wBuffer, 1, rBuffer, 1) != true){
		return false;
	}

	if(rBuffer[0] != BME280_ID) return false;

	wBuffer[0] = BME280_REG_SOFTRESET;
	wBuffer[1] = 0xB6;
	if(!IIC_WriteBuffer(BME280_ADDRESS, wBuffer, 2)){
		return false;
	}

	DelayMs(300);

	while(Bme280_IsReadingCalibration()){
		DelayMs(100);
	}

	Bme280_ReadCoefficients();

	Bme280_SetWeatherSampling();

	return true;
}

bool Bme280_IsReadingCalibration(void){
	uint8_t status = 0;

	uint8_t wBuffer[1] = {BME280_REG_STATUS};
	uint8_t rBuffer[1];
	if(IIC_WriteReadBuffer(BME280_ADDRESS, wBuffer, 1, rBuffer, 1) != true){
		return false;
	}

	return (status & (1 << 0)) != 0;
}

bool Bme280_ReadCoefficients(void){
	uint8_t Bme280_CalibrationRegisters1[25];
	uint8_t Bme280_CalibrationRegisters2[8];

	uint8_t wBuffer[1] = {BME280_REG_DIGT1};
	if(IIC_WriteReadBuffer(BME280_ADDRESS, wBuffer, 1, Bme280_CalibrationRegisters1, 25) != true){
		return false;
	}

	wBuffer[0] = BME280_REG_DIGH2;
	if(IIC_WriteReadBuffer(BME280_ADDRESS, wBuffer, 1, Bme280_CalibrationRegisters2, 8) != true){
		return false;
	}

	bme280_calib.dig_T1 = (uint16_t) (Bme280_CalibrationRegisters1[1] << 8 | Bme280_CalibrationRegisters1[0]);
	bme280_calib.dig_T2 = (int16_t) (Bme280_CalibrationRegisters1[3] << 8 | Bme280_CalibrationRegisters1[2]);
	bme280_calib.dig_T3 = (int16_t) (Bme280_CalibrationRegisters1[5] << 8 | Bme280_CalibrationRegisters1[4]);

	bme280_calib.dig_P1 = (uint16_t) (Bme280_CalibrationRegisters1[7] << 8 | Bme280_CalibrationRegisters1[6]);
	bme280_calib.dig_P2 = (int16_t) (Bme280_CalibrationRegisters1[9] << 8 | Bme280_CalibrationRegisters1[8]);
	bme280_calib.dig_P3 = (int16_t) (Bme280_CalibrationRegisters1[11] << 8 | Bme280_CalibrationRegisters1[10]);
	bme280_calib.dig_P4 = (int16_t) (Bme280_CalibrationRegisters1[13] << 8 | Bme280_CalibrationRegisters1[12]);
	bme280_calib.dig_P5 = (int16_t) (Bme280_CalibrationRegisters1[15] << 8 | Bme280_CalibrationRegisters1[14]);
	bme280_calib.dig_P6 = (int16_t) (Bme280_CalibrationRegisters1[17] << 8 | Bme280_CalibrationRegisters1[16]);
	bme280_calib.dig_P7 = (int16_t) (Bme280_CalibrationRegisters1[19] << 8 | Bme280_CalibrationRegisters1[18]);
	bme280_calib.dig_P8 = (int16_t) (Bme280_CalibrationRegisters1[21] << 8 | Bme280_CalibrationRegisters1[20]);
	bme280_calib.dig_P9 = (int16_t) (Bme280_CalibrationRegisters1[23] << 8 | Bme280_CalibrationRegisters1[22]);

	bme280_calib.dig_H1 = (uint8_t) (Bme280_CalibrationRegisters1[24]);
	bme280_calib.dig_H2 = (int16_t) (Bme280_CalibrationRegisters2[1] << 8 | Bme280_CalibrationRegisters2[0]);
	bme280_calib.dig_H3 = (uint8_t) (Bme280_CalibrationRegisters2[2]);
	bme280_calib.dig_H4 = (int16_t) ((Bme280_CalibrationRegisters2[3] << 4) | (Bme280_CalibrationRegisters2[5] & 0xF));
	bme280_calib.dig_H5 = (int16_t) (Bme280_CalibrationRegisters2[6] << 4 | Bme280_CalibrationRegisters2[5] >> 4);
	bme280_calib.dig_H6 = (int8_t) (Bme280_CalibrationRegisters2[7]);

	return true;
}

bool Bme280_SetWeatherSampling(void){
	uint8_t wBuffer[2] = {BME280_REG_CTRL_HUM, BME280_OVERSAMPLING1};
	if(!IIC_WriteBuffer(BME280_ADDRESS, wBuffer, 2)){
		return false;
	}

	wBuffer[0] = BME280_REG_CTRL_MEAS;
	wBuffer[1] = BME280_OVERSAMPLING1 << 5 | BME280_OVERSAMPLING1 << 2 | BME280_FORCEDMODE;
	if(!IIC_WriteBuffer(BME280_ADDRESS, wBuffer, 2)){
		return false;
	}

	wBuffer[0] = BME280_REG_CONFIG;
	wBuffer[1] = BME280_STANDBY_1000 << 5 | BME280_FILTEROFF;
	if(!IIC_WriteBuffer(BME280_ADDRESS, wBuffer, 2)){
		return false;
	}

	return true;
}

bool Bme280_TakeForcedMeasurement(void){
	uint8_t wBuffer[2] = {BME280_REG_CTRL_MEAS, BME280_OVERSAMPLING1 << 5 | BME280_OVERSAMPLING4 << 2 | BME280_FORCEDMODE};
	if(IIC_WriteBuffer(BME280_ADDRESS, wBuffer, 1) != true){
		return false;
	}

	uint8_t rBuffer[1] = {0};
	do{
		uint8_t wBuffer[1] = {BME280_REG_STATUS};
		if(!IIC_WriteReadBuffer(BME280_ADDRESS, wBuffer, 1, rBuffer, 1)){
			return false;
		}

		DelayMs(1);
	} while(rBuffer[0] & 0x08);

	return true;
}

static bool Bme280_ReadTemperatureFine(int32_t *t_fine){
	int32_t var1, var2;

	uint8_t wBuffer[1] = {BME280_REG_TEMPDATA};
	uint8_t rBuffer[3];
	if(IIC_WriteReadBuffer(BME280_ADDRESS, wBuffer, 1, rBuffer, 3) != true){
		return false;
	}

	int32_t adc_T = rBuffer[0]<<16 | rBuffer[1]<<8 | rBuffer[2];

	if(adc_T == 0x800000){
		return false;
	}

	adc_T >>= 4;

	var1 = ((((adc_T>>3) - ((int32_t)bme280_calib.dig_T1 <<1))) *
			((int32_t)bme280_calib.dig_T2)) >> 11;

	var2 = (((((adc_T>>4) - ((int32_t)bme280_calib.dig_T1)) *
			  ((adc_T>>4) - ((int32_t)bme280_calib.dig_T1))) >> 12) *
			((int32_t)bme280_calib.dig_T3)) >> 14;

	*t_fine = var1 + var2;

	return true;
}

bool Bme280_ReadTemperature(int32_t *temp){
	int32_t t_fine = 0;
	if(!Bme280_ReadTemperatureFine(&t_fine)){
		return false;
	}
	*temp = (t_fine * 5 + 128) >> 8;
	return true;
}

bool Bme280_ReadPressure(int32_t *pres){
	int64_t var1, var2, p;

	uint8_t wBuffer[1] = {BME280_REG_PRESDATA};
	uint8_t rBuffer[3];
	if(IIC_WriteReadBuffer(BME280_ADDRESS, wBuffer, 1, rBuffer, 3) != true){
		return false;
	}

	int32_t adc_P = rBuffer[0]<<16 | rBuffer[1]<<8 | rBuffer[2];

	int32_t t_fine = 0;
	if(!Bme280_ReadTemperatureFine(&t_fine)){
		return false;
	}

	if (adc_P == 0x800000){ // value in case pressure measurement was disabled
		return false;
	}
	adc_P >>= 4;

	var1 = ((int64_t)t_fine) - 128000;
	var2 = var1 * var1 * (int64_t)bme280_calib.dig_P6;
	var2 = var2 + ((var1*(int64_t)bme280_calib.dig_P5)<<17);
	var2 = var2 + (((int64_t)bme280_calib.dig_P4)<<35);
	var1 = ((var1 * var1 * (int64_t)bme280_calib.dig_P3)>>8) +
		   ((var1 * (int64_t)bme280_calib.dig_P2)<<12);
	var1 = (((((int64_t)1)<<47)+var1))*((int64_t)bme280_calib.dig_P1)>>33;

	if (var1 == 0) {
		*pres = 0;
		return true; // avoid exception caused by division by zero
	}
	p = 1048576 - adc_P;
	p = (((p<<31) - var2)*3125) / var1;
	var1 = (((int64_t)bme280_calib.dig_P9) * (p>>13) * (p>>13)) >> 25;
	var2 = (((int64_t)bme280_calib.dig_P8) * p) >> 19;

	p = ((p + var1 + var2) >> 8) + (((int64_t)bme280_calib.dig_P7)<<4);

	*pres = (int32_t) ((float)p/256.0);
	return true;
}

bool Bme280_ReadHumidity(int32_t *hum){
	int32_t t_fine = 0;
	Bme280_ReadTemperatureFine(&t_fine);

	uint8_t wBuffer[1] = {BME280_REG_HUMDATA};
	uint8_t rBuffer[2];
	if(IIC_WriteReadBuffer(BME280_ADDRESS, wBuffer, 1, rBuffer, 2) != true){
		return false;
	}

	uint16_t adc_H = rBuffer[0]<<8 | rBuffer[1];

	if (adc_H == 0x8000){ // value in case humidity measurement was disabled
		return false;
	}

	int32_t v_x1_u32r;

	v_x1_u32r = (t_fine - ((int32_t)76800));

	v_x1_u32r = (((((adc_H << 14) - (((int32_t)bme280_calib.dig_H4) << 20) -
					(((int32_t)bme280_calib.dig_H5) * v_x1_u32r)) + ((int32_t)16384)) >> 15) *
				 (((((((v_x1_u32r * ((int32_t)bme280_calib.dig_H6)) >> 10) *
					  (((v_x1_u32r * ((int32_t)bme280_calib.dig_H3)) >> 11) + ((int32_t)32768))) >> 10) +
					((int32_t)2097152)) * ((int32_t)bme280_calib.dig_H2) + 8192) >> 14));

	v_x1_u32r = (v_x1_u32r - (((((v_x1_u32r >> 15) * (v_x1_u32r >> 15)) >> 7) *
							   ((int32_t)bme280_calib.dig_H1)) >> 4));

	v_x1_u32r = (v_x1_u32r < 0) ? 0 : v_x1_u32r;
	v_x1_u32r = (v_x1_u32r > 419430400) ? 419430400 : v_x1_u32r;
	int32_t h = (v_x1_u32r>>12);
	*hum =  h / 102.40;

	return true;
}

