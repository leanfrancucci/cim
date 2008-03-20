/*
 *	sci.h
 *	Routines handle SCI service
 *
 *	Programmer: Darío S. Baliña.
 *	Date : 29/05/06.
 */

#ifndef __SCI_H__
#define __SCI_H__

#include "mytypes.h"
#include "serdefs.h"

typedef void(*SCI_CALLBACK)( MUInt );

#undef __8MHz__
#ifdef __8MHz__

#define	UC_B300		0xD05	// 0.0% error
#define	UC_B600		0xC83	// 0.0% error
#define	UC_B1200	0x341	// 0.0% error
#define	UC_B2400	0x1A1	// 0.1% error
#define	UC_B4800 	0xD0	// 0.2% error
#define	UC_B9600	0x68	// 0.2% error
#define	UC_B19200	0x34	// 0.2% error
#define	UC_B38400	0x32	// 0.2% error
#define	UC_B57600	0x11	// 2.1% error

#else

#define	UC_B300		0xD55	// 0.0% error
#define	UC_B600		0x6AB	// 0.0% error
#define	UC_B1200	0x355	// 0.0% error
#define	UC_B2400	0x1AB	// 0.1% error
#define	UC_B4800	0xD5	// 0.2% error
#define	UC_B9600	0x6B	// 0.3% error
#define	UC_B19200	0x35	// 0.6% error
#define	UC_B38400	0x1B	// 1.2% error
#define	UC_B57600	0x12	// 1.2% error

#endif

void host_sci1_init( SERIAL_T *p );
void host_sci1_setbd( unsigned short baud_rate );
void host_sci1_sendchar( unsigned char c );
void val_sci2_init( SERIAL_T *p );
void val_sci2_setbd( unsigned short baud_rate );
void val_sci2_sendchar( unsigned char c );

void disable_rcv_interrupt( MUInt ch );
void enable_rcv_interrupt( MUInt ch );

#endif
