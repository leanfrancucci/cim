/*
 * cddevs.h
 */


#define LOCKER0_MASK		0x0002
#define LOCKER1_MASK		0x0004


/*
 * 		Indicate the desired device.
 */

typedef enum
{
	VAL0,
	VAL1,
	LOCKER0,
	PLUNGER0,
	LOCKER1,
	PLUNGER1,
	ALARM0,
	ALARM1,
	SAFEBOX,
	STACKER0,
	STACKER1,

	NUM_DEVICES
} DEV_T;
