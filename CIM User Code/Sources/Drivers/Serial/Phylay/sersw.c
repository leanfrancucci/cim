/*
 * sersw.c
 */

#include "mytypes.h"
#include "sersw.h"
#include "settings.h"
#include "ioports.h"

#if DEBUG
#include <stdio.h>
#endif

#if DEBUG
static const char *mch[] =
{
	"CH0", "CH1"
};
#endif

/*
 * init_sersw:
 * 	Initialize serial switch
 */

void 
init_sersw( void )
{
	set_sersw( CH0 );
}

/*
 * set_sersw:
 *	Select desired channel
 */

void 
set_sersw( MUInt ch )
{
	dprintf( printf( "\nSelect serial channel %s\n", ch ? mch[ 1 ]:mch[ 0 ] ) );

	if( ch == CH0 )
		BVSEL = 1;
	else
		BVSEL = 0;
}
