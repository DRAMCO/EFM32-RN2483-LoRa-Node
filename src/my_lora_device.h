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
 *         File: my_lora_device.h
 *      Created: 2018-03-21
 *       Author: Geoffrey Ottoy
 *      Version: 1.0
 *
 *  Description: TODO
 */

#ifndef MY_LORA_DEVICE_H_
#define MY_LORA_DEVICE_H_

/* OVER_THE_AIR_ACTIVATION:
 * 	- set to 1 to use the Over-the-Air activation procedure
 * 	- set to 0 t* use the Personalization activation procedure
 */
#define OVER_THE_AIR_ACTIVATION		1


#define DEFAULT_DATA_RATE			SF8_BW125

#if OVER_THE_AIR_ACTIVATION == 1
// Copy your settings here
/* 64-bit device ID */
#define LORAWAN_DEVICE_EUI			"00D1098D1AEF0A52"
/* 64-bit application ID */
#define LORAWAN_APPLICATION_EUI     "70B3D57EF0006CA6"
/* 128-bit AES key */
#define LORAWAN_APPLICATION_KEY     "2A5B8F6B478DD2C4C2D259C98274A7DC"
// Don't change
#define JOIN_MECHANISM				OTAA

#elif OVER_THE_AIR_ACTIVATION == 0
// Copy your settings here
/* 32-bit device address */
#define LORAWAN_DEVICE_ADDRESS     	"01234567"
/* 128-bit AES key */
#define LORAWAN_NWKSKEY             "0123456789ABCDEFFEDCBA9876543210"
/* 128-bit AES key */
#define LORAWAN_APPSKEY             "FEDCBA98765432100123456789ABCDEF"
// Don't change
#define JOIN_MECHANISM				ABP
#else
#error "OVER_THE_AIR_ACTIVATION has the wrong value"
#endif

/*****************************************************************************************
 * DO NOT CHANGE !
 *****************************************************************************************/
#if OVER_THE_AIR_ACTIVATION == 1
#define LORA_INIT_MY_DEVICE                                         \
{	JOIN_MECHANISM,						/* Activation mechanism */	\
	DEFAULT_DATA_RATE,					/* Data Rate on start-up */ \
	LORAWAN_DEVICE_EUI,					/* Device EUI */            \
	LORAWAN_APPLICATION_EUI,			/* Application EUI */       \
	LORAWAN_APPLICATION_KEY,			/* Application key */       \
	"",									/* Device address */ 		\
	"",									/* Network session key*/ 	\
	"",									/* App session key*/ 		\
}
#else
#define LORA_INIT_MY_DEVICE                                         \
{	JOIN_MECHANISM,						/* Activation mechanism */	\
	DEFAULT_DATA_RATE,					/* Data Rate on start-up */ \
	"",                 				/* Device EUI */            \
	"",                					/* Application EUI */       \
	"",									/* Application key */       \
	LORAWAN_DEVICE_ADDRESS,				/* Device address */ 		\
	LORAWAN_NWKSKEY, 					/* Network session key*/ 	\
	LORAWAN_APPSKEY, 					/* App session key*/ 		\
}
#endif

#endif /* MY_LORA_DEVICE_H_ */
