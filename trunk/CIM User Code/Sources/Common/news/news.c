/*
 * news.c
 */

#include "news.h"
#include "mpdata.h"
#include "nqueue.h"

const NEWS_T def_news[] =
{
	{	CT_PROT,			TIM,		TBET_BYTE	},
	{	CT_PROT,			TIM,		TXMITCT0	},
	{	CT_PROT,			TIM,		TXMITCT1	},
	{	CT_PROT,			PROT,		VALSENT		},

	{	VAL_PROT,			PROT,		VALBLOCK	},
	{	VAL_PROT,			PROT,		VALUNBLOCK	},
	{	CT_PROT,			PROT,		VALRESP		},	
	{	VAL_PROT,			PROT,		TOUT_VAL	},
	{	CT_PROT,			TIM,		TINACTIVITY	},	

	{	DOORWS_CTRL,		TIM,		TOUT_DOOR	},
	{	DOORWS_CTRL,		TIM,		TOUT_ULE	},
	{	DOORWS_CTRL,		PLUNGER,	OPEN_DOOR	},
	{	DOORWS_CTRL,		PLUNGER,	CLOSE_DOOR	},
	{	DOORWS_CTRL,		LOCKER,		LOCKED		},
	{	DOORWS_CTRL,		LOCKER,		UNLOCKED	},
	{	DOORWS_CTRL,		PROT,		LOCK_DOOR	},
	{	DOORWS_CTRL,		PROT,		UNLOCK_DOOR	},
	{	DOORWS_CTRL,		LOCKER,		OPEN_KEYS	},
	{	DOORWS_CTRL,		LOCKER,		CLOSE_KEYS	},
	{	DOORWS_CTRL,		LOCKER,		TO_LOCKED	},
	{	DOORWS_CTRL,		LOCKER,		TO_UNLOCKED	},

	{	DOORWOS_CTRL,		TIM,		TOUT_DOOR	},
	{	DOORWOS_CTRL,		TIM,		TOUT_ULE	},
	{	DOORWOS_CTRL,		PLUNGER,	OPEN_DOOR	},
	{	DOORWOS_CTRL,		PLUNGER,	CLOSE_DOOR	},
	{	DOORWOS_CTRL,		LOCKER,		LOCKED		},
	{	DOORWOS_CTRL,		LOCKER,		UNLOCKED	},
	{	DOORWOS_CTRL,		PROT,		LOCK_DOOR	},
	{	DOORWOS_CTRL,		PROT,		UNLOCK_DOOR	},
	{	DOORWOS_CTRL,		LOCKER,		OPEN_KEYS	},
	{	DOORWOS_CTRL,		LOCKER,		CLOSE_KEYS	},
	{	DOORWOS_CTRL,		LOCKER,		TO_LOCKED	},
	{	DOORWOS_CTRL,		LOCKER,		TO_UNLOCKED	},

	{	PWRCTRL_CTRL,		PWRCTRL,	PWR_FAIL	},
	{	PWRCTRL_CTRL,		PWRCTRL,	PWR_OK		},
	{	PWRCTRL_CTRL,		PWRCTRL,	SHTDWN		},
	{	PWRCTRL_CTRL,		PWRCTRL,	BATT_REM	},
	{	PWRCTRL_CTRL,		PWRCTRL,	BATT_OK		},
	{	PWRCTRL_CTRL,		TIM,		TOUT_PWR	},
	{	PWRCTRL_CTRL,		TIM,		TOUT_FSYNC	}


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

