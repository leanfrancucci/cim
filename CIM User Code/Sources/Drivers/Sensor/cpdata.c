/*
 *	cpdata.c
 */
#include "mytypes.h"
#include "cparse.h"
#include "cpdata.h"
#include "wouts.h"
#include "news.h"
#include "nqdata.h"
#include "grstatus.h"
#include "csdevs.h"

/*
 * Validators pseudo news definitions
 * 	this devices dont inform events thru the news queue,
 * 	sets status flag in GRSTATUS struct.
 */
const short sens_tout[MAX_PARSERS][NUM_TIMEVAL] = 
{
	{ LOCKS1_OPENT, LOCKS1_CLOSET },
	{ PLGS1_OPENT,  PLGS1_CLOSET  },
	{ LOCKS2_OPENT, LOCKS2_CLOSET },
	{ PLGS2_OPENT,  PLGS2_CLOSET  },
	{ VAL1STCK_OPENT, VAL1STCK_CLOSET },
	{ VAL2STCK_OPENT, VAL2STCK_CLOSET },
	{ KEYSWITCH_OPENT,  KEYSWITCH_CLOSET  },
	{ LOCKS3_OPENT, LOCKS3_CLOSET },
	{ LOCKS4_OPENT, LOCKS4_CLOSET },
	{ PLGS3_OPENT,  PLGS3_CLOSET  },
	{ PLGS4_OPENT,  PLGS4_CLOSET  }
};

/*
 * 	Sens parser
 */

static
void
nil( void )
{
}


static
void
inf_close( void )
{
	switch( which_sensor() )
	{
		case VAL1STCK:
			set_stacker_state( STACKER0, INSTALLED );
			break;
		case VAL2STCK:
			set_stacker_state( STACKER1, INSTALLED );
			break;
		case KEYSWITCH:
			put_nqueue( NEWS_QUEUE, def_news[CLOSE_KEYS_D1_IX] );
			put_nqueue( NEWS_QUEUE, def_news[CLOSE_KEYS_D2_IX] );
			put_nqueue( NEWS_QUEUE, def_news[CLOSE_KEYS_D3_IX] );
			put_nqueue( NEWS_QUEUE, def_news[CLOSE_KEYS_D4_IX] );
			set_keyswitch_state( CLOSED );
			break;
		case LOCKS1:
			set_locker_state( LOCKER0, LOCKED_STATUS );
			break;
		case LOCKS2:
			set_locker_state( LOCKER1, LOCKED_STATUS );
			break;
		case LOCKS3:
			set_locker_state( LOCKER2, LOCKED_STATUS );
			break;
		case LOCKS4:
			set_locker_state( LOCKER3, LOCKED_STATUS );
			break;
		case PLGS1:
			set_plunger_state( PLUNGER0, CLOSED );
			break;
		case PLGS2:
			set_plunger_state( PLUNGER1, CLOSED );
			break;						
		case PLGS3:
			set_plunger_state( PLUNGER2, CLOSED );
			break;
		case PLGS4:
			set_plunger_state( PLUNGER3, CLOSED );
			break;			
		default:
			break;
	}
}

static
void
inf_open( void )
{
	switch( which_sensor() )
	{
		case VAL1STCK:
			set_stacker_state( STACKER0, REMOVED );
			break;
		case VAL2STCK:
			set_stacker_state( STACKER1, REMOVED );
			break;
		case KEYSWITCH:
			put_nqueue( NEWS_QUEUE, def_news[OPEN_KEYS_D1_IX] );
			put_nqueue( NEWS_QUEUE, def_news[OPEN_KEYS_D2_IX] );
			put_nqueue( NEWS_QUEUE, def_news[OPEN_KEYS_D3_IX] );
			put_nqueue( NEWS_QUEUE, def_news[OPEN_KEYS_D4_IX] );
			set_keyswitch_state( OPENED );
			break;
		case LOCKS1:
			set_locker_state( LOCKER0, UNLOCKED_STATUS );
			break;
		case LOCKS2:
			set_locker_state( LOCKER1, UNLOCKED_STATUS  );
			break;
		case LOCKS3:
			set_locker_state( LOCKER2, UNLOCKED_STATUS );
			break;
		case LOCKS4:
			set_locker_state( LOCKER3, UNLOCKED_STATUS );
			break;
		case PLGS1:
			set_plunger_state( PLUNGER0, OPENED );
			break;
		case PLGS2:
			set_plunger_state( PLUNGER1, OPENED );
			break;						
		case PLGS3:
			set_plunger_state( PLUNGER2, OPENED );
			break;
		case PLGS4:
			set_plunger_state( PLUNGER3, OPENED );
			break;						
		default:
			break;
	}
}


const CPARSE senspar[SNUM][NUM_ENTRIES] =
{
	{				/*	CLOSE_ST	*/
		{ OPENING_ST,	nil,		IX_CLOSE_TIME	},
		{ CLOSE_ST,		nil,		TNOTOUT			}
	},
	{				/*	OPENING_ST	*/
		{ CLOSE_ST,		nil,		TNOTOUT			},
		{ OPEN_ST,		inf_open,	TNOTOUT			}
	},
	{				/*	OPEN_ST	*/
		{ CLOSING_ST,	nil,		IX_OPEN_TIME	},
		{ OPEN_ST,		nil,		TNOTOUT			}
	},
	{				/*	CLOSING_ST	*/
		{ OPEN_ST,		nil,		TNOTOUT			},
		{ CLOSE_ST,		inf_close,	TNOTOUT			},
		
	},
};


