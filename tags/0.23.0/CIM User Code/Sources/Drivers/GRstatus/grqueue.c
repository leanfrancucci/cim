/**
 *	grqueue.c
 *		This module implements a generic simple queue manager.
 */

#include "grqueue.h"
#include "enaint.h"

#define eoq()		(p->queue + p->num_elems)

static GR1ST_T grq[GRQ_MAX_ELEMENTS];

static GRQUEUE grqueue = 
{
	(GR1ST_T*)grq,
	(GR1ST_T*)grq,
	(GR1ST_T*)grq,
	0, GRQ_MAX_ELEMENTS
};

MUInt
is_empty_grqueue( void )
{
	MUInt qty;

	sem_disable();
	qty = grqueue.elem_qty;
	sem_enable();

	return ( qty == 0 );
}

MUInt
get_grq_qty( void )
{
	MUInt qty;

	sem_disable();
	qty = grqueue.elem_qty;
	sem_enable();
	return qty;
}

MInt
remove_grqueue( GR1ST_T *elem )
{
	GRQUEUE *p;

	p = &grqueue;

	if ( is_empty_grqueue() )
		return -EMPTY_GQUEUE;
	
	sem_disable();
	*elem = *p->pout;
	if( ++p->pout >= eoq() )
		p->pout = p->queue;
	--p->elem_qty;
	sem_enable();
	
	return OK_GQUEUE;
}

MInt
put_grqueue( GR1ST_T *elem )
{
	GRQUEUE *p;

	p = &grqueue;

	sem_disable();
	if( p->elem_qty >= p->num_elems )
	{
		sem_enable();
		return -FULL_GQUEUE;
	}

	*p->pin = *elem;
	if( ++p->pin >= eoq() )
		p->pin = p->queue;
	++p->elem_qty;
	sem_enable();

	return OK_GQUEUE;
}


void
deplete_grqueue( void )
{
	GRQUEUE *p;

	p = &grqueue;

	sem_disable();
	p->elem_qty = 0;
	p->pin = p->pout = p->queue;
	sem_enable();

	return;
}
