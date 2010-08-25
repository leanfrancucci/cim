/*
 * 	serlog.c
 */

#include "mytypes.h"
#include "serdefs.h"
#include "slogcom1.h"
#include "slogcom2.h"

void
init_serlog( MUInt channel, const SERIAL_T *p )
{
	if( channel == COM1CH )
		com1_init(p);
	else
		com2_init(p);	
}

void
serlog_setbd( MUInt channel, MUInt bd)
{
	if( channel == COM1CH )
		com1_setbd( bd );
	else
		com2_setbd( bd );
}

void
serlog_sendchar( MUInt channel, const unsigned char *p )
{
	if( channel == COM1CH )
		com1_sendchar( *p );
	else
		com2_sendchar( *p );
}


