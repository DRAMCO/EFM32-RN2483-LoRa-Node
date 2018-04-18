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
 *         File: rtcdrv_config.h
 *      Created: 2018-03-26
 *       Author: Geoffrey Ottoy
 *
 *  Description: Allows overriding of default settings of the RTC Driver.
 */


#ifndef _RTCDRV_CONFIG_H_
#define _RTCDRV_CONFIG_H_

// Define how many timers RTCDRV provide.
#define EMDRV_RTCDRV_NUM_TIMERS     (2)

// Uncomment the following line to include wallclock functionality.
//#define EMDRV_RTCDRV_WALLCLOCK_CONFIG

// Uncomment the following line to enable integration with SLEEP driver.
//#define EMDRV_RTCDRV_SLEEPDRV_INTEGRATION

// Uncomment the following line to let RTCDRV clock on LFRCO, default is LFXO.
//#define EMDRV_RTCDRV_USE_LFRCO

#endif /* _RTCDRV_CONFIG_H_ */
