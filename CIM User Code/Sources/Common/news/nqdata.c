/*
 * qdata.c
 */

#include "nqdata.h"

NQUEUE nqueues[] =
{
	{	
		emer_queue,	emer_queue,	emer_queue,
		0, MAX_EMER_ELEMS 
	},
	
	{	
		cond_queue,	cond_queue,	cond_queue,
		0, MAX_CONDITIONAL_ELEMS 
	},
	
	{	
		news_queue,	news_queue,	news_queue,
		0, MAX_NEWS_ELEMS 
	}
};
