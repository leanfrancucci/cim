/*
 * contick.c
 */

#include "mytypes.h"
#include "console.h"


#define is_cmd_timeout()		(tcmd != 0)

/*
 * 		It is upgrade from timer isr.
 */

MUInt tcmd;

/*
 * set_cmd_timeout:
 *
 * 		It set the command shell abort time.
 */

void 
set_cmd_timeout( MUInt tout )
{
	tcmd = tout < CONFIG_CMD_TOUT_MIN ? CONFIG_CMD_TOUT_MIN : tout;
}
