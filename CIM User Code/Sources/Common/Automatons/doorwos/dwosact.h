/*
 * dwosact.h
 */

#ifndef __DWOSACT_H__
#define __DWOSACT_H__

#include "news.h"

/*
 * Define initialization routine
 */

void init_dwosctrl( void );

/*
 * Define action routines
 */

void lked2ulked_wos( NEWS_T *in );
void lked2pulking_wos( NEWS_T *in );
void pulking2ulked_wos( NEWS_T *in );
void pulking2ulking_wos( NEWS_T *in );
void pulking2lked_wos( NEWS_T *in );
void ulking2ulked_wos( NEWS_T *in );
void ulking2lked_wos( NEWS_T *in );
void ulked2lked_wos( NEWS_T *in );
void lked2lked_wos( NEWS_T *in );
void ulked2lking_wos( NEWS_T *in );
void lking2lked_wos( NEWS_T *in );

/*
 * Define state's preproccessor routines
 */

NARG_T dwos_ppro( const NEWS_T *in );

#endif
