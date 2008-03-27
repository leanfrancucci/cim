/*
 * main.c
 */
 
#include "mytypes.h"
#include "initsys.h"
#include "watchdog.h"
#include "serial.h"
#include "console.h"

#define forever() 		for(;;)

void
main( void )
{
	init_system();

	forever()
	{
		command_shell();
		kick_watchdog();
	}
}
