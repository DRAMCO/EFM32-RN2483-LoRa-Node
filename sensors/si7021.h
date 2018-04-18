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
 *         File: si7021.h
 *      Created: 2018-03-22
 *       Author: Geoffrey Ottoy
 *
 *  Description: Header file for si7021.c
 */

#ifndef _SI7021_H_
#define _SI7021_H_

bool Si7021_Detect(void);
bool Si7021_MeasureRHAndTemp(uint32_t *rhData, int32_t *tData);

#endif /* _SI7021_H_ */
