/*
 * 	sertable.c
 */

#include "mytypes.h"

#include "sertable.h"
#include "serdefs.h"

unsigned char com1rx_queue[ COM1_MAX_CH ];
unsigned char com2rx_queue[ COM2_MAX_CH ];

const SERIAL_T serial_default[] =
{
//	{	B19200, BITS8, NO_PAR	},
	{	B38400, BITS8, NO_PAR	},
	{	B38400, BITS8, NO_PAR	},
};
