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
 *         File: lpp.h
 *      Created: 2018-03-23
 *       Author: Geoffrey Ottoy
 *      Version: 1.0
 *
 *  Description: TODO
 */


#ifndef LORA_LPP_H_
#define LORA_LPP_H_

#include <em_device.h>
#include <stdbool.h>

typedef struct{
	uint8_t * buffer;
	uint8_t fill;
	uint8_t length;
} LPP_Buffer_t;

bool LPP_InitBuffer(LPP_Buffer_t * b, uint8_t size);
void LPP_ClearBuffer(LPP_Buffer_t * b);

bool LPP_AddDigital(LPP_Buffer_t *b, uint8_t data);
bool LPP_AddAnalog(LPP_Buffer_t *b, int16_t data);
bool LPP_AddTemperature(LPP_Buffer_t *b, int16_t data);
bool LPP_AddHumidity(LPP_Buffer_t *b, uint8_t data);
bool LPP_AddAccelerometer(LPP_Buffer_t *b, int16_t x, int16_t y, int16_t z);

#endif /* LORA_LPP_H_ */
