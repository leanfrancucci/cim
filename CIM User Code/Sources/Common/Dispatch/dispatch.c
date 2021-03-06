/*
 * dispatch.c
 */

#include "nqdata.h"
#include "mparser.h"
#include "news.h"
#include "mpdata.h"
#include "dfsys.h"

/*
 * init_dispatcher:
 */

void
init_dispatcher( void )
{
	init_mparsers( CT_PROT );
	init_mparsers( VAL_PROT );
	init_mparsers( LOCK0_CTRL );
	init_mparsers( LOCK1_CTRL );
	init_mparsers( LOCK2_CTRL );
	init_mparsers( LOCK3_CTRL );
}

/*
 * dispatcher:
 */
void
dispatcher( void )
{
	NEWS_T news;
	
	while( remove_nqueue( EMER_QUEUE, &news ) >= 0 )
	{	
		if( news.news == TOUT_FSYNC )
			dfilesys_sync();
		else
			exec_mparser( &news );
	}
	if( remove_nqueue( COND_QUEUE, &news ) >= 0 )
		exec_mparser( &news );
	if( remove_nqueue( NEWS_QUEUE, &news ) >= 0 )
		exec_mparser( &news );
}
