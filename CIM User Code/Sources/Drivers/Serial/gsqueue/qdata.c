/**
 * qdata.c
 */

#include "qdata.h"

GSQUEUE gsqueues[] =
{
	{
		(ELEM_TYPE*)hostrx_queue,
		(ELEM_TYPE*)hostrx_queue,
		(ELEM_TYPE*)hostrx_queue,
		0, HOSTRX_MAX_CH
	},

	{
		(ELEM_TYPE*)valrx_queue,
		(ELEM_TYPE*)valrx_queue,
		(ELEM_TYPE*)valrx_queue,
		0, VALRX_MAX_CH
	},	
};
