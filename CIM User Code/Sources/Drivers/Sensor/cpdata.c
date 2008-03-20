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

#undef LOG_SENSORS
#ifdef LOG_SENSORS
#define log_val1stck(a)	(PTFD_PTFD0=a)
#define log_val2stck(a)	(PTFD_PTFD1=a)
#define log_keys(a)		(PTFD_PTFD2=a)
#define log_sens(a)		(PTFD_PTFD3=a)
#else
#define log_val1stck(a)	
#define log_val2stck(a)
#define log_keys(a)	
#define log_sens(a)
#endif

/*
 * Validators pseudo news definitions
 * 	this devices dont inform events thru the news queue,
 * 	sets status flag in GRSTATUS struct.
 */
enum
{
   OPEN_VAL1STCK_IX,
   CLOSE_VAL1STCK_IX,
   OPEN_VAL2STCK_IX,
   CLOSE_VAL2STCK_IX
};

const short sens_news[MAX_PARSERS][NUM_NEWSTYPE] = 
{
	{ UNLOCKED_DOOR1_IX,	LOCKED_DOOR1_IX },
	{ OPEN_DOOR1_IX,  		CLOSE_DOOR1_IX  },
	{ UNLOCKED_DOOR2_IX,	LOCKED_DOOR2_IX },
	{ OPEN_DOOR2_IX,  		CLOSE_DOOR2_IX  },
	{ OPEN_VAL1STCK_IX,		CLOSE_VAL1STCK_IX },
	{ OPEN_VAL2STCK_IX,		CLOSE_VAL2STCK_IX },
	{ OPEN_KEYS_D1_IX,		CLOSE_KEYS_D1_IX }
};

const short sens_tout[MAX_PARSERS][NUM_TIMEVAL] = 
{
	{ LOCKS1_OPENT, LOCKS1_CLOSET },
	{ PLGS1_OPENT,  PLGS1_CLOSET  },
	{ LOCKS2_OPENT, LOCKS2_CLOSET },
	{ PLGS2_OPENT,  PLGS2_CLOSET  },
	{ VAL1STCK_OPENT, VAL1STCK_CLOSET },
	{ VAL2STCK_OPENT, VAL2STCK_CLOSET },
	{ KEYSWITCH_OPENT,  KEYSWITCH_CLOSET  }
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
			log_val1stck(0);
			break;
		case VAL2STCK:
			set_stacker_state( STACKER1, INSTALLED );
			log_val2stck(0);
			break;
		case KEYSWITCH:
			put_nqueue( NEWS_QUEUE, def_news[CLOSE_KEYS_D1_IX] );
			put_nqueue( NEWS_QUEUE, def_news[CLOSE_KEYS_D2_IX] );
			log_keys(0);
			break;
		default:
			put_nqueue( NEWS_QUEUE, 
				def_news[sens_news[which_sensor()][CLOSE_NEWS]] );
			log_sens(0);
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
			log_val1stck(1);
			break;
		case VAL2STCK:
			set_stacker_state( STACKER1, REMOVED );
			log_val2stck(1);
			break;
		case KEYSWITCH:
			put_nqueue( NEWS_QUEUE, def_news[OPEN_KEYS_D1_IX] );
			put_nqueue( NEWS_QUEUE, def_news[OPEN_KEYS_D2_IX] );
			log_keys(1);
			break;
		default:
			put_nqueue( NEWS_QUEUE, 
				def_news[sens_news[which_sensor()][OPEN_NEWS]] );
			log_sens(1);
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


