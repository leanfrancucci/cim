/*
 * 	dataflsh.c
 * 		Upper layer for addressing
 * 		dataflash
 *
 * 		version 1.0 07/07/2007 11:53 ARG
 * 		version 2.0 08/19/2007 15:00 ARG
 *			Change argumets to poiter of structure
 *
 * 		Eduardo Martinez
 * 		Delsat Group S.A.
 */
#include <stdio.h>

/*
 * 	project includes
 */

#include "allinc.h"

#include "dataflsh.h"

#include "flash.h"
#include "dftypes.h"
#include "dfpwf.h"
#include "dfrefrsh.h"

/*
 * 	Defines
 */

#define DENSITY_POS				2
#define DENSITY_MASK			0x0f
#define FLASH_COMPARE			0x40

/*
 * 	Static variables
 */

/*
 * 	Static functions
 */

#ifndef	__RELEASE__

void
verify_block_address( DFLASH_T *pf	)
{
	if( pf->device >= NUM_FLASH_DEVICES )
		dataflash_fatal( BAD_DEVICE );
	if( pdflash[ pf->device ]->page_count/PAGE_NUM_PER_BLOCK <= pf->address)
		dataflash_fatal( BAD_ADDRESS );
}

void
verify_page_address( DFLASH_T *pf )
{
	if( pf->device >= NUM_FLASH_DEVICES )
		dataflash_fatal( BAD_DEVICE );
	if( pdflash[ pf->device ]->page_count <= pf->base_page + pf->page )
		dataflash_fatal( BAD_ADDRESS );
}

void
verify_byte_address( DFLASH_T *pf )
{
	if( pf->device >= NUM_FLASH_DEVICES )
		dataflash_fatal( BAD_DEVICE );
	if( pdflash[ pf->device ]->page_size <= pf->address )
		dataflash_fatal( BAD_ADDRESS );
}

#endif

/*
 * 	df_refresh
 * 		See if a page need to be refreshed
 * 		through 'must_refresh' which returns
 * 		page to be refreshed or a negative number
 * 		in case no refresh must be made
 * 		In affirmative case, calls autopage_rewrite
 */
static
int
df_refresh( MUInt device, uint page )
{
	int ref_page;

	if( ( ref_page = must_refresh( device, page ) ) >= 0 && ref_page != page )
	{
		dataflash_autopage_rewrite( device, ref_page );
		return 1;
	}
	return 0;
}


/*
 * 	dataflash_classify:
 * 		Receives density in 4 bits
 * 		Searches avail_flash for matching device_id
 * 		If found, sets pdflash pointing to table entry
 * 		and returns size in MBit
 * 		Else, sets pdflash to NULL and returns 0
 */

static
uint
dataflash_classify( MUInt dev_no, MUInt density )
{
	const DATAFL_T **pd;

	pd = &pdflash[ dev_no ];
	for( *pd = avail_flash, order_num = 0 ; order_num < NUM_AVAIL_DATAFLASH ; ++(*pd), ++order_num )
		if( (*pd)->device_id == density )
			return (*pd)->size;
	*pd = NULL;
	return 0;
}

/*
 * 	Public functions
 */

/*
 * 	dataflash_init:
 * 		Inits all data structures reated with this level
 * 		and calls to low level initializations
 */

void
dataflash_init( void )
{
	flash_init();
}


/*
 * 	dataflash_query_type:
 *		This function will read the flash and determine
 *		which part is being communicated with.
 *		Returns size of flash in Mbits
 *		or 0 if flash not found in internal table
 *		If found, a global variable (pdflash) points
 *		to table entry for this device
 */

uint
dataflash_query_type( MUInt dev_no )
{
	uchar density;

#ifndef	__RELEASE__
	if( dev_no >= NUM_FLASH_DEVICES )
		dataflash_fatal( BAD_DEVICE );
#endif
	flash_send_command( dev_no, STATUS_READ, DFNULL, DFNULL );
	flash_read( dev_no, &density, 1 );
	density = ( density >> DENSITY_POS ) & DENSITY_MASK;
	return dataflash_classify( dev_no, density );
}

/*
 * 	dataflash_page_size
 * 		Returns page size of 'dev_no'
 */

uint
dataflash_page_size( MUInt dev_no )
{
	return pdflash[ dev_no ]->page_size;
}

/*
 *	dataflash_page_num:
 *		Returns num_pages of 'dev_no'
 */

uint
dataflash_page_num( MUInt dev_no )
{
	return pdflash[ dev_no ]->page_count;
}

#ifdef __DFFULL__

/*
 * 	dataflash_read_continuous:
 * 		Reads 'qty' bytes from memory array continuously
 * 		from 'page_address' and 'byte_address'
 * 		in buffer pointed by 'prx'.
 * 		Reading can cross page boundaries and if last byte
 * 		of last page is hit, make a wraparound over page 0
 */

void
dataflash_read_continuous( DFLASH_T *pf )
{
#ifndef	__RELEASE__
	verify_page_address( pf );
	verify_byte_address( pf );
#endif
	flash_send_command( pf->device, CONT_AREAD, pf->base_page + pf->page, pf->address );
	flash_read( pf->device, pf->pbuff, pf->size );
}

/*
 * 	dataflash_main_memory_page_read:
 *		Reads from main memory 'qty' bytes
 *		beginning at 'page_address' and
 *		'byte_address' in buffer pointed by
 *		'prx'. If last byte of page is hit,
 *		make a wraparound over the same page
 */ 		

void
dataflash_main_memory_page_read( DFLASH_T *pf )
{
#ifndef	__RELEASE__
	verify_page_address( pf );
	verify_byte_address( pf );
#endif
	flash_send_command( pf->device, MM_PAGE_READ, pf->base_page + pf->page, pf->address );
	flash_read( pf->device, pf->pbuff, pf->size );
}

#endif

/*
 * 	dataflash_buffer_read:
 * 		Reads 'qty' bytes directly from one
 * 		of the two RAM buffers ('which'), beginning
 * 		in 'byte_address' and writing buffer
 * 		pointed by 'prx'. If last byte of
 * 		page is hit, makes a wraparound over
 * 		the beginning of buffer
 */

void
dataflash_buffer_read( DFLASH_T *pf )
{
#ifndef	__RELEASE__
	verify_byte_address( pf );
#endif
	flash_send_command( pf->device, pf->wbuff ? B2_READ : B1_READ, 0, pf->address );
	flash_read( pf->device, pf->pbuff, pf->size );
}

/*
 * 	dataflash_main_memory_page_to_buffer_xfer:
 * 		Transfers a memory 'page_address' in buffer
 * 		'which'
 */

void
dataflash_main_memory_page_to_buffer_xfer( DFLASH_T *pf )
{
#ifndef	__RELEASE__
	verify_page_address( pf );
#endif
	flash_send_command( pf->device, pf->wbuff ? MM2B2_XFER : MM2B1_XFER, pf->base_page + pf->page, 0 );
	flash_wait_ready( pf->device );
}

/*
 * 	dataflash_main_memory_page_to_buffer_compare:
 * 		Compares internal buffer 'which' with
 * 		main memory 'page_address'.
 * 		Returns 1 if comparison is OK,
 * 		0 otherwise
 */

MUInt
dataflash_main_memory_page_to_buffer_compare( DFLASH_T *pf )
{
	MUInt status;

#ifndef	__RELEASE__
	verify_page_address( pf );
#endif
	flash_send_command( pf->device, pf->wbuff ? MMCMP_B2 : MMCMP_B1, pf->base_page + pf->page, 0 );
	flash_wait_ready( pf->device );
	
	status = flash_read_status( pf->device );
	return !(status & FLASH_COMPARE);
}

/*
 * 	dataflash_buffer_write:
 * 		Writes 'qty' bytes from 'prx' into
 * 		buffer 'which'. If last byte of buffer
 * 		is hit, overflows to beginning of buffer.
 * 		Buffer positions not written retain
 * 		previous information
 */

void
dataflash_buffer_write( DFLASH_T *pf )
{
#ifndef	__RELEASE__
	verify_byte_address( pf );
#endif
	flash_send_command( pf->device, pf->wbuff ? B2_WRITE : B1_WRITE, 0, pf->address );
	flash_write( pf->device, pf->pbuff, pf->size );
}

/*
 * 	dataflash_buffer_to_main_memory_program:
 * 		Program with buffer 'which' contents to
 * 		main memory 'page_address' with builtin erase
 * 		if 'built_erase' != 0
 */

void
dataflash_buffer_to_main_memory_program( DFLASH_T *pf )
{
	MUInt command;
	uint page;

#ifndef	__RELEASE__
	verify_page_address( pf );
#endif

	disable_pwf_interrupt();
	
	command = pf->berase ?
			( pf->wbuff ? MMPRG_B2_BIN_ERASE 		: MMPRG_B1_BIN_ERASE ) 	:
			( pf->wbuff ? MMPRG_B2_WOUT_BIN_ERASE 	: MMPRG_B1_WOUT_BIN_ERASE );
	page = pf->base_page + pf->page;
	flash_send_command( pf->device, command, page, 0 );
	flash_wait_ready( pf->device );
	if( df_refresh( pf->device, page ) && pf->wbuff == 0 )
		dataflash_main_memory_page_to_buffer_xfer( pf );
	enable_pwf_interrupt();
}

#ifdef __DFFULL__

/*
 * 	dataflash_main_memory_page_program_through_buffer:
 * 		This function transfers 'qty' bytes from 'ptx'
 * 		into buffer 'which' beginning at 'byte_address'
 * 		If last byte of buffer is hit, overflows to first
 * 		byte of buffer.
 * 		After 'qty' bytes are written, 'page_address' of
 * 		main memory is erased and then programmed with buffer
 * 		contents
 */

void
dataflash_main_memory_page_program_through_buffer( DFLASH_T *pf )
{
	MUInt command;
	uint page;

#ifndef	__RELEASE__
	verify_page_address( pf );
#endif
	disable_pwf_interrupt();
	
#if 0
	flash_send_command( pf->device, pf->wbuff ? MMPRG_B2 : MMPRG_B1, pf->base_page + pf->page, pf->address);
#else
	flash_send_command( pf->device, pf->wbuff ? B2_WRITE : B1_WRITE, 0, pf->address );
	flash_write( pf->device, pf->pbuff, pf->size );

	command = pf->berase ?
			( pf->wbuff ? MMPRG_B2_BIN_ERASE 		: MMPRG_B1_BIN_ERASE ) 	:
			( pf->wbuff ? MMPRG_B2_WOUT_BIN_ERASE 	: MMPRG_B1_WOUT_BIN_ERASE );
	page = pf->base_page + pf->page;
	flash_send_command( pf->device, command, page, 0 );
	flash_wait_ready( pf->device );
#ifdef BOSTA
	if( df_refresh( pf->device, page ) && pf->wbuff == 0 )
		dataflash_main_memory_page_to_buffer_xfer( pf );
#else
	df_refresh( pf->device, page );
	dataflash_main_memory_page_to_buffer_xfer( pf );
#endif
#endif
	enable_pwf_interrupt();
}

/*
 * 	dataflash_page_erase:
 * 		Main memory 'page_address" is erased
 */

void
dataflash_page_erase( DFLASH_T *pf )
{
#ifndef	__RELEASE__
	verify_page_address( pf );
#endif
	flash_send_command( pf->device, PAGE_ERASE, pf->base_page + pf->page, 0 );
	flash_wait_ready( pf->device );
}

/*
 * 	dataflash_block_erase:
 * 		Main memory 'block_address' is erased
 */

void
dataflash_block_erase( DFLASH_T *pf )
{
#ifndef	__RELEASE__
	verify_block_address( pf );
#endif
	flash_send_command( pf->device, BLOCK_ERASE, pf->address << 3, 0 );
	flash_wait_ready( pf->device );
}

/*
 * 	dataflash_autopage_rewrite:
 * 		Refreshes the 'page_address' of main memory
 * 		First the contents of that page is written in
 * 		buffer 'which', then the contents of that page
 * 		are reprogrammed from buffer
 */

void
dataflash_autopage_rewrite( MUInt device, uint page )
{
	flash_send_command( device, APRW_B1, page, 0 );
	flash_wait_ready( device );
}

#endif

