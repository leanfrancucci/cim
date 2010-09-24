/*
 * news.c
 */

#include "news.h"
#include "mpdata.h"
#include "nqueue.h"

const NEWS_T def_news[] =
{
	{	CT_PROT,		TIM,		TBET_BYTE	},
	{	CT_PROT,		TIM,		TXMITCT0	},
	{	CT_PROT,		TIM,		TXMITCT1	},
	{	CT_PROT,		PROT,		VALSENT		},

	{	VAL_PROT,		PROT,		VALBLOCK	},
	{	VAL_PROT,		PROT,		VALUNBLOCK	},
	{	CT_PROT,		PROT,		VALRESP		},	
	{	VAL_PROT,		PROT,		TOUT_VAL	},
	{	CT_PROT,		TIM,		TINACTIVITY	},	

	{	LOCK0_CTRL,		TIM,		TOUT_DOOR	},
	{	LOCK0_CTRL,		TIM,		TOUT_ULE	},
	{	LOCK0_CTRL,		PROT,		LOCK_DOOR	},
	{	LOCK0_CTRL,		PROT,		UNLOCK_DOOR	},
	{	LOCK0_CTRL,		LOCKER,		OPEN_KEYS	},
	{	LOCK0_CTRL,		LOCKER,		CLOSE_KEYS	},

	{	LOCK1_CTRL,		TIM,		TOUT_DOOR	},
	{	LOCK1_CTRL,		TIM,		TOUT_ULE	},
	{	LOCK1_CTRL,		PROT,		LOCK_DOOR	},
	{	LOCK1_CTRL,		PROT,		UNLOCK_DOOR	},
	{	LOCK1_CTRL,		LOCKER,		OPEN_KEYS	},
	{	LOCK1_CTRL,		LOCKER,		CLOSE_KEYS	},

	{	LOCK2_CTRL,		TIM,		TOUT_DOOR	},
	{	LOCK2_CTRL,		TIM,		TOUT_ULE	},
	{	LOCK2_CTRL,		PROT,		LOCK_DOOR	},
	{	LOCK2_CTRL,		PROT,		UNLOCK_DOOR	},
	{	LOCK2_CTRL,		LOCKER,		OPEN_KEYS	},
	{	LOCK2_CTRL,		LOCKER,		CLOSE_KEYS	},
	
	{	LOCK3_CTRL,		TIM,		TOUT_DOOR	},
	{	LOCK3_CTRL,		TIM,		TOUT_ULE	},
	{	LOCK3_CTRL,		PROT,		LOCK_DOOR	},
	{	LOCK3_CTRL,		PROT,		UNLOCK_DOOR	},
	{	LOCK3_CTRL,		LOCKER,		OPEN_KEYS	},
	{	LOCK3_CTRL,		LOCKER,		CLOSE_KEYS	},

	{	PWRCTRL_CTRL,	PWRCTRL,	PWR_FAIL	},
	{	PWRCTRL_CTRL,	PWRCTRL,	PWR_OK		},
	{	PWRCTRL_CTRL,	PWRCTRL,	SHTDWN		},
	{	PWRCTRL_CTRL,	PWRCTRL,	BATT_REM	},
	{	PWRCTRL_CTRL,	PWRCTRL,	BATT_OK		},
	{	PWRCTRL_CTRL,	TIM,		TOUT_PWR	},
	{	PWRCTRL_CTRL,	TIM,		TOUT_FSYNC	}


};

/*
 * Maintain the emergency news to be
 * removed as soon as possible.
 */
NEWS_T emer_queue[ MAX_EMER_ELEMS ];

/*
 * Maintain the conditional news enqueued
 * by automaton to make a conditional operation.
 */
NEWS_T cond_queue[ MAX_CONDITIONAL_ELEMS ];

/*
 * Maintain the news enqueued by lower levels.
 */
NEWS_T news_queue[ MAX_NEWS_ELEMS ];

