/*
 * alphact.c
 */

#include "news.h"
#include "logprot.h"
#include "mytypes.h"
#include "alphact.h"
#include "nqueue.h"
#include "nqdata.h"
#include "stimers.h"
#include "phylay.h"
#include "settings.h"
#include "string.h"
#include "pwrsys.h"
#include "utils.h"

#include <ctype.h>

#define is_dev()			(in->news < NUM_DEVICES)
#define is_cmd()			(in->news < NUM_CMDS)


static unsigned char *ppayload;
static ushort count_data;
static uchar num_data[ 2 ];
RFRAME_T rcv_frame;
static MInt sum;
static MInt tbet_byte, txmit1, tinactivity;

/*
 * Define initialization routine
 */

void
init_prot( void )
{
	init_logprot();
	tbet_byte = assign_timer( ONESHOT, NULL, TBET_BYTE_IX );
	txmit1 = assign_timer( ONESHOT, NULL, TXMITCT1_IX );
	tinactivity = assign_timer( ONESHOT, NULL, TINACTIVITY_IX );
	kick_timer( tinactivity, TOUT_TINACTIVITY );
}

/*
 * Define action routines
 */

void
init_frame( const NEWS_T *in )
{
	kill_timer( tinactivity );
	kick_timer( tbet_byte, TOUT_TBET_BYTE );
}

void
master_restart( const NEWS_T *in )
{
	host_reset();
	kick_timer( tinactivity, TOUT_TINACTIVITY );
}

void
store_dev( const NEWS_T *in )
{
	kick_timer( tbet_byte, TOUT_TBET_BYTE );
	sum = 0;
	sum += rcv_frame.curr_dev = in->news;
}

void
store_cmd( const NEWS_T *in )
{
	kick_timer( tbet_byte, TOUT_TBET_BYTE );
	sum += rcv_frame.curr_cmd = in->news;
	count_data = 0;
}

void
in_numdata( const NEWS_T *in )
{
	kick_timer( tbet_byte, TOUT_TBET_BYTE );
}

void
store_numdata( const NEWS_T *in )
{
	kick_timer( tbet_byte, TOUT_TBET_BYTE );
	count_data = 0;
	ppayload = rcv_frame.payload;
}

void
store_data( const NEWS_T *in )
{
	kick_timer( tbet_byte, TOUT_TBET_BYTE );
	++count_data;
	sum += *ppayload++ = in->news;
}

void
rec_frame( const NEWS_T *in )
{
	MInt c;

	kill_timer( tbet_byte );

	if( (c = (MInt)in->news + sum) != 0 )
		kick_timer( txmit1, TOUT_TXMIT1 );
	else
		proc_rcv_frame( &rcv_frame );
}

void
drop_frame( const NEWS_T *in )
{
	kick_timer( tinactivity, TOUT_TINACTIVITY );
	kill_timer( tbet_byte );
}

void
send_nack( const NEWS_T *in )
{
	kick_timer( tinactivity, TOUT_TINACTIVITY );
	set_dir_phymed( TOXMIT );
	send_nack_resp( rcv_frame.curr_dev );
	set_dir_phymed( TORCV );
}

void
send_resp( const NEWS_T *in )
{
	kick_timer( tinactivity, TOUT_TINACTIVITY );
	set_dir_phymed( TOXMIT );
	proc_rcv_cmd( &rcv_frame );
	set_dir_phymed( TORCV );
}

void
block_val( const NEWS_T *in )
{
	put_nqueue( COND_QUEUE, def_news[ VALBLOCK_IX ] );
	init_frame( in );
}

void
toidle( const NEWS_T *in )
{
	kick_timer( tinactivity, TOUT_TINACTIVITY );
}

/*
 * Define state's preprocessor routines
 */

NARG_T
idle_ppro( const NEWS_T *in )
{
	switch( in->news_fly )
	{
		case UART:
			return in->news == PROT_MARK ? MARK : UNKNOWN_DATA;
		case TIM:
			return in->news;
		default:
			return UNKNOWN_DATA;
	}
}

NARG_T
wait_dev_ppro( const NEWS_T *in )
{
	switch( in->news_fly )
	{
		case UART:
			return is_dev() ? DATA_VALID : INVALID_DATA;
		case TIM:
			return in->news;
		default:
			return UNKNOWN_DATA;
	}
}

NARG_T
wait_cmd_ppro( const NEWS_T *in )
{
	switch( in->news_fly )
	{
		case UART:
			return is_cmd() ? DATA_VALID : INVALID_DATA;
		case TIM:
			return in->news;
		default:
			return UNKNOWN_DATA;
	}
}

NARG_T
wait_ndata_ppro( const NEWS_T *in )
{
	switch( in->news_fly )
	{
		case UART:
			sum += num_data[ count_data ] = in->news;
			if( ++count_data < NDATA_SIZE )
				return NDATA_VALID;
			else
			{
				conv_endians( &rcv_frame.curr_ndata, num_data, NDATA_SIZE );
				return rcv_frame.curr_ndata <= MAX_NDATA_LEN ? DATA_VALID : INVALID_DATA;
			}
		case TIM:
			return in->news;
		default:
			return UNKNOWN_DATA;
	}
}

NARG_T
in_data_ppro( const NEWS_T *in )
{
	switch( in->news_fly )
	{
		case UART:
			if( count_data < rcv_frame.curr_ndata )
				return DATA_VALID;
			return END_PAYLOAD;
		case TIM:
			return in->news;
		default:
			return UNKNOWN_DATA;	
	}
}

NARG_T
pframe_ppro( const NEWS_T *in )
{
	return in->news_fly == UART ? UNKNOWN_DATA : in->news;
}

NARG_T
wait_vresp_ppro( const NEWS_T *in )
{
	if( in->news_fly == UART && in->news == PROT_MARK )
	   return MARK;
	if( in->news == VALRESP )
		return VALRESP;
	else
		return UNKNOWN_DATA;
}
