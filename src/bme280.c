#ifndef BME280_C_
#define BME280_C_

#include <stddef.h>
#include "i2cspm.h"
#include "stddef.h"
#include "bme280.h"
#include "delay.h"

Bme280_CalibrationData_TypeDef bme280_calib;

int32_t Bme280_Read8(I2C_TypeDef *i2c, uint8_t reg_addr, uint8_t *data){
  I2C_TransferSeq_TypeDef    seq;
  I2C_TransferReturn_TypeDef ret;
  uint8_t                    i2c_read_data[1];
  uint8_t                    i2c_write_data[1];

  seq.addr  = BME280_ADDR<<1;
  seq.flags = I2C_FLAG_WRITE_READ;
  /* Select command to issue */
  i2c_write_data[0] = reg_addr;
  seq.buf[0].data   = i2c_write_data;
  seq.buf[0].len    = 1;
  /* Select location/length of data to be read */
  seq.buf[1].data = i2c_read_data;
  seq.buf[1].len  = 1;

  ret = I2CSPM_Transfer(i2c, &seq);
  if (ret != i2cTransferDone){
    return ret;
  }
  if (NULL != data){
    *data = i2c_read_data[0];
  }
  return (uint32_t) i2cTransferDone;
}

int32_t Bme280_Read16(I2C_TypeDef *i2c, uint8_t reg_addr, uint16_t *data){
  I2C_TransferSeq_TypeDef    seq;
  I2C_TransferReturn_TypeDef ret;
  uint8_t                    i2c_read_data[1];
  uint8_t                    i2c_write_data[1];

  seq.addr  = BME280_ADDR<<1;
  seq.flags = I2C_FLAG_WRITE_READ;
  /* Select command to issue */
  i2c_write_data[0] = reg_addr;
  seq.buf[0].data   = i2c_write_data;
  seq.buf[0].len    = 1;
  /* Select location/length of data to be read */
  seq.buf[1].data = i2c_read_data;
  seq.buf[1].len  = 2;

  ret = I2CSPM_Transfer(i2c, &seq);
  if (ret != i2cTransferDone){
    return ret;
  }
  if (NULL != data){
    *data = i2c_read_data[0]<<8 | i2c_read_data[1];
  }
  return (uint32_t) i2cTransferDone;
}

int32_t Bme280_Read24(I2C_TypeDef *i2c, uint8_t reg_addr, uint32_t *data){
  I2C_TransferSeq_TypeDef    seq;
  I2C_TransferReturn_TypeDef ret;
  uint8_t                    i2c_read_data[3];
  uint8_t                    i2c_write_data[1];

  seq.addr  = BME280_ADDR<<1;
  seq.flags = I2C_FLAG_WRITE_READ;
  /* Select command to issue */
  i2c_write_data[0] = reg_addr;
  seq.buf[0].data   = i2c_write_data;
  seq.buf[0].len    = 1;
  /* Select location/length of data to be read */
  seq.buf[1].data = i2c_read_data;
  seq.buf[1].len  = 3;

  ret = I2CSPM_Transfer(i2c, &seq);
  if (ret != i2cTransferDone){
    return ret;
  }
  if (NULL != data){
    *data = i2c_read_data[0]<<16 | i2c_read_data[1]<<8 | i2c_read_data[2];
  }
  return (uint32_t) i2cTransferDone;
}

int32_t Bme280_Write8(I2C_TypeDef *i2c, uint8_t reg_addr, uint8_t data){
  I2C_TransferSeq_TypeDef    seq;
  I2C_TransferReturn_TypeDef ret;
  uint8_t                    i2c_read_data[1];
  uint8_t                    i2c_write_data[2];

  seq.addr  = BME280_ADDR<<1;
  seq.flags = I2C_FLAG_WRITE;
  /* Select command to issue */
  i2c_write_data[0] = reg_addr;
  i2c_write_data[1] = data;
  seq.buf[0].data   = i2c_write_data;
  seq.buf[0].len    = 2;
  /* Select location/length of data to be read */
  seq.buf[1].data = i2c_read_data;
  seq.buf[1].len  = 0;

  ret = I2CSPM_Transfer(i2c, &seq);
  if (ret != i2cTransferDone){
    return ret;
  }

  return (uint32_t) 0;
}

bool Bme280_IsReadingCalibration(I2C_TypeDef *i2c){
	uint8_t status = 0;
	Bme280_Read8(i2c, BME280_REG_STATUS, &status);
	return (status & (1 << 0)) != 0;
}

int32_t Bme280_ReadCoefficients(I2C_TypeDef *i2c){
	I2C_TransferSeq_TypeDef    seq;
	I2C_TransferReturn_TypeDef ret;
	uint8_t                    Bme280_CalibrationRegisters1[25];
	uint8_t                    Bme280_CalibrationRegisters2[8];
	uint8_t                    i2c_write_data[1];

	seq.addr  = BME280_ADDR<<1;
	seq.flags = I2C_FLAG_WRITE_READ;

	/* First calibration register batch */
	/* Select command to issue */
	i2c_write_data[0] = BME280_REG_DIGT1;
	seq.buf[0].data   = i2c_write_data;
	seq.buf[0].len    = 1;
	/* Select location/length of data to be read */
	seq.buf[1].data = Bme280_CalibrationRegisters1;
	seq.buf[1].len  = 24;

	ret = I2CSPM_Transfer(i2c, &seq);
	if (ret != i2cTransferDone){
	  return ret;
	}

	/* Second calibration register batch */
	i2c_write_data[0] = BME280_REG_DIGH2;
	seq.buf[0].data   = i2c_write_data;
	seq.buf[0].len    = 1;
	/* Select location/length of data to be read */
	seq.buf[1].data = Bme280_CalibrationRegisters2;
	seq.buf[1].len  = 8;

	ret = I2CSPM_Transfer(i2c, &seq);
	if (ret != i2cTransferDone){
	  return ret;
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

	bme280_calib.dig_H1 = (uint8_t) (Bme280_CalibrationRegisters1[25]);
	bme280_calib.dig_H2 = (int16_t) (Bme280_CalibrationRegisters2[1] << 8 | Bme280_CalibrationRegisters2[0]);
	bme280_calib.dig_H3 = (uint8_t) (Bme280_CalibrationRegisters2[2]);
	bme280_calib.dig_H4 = (int16_t) ((Bme280_CalibrationRegisters2[3] << 4) | (Bme280_CalibrationRegisters2[5] & 0xF));
	bme280_calib.dig_H5 = (int16_t) (Bme280_CalibrationRegisters2[6] << 4 | Bme280_CalibrationRegisters2[5] >> 4);
	bme280_calib.dig_H6 = (uint8_t) (Bme280_CalibrationRegisters2[7]);

	return (uint32_t) 0;
}

int32_t Bme280_SetWeatherSampling(I2C_TypeDef *i2c){
	Bme280_Write8(i2c, BME280_REG_CTRL_HUM, BME280_OVERSAMPLING1);
	Bme280_Write8(i2c, BME280_REG_CTRL_MEAS, BME280_OVERSAMPLING1 << 5 | BME280_OVERSAMPLING1 << 2 | BME280_FORCEDMODE);
	Bme280_Write8(i2c, BME280_REG_CONFIG, BME280_STANDBY_1000 << 5 | BME280_FILTEROFF);
	return 0;
}

bool Bme280_Init(I2C_TypeDef *i2c){
	uint8_t status = 0;
	Bme280_Read8(i2c, BME280_REG_ID, &status);
	if(status != BME280_ID) return false;

	Bme280_Write8(i2c, BME280_REG_SOFTRESET, 0xB6);
	DelayMs(300);

	while(Bme280_IsReadingCalibration(i2c)){
		DelayMs(100);
	}

	Bme280_ReadCoefficients(i2c);

	Bme280_SetWeatherSampling(i2c);


	return true;
}

int32_t Bme280_TakeForcedMeasurement(I2C_TypeDef *i2c){
	Bme280_Write8(i2c, BME280_REG_CTRL_MEAS, BME280_OVERSAMPLING1 << 5 | BME280_OVERSAMPLING1 << 2 | BME280_FORCEDMODE);

	uint8_t data = 0;
	do{
		Bme280_Read8(i2c, BME280_REG_STATUS, &data);
		DelayMs(1);
	}while(data & 0x08);

	return 0;
}

int32_t Bme280_ReadTemperature(I2C_TypeDef *i2c, int32_t *temp){
	int32_t var1, var2;
	uint32_t adc_T;
	Bme280_Read24(i2c, BME280_REG_TEMPDATA, &adc_T);

	if(adc_T == 0x800000){
		return -1;
	}

	adc_T >>= 4;

	var1 = ((((adc_T>>3) - ((int32_t)bme280_calib.dig_T1 <<1))) *
			((int32_t)bme280_calib.dig_T2)) >> 11;

	var2 = (((((adc_T>>4) - ((int32_t)bme280_calib.dig_T1)) *
			  ((adc_T>>4) - ((int32_t)bme280_calib.dig_T1))) >> 12) *
			((int32_t)bme280_calib.dig_T3)) >> 14;

	int32_t t_fine = var1 + var2;

	*temp = (t_fine * 5 + 128) >> 8;
	return 0;
}

int32_t Bme280_ReadPressure(I2C_TypeDef *i2c, int32_t *pres){
	int64_t var1, var2, p;

	uint32_t adc_UP;
	Bme280_Read24(i2c, BME280_REG_PRESDATA, &adc_UP);

	int32_t adc_P = (int32_t) adc_UP;

	int32_t t_fine;
	Bme280_ReadTemperature(i2c, &t_fine);

	if (adc_P == 0x800000) // value in case pressure measurement was disabled
		return -1;
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
		return 0; // avoid exception caused by division by zero
	}
	p = 1048576 - adc_P;
	p = (((p<<31) - var2)*3125) / var1;
	var1 = (((int64_t)bme280_calib.dig_P9) * (p>>13) * (p>>13)) >> 25;
	var2 = (((int64_t)bme280_calib.dig_P8) * p) >> 19;

	p = ((p + var1 + var2) >> 8) + (((int64_t)bme280_calib.dig_P7)<<4);

	*pres = p/256;
	return 0;
}

int32_t Bme280_ReadHumidity(I2C_TypeDef *i2c, int32_t *hum){
	int32_t t_fine;
	Bme280_ReadTemperature(i2c, &t_fine);

	uint16_t adc_H;
	Bme280_Read16(i2c, BME280_REG_HUMDATA, &adc_H);

	if (adc_H == 0x8000) // value in case humidity measurement was disabled
		return -1;

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

	return 0;
}

#endif /* BME280_C_ */
