/*
 * 	dftypes.c
 * 		Data Flash types
 * 		definitions
 */

#include "mytypes.h"
#include "dftypes.h"

/*
 * 	Public initialized constant
 * 	variables
 */

/*
 * 	avail_flash:
 * 		Table of available dataflash
 */

#ifdef __DF_COMPLETE__

/*
 * 	Sizes of each block in sectors
 * 	and for each dataflash
 */

static const uchar bsc01[] = {	1, 31, 32 };
static const uchar bsc02[] = {	1, 31, 32, 64 };
static const uchar bsc04[] = {	1, 31, 32, 64, 64, 64 };
static const uchar bsc08[] = {	1, 31, 32, 64, 64, 64, 64, 64, 64, 64 };
static const uchar bsc16[] = {	1, 31, 32, 32, 32, 32, 32, 32,
								32, 32, 32, 32, 32, 32, 32, 32, 32 };
static const uchar bsc32[] = {	1, 63, 64, 64, 64, 64, 64, 64,
								64, 64, 64, 64, 64, 64, 64, 64, 64 };
static const uchar bsc64[] = { 	1, 31, 32, 32, 32, 32, 32, 32,
								32, 32, 32, 32, 32, 32, 32, 32,
								32, 32, 32, 32, 32, 32, 32, 32,
								32, 32, 32, 32, 32, 32, 32, 32, 32 };

DATAFL_T const avail_flash[ NUM_AVAIL_DATAFLASH ] =
{

/*		device_id	baddr_bits	size	sector_count	page_size	page_count	pbsc		Atmel number	*/

	{		0x03,		9,		1,			3,			264,		 512,		bsc01	},	/* AT45DB011B	1  Mbit	*/
	{   	0x05,		9,		2,			4,			264,		1024,		bsc02	},	/* AT45DB021B	2  Mbit	*/
	{   	0x07,		9,		4,			6,			264,		2048,		bsc04	},	/* AT45DB041B	4  Mbit	*/
	{   	0x09,		9,		8,			10,			264,		4096,		bsc08	},	/* AT45DB081B	8  Mbit	*/
    {   	0x0B,		10,		16,			17,			528,		4096,		bsc16	},	/* AT45DB161B	16 Mbit	*/
    {   	0x0D,		10,		32,			17,			528,		8192,		bsc32	},	/* AT45DB321B	32 Mbit	*/
	{   	0x0F,		11,		64,			33,			1056,		8192,		bsc64	},	/* AT45DB642	64 Mbit */
};

#else

/*
 * 	Sizes of each block in sectors
 * 	and for each dataflash
 */

static const uchar bsc16[] = {	1, 31, 32, 32, 32, 32, 32, 32,
								32, 32, 32, 32, 32, 32, 32, 32, 32 };
static const uchar bsc32[] = {	1, 63, 64, 64, 64, 64, 64, 64,
								64, 64, 64, 64, 64, 64, 64, 64, 64 };

DATAFL_T const avail_flash[ NUM_AVAIL_DATAFLASH ] =
{

/*		device_id	baddr_bits	size	sector_count	page_size	page_count	pbsc		Atmel number	*/

    {   	0x0B,		10,		16,			17,			528,		4096,		bsc16	},	/* AT45DB161B	16 Mbit	*/
    {   	0x0D,		10,		32,			17,			528,		8192,		bsc32	},	/* AT45DB321B	32 Mbit	*/
};

#endif


/*
 * 	Public uninitialized
 * 	variables
 */

const DATAFL_T *pdflash[ NUM_FLASH_DEVICES ];		/*	Pointer to dataflash detected	*/
MUInt order_num;
