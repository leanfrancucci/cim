/*
 * mparser.h
 */

#ifndef __MPARSER_H__
#define __MPARSER_H__

#include "mytypes.h"
#include "news.h"


#define MPANY		0xFF

typedef void (*INIT_ACT)( void );
typedef void (*ACT)( const NEWS_T *in );
typedef NARG_T (*PREPRO)( const NEWS_T *in );

typedef struct trans_t
{
	NARG_T news;
	const struct state_t *const next_state;
	ACT action;
} TRAN;

typedef struct state_t
{
	const TRAN *const trans;
	PREPRO ppro;
} STATE;

typedef struct
{
	const STATE *const init_state;
	const STATE *state;
	INIT_ACT init_action;
} MPARSE;

void exec_mparser( const NEWS_T *pnews );
void init_mparsers( MUInt hp );

#endif
