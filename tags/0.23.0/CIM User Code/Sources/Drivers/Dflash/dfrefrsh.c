/*
 * 		dfrefrsh.c
 * 			Module to make data flash refresh
 * 			as stated in AN-4 of Atmel Corp.
 *
 * 			Eduardo A. Martinez
 * 			DelsatGroup S.A.
 *
 * 			First editiom: 2/23/2008
 */

#include "mytypes.h"
#include "allinc.h"
#include "dfrefrsh.h"
#include "dftypes.h"

/*
 * 	Static variables
 */



static uint page_counters[ NUM_FLASH_DEVICES ][ NUM_SECTORS ];

/*
 * 	must_refresh:
 * 		This function only tests if refresh
 * 		must be done in 'page' of 'device'
 * 		If so, returns page to be refreshed,
 * 		else returns negative number.
 * 		Implemented according to first method
 * 		of referred Application Note AN-4
 */

int
must_refresh( MUInt device, int page )
{
	const uchar *p;
	MUInt sec_count;
	uint accu_page, next_border, ref_page;

	p = avail_flash[ order_num ].pbsc;
	for( sec_count = accu_page = 0 ; sec_count < avail_flash[ order_num ].sector_count ; ++p, ++sec_count )
	{
		next_border = accu_page + *p * PAGE_NUM_PER_BLOCK;
		if( page >= accu_page && page < next_border )
		{
			ref_page = accu_page + page_counters[ device ][ sec_count ]++;
			if( page_counters[ device ][ sec_count ] >= *p * PAGE_NUM_PER_BLOCK )
				page_counters[ device ][ sec_count ] = 0;
			return ref_page;
		}
		accu_page = next_border;
	}
	return NO_REFRESH;
}


