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
 *         File: pm.h
 *      Created: 2018-03-21
 *       Author: Geoffrey Ottoy
 *      Version: 1.0
 *
 *  Description: TODO
 */

#ifndef SYSTEM_PM_H_
#define SYSTEM_PM_H_

typedef enum PM_subsystems{
	PM_SENS_GECKO,
	PM_SENS_EXT,
	PM_RN2483,
	PM_ALL
} PM_SubSystem_t;

void PM_Init(void);

void PM_Enable(PM_SubSystem_t pmss);

void PM_Disable(PM_SubSystem_t pmss);

#endif /* SYSTEM_PM_H_ */
