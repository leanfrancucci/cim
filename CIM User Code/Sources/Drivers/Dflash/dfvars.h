/*
 * 	dfvars.h
 * 		Global variables
 */

#include "mytypes.h"

extern ushort page_eff_size;
extern ushort df_num_pages;
extern ushort fs_num_pages;
extern ushort num_users_per_page;
extern uchar emerg;
extern ushort page_size;
extern uchar user_buff[ WPAGE_SIZE ];
extern uchar fs_buff[ WPAGE_SIZE ];
extern const ushort fd2bit[ 16 ];
extern uchar mem_status;
