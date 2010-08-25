/*
 * pwrdefs.h
 */

#include "stimers.h"

/*
 * AD convertion Constants
 */
#define VBATT_K			(270000/127)
#define ADC_RES			(33000/1024)
//#define ADC_K			(VBATT_K/ADC_RES) 	/* ADC_K by calculation */
#define ADC_K			60					/* Adjusted by medisions */

/*
 * Battery levels definitions
 */
#define BATTREM_LVL		(5*ADC_K)
#define BATTLOW_LVL		(9*ADC_K)

/*
 * Power Control Timers Definitions
 */
#define EXTCHRG_BATTM_TIME		((1000/STIMER_BASE)*60)
#define BACKUP_BATTM_TIME		((1000)/STIMER_BASE)
#define	BATTREM_TIME 			((1000)/STIMER_BASE)
#define CHRG_DCONN_TIME			(1000/STIMER_BASE)
#define FSYNC_TIME				((1000/STIMER_BASE)*60)

/*
 * Expired forces shutdown.
 * expressed as a multiple of BACKUP_BATTM_TIME
 */
#define ONBACKUP_TIME			(5400) /** 90 minutes */

