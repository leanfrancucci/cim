/*
 * cmdshell.c
 */

#include "mytypes.h"
#include "command.h"
#include "conser.h"
#include "formats.h"


MInt 
do_shell( const CMD_TABLE *p, MInt argc, char *argv[] )
{
	myprintf( 0, "\n\tCommand shell setup: \n\n" );	
	myprintf( 0, "\t%sdefined longhelp\n", LONGHELP ? "" : "un" );
	myprintf( 0, "\tdefined maximum arguments = %01d\n\n", MAXARGS );

	return 0;
}
