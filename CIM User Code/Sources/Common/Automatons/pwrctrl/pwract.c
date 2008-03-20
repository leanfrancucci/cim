/*
 * pwract.c
 */

#include "news.h"
#include "mpdata.h"
#include "mytypes.h"
#include "wouts.h"
#include "grstatus.h"
#include "stimers.h"
#include "news.h"
#include "pwract.h"
#include "nqdata.h"
#include "pwrhdl.h"
#include "pwrsys.h"
#include "pwrdefs.h"
#include "dfsys.h"
#include "irq.h"
#include <stdlib.h>


static MInt pwrctrl_t, fsync_t;

/*
 * Define initialization routine
 */

void
init_pwrctrl( void )
{
	MUInt battlvl;

	pwrctrl_t = assign_timer( ONESHOT, NULL, TOUT_PWR_IX );
	fsync_t = assign_timer( ONESHOT, NULL, TOUT_FSYNC_IX );

	battlvl = get_batt_lvl();
	if(battlvl == BATTREM )
	{
		put_nqueue(NEWS_QUEUE, def_news[BATT_REM_IX] );
		kick_timer(pwrctrl_t, BATTREM_TIME );
	}
	else
	{
		put_nqueue(NEWS_QUEUE, def_news[BATT_OK_IX] );
		kick_timer( pwrctrl_t, EXTCHRG_BATTM_TIME );
		connect_bchrg();	
	}

	set_batt_st( battlvl );
	kick_timer( fsync_t, FSYNC_TIME );

	init_pwrfail_det();
}

/*
 * Define static routines
 */

/*
 * Define action routines
 */
void
flash_sync( NEWS_T *in )
{
	dfilesys_sync();
	kick_timer( fsync_t, FSYNC_TIME );
}

void
ext2backup( NEWS_T *in )
{
	set_pwr_st(BACKUP);
	disconnect_bchrg();
	kick_timer(pwrctrl_t, BACKUP_BATTM_TIME );
}

void
chrg2nchrg( NEWS_T *in )
{
	disconnect_bchrg();
	kick_timer(pwrctrl_t, CHRG_DCONN_TIME );
}

void
checkbatt( NEWS_T *in )
{
	MUInt battlvl;

	battlvl = get_batt_lvl();
	if(battlvl == BATTREM )
		put_nqueue(NEWS_QUEUE, def_news[BATT_REM_IX] );
	else
		put_nqueue(NEWS_QUEUE, def_news[BATT_OK_IX] );

	set_batt_st( battlvl );
}

void
nchrg2chrg( NEWS_T *in )
{
	connect_bchrg();
	kick_timer(pwrctrl_t, EXTCHRG_BATTM_TIME );
}

void
batt2nbatt( NEWS_T *in )
{
	kick_timer(pwrctrl_t, BATTREM_TIME );
}

void
ext2ext( NEWS_T *in )
{
	MUInt battlvl;

	battlvl = get_batt_lvl();
	if(battlvl == BATTREM )
		kick_timer(pwrctrl_t, BATTREM_TIME );
	else
		put_nqueue(NEWS_QUEUE, def_news[BATT_OK_IX] );

	set_batt_st( battlvl );
}

void
backup2ext( NEWS_T *in )
{
	set_pwr_st(EXT);
	connect_bchrg();
	kick_timer(pwrctrl_t, EXTCHRG_BATTM_TIME );
	kick_timer( fsync_t, FSYNC_TIME );
}

void
back_bmes( NEWS_T *in )
{
	MUInt battlvl;

	battlvl= get_batt_lvl();

	set_batt_st( battlvl );
	kick_timer(pwrctrl_t, BACKUP_BATTM_TIME );
	if( battlvl == BATTLOW )
		put_nqueue( NEWS_QUEUE, def_news[SHTDWN_IX] );
}

void
shtdwn( NEWS_T *ix )
{
	disconnect_battery();	
	for(;;);
}

/*
 * Define state's preproccessor routines
 */

NARG_T
pwrctrl_ppro( const NEWS_T *in )
{
	switch( in->news_fly )
	{
		case PWRCTRL:
		case TIM:
			return in->news;
		default:
			return UNKNOWN;
	}
}

