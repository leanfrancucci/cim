/*
 * 	serlog.h
 */

#include "mytypes.h"
#include "serdefs.h"

void init_serlog( MUInt channel, const SERIAL_T *p );
void serlog_setbd( MUInt channel, MUInt bd );
void serlog_sendchar( MUInt channel, const unsigned char *p );

