/*
 * valact.c
 */

#include "news.h"
#include "mytypes.h"
#include "valact.h"
#include "valprot.h"
#include "logprot.h"
#include "phylay.h"
#include "stimers.h"
#include "nqueue.h"
#include "nqdata.h"
#include "settings.h"
#include "valconf.h"

#include <ctype.h>
#include <stdlib.h>

#define get_frlen()		rframe.qty
#define is_valid()		(rframe.qty < VAL_MAX_FRAME_SIZE)

static VAL_BUFF_T rframe;
static unsigned char *pfr;
static MInt val_timer;

/*
 * Define initialization routine
 */

void
init_val( void )
{
	/* deplete serial queue */
	val_timer = assign_timer( ONESHOT, NULL, TOUT_VAL_IX );
}

/*
 * Define action routines
 */

void
set_start_rcv( const NEWS_T *in )
{
	kick_timer( val_timer, get_start_val_timeout( get_curr_val() ) );
}

void
init_vframe( const NEWS_T *in )
{
	pfr = rframe.buff;
	rframe.qty = 1;
	*pfr++ = in->news;
	kick_timer( val_timer, TOUT_BETVAL );
}

void
store_vdata( const NEWS_T *in )
{
	++rframe.qty;
	*pfr++ = in->news;
	kick_timer( val_timer, TOUT_BETVAL );
}

void
rec_vframe( const NEWS_T *in )
{
	set_dir_phymed( TOXMIT );
	put_nqueue( COND_QUEUE, def_news[ VALRESP_IX ] );
	send_sbox_frame( prepare_resp_sbox( get_curr_val(),	NO_CMD, get_frlen(), &rframe ) );
	set_dir_phymed( TORCV );
}

void
drop_vframe( const NEWS_T *in )
{
	kill_timer( val_timer );
}

/*
 * Define state's preprocessor routines
 */

NARG_T
vblocked_ppro( const NEWS_T *in )
{
	if( in->news_fly == PROT && in->news == VALUNBLOCK )
		return VALUNBLOCK;
	return UNKNOWN_DATA;
}

NARG_T
idle_val_ppro( const NEWS_T *in )
{
	switch( in->news_fly )
	{
		case UART:
			return DATA_VALID;
		case PROT:
		case TIM:
			return in->news;
		default:
			return UNKNOWN_DATA;
	}
}

NARG_T
in_frame_ppro( const NEWS_T *in )
{
	switch( in->news_fly )
	{
		case UART:
			return is_valid() ? DATA_VALID : INVALID_DATA;
		case TIM:
		case PROT:
			return in->news;
		default:
			return UNKNOWN_DATA;
	}
}
