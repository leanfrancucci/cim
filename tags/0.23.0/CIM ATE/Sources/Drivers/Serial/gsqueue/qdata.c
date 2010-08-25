/**
 * qdata.c
 */

#include "qdata.h"

GSQUEUE gsqueues[] =
{
	{
		(ELEM_TYPE*)com1rx_queue,
		(ELEM_TYPE*)com1rx_queue,
		(ELEM_TYPE*)com1rx_queue,
		0, COM1_MAX_CH
	},

	{
		(ELEM_TYPE*)com2rx_queue,
		(ELEM_TYPE*)com2rx_queue,
		(ELEM_TYPE*)com2rx_queue,
		0, COM2_MAX_CH
	},	
};
