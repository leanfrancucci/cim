/*
 * alphact.h
 */

#ifndef __ALPHACT_H__
#define __ALPHACT_H__

#include "news.h"


/*
 * Define initialization routine
 */

void init_prot( void );

/*
 * Define action routines
 */

void init_frame( const NEWS_T *in );
void master_restart( const NEWS_T *in );
void store_dev( const NEWS_T *in );
void store_cmd( const NEWS_T *in );
void in_numdata( const NEWS_T *in );
void store_numdata( const NEWS_T *in );
void store_data( const NEWS_T *in );
void rec_frame( const NEWS_T *in );
void drop_frame( const NEWS_T *in );
void send_nack( const NEWS_T *in );
void send_resp( const NEWS_T *in );
void block_val( const NEWS_T *in );
void toidle( const NEWS_T *in );

/*
 * Define state's preprocessor routines
 */

NARG_T idle_ppro( const NEWS_T *in );
NARG_T wait_dev_ppro( const NEWS_T *in );
NARG_T wait_cmd_ppro( const NEWS_T *in );
NARG_T wait_ndata_ppro( const NEWS_T *in );
NARG_T in_data_ppro( const NEWS_T *in );
NARG_T pframe_ppro( const NEWS_T *in );
NARG_T wait_vresp_ppro( const NEWS_T *in );


#endif
