/*
 * 	slogcom1.c
 */

#ifndef __SLOGCOM1_H__
#define __SLOGCOM1_H__

#include "serdefs.h"

void com1_init( const SERIAL_T *p );

void com1_setbd( MUInt bd );

void com1_sendchar( unsigned char c );

#endif
