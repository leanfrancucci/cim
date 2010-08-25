/*
 * main.c
 */
 
#include "mytypes.h"
#include "initsys.h"
#include "watchdog.h"
#include "dispatch.h"
#include "logprot.h"
#include "serial.h"
#include "sersw.h"
#include "sleep.h"

#define forever() 		for(;;)

void
main( void )
{
	init_system();

	forever()
	{
		exec_protocol( CTCH );
		exec_protocol( VALCH );
		dispatcher();
		kick_watchdog();
	}
}
