/*
 * 		flash.h
 * 			Lower level dataflash functions
 */

#include "mytypes.h"

/*
 * 	Opcodes internals
 */

enum
{
	BLOCK_ERASE,
	MM_PAGE_READ,
	MM2B1_XFER,
	B1_READ,
	MM2B2_XFER,	
	B2_READ,		
	STATUS_READ,	
	APRW_B1,		
	APRW_B2,		
	MMCMP_B1,
	MMCMP_B2,	
	CONT_AREAD,	
	PAGE_ERASE,	
	MMPRG_B1,	
	MMPRG_B1_BIN_ERASE,
	B1_WRITE,
	MMPRG_B2,		  
	MMPRG_B2_BIN_ERASE,
	B2_WRITE,
	MMPRG_B1_WOUT_BIN_ERASE,
	MMPRG_B2_WOUT_BIN_ERASE,

	DF_NUM_OPCODES,
};


/*	For page or byte address invalid	*/

#define DFNULL	-1

/*
 * 		Public functions
 */

/*
 * 	flash_init:
 * 		Inits whatever needed and
 * 		calls lower level to initialize
 */

void flash_init( void );

/*
 * 	flash_send_command:
 *		This function is a generic function that setups
 *		the correct command to the Atmel flash part.
 *		First, the device is selected,
 *		Then the command is issued
 *		If, after the command, something must be read or written,
 *		CS is left active, otherwise the device is deselected
 */

void flash_send_command( MUInt dev_no, MUInt command, int page_address, int byte_address );

/*
 * 	flash_read_status:
 * 		Flash command STATUS_READ is issued
 * 		and status is returned
 */

MUInt flash_read_status( MUInt dev_no );

/*
 * 	flash_wait_ready:
 * 		Waits till flash is ready
 */

void flash_wait_ready( MUInt dev_no );

/*
 * 	flash_read:
 * 		Reads 'qty' bytes from SPI into 'prx'
 * 		Then deselects device
 */

void flash_read( MUInt dev_no, uchar *prx, uint qty );

/*
 * 	flash_write:
 * 		Writes 'qty' bytes into SPI from 'ptx'
 * 		Then deselects device
 */

void flash_write( MUInt dev_no, uchar *ptx, uint qty );

