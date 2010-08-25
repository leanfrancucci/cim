/*
 * utils.c
 */

#include "mytypes.h"
#include "utils.h"

#include <string.h>

unsigned char 
get_checksum( void *p, MUInt qty )
{
	MUInt sum, i;
	MUInt *q;
	
	for( i = 0, q = p, sum = 0; i < qty; ++i, ++q )
		sum += *q;
	sum = ~sum;

	return sum += 1;
}

void
conv_endians( void *to, const void *from, MUInt qty )
{
#if LITTLE_ENDIAN
	MUInt i, j;
	MUInt *p, *q;

	p = (unsigned char*)to;
	q = (unsigned char*)from;
	for( i = 0, j = qty; i < qty; ++i, --j )
		p[ i ] = q[ j - 1 ];
#else
	memcpy( to, from, qty );
#endif
}
