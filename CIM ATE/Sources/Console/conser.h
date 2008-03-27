/*
 *	conser.h
 *
 * 		This module presents a several routines used by
 * 		command shell's modules. These routines are dependent 
 * 		of platform.
 */

#ifndef __CONSER_H__
#define __CONSER_H__

#include "mytypes.h"

void conser_init( void );

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

MUInt conser_tstc( void );
void conser_putc( const char c );
void conser_puts( const char *s );

/*
 * conser_getc:
 *
 * 		Blocking function that return one character 
 * 		from attached serial line.
 */

MUInt conser_getc( void );

#endif
