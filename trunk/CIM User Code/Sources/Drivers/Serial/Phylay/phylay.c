/*
 * phylay.c
 */

#include "mytypes.h"
#include "phylay.h"
#include "ioports.h"

#ifdef __PRINTF__
#include <stdio.h>
#endif

/*
 * init_phymed:
 *	Initialize this module and lower
 *	layers
 */

void
init_phymed( void )
{
	set_dir_phymed( TORCV );
}

/*
 * set_dir_phymed:
 *	Select direction of RS485 channel
 */

void
set_dir_phymed( MUInt dir )
{
#ifdef __PRINTF__
	printf( "\nRS485 DIR %s\n", dir == TORCV ? "RCV" : "XMIT" );
#endif
	if( dir == TORCV )
		UART1DIR = 0;
	else
		UART1DIR = 1;
}
