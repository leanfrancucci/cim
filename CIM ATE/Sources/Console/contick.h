/*
 * contick.h
 */

#include "mytypes.h"


#define is_cmd_timeout()		(tcmd != 0)

/*
 * 		It is upgrade from timer isr.
 */

extern MUInt tcmd;

/*
 * set_cmd_timeout:
 *
 * 		It set the command shell abort time.
 */

void set_cmd_timeout( MUInt tout );
