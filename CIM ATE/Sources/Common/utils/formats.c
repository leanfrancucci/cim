/*
 * formats.c
 */

#include <stdio.h>
#include <stdarg.h>

#include "formats.h"
#include "conser.h"

static char buff[ MAX_CHARS_SERIAL ];

void
myprintf( MUInt ch, const char *fmt, ... )
{
	va_list args;

	va_start( args, fmt );
	vsprintf( buff, fmt, args );
	va_end( args );
	conser_puts( buff );
}

char *
adc2ascci( int val )
{
	char *p;
	static char valbuff[5];

	sprintf(valbuff,"%d",val);
	p = valbuff + strlen(valbuff);
	*p = *(p - 1);
	*(p - 1) = '.';
	*(p + 1) = '\0';
	return valbuff;
}

