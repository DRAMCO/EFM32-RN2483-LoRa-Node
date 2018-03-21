/*
 * lis3dh.c
 *
 *  Created on: 26-feb.-2018
 *      Author: Guus Leenders
 */

#ifndef LIS3DH_C_
#define LIS3DH_C_

#include <stddef.h>
#include "i2cspm.h"
#include "stddef.h"
#include "lis3dh.h"
#include "delay.h"

int32_t Lis3dh_Read8(I2C_TypeDef *i2c, uint8_t reg_addr, uint8_t *data){
  I2C_TransferSeq_TypeDef    seq;
  I2C_TransferReturn_TypeDef ret;
  uint8_t                    i2c_read_data[1];
  uint8_t                    i2c_write_data[1];

  seq.addr  = LIS3DH_ADDR<<1;
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

int32_t Lis3dh_Read16(I2C_TypeDef *i2c, uint8_t reg_addr, uint16_t *data){
  I2C_TransferSeq_TypeDef    seq;
  I2C_TransferReturn_TypeDef ret;
  uint8_t                    i2c_read_data[2];
  uint8_t                    i2c_write_data[1];

  seq.addr  = LIS3DH_ADDR<<1;
  seq.flags = I2C_FLAG_WRITE_READ;
  /* Select command to issue */
  i2c_write_data[0] = reg_addr | 0x80; // 0x80 for autoincrement;
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
	  *data = i2c_read_data[1]<<8 | i2c_read_data[0];
  }
  return (uint32_t) i2cTransferDone;
}

int32_t Lis3dh_Write8(I2C_TypeDef *i2c, uint8_t reg_addr, uint8_t data){
  I2C_TransferSeq_TypeDef    seq;
  I2C_TransferReturn_TypeDef ret;
  uint8_t                    i2c_read_data[1];
  uint8_t                    i2c_write_data[2];

  seq.addr  = LIS3DH_ADDR<<1;
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

bool Lis3dh_Init(I2C_TypeDef *i2c){
	if(Lis3dh_ReadWhoAmI(i2c) != 0x33)
		return false;

	// Enable all axes, low power mode
	Lis3dh_Write8(i2c, LIS3DH_REG_CTRL1, 0xF);

	// Enable BDU, disable high resolution
	Lis3dh_Write8(i2c, LIS3DH_REG_CTRL4, 0x80);

	// Temperature sensor enabled
	Lis3dh_Write8(i2c, LIS3DH_REG_TEMPCFG, 0x40);

	// Set datarate to
	Lis3dh_SetDataRate(i2c, LIS3DH_DATARATE_10_HZ);

	return true;
}


uint8_t Lis3dh_ReadWhoAmI(I2C_TypeDef *i2c){
	uint8_t status = 0;
	Lis3dh_Read8(i2c, LIS3DH_REG_WHOAMI, &status);
	return status;
}

uint8_t Lis3dh_SetDataRate(I2C_TypeDef *i2c, uint8_t dr){
	uint8_t ctl1 = 0;
	Lis3dh_Read8(i2c, LIS3DH_REG_CTRL1, &ctl1);
	ctl1 &= ~(0xF0); // mask off bits
	ctl1 |= (dr << 4);
	return Lis3dh_Write8(i2c, LIS3DH_REG_CTRL1, ctl1);
}

uint8_t Lis3dh_ReadValues(I2C_TypeDef *i2c, uint16_t * x, uint16_t * y, uint16_t * z){
	I2C_TransferSeq_TypeDef    seq;
	I2C_TransferReturn_TypeDef ret;
	uint8_t                    i2c_read_data[6];
	uint8_t                    i2c_write_data[1];

	seq.addr  = LIS3DH_ADDR<<1;
	seq.flags = I2C_FLAG_WRITE_READ;
	/* Select command to issue */
	i2c_write_data[0] = LIS3DH_REG_OUT_X_L | 0x80; // 0x80 for autoincrement
	seq.buf[0].data   = i2c_write_data;
	seq.buf[0].len    = 1;
	/* Select location/length of data to be read */
	seq.buf[1].data = i2c_read_data;
	seq.buf[1].len  = 6;

	ret = I2CSPM_Transfer(i2c, &seq);
	if (ret != i2cTransferDone){
		return ret;
	}
	*x = i2c_read_data[0] | i2c_read_data[1] << 8;
	*y = i2c_read_data[2] | i2c_read_data[3] << 8;
	*z = i2c_read_data[4] | i2c_read_data[5] << 8;

	return (uint32_t) i2cTransferDone;
}

uint8_t Lis3dh_InitShakeDetection(I2C_TypeDef *i2c){
	Lis3dh_Write8(i2c, LIS3DH_REG_CTRL3, 0x40); 	// Enable IA1 to INT1
	Lis3dh_Write8(i2c, LIS3DH_REG_INT1CFG, 0x0A);	// Enable interrupt on Y and X high values
	Lis3dh_Write8(i2c, LIS3DH_REG_INT1SRC, 0x00);	// Clear interrupt sources
	Lis3dh_Write8(i2c, LIS3DH_REG_INT1THS, 0x20);	// Set threshold to 512mg (0x20 * 1LSB, 1LSB = 16mg)
	Lis3dh_Write8(i2c, LIS3DH_REG_INT1DUR, 0x04); 	// Clear minimum duration

	GPIO_PinModeSet(LIS3DH_INT_PORT, LIS3DH_INT_PIN, gpioModeInput, 0);
	GPIO_ExtIntConfig(LIS3DH_INT_PORT, LIS3DH_INT_PIN, LIS3DH_INT_PIN, true, false, true);
	GPIO_IntClear(1	<<	LIS3DH_INT_PIN);	// Clear pending interrupts
	NVIC_EnableIRQ(GPIO_ODD_IRQn);
	return 0;
}

/*uint8_t Lis3dh_ReadTemperature(I2C_TypeDef *i2c, int8_t * temperature){
 	// Not useful, no reference point
	Lis3dh_Write8(i2c, LIS3DH_REG_TEMPCFG, 0xC0);
	DelayMs(200);
	uint16_t adc3 = 2;
	Lis3dh_Read16(i2c, LIS3DH_REG_OUTADC3_L, &adc3);


	uint8_t low = 0;
	Lis3dh_Read8(i2c, LIS3DH_REG_OUTADC3_L, &low);

	uint8_t high = 0;
	Lis3dh_Read8(i2c, LIS3DH_REG_OUTADC3_H, &high);

	uint8_t test = 0;
	return 0;
}*/

#endif /* LIS3DH_C_ */
