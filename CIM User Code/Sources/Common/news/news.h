/*
 * news.h
 */

#ifndef __NEWS_H__
#define __NEWS_H__


#define MAX_EMER_ELEMS				2
#define MAX_CONDITIONAL_ELEMS		4
#define MAX_NEWS_ELEMS				8

typedef unsigned char NARG_T;

typedef struct
{
	unsigned who:3;
	unsigned news_fly:5;
	NARG_T news;
} NEWS_T;

#define BROADCAST					0x7

/*
 * Here are defined the news used by any automata to parse.
 * Generally, these news are delivered by state's preprocessor.
 * Every news is composed by two parameters, group or family
 * and news to parse, according to 'news_fly' and 'news' fields
 * of NEWS_T structure.
 */

/*
 * News family
 */

enum
{
	PROT, UART, TIM, LOCKER, PLUNGER, PWRCTRL, COMMON
};

/*
 * News to parse
 */

enum
{
	/* PROT family */
	
	MARK, 
	DATA_VALID, 
	NDATA_VALID,
	DATA_LEN,
	UNKNOWN_DATA, 
	END_PAYLOAD, 
	END_FRAME,
	INVALID_DATA,
	SYNC,
	END_VFRAME,
	LOCK_DOOR,
	UNLOCK_DOOR,
	VALSENT,
	VALBLOCK,
	VALUNBLOCK,
	VALRESP,

	/* Timer family */

	TBET_BYTE,
	TXMITCT0,
	TXMITCT1,
	TINACTIVITY,
	TOUT_DOOR,
	TOUT_ULE,
	TOUT_VAL,

	/* Plunger family */

	OPEN_DOOR,
	CLOSE_DOOR,

	/* Locker family */

	LOCKED,
	UNLOCKED,
	OPEN_KEYS,
	CLOSE_KEYS,
	TO_LOCKED,
	TO_UNLOCKED,

	/* Power Control family */
	
	PWR_FAIL,
	PWR_OK,
	BATT_REM,
	BATT_OK,
	SHTDWN,
	TOUT_PWR,
	TOUT_FSYNC,

	/* Common family */
	
	UNKNOWN
};

/*
 * Here are defined all news to enqueue from any process, 
 * automata or isr. These news are perfectly linked in 
 * 'def_news' array and composed by three parameters: process, 
 * group or family and news to parse, according to 'who', 
 * 'news_fly' and 'news' fields from NEWS_T structure.
 */
enum
{
	TBET_BYTE_IX,
	TXMITCT0_IX,
	TXMITCT1_IX,
	VALSENT_IX,

	VALBLOCK_IX,
	VALUNBLOCK_IX,
	VALRESP_IX,	
	TOUT_VAL_IX,
	TINACTIVITY_IX,	

	TOUT_LOCK0_IX,
	TOUTUE_LOCK0_IX,
	LOCK_DOOR1_IX,
	UNLOCK_DOOR1_IX,
	OPEN_KEYS_D1_IX,
	CLOSE_KEYS_D1_IX,

	TOUT_LOCK1_IX,
	TOUTUE_LOCK1_IX,
	LOCK_DOOR2_IX,
	UNLOCK_DOOR2_IX,
	OPEN_KEYS_D2_IX,
	CLOSE_KEYS_D2_IX,

	TOUT_LOCK2_IX,
	TOUTUE_LOCK2_IX,
	LOCK_DOOR3_IX,
	UNLOCK_DOOR3_IX,
	OPEN_KEYS_D3_IX,
	CLOSE_KEYS_D3_IX,

	TOUT_LOCK3_IX,
	TOUTUE_LOCK3_IX,
	LOCK_DOOR4_IX,
	UNLOCK_DOOR4_IX,
	OPEN_KEYS_D4_IX,
	CLOSE_KEYS_D4_IX,

	PWR_FAIL_IX,
	PWR_OK_IX,
	SHTDWN_IX,
	BATT_REM_IX,
	BATT_OK_IX,
	TOUT_PWR_IX,
	TOUT_FSYNC_IX,

	NUM_DEF_NEWS
};

/*
 * Maintain the emergency news to be
 * removed as soon as possible.
 */
extern NEWS_T emer_queue[ MAX_EMER_ELEMS ];

/*
 * Maintain the conditional news enqueued
 * by automaton to make a conditional operation.
 */
extern NEWS_T cond_queue[ MAX_CONDITIONAL_ELEMS ];

/*
 * Maintain the news enqueued by lower levels.
 */
extern NEWS_T news_queue[ MAX_NEWS_ELEMS ];

/*
 * This news family is delivered to all process or automata
 * declared.
 */

extern const NEWS_T def_news[ NUM_DEF_NEWS ];

#endif
