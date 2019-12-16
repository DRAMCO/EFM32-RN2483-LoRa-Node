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
 *         File: iic.c
 *      Created: 2018-03-22
 *       Author: Geoffrey Ottoy
 *      Version: 1.0
 *
 *  Description: TODO
 */

#include <i2cspm.h>

#include "iic.h"

I2CSPM_Init_TypeDef i2cInit = I2CSPM_INIT_DEFAULT;

void IIC_Init(void){
	I2CSPM_Init(&i2cInit);
}

bool IIC_WriteBuffer(uint8_t iicAddress, uint8_t * wBuffer, uint8_t wLength){
	I2C_TransferSeq_TypeDef seq;
	I2C_TransferReturn_TypeDef ret;
	uint8_t i2c_read_data[0];

	seq.addr  = iicAddress;
	seq.flags = I2C_FLAG_WRITE;
	/* Point to write buffer (contains command & data) */
	seq.buf[0].data   = wBuffer;
	seq.buf[0].len    = wLength;
	/* Select location/length of data to be read */
	seq.buf[1].data = i2c_read_data;
	seq.buf[1].len  = 0;

	ret = I2CSPM_Transfer(i2cInit.port, &seq);

	if (ret != i2cTransferDone) {
		return false;
	}

	return true;
}

bool IIC_ReadBuffer(uint8_t iicAddress, uint8_t * rBuffer, uint8_t rLength){
	I2C_TransferSeq_TypeDef seq;
	I2C_TransferReturn_TypeDef ret;

	seq.addr  = iicAddress;
	seq.flags = I2C_FLAG_READ;

	/* Select location/length of data to be read */
	seq.buf[0].data = rBuffer;
	seq.buf[0].len  = rLength;

	ret = I2CSPM_Transfer(i2cInit.port, &seq);

	if (ret != i2cTransferDone) {
		*rBuffer = 0;
		return false;
	}
	return true;
}

bool IIC_WriteReadBuffer(uint8_t iicAddress, uint8_t * wBuffer, uint8_t wLength, uint8_t *rBuffer, uint8_t rLength){
	I2C_TransferSeq_TypeDef seq;
	I2C_TransferReturn_TypeDef ret;

	seq.addr  = iicAddress;
	seq.flags = I2C_FLAG_WRITE_READ;
	/* Point to write buffer (contains command & data) */
	seq.buf[0].data   = wBuffer;
	seq.buf[0].len    = wLength;

	/* Select location/length of data to be read */
	seq.buf[1].data = rBuffer;
	seq.buf[1].len  = rLength;

	ret = I2CSPM_Transfer(i2cInit.port, &seq);

	if (ret != i2cTransferDone) {
		*rBuffer = 0;
		return false;
	}

	return true;
}
