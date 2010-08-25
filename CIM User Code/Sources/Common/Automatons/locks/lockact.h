/*
 * lockact.h
 */

#ifndef __LOCKACT_H__
#define __LOCKACT_H__

#include "news.h"

/*
 * Define initialization routine
 */

void init_lock0( void );
void init_lock1( void );
void init_lock2( void );
void init_lock3( void );

/*
 * Define action routines
 */

void chk_keysw( NEWS_T *in );
void feed_lock( NEWS_T *in );
void release_lock( NEWS_T *in );

/*
 * Define state's preproccessor routines
 */

NARG_T lock_ppro( const NEWS_T *in );

#endif
