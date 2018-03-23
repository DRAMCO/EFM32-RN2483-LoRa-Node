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
 *         File: lis3dh.c
 *      Created: 2018-02-26
 *       Author: Guus Leenders
 *      Version: 1.0
 *
 *  Description: TODO
 */

#include <stddef.h>
#include <em_device.h>

#include "iic.h"
#include "lis3dh.h"

#define LIS3DH_IIC_ADDRESS  (0x18<<1)

#define LIS3DH_REG_STATUS1       0x07
#define LIS3DH_REG_OUTADC1_L     0x08
#define LIS3DH_REG_OUTADC1_H     0x09
#define LIS3DH_REG_OUTADC2_L     0x0A
#define LIS3DH_REG_OUTADC2_H     0x0B
#define LIS3DH_REG_OUTADC3_L     0x0C
#define LIS3DH_REG_OUTADC3_H     0x0D
#define LIS3DH_REG_INTCOUNT      0x0E
#define LIS3DH_REG_WHOAMI        0x0F
#define LIS3DH_REG_TEMPCFG       0x1F
#define LIS3DH_REG_CTRL1         0x20
#define LIS3DH_REG_CTRL2         0x21
#define LIS3DH_REG_CTRL3         0x22
#define LIS3DH_REG_CTRL4         0x23
#define LIS3DH_REG_CTRL5         0x24
#define LIS3DH_REG_CTRL6         0x25
#define LIS3DH_REG_REFERENCE     0x26
#define LIS3DH_REG_STATUS2       0x27
#define LIS3DH_REG_OUT_X_L       0x28
#define LIS3DH_REG_OUT_X_H       0x29
#define LIS3DH_REG_OUT_Y_L       0x2A
#define LIS3DH_REG_OUT_Y_H       0x2B
#define LIS3DH_REG_OUT_Z_L       0x2C
#define LIS3DH_REG_OUT_Z_H       0x2D
#define LIS3DH_REG_FIFOCTRL      0x2E
#define LIS3DH_REG_FIFOSRC       0x2F
#define LIS3DH_REG_INT1CFG       0x30
#define LIS3DH_REG_INT1SRC       0x31
#define LIS3DH_REG_INT1THS       0x32
#define LIS3DH_REG_INT1DUR       0x33
#define LIS3DH_REG_CLICKCFG      0x38
#define LIS3DH_REG_CLICKSRC      0x39
#define LIS3DH_REG_CLICKTHS      0x3A
#define LIS3DH_REG_TIMELIMIT     0x3B
#define LIS3DH_REG_TIMELATENCY   0x3C
#define LIS3DH_REG_TIMEWINDOW    0x3D
#define LIS3DH_REG_ACTTHS        0x3E
#define LIS3DH_REG_ACTDUR        0x3F

#define LIS3DH_WHOAMI_DEFAULT    0x33

typedef enum
{
  LIS3DH_RANGE_16_G     = 0b11,   // +/- 16g
  LIS3DH_RANGE_8_G      = 0b10,   // +/- 8g
  LIS3DH_RANGE_4_G      = 0b01,   // +/- 4g
  LIS3DH_RANGE_2_G      = 0b00    // +/- 2g (default value)
} lis3dh_range_t;

typedef enum
{
  LIS3DH_AXIS_X         = 0x0,
  LIS3DH_AXIS_Y         = 0x1,
  LIS3DH_AXIS_Z         = 0x2,
} lis3dh_axis_t;

/* Used with register 0x2A (LIS3DH_REG_CTRL_REG1) to set bandwidth */
typedef enum
{
  LIS3DH_DATARATE_400_HZ     = 0b0111, //  400Hz
  LIS3DH_DATARATE_200_HZ     = 0b0110, //  200Hz
  LIS3DH_DATARATE_100_HZ     = 0b0101, //  100Hz
  LIS3DH_DATARATE_50_HZ      = 0b0100, //   50Hz
  LIS3DH_DATARATE_25_HZ      = 0b0011, //   25Hz
  LIS3DH_DATARATE_10_HZ      = 0b0010, // 10 Hz
  LIS3DH_DATARATE_1_HZ       = 0b0001, // 1 Hz
  LIS3DH_DATARATE_POWERDOWN  = 0,
  LIS3DH_DATARATE_LOWPOWER_1K6HZ  = 0b1000,
  LIS3DH_DATARATE_LOWPOWER_5KHZ  =  0b1001,

} lis3dh_dataRate_t;

//int32_t Lis3dh_Read8(I2C_TypeDef *i2c, uint8_t reg_addr, uint8_t *data){
//  I2C_TransferSeq_TypeDef    seq;
//  I2C_TransferReturn_TypeDef ret;
//  uint8_t                    i2c_read_data[1];
//  uint8_t                    i2c_write_data[1];
//
//  seq.addr  = LIS3DH_ADDR<<1;
//  seq.flags = I2C_FLAG_WRITE_READ;
//  /* Select command to issue */
//  i2c_write_data[0] = reg_addr;
//  seq.buf[0].data   = i2c_write_data;
//  seq.buf[0].len    = 1;
//  /* Select location/length of data to be read */
//  seq.buf[1].data = i2c_read_data;
//  seq.buf[1].len  = 1;
//
//  ret = I2CSPM_Transfer(i2c, &seq);
//  if (ret != i2cTransferDone){
//    return ret;
//  }
//  if (NULL != data){
//    *data = i2c_read_data[0];
//  }
//  return (uint32_t) i2cTransferDone;
//}

//int32_t Lis3dh_Write8(I2C_TypeDef *i2c, uint8_t reg_addr, uint8_t data){
//  I2C_TransferSeq_TypeDef    seq;
//  I2C_TransferReturn_TypeDef ret;
//  uint8_t                    i2c_read_data[1];
//  uint8_t                    i2c_write_data[2];
//
//  seq.addr  = LIS3DH_ADDR<<1;
//  seq.flags = I2C_FLAG_WRITE;
//  /* Select command to issue */
//  i2c_write_data[0] = reg_addr;
//  i2c_write_data[1] = data;
//  seq.buf[0].data   = i2c_write_data;
//  seq.buf[0].len    = 2;
//  /* Select location/length of data to be read */
//  seq.buf[1].data = i2c_read_data;
//  seq.buf[1].len  = 0;
//
//  ret = I2CSPM_Transfer(i2c, &seq);
//  if (ret != i2cTransferDone){
//    return ret;
//  }
//
//  return (uint32_t) 0;
//}

bool Lis3dh_Init(void){
	if(!Lis3dh_Detect())
		return false;

	// Enable all axes, low power mode
	//Lis3dh_Write8(i2c, LIS3DH_REG_CTRL1, 0xF);

	// Enable BDU, disable high resolution
	//Lis3dh_Write8(i2c, LIS3DH_REG_CTRL4, 0x8);

	// Temperature sensor enabled
	//Lis3dh_Write8(i2c, LIS3DH_REG_TEMPCFG, 0x40);

	// Set datarate to
	//Lis3dh_SetDataRate(i2c, LIS3DH_DATARATE_10_HZ);

	return true;
}


bool Lis3dh_Detect(void){
	uint8_t wBuffer[1] = {LIS3DH_REG_WHOAMI};
	uint8_t rBuffer[1];

	if(IIC_WriteReadBuffer(LIS3DH_IIC_ADDRESS, wBuffer, 1, rBuffer, 1) != true){
		return false;
	}

	if(LIS3DH_WHOAMI_DEFAULT != rBuffer[0]){
		return false;
	}
	return true;
}

//
//uint8_t Lis3dh_SetDataRate(uint8_t dr){
//	uint8_t ctl1 = 0;
//	Lis3dh_Read8(i2c, LIS3DH_REG_CTRL1, &ctl1);
//	ctl1 &= ~(0xF0); // mask off bits
//	ctl1 |= (dr << 4);
//	return Lis3dh_Write8(i2c, LIS3DH_REG_CTRL1, ctl1);
//}
//
//
//uint8_t Lis3dh_ReadValues(void, uint16_t * x, uint16_t * y, uint16_t * z){
//
//
//	I2C_TransferSeq_TypeDef    seq;
//	I2C_TransferReturn_TypeDef ret;
//	uint8_t                    i2c_read_data[6];
//	uint8_t                    i2c_write_data[1];
//
//	seq.addr  = LIS3DH_ADDR<<1;
//	seq.flags = I2C_FLAG_WRITE_READ;
//	/* Select command to issue */
//	i2c_write_data[0] = LIS3DH_REG_OUT_X_L | 0x80; // 0x80 for autoincrement
//	seq.buf[0].data   = i2c_write_data;
//	seq.buf[0].len    = 1;
//	/* Select location/length of data to be read */
//	seq.buf[1].data = i2c_read_data;
//	seq.buf[1].len  = 6;
//
//	ret = I2CSPM_Transfer(i2c, &seq);
//	if (ret != i2cTransferDone){
//		return ret;
//	}
//	*x = i2c_read_data[0] | i2c_read_data[1] << 8;
//	*y = i2c_read_data[2] | i2c_read_data[3] << 8;
//	*z = i2c_read_data[4] | i2c_read_data[5] << 8;
//
//	return (uint32_t) i2cTransferDone;
//
//}
