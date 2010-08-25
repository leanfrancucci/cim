/*
 *	conser.c
 *
 * 		This module presents a several routines used by
 * 		command shell's modules. These routines are directly
 * 		dependent of platform.
 */


#include "mytypes.h"
#include "console.h"

#ifdef DOS_PLATFORM
#include <stdio.h>
#include <conio.h>
#include <process.h>
#else
#include "gsqueue.h"
#include "qdata.h"
#include "serial.h"
#endif


void
conser_init( void )
{
#ifdef DOS_PLATFORM
	system("cls");
#endif
}

/*
 * conser_tstc:
 *
 * 		Checks to see if a key is currently available
 *
 * 		Returns:
 *
 * 			0 - on success
 * 			1 - key is not available
 */

MUInt
conser_tstc( void )
{
#ifdef DOS_PLATFORM
	return  0;
#else
	if( is_empty_gsqueue( COM1_QUEUE ) == EMPTY_QUEUE )
		return 1;
	else
		return 0;
#endif
}

void
conser_putc( const char c )
{
#ifdef DOS_PLATFORM
	putc( c, stdout );
#else
	put_char( COM1CH, c );
#endif
}

void
conser_puts( const char *s )
{
#ifdef DOS_PLATFORM
	while( *s )
		conser_putc( *s++ );
#else
	put_string( COM1CH, s );
#endif
}

/*
 * conser_getc:
 *
 * 		Blocking function that return one character 
 * 		from attached serial line.
 */

MUInt
conser_getc( void )
{
#ifdef DOS_PLATFORM
	return getch();
#else
	unsigned char c;
	if( get_char( COM1CH, &c ) == EMPTY_QUEUE )	
		return 0xFF;
	else	
		return c;
#endif
}
