/*
 * settings.h
 */

#ifndef __SETTINGS_H__
#define __SETTINGS_H__

#include "stimers.h"

/*
 * Firmware and hardware version string
 */
#define VER_STR_LEN			17+1
#define VERSION_STRING		"SW00.11.00HW02.00"

/*
 * The model determine a specified hardware 
 * configuration
 */

#define MODEL_CONFIG		2

/*
 * Maximum number of installed locker to
 * model 0
 */

#define NUM_LOCKERS			2

/*
 * Timeout used by system
 */

#define TOUT_TBET_BYTE		(100/STIMER_BASE)
#define TOUT_TXMIT0			(40/STIMER_BASE)
#define TOUT_TXMIT1			(40/STIMER_BASE)
#define TOUT_TINACTIVITY	(15*60*(1000/STIMER_BASE))
#define TOUT_TLOCK			(3000/STIMER_BASE)
#define TOUT_TUNLOCK		0
#define MAX_TUNLOCK_TIME	20	/* minutes */
#define SECMCU_BOOT_DELAY	(5000/SLEEP_BASE)
#define HOST_RESET_TIME 	(3000/SLEEP_BASE)
#define BATT_DISCONNET_TIME (2000/SLEEP_BASE)

/*
 * Used to wait a start of frame from validator.
 */

#define DEF_TOUT_STVAL		5	/* seconds */

/*
 * Used to wait between data from validator.
 */

#define TOUT_BETVAL			(100/STIMER_BASE)

/*
 * Debugging
 */

#define DEBUG				0

#if DEBUG
#define dprintf(x)			(x)
#else
#define dprintf(x)
#endif

#endif
