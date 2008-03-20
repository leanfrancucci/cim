/*
 * 	dftypes.h
 * 		Data Flash types
 * 		definitions
 */

#include "mytypes.h"
#include "vaultdef.h"

/*
 * 	Structure type for
 * 	dataflash definitions
 */

typedef struct
{
			uchar	device_id;
			uchar	baddr_bits;
			ushort	size;			/*	In Megabit	*/
			uchar	sector_count;
			uint	page_size;
			uint	page_count;
	const	uchar	*pbsc;		/*	Pointer to block sector count	*/
} DATAFL_T;

#define PAGE_NUM_PER_BLOCK		8


#ifdef __DF_COMPLETE__

	#define NUM_AVAIL_DATAFLASH		2
	#define NUM_SECTORS				33

#else

	#define NUM_AVAIL_DATAFLASH		7
	#define NUM_SECTORS				17

#endif

extern DATAFL_T const avail_flash[ NUM_AVAIL_DATAFLASH ];   /*	Table of available dataflash	*/

extern const DATAFL_T *pdflash[ NUM_FLASH_DEVICES ];		/*	Pointer to dataflash detected	*/
extern MUInt order_num;										/*	Index to dataflash array		*/

