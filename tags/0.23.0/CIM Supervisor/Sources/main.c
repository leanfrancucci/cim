/*
 * main.c
 */

#include "mytypes.h"
#include "drivers.h"
#include "watchdog.h"

#define forever for(;;)

void
main(void)
{
	drivers_init();
	forever
		kick_watchdog();
}

