/*
 * 	dataflsh.h
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

#include "mytypes.h"

#define __DFFULL__

enum
{
	NO_BINERASE, BINERASE
};

/*
 * 	dataflash_init:
 * 		Inits all data structures reated with this level
 * 		and calls to low level initializations
 */

void dataflash_init( void );

/*
 * 	dataflash_query_type:
 *		This function will read the flash and determine
 *		which part is being communicated with.
 *		Returns size of flash in Mbits
 *		or 0 if flash not found in internal table
 *		If found, a global variable (pdflash) points
 *		to table entry for this device
 */

uint dataflash_query_type( MUInt dev_no );

/*
 * 	dataflash_page_size
 * 		Returns page size of 'dev_no'
 */

uint dataflash_page_size( MUInt dev_no );

/*
 *	dataflash_page_num:
 *		Returns num_pages of 'dev_no'
 */

uint dataflash_page_num( MUInt dev_no );

#ifdef __DFFULL__

/*
 * 	dataflash_read_continuous:
 * 		Reads 'qty' bytes from memory array continuously
 * 		from 'page_address' and 'byte_address'
 * 		in buffer pointed by 'prx'.
 * 		Reading can cross page boundaries and if last byte
 * 		of last page is hit, make a wraparound over page 0
 */

void dataflash_read_continuous( DFLASH_T *pf );

/*
 * 	dataflash_main_memory_page_read:
 *		Reads from main memory 'qty' bytes
 *		beginning at 'page_address' and
 *		'byte_address' in buffer pointed by
 *		'prx'. If last byte of page is hit,
 *		make a wraparound over the same page
 */ 		

void dataflash_main_memory_page_read( DFLASH_T *pf );

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

void dataflash_buffer_read( DFLASH_T *pf );

/*
 * 	dataflash_main_memory_page_to_buffer_xfer:
 * 		Transfers a memory 'page_address' in buffer
 * 		'which'
 */

void dataflash_main_memory_page_to_buffer_xfer( DFLASH_T *pf );
/*
 * 	dataflash_main_memory_page_to_buffer_compare:
 * 		Compares internal buffer 'which' with
 * 		main memory 'page_address'.
 * 		Returns 1 if comparison is OK,
 * 		0 otherwise
 */

MUInt dataflash_main_memory_page_to_buffer_compare( DFLASH_T *pf );

/*
 * 	dataflash_buffer_write:
 * 		Writes 'qty' bytes from 'prx' into
 * 		buffer 'which'. If last byte of buffer
 * 		is hit, overflows to beginning of buffer.
 * 		Buffer positions not written retain
 * 		previous information
 */

void dataflash_buffer_write( DFLASH_T *pf );

/*
 * 	dataflash_buffer_to_main_memory_program:
 * 		Program with buffer 'which' contents to
 * 		main memory 'page_address' with builtin erase
 * 		if 'built_erase' != 0
 */

void dataflash_buffer_to_main_memory_program( DFLASH_T *pf );

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

void dataflash_main_memory_page_program_through_buffer( DFLASH_T *pf );

/*
 * 	dataflash_page_erase:
 * 		Main memory 'page_address" is erased
 */

void dataflash_page_erase( DFLASH_T *pf );

/*
 * 	dataflash_block_erase:
 * 		Main memory 'block_address' is erased
 */

void dataflash_block_erase( DFLASH_T *pf );

/*
 * 	dataflash_autopage_rewrite:
 * 		Refreshes the 'page_address' of main memory
 * 		First the contents of that page is written in
 * 		buffer 'which', then the cotents of that page
 * 		are reprogrammed from buffer
 */

void dataflash_autopage_rewrite( MUInt device, uint page );
#endif


