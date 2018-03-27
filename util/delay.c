/*******************************************************************************
 * @file delay.c
 * @brief Delay functions
 * @author Silicon Labs
 * @version 1.03
 *******************************************************************************
 * @section License
 * <b>(C) Copyright 2014 Silicon Labs, http://www.silabs.com</b>
 *******************************************************************************
 *
 * Permission is granted to anyone to use this software for any purpose,
 * including commercial applications, and to alter it and redistribute it
 * freely, subject to the following restrictions:
 *
 * 1. The origin of this software must not be misrepresented; you must not
 *    claim that you wrote the original software.
 * 2. Altered source versions must be plainly marked as such, and must not be
 *    misrepresented as being the original software.
 * 3. This notice may not be removed or altered from any source distribution.
 *
 * DISCLAIMER OF WARRANTY/LIMITATION OF REMEDIES: Silicon Labs has no
 * obligation to support this Software. Silicon Labs is providing the
 * Software "AS IS", with no express or implied warranties of any kind,
 * including, but not limited to, any implied warranties of merchantability
 * or fitness for any particular purpose or warranties against infringement
 * of any proprietary rights of a third party.
 *
 * Silicon Labs will not be liable for any consequential, incidental, or
 * special damages, or any other relief, or for any claim by any third party,
 * arising from your use of this Software.
 *
 ******************************************************************************/

#include <em_device.h>
#include <em_timer.h>
#include <em_emu.h>
#include <rtcdriver.h>
#include "rtcdrv_config.h"

RTCDRV_TimerID_t xTimerForWakeUp;

void Delay_Init(){
	RTCDRV_Init();
	RTCDRV_AllocateTimer(&xTimerForWakeUp);
}

/**********************************************************
 * Delay a number of milliseconds
 **********************************************************/
void DelayMs(int ms){
	RTCDRV_StartTimer(xTimerForWakeUp, rtcdrvTimerTypeOneshot, ms, NULL, NULL);
	EMU_EnterEM2(true);
}


