/*
 * pwract.h
 */

#ifndef __PWRACT_H__
#define __PWRACT_H__

#include "news.h"

/*
 * Define initialization routine
 */

void init_pwrctrl( void );

/*
 * Define action routines
 */

void flash_sync( NEWS_T *in );
void ext2backup( NEWS_T *in );
void chrg2nchrg( NEWS_T *in );
void checkbatt( NEWS_T *in );
void nchrg2chrg( NEWS_T *in );
void batt2nbatt( NEWS_T *in );
void ext2ext( NEWS_T *in );
void backup2ext( NEWS_T *in );
void back_bmes( NEWS_T *in );
void shtdwn( NEWS_T *ix );

/*
 * Define state's preproccessor routines
 */

NARG_T pwrctrl_ppro( const NEWS_T *in );

#endif
