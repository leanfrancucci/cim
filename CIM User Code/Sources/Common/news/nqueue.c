/*
 *	nqueue.c
 */

#include "nqueue.h"
#include "nqdata.h"
#include "enaint.h"

#define eoq()		(p->queue + p->num_elems)

MUInt
is_empty_nqueue( MUInt qh )
{
	return nqueues[ qh ].elem_qty == 0;
}

MInt
remove_nqueue( MUInt qh, NEWS_T *elem )
{
	NQUEUE *p;

	p = &nqueues[ qh ];
	if ( is_empty_nqueue( qh ) )
		return -EMPTY_NQUEUE;

	sem_disable();
	*elem = *p->pout;
	if( ++p->pout >= eoq() )
		p->pout = p->queue;
	--p->elem_qty;
	sem_enable();
	
	return OK_NQUEUE;
}

#include "stack.h"

MInt
put_nqueue( MUInt qh, NEWS_T elem )
{
	NQUEUE *p;

	p = &nqueues[ qh ];
	if( p->elem_qty >= p->num_elems )
		return -FULL_NQUEUE;

	sem_disable();
	test_sp();
	*p->pin = elem;
	if( ++p->pin >= eoq() )
		p->pin = p->queue;
	++p->elem_qty;
	sem_enable();

	return OK_NQUEUE;
}
