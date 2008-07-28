/**
 *	gsqueue.c
 *		This module implements a generic simple queue manager.
 */

#include "gsqueue.h"
#include "qdata.h"
#include "enaint.h"

#define eoq()		(p->queue + p->num_elems)

MUInt
is_empty_gsqueue( QHDLR qh )
{
	MUInt qty;

	sem_disable();
	qty = gsqueues[ qh ].elem_qty;
	sem_enable();

	return ( qty == 0 );
}

MUInt
get_gsq_qty( QHDLR qh )
{
	MUInt qty;

	sem_disable();
	qty = gsqueues[ qh ].elem_qty;
	sem_enable();
	return qty;
}

MInt
remove_gsqueue( QHDLR qh, ELEM_TYPE *elem )
{
	GSQUEUE *p;

	p = &gsqueues[ qh ];

	if ( is_empty_gsqueue( qh ) )
		return -EMPTY_QUEUE;
	
	sem_disable();
	*elem = *p->pout;
	if( ++p->pout >= eoq() )
		p->pout = p->queue;
	--p->elem_qty;
	sem_enable();
	
	return OK_QUEUE;
}

#include "stack.h"
MInt
put_gsqueue( QHDLR qh, ELEM_TYPE elem )
{
	GSQUEUE *p;

	test_sp();
	p = &gsqueues[ qh ];

	sem_disable();
	if( p->elem_qty >= p->num_elems )
	{
		sem_enable();
		return -FULL_QUEUE;
	}

	*p->pin = elem;
	if( ++p->pin >= eoq() )
		p->pin = p->queue;
	++p->elem_qty;
	sem_enable();

	return OK_QUEUE;
}


void
deplete_gsqueue( QHDLR qh )
{
	GSQUEUE *p;

	p = &gsqueues[ qh ];

	sem_disable();
	p->elem_qty = 0;
	p->pin = p->pout = p->queue;
	sem_enable();

	return;
}
