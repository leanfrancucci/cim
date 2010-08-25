/*
 * pwrhdl.c
 */

#include "mytypes.h"
#include "csdevs.h"
#include "ioports.h"
#include "pwrhdl.h"
#include "pwrsys.h"
#include "grstatus.h"
#include "vaultdef.h"
#include "wouts.h"
#include "adc.h"
#include "irq.h"
#include "pwrdefs.h"
#include "news.h"
#include "nqdata.h"
#include "dfpwf.h"
#include "pwfhal.h"
#include "reset.h"
#include "settings.h"
#include "stimers.h"

enum
{
	EXT_ST, BACKUP_ST
};

static MUInt pwrstat_st;
static MUInt pwrfail_t;

void
inform_pwrfail( MInt thdlr )
{
	put_nqueue( NEWS_QUEUE, def_news[PWR_FAIL_IX]);
}

void
init_pwrhdl( void )
{
	pwrfail_t = assign_timer( ONESHOT, inform_pwrfail, 0 );
}

/*
 * MCU Id functions
 */
MUInt
get_mcu_id( void )
{
	return MCU_ID ? MAIN_DEVICE : SECONDARY_DEVICE;
}

/*
 * Battery functions
 */
MUInt
get_batt_lvl( void )
{
#ifdef BATTLVL_PROMEDIATION
	MUInt i;
	static unsigned short blvl_buff[BATTLVL_BUFF_SIZE];
#endif
	static unsigned short blvl;

#ifdef BATTLVL_PROMEDIATION
	for( i=0; i<BATTLVL_BUFF_SIZE; ++i )
		blvl_buff[i] = read_adc();

	for( i=1, blvl=blvl_buff[0]; i<BATTLVL_BUFF_SIZE; ++i )
	{
		blvl += blvl_buff[i];
		blvl >>= 1;
	}
#else
	blvl = read_adc();
#endif

	if( blvl < BATTREM_LVL )
		return BATTREM;
	if( blvl < BATTLOW_LVL )
		return BATTLOW;
	return BATTOK;
}

void
connect_battery( void )
{
	BATT_CTRL = 1;
}

void
disconnect_battery( void )
{
	BATT_CTRL = 0;
}	

/*
 * Battery Charger functions
 */
void
connect_bchrg( void )
{
	CHRG_CTRL0 = 0;
	CHRG_CTRL1 = 1;
}

void
disconnect_bchrg( void )
{
//	CHRG_CTRL0 = 1;
	CHRG_CTRL1 = 0;
}

/*
 * Power Source detection functions
 */
MUInt
in_pwf( void )
{
	return pwrstat_st == BACKUP_ST;
}

void
proc_pwrstatus_irq( void )
{	
	kick_timer( pwrfail_t, TRUE_PWRFAIL_TIME );
	pwrstat_st = BACKUP_ST; 
}

void
proc_pwrstatus_rti( void )
{
	if( pwrstat_st == EXT_ST )
		return;

	if( is_irqpin_set() )
	{
		kill_timer( pwrfail_t );
		pwrstat_st = EXT_ST; 
		do_enable_pwf_interrupt();
		put_nqueue( NEWS_QUEUE, def_news[PWR_OK_IX]);
	}
}

/*
 * Host Power control function
 */
void
set_host_pwr( MUInt ctrl )
{
	HOST_PWR = ctrl;
}

/*
 * Forced Primary shutdown from Secondary MCU
 */
void
primary_shut_down( void )
{
	PRIPWRCTRL = 1;
}

void
set_valpwr( MUInt who, MUInt pwr )
{
	if( who == VAL0 )
		VAL0PWR = pwr;
	else
		VAL1PWR = pwr;
}
