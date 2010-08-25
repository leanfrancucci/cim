/*
 * console.h
 */

#include "mytypes.h"

#define PRINT_FORMATS			1
#define DELETE_CHAR				1
#define CONFIG_CMD_TOUT			0
#define CONFIG_CMD_TOUT_MIN 	1
#define CONFIG_CMD_TIME			3 /* seconds */


/*
 * init_command_shell:
 *
 * 		Initialize this module.
 */

void init_command_shell( void );


/*
 * command_shell:
 *
 * 		Entry point to use the command shell.
 * 		Every received character from attached
 * 		serial channel is parsed on-line.
 */

void command_shell( void );
