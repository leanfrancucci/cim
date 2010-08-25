/*
 * utils.h
 */

#ifndef __UTILS_H__
#define __UTILS_H__

#include "mytypes.h"

#define LITTLE_ENDIAN		0

unsigned char get_checksum( void *p, MUInt qty );
void conv_endians( void *to, const void *from, MUInt qty );

#endif
