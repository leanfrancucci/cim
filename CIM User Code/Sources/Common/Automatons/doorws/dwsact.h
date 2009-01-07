/*
 * dwsact.h
 */

#ifndef __DWSACT_H__
#define __DWSACT_H__

#include "news.h"

/*
 * Define initialization routine
 */

void init_dwsctrl( void );

/*
 * Define action routines
 */

void inform_open_ws( NEWS_T *in );
void inform_close_ws( NEWS_T *in );
void lked2pulking_ws( NEWS_T *in );
void lked2unlked_ws( NEWS_T *in );
void pulking2ulking_ws( NEWS_T *in );
void pulking2lked_ws( NEWS_T *in );
void pulking2ulked_ws( NEWS_T *in );
void lock_and_wait( NEWS_T *in );
void ulking2lked_ws( NEWS_T *in );
void ulking2ulked_ws( NEWS_T *in );
void ulked2lking_ws( NEWS_T *in );
void ulked2lked_ws( NEWS_T *in );
void lking2lked_ws( NEWS_T *in );

/*
 * Define state's preproccessor routines
 */

NARG_T dws_ppro( const NEWS_T *in );

#endif
