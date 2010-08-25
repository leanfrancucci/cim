/*
 *	serial.c
 * 		Manages serial input and output channels
 */

#include <string.h>

#include "mytypes.h"
#include "watchdog.h"

#include "serial.h"
#include "sertable.h"
#include "serlog.h"
#include "watchdog.h"
#include "ioports.h"

/*
 *	serial_init:
 * 		Initialize a channel 
 */
void
serial_init( MUInt channel )
{
	init_serlog( channel, &serial_default[channel] );
}

/*
 * put_nchar:
 * 		Sends n bytes to output serial interface
 * 		conected to channel. 
 * 		Receives a pointer to first char 'p'
 * 		and 'qty' to send.
 */
void
put_nchar( MUInt channel, const unsigned char *p, ushort qty )
{
	if( qty == 0 )
		return;

	loging_led();
	while( qty-- )
	{
		serlog_sendchar( channel, p++ );
		kick_watchdog();
	}
}

/*
 *	put_string:
 * 		Send the null-terminated string p to the
 * 		serial interface connected to channel.
 */

void
put_string( MUInt channel, const char *p )
{
	put_nchar( channel, (unsigned char *)p, (MUInt) strlen(p) );
}

/*
 *	put_char:
 * 		Send the character given by c to the
 * 		serial interface connected to channel.
 */
void
put_char( MUInt channel, const unsigned char c )
{
	put_nchar( channel, &c, 1 );
}

/*
 *	get_char:
 * 		-----Returns character in p and 0 if succes.
 * 		Negative if empty queue.
 */
MInt
get_char( MUInt channel, unsigned char *p )
{
	return remove_gsqueue( channel, p );
}

/*
 * set_baudrate:
 * 		Sets baudrate
 */
void
set_baudrate( MUInt channel, MUInt bd )
{
	serlog_setbd( channel, bd );
}
