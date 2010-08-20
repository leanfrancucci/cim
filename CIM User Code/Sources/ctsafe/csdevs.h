/*
 * cddevs.h
 */


#define LOCKER0_MASK		0x0002
#define LOCKER1_MASK		0x0004
#define LOCKER2_MASK		0x0008
#define LOCKER3_MASK		0x0010

#define NUM_DEVS_LIST		4


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
	LOCKER2,
	LOCKER3,
	PLUNGER2,
	PLUNGER3,
	KEYSW,

	NUM_DEVICES
} DEV_T;
