/*
 * 	slogval.c
 */

#ifndef __SLOGVAL_H__
#define __SLOGVAL_H__

#include "serdefs.h"

void val_init( SERIAL_T *p );

void val_setbd( MUInt bd );

void val_sendchar( unsigned char c );

#endif
