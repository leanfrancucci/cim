/*
 * pwrsys.c
 */

#include "mytypes.h"
#include "pwrsys.h"
#include "pwrhdl.h"
#include "settings.h"
#include "enaint.h"
#include "vaultdef.h"
#include "sleep.h"
#include "grstatus.h"
#include "dfvars.h"
#include "reset.h"
#include "watchdog.h"

#if DEBUG
#include <stdio.h>
#endif

/*
 * Maintains the operating power system
 * and battery status
 */
typedef union
{
	unsigned char ps_reg;
	struct
	{
		unsigned bs		:2;
		unsigned s		:1;
		unsigned ms		:4;
		unsigned ps		:1;
	} bm;
}PWRSYS;

static PWRSYS pwrsys;

/*
static union 
{
	unsigned char ps_reg;
	struct
	{
		unsigned bs		:2;
		unsigned s		:1;
		unsigned ms		:4;
		unsigned ps		:1;
	} bm;
} pwrsys;
*/
#if DEBUG
static const char *mps[] =
{
	"EXT", "BACKUP", "TOSHUTDOWN"
};	
	
static const char *mbs[] =
{
	"BATTLOW", "BATTREM", "BATTOK"
};

static const char *ms[] =
{
	"PRIMARY", "SECONDARY"
};
#endif

/*
 * init_pwrsys:
 * 	The system always start from external power
 * 	by hardware design reason. This function is
 * 	called on cold boot for loading the battery 
 * 	status and operating power system.
 */

MUInt 
init_pwrsys( void )
{
	set_host_pwr(HOST_ON);
	connect_battery();
	pwrsys.bm.ps = EXT;
	pwrsys.bm.s = get_mcu_id();
	pwrsys.bm.bs = get_batt_lvl();
	set_safebox_st( pwrsys.ps_reg );
	return pwrsys.bm.s;
}

/*
 * set_batt_st:
 * 		BATTLOW/BATTREM/BATTOK
 */
void
set_batt_st( MUInt state )
{
	pwrsys.bm.bs = state;
	set_safebox_st( pwrsys.ps_reg );
}

/*
 * set_pwr_st:
 * 		EXT/BACKUP
 */
void
set_pwr_st( MUInt state )
{
	pwrsys.bm.ps = state;
	set_safebox_st( pwrsys.ps_reg );
}

/*
 * shutdown:
 * 	Force a system shutdown, now.
 */
void
host_reset( void )
{
	set_host_pwr( HOST_OFF );
	sleep( HOST_RESET_TIME );
	if( pwrsys.bm.ps == BACKUP )
	{
		disconnect_battery();
		for(;;)
			kick_watchdog();
	}
	else
	{
		disconnect_battery();
		sleep(BATT_DISCONNET_TIME);
		reset_now();
	}
}

void
shutdown( void )
{
	set_host_pwr( HOST_OFF );
	sleep( HOST_RESET_TIME );
	if( pwrsys.bm.ps == BACKUP )
	{
		disconnect_battery();
		for(;;)
			kick_watchdog();
	}
	else
	{
		disconnect_battery();
		sleep(BATT_DISCONNET_TIME);
		reset_now();
	}
	
	dprintf( printf( "\nForce a shutdown!!\n" ) );
}

unsigned char
set_memstat( unsigned char safe_stat )
{
	PWRSYS new_sstat;

	new_sstat.ps_reg = safe_stat;
	new_sstat.bm.ms = mem_status;
	return new_sstat.ps_reg;
}
