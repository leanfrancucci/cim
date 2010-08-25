/*
 * mparser.c
 * 	
 * 		This module implements a autumata management.
 */

#include "mparser.h"
#include "mpdata.h"
#include "news.h"
#include "mytypes.h"

#include <stdlib.h>

static NARG_T in;

static
void
do_mparse( const NEWS_T *pnews )
{
	MPARSE *p;
	const STATE *q;
	const TRAN *s;

	p = &mparsers[ pnews->who ];
	q = p->state;
	
	in = q->ppro != NULL ? (*q->ppro)( pnews ) : pnews->news;
	for( s = p->state->trans; s->news != in && s->news != MPANY; ++s )
		;
	p->state = s->next_state;
	if( s->action != NULL )
		(*s->action)( pnews );
}

void
exec_mparser( const NEWS_T *pnews )
{
	MUInt hp;
	
	if( pnews->who == BROADCAST )
		for( hp = 0; hp < NUM_MPARSERS - 1; ++hp )
			do_mparse( pnews );
	else
		do_mparse( pnews );
}

void
init_mparsers( MUInt hp )
{
	MPARSE *p;

	p = &mparsers[ hp ];
	
	p->state = p->init_state;
	if( p->init_action != NULL )
		(*p->init_action)();
}
