/*
 * valact.h
 */

#ifndef __VALACT_H__
#define __VALACT_H__

#include "news.h"
#include "mytypes.h"

/*
 * Define initialization routine
 */

void init_val( void );

/*
 * Define action routines
 */

void init_vframe( const NEWS_T *in );
void store_vdata( const NEWS_T *in );
void rec_vframe( const NEWS_T *in );
void set_start_rcv( const NEWS_T *in );
void drop_vframe( const NEWS_T *in );
	
/*
 * Define state's preprocessor routines
 */

NARG_T vblocked_ppro( const NEWS_T *in );
NARG_T idle_val_ppro( const NEWS_T *in );
NARG_T in_frame_ppro( const NEWS_T *in );


#endif
