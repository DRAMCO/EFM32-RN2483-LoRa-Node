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
#include "irq.h"
#include "lis3dh.h"
#include "pin_mapping.h"

#define LIS3DH_IIC_ADDRESS  (0x19<<1)

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

bool Lis3dh_Init(void){
	if(!Lis3dh_Detect()){
		return false;
	}

	// Enable all axes, low power mode
	uint8_t wBuffer[2] = {LIS3DH_REG_CTRL1, 0x0F};
	if(!IIC_WriteBuffer(LIS3DH_IIC_ADDRESS, wBuffer, 2)){
		return false;
	}

	// Enable BDU, disable high resolution
	wBuffer[0] = LIS3DH_REG_CTRL4;
	wBuffer[1] = 0x08;
	if(!IIC_WriteBuffer(LIS3DH_IIC_ADDRESS, wBuffer, 2)){
		return false;
	}

	// Set datarate to
	return Lis3dh_SetDataRate(LIS3DH_DATARATE_10_HZ);
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


bool Lis3dh_SetDataRate(uint8_t dr){
	uint8_t wBuffer[2] = {LIS3DH_REG_CTRL1, 0x00};
	uint8_t rBuffer[1];

	if(IIC_WriteReadBuffer(LIS3DH_IIC_ADDRESS, wBuffer, 1, rBuffer, 1) != true){
		return false;
	}

	uint8_t ctl1 = rBuffer[0];
	ctl1 &= (0x0F); // mask off bits
	ctl1 |= (dr << 4);
	wBuffer[1] = ctl1;
	if(IIC_WriteBuffer(LIS3DH_IIC_ADDRESS, wBuffer, 2) != true){
		return false;
	}
	IIC_WriteReadBuffer(LIS3DH_IIC_ADDRESS, wBuffer, 1, rBuffer, 1);

	return true;
}

bool Lis3dh_InitShakeDetection(void){
	// Enable IA1 to INT1
	uint8_t wBuffer[2] = {LIS3DH_REG_CTRL3, 0x40};
	if(!IIC_WriteBuffer(LIS3DH_IIC_ADDRESS, wBuffer, 2)){
		return false;
	}
	// Enable interrupt on Y and X high values
	wBuffer[0] = LIS3DH_REG_INT1CFG;
	wBuffer[1] = 0x0A;
	if(!IIC_WriteBuffer(LIS3DH_IIC_ADDRESS, wBuffer, 2)){
		return false;
	}
	// Clear interrupt sources
	wBuffer[0] = LIS3DH_REG_INT1SRC;
	wBuffer[1] = 0x00;
	if(!IIC_WriteBuffer(LIS3DH_IIC_ADDRESS, wBuffer, 2)){
		return false;
	}
	// Set threshold to 512mg (0x20 * 1LSB, 1LSB = 16mg)
	wBuffer[0] = LIS3DH_REG_INT1THS;
	wBuffer[1] = 0x20;
	if(!IIC_WriteBuffer(LIS3DH_IIC_ADDRESS, wBuffer, 2)){
		return false;
	}
	// Clear minimum duration
	wBuffer[0] = LIS3DH_REG_INT1DUR;
	wBuffer[1] = 0x04;
	if(!IIC_WriteBuffer(LIS3DH_IIC_ADDRESS, wBuffer, 2)){
		return false;
	}

	GPIO_PinModeSet(LIS3DH_INT_PORT, LIS3DH_INT_PIN, gpioModeInput, 0);
	GPIO_ExtIntConfig(LIS3DH_INT_PORT, LIS3DH_INT_PIN, LIS3DH_INT_PIN, false, true, true);
	GPIO_IntClear(1	<< LIS3DH_INT_PIN);	// Clear pending interrupts

	return true;
}

bool Lis3dh_ReadValues(uint16_t * x, uint16_t * y, uint16_t * z){
	uint8_t rBuffer[6];
	uint8_t wBuffer[1] = {LIS3DH_REG_OUT_X_L | 0x80}; // 0x80 for autoincrement

	if(IIC_WriteReadBuffer(LIS3DH_IIC_ADDRESS, wBuffer, 1, rBuffer, 6) != true){
		return false;
	}

	*x = rBuffer[0] | rBuffer[1] << 8;
	*y = rBuffer[2] | rBuffer[3] << 8;
	*z = rBuffer[4] | rBuffer[5] << 8;

	return true;
}

void Lis3dh_DisableInterruptPin(void){
	GPIO_ExtIntConfig(LIS3DH_INT_PORT, LIS3DH_INT_PIN, LIS3DH_INT_PIN, false, true, false);
	GPIO_IntClear(1	<< LIS3DH_INT_PIN);	// Clear pending interrupts
	GPIO_PinModeSet(LIS3DH_INT_PORT, LIS3DH_INT_PIN, gpioModePushPull, 0);
}

void Lis3dh_AttachInterrupt(void * cb){
	IRQ_AttachInterrupt(cb, LIS3DH_INT_PIN);
}
