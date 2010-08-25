/*
 * formats.h
 */

#ifndef __FORMATS_H__
#define __FORMATS_H__

#include "mytypes.h"

#define MAX_CHARS_SERIAL		64	

void myprintf( MUInt ch, const char *fmt, ... );
char *adc2ascci( int val );

#endif
