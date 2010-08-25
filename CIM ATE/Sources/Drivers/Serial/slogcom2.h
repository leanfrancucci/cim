/*
 * 	slogcom2.c
 */

#ifndef __SLOGCOM2_H__
#define __SLOGCOM2_H__

#include "serdefs.h"

void com2_init( const SERIAL_T *p );

void com2_setbd( MUInt bd );

void com2_sendchar( unsigned char c );

#endif
