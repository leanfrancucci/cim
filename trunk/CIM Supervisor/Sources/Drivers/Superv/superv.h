/*
 * superv.h
 */

#include "mytypes.h"
#include "sleep.h"

#define SUP_BASE	8

#define	WDOG_TOUT		(1000/SUP_BASE)
#define	VRLY_TOUT		(1000/SUP_BASE)
#define	VLOCK_TOUT		(1000/SUP_BASE)
#define ON_RESET_TIME	(200/SLEEP_BASE)
#define POST_RESET_TIME	(200/SLEEP_BASE)

enum
{
	WDOG_SUP, VRLY_SUP, VLOCK_SUP,
	NUM_WDOG_SUPS
};

void supervisor_init( void );
void do_supervisor( void );

