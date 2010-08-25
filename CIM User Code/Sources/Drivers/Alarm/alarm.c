/*
 * alarm.c
 */

#include "mytypes.h"
#include "alarm.h"
#include "logprot.h"
#include "settings.h"
#include "wouts.h"
#include "ioports.h"
#include "wouts.h"

#if DEBUG
#include <stdio.h>
#endif

/*
 * set_alarm:
 * 	Active or deactive a desired alarm.
 * 	The device number is extracted from 
 * 	devices table.
 */

void
set_alarm( MUInt which, MUInt ctrl )
{
	dprintf( printf( "\nAlarm %01d %s\n", which == ALARM0 ? 0 : 1,
		ctrl ? "DISABLE" : "ENABLE" ) );
	if(	ctrl == ENABLE )
	{
		if( which == ALARM1 )
			DURESS_AL = 1;
		else
			BURGLAR_AL = 1;
	}
	else
	{
		if( which == ALARM1 )
			DURESS_AL = 0;
		else
			BURGLAR_AL = 0;
	}
}
