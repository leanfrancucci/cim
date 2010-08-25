/**
 * initsys.c
 */

/*---- Hardware module includes ----*/

#include "drivers.h"
#include "sleep.h"
#include "console.h"

/*---- Software module includes ----*/

/**
 * init_system:
 * 	Called after processor reset.
 */
void
init_system( void )
{
	/*---- Hardware initialization ----*/

	drivers_init();

	/*---- Software initialization ----*/
	
	init_command_shell();
}
