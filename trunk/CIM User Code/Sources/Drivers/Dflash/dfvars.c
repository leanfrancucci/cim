/*
 * 	dfvars.c
 * 		Global variables
 */

#include "mytypes.h"

#include "vaultdef.h"
#include "dfvars.h"

ushort page_eff_size;
ushort df_num_pages;
ushort fs_num_pages;
ushort num_users_per_page;
uchar emerg;
ushort page_size;
uchar user_buff[ WPAGE_SIZE ];
uchar fs_buff[ WPAGE_SIZE ];
const ushort fd2bit[ 16 ] =
{
	0x0001, 0x0002, 0x0004, 0x0008,
	0x0010, 0x0020, 0x0040, 0x0080,
	0x0100, 0x2000, 0x0400, 0x0800,
	0x1000, 0x2000, 0x4000, 0x8000
};
uchar mem_status;