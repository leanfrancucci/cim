/*
 * 	serdefs.h
 */

#ifndef __SERDEFS_H__
#define __SERDEFS_H__

#include "gsqueue.h"

#define COM1_MAX_CH		64
#define COM2_MAX_CH		64

/*
 * Maintain the COM1 received chars
 */
extern ELEM_TYPE com1rx_queue[ COM1_MAX_CH ];
/*
 * Maintain the COM2 received chars
 */
extern ELEM_TYPE com2rx_queue[ COM2_MAX_CH ];

enum
{
	COM1CH, COM2CH,
	NUM_CHANNELS
};

enum
{
	B300, B600, B1200, B2400, B4800, B9600, B19200,
	B38400, B57600,
	NUM_BAUD
};

enum
{
	BITS7, BITS8,
	NUM_BITS
};

enum
{
	NO_PAR, EVEN_PAR, ODD_PAR,
	NUM_PAR
};

typedef struct
{
	MUInt baud;
	MUInt bits;
	MUInt parity;
} SERIAL_T;

#endif
