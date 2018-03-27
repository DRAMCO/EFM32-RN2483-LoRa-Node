/*
 * rtcdrv_config.h
 *
 *  Created on: 26-mrt.-2018
 *      Author: geoffrey
 */

#ifndef DRIVERS_RTCDRV_CONFIG_H_
#define DRIVERS_RTCDRV_CONFIG_H_

#ifndef SILICON_LABS_RTCDRV_CONFIG_H
#define SILICON_LABS_RTCDRV_CONFIG_H

// Define how many timers RTCDRV provide.
#define EMDRV_RTCDRV_NUM_TIMERS     (2)

// Uncomment the following line to include wallclock functionality.
//#define EMDRV_RTCDRV_WALLCLOCK_CONFIG

// Uncomment the following line to enable integration with SLEEP driver.
//#define EMDRV_RTCDRV_SLEEPDRV_INTEGRATION

// Uncomment the following line to let RTCDRV clock on LFRCO, default is LFXO.
//#define EMDRV_RTCDRV_USE_LFRCO

#endif

#endif /* DRIVERS_RTCDRV_CONFIG_H_ */
