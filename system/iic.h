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
 *         File: iic.h
 *      Created: 2018-03-22
 *       Author: Geoffrey Ottoy
 *      Version: 1.0
 *
 *  Description: TODO
 */

#ifndef SYSTEM_IIC_H_
#define SYSTEM_IIC_H_

#include <stdbool.h>

void IIC_Init(void);
bool IIC_WriteBuffer(uint8_t iicAddress, uint8_t * wBuffer, uint8_t wLength);
bool IIC_ReadBuffer(uint8_t iicAddress, uint8_t regCommand, uint8_t * rBuffer, uint8_t rLength);
bool IIC_WriteReadBuffer(uint8_t iicAddress, uint8_t * wBuffer, uint8_t wLength, uint8_t *rBuffer, uint8_t rLength);

#endif /* SYSTEM_IIC_H_ */
