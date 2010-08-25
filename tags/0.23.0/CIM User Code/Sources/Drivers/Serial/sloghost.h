/*
 * 	sloghost.c
 */

#ifndef __SLOGHOST_H__
#define __SLOGHOST_H__

#include "serdefs.h"

void host_init( SERIAL_T *p );

void host_setbd( MUInt bd );

void host_sendchar( unsigned char c );

#endif
