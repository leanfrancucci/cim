/*
 * 		flash.c
 * 			Implements 'flash.h'
 * 			Lower level dataflash functions
 */

#include "mytypes.h"

#include "flash.h"

#include "spihal.h"
#include "dftypes.h"
#include "dffatal.h"
#include "cop.h"

#define RDY				0x80
#define ERROR0_MEMORY	0x00
#define ERROR1_MEMORY	0xFF
#define MAX_STATUS_WAIT_RESP 0x02FF // ~ 66ms

typedef struct
{
	uchar	opcode;
	uchar	mode;
	uchar	num_bytes;
	uchar	additional;
	uchar	wait_term;
	uchar	read_write;
}OPC_T;

/*
 * 	+ => page 15
 * 	- => page 7
 */

static OPC_T const opc_actions[ DF_NUM_OPCODES ] =
{
	{	0x50,	0,	4,	0,	1,	0 	},	/*	BLOCK_ERASE				+*/
	{	0x52,	1,	4,	4,	0,	1	},	/*	MM_PAGE_READ			*/
	{	0x53,	0,	4,	0,	1,	0	},	/*	MM2B1_XFER				-*/
	{	0x54,	1,	4,	1,	0,	1	},	/*	B1_READ					*/
	{	0x55,	0,	4,	0,	1,	0	},	/*	MM2B2_XFER				-*/
	{	0x56,	1,	4,	1,	0,	1	},	/*	B2_READ					*/
	{	0x57,	1,	1,	0,	0,	1	},	/*	STATUS_READ				*/
	{	0x58,	0,	4,	0,	1,	0	},	/*	APRW_B1					-*/
	{	0x59,	0,	4,	0,	1,	0	},	/*	APRW_B2					-*/
	{	0x60,	0,	4,	0,	1,	0	},	/*	MMCMP_B1				-*/
	{	0x61,	0,	4,	0,	1,	0	},	/*	MMCMP_B2				-*/
	{	0x68,	1,	4,	4,	0,	1	},	/*	CONT_AREAD				*/
	{	0x81,	0,	4,	0,	1,	0	},	/*	PAGE_ERASE				+*/
	{	0x82,	0,	4,	0,	1,	0	},	/*	MMPRG_B1				-*/
	{	0x83,	0,	4,	0,	1,	0	},	/*	MMPRG_B1_BIN_ERASE		-*/
	{	0x84,	0,	4,	0,	0,	1	},	/*	B1_WRITE				*/
	{	0x85,	0,	4,	0,	1,	0	},	/*	MMPRG_B2				-*/
	{	0x86,	0,	4,	0,	1,	0	},	/*	MMPRG_B2_BIN_ERASE		-*/
	{	0x87,	0,	4,	0,	0,	1	},	/*	B2_WRITE				*/
	{	0x88,	0,	4,	0,	1,	0	},	/*	MMPRG_B1_WOUT_BIN_ERASE	-*/
	{	0x89,	0,	4,	0,	1,	0	}	/*	MMPRG_B2_WOUT_BIN_ERASE	-*/
};

static uchar cmd_buf[4];

/*
 * 	flash_compose_address
 *		Sets address data according to
 *		page and byte address and type of memory
 */

static
void
flash_compose_address( MUInt dev_no, uint page_addr, uint byte_addr )
{
	DATAFL_T const *p;
	ulong addr;

	p = pdflash[ dev_no ];
 
 	addr = ((ulong)page_addr << p->baddr_bits) | (byte_addr & ((1<<p->baddr_bits) - 1));

    cmd_buf[1] = (uchar)(addr >> 16);
    cmd_buf[2] = (uchar)(addr >>  8);
    cmd_buf[3] = (uchar)addr;
}

/*
 * 	flash_flush_command:
 * 		Sends an array of bytes to SPI
 * 		that are in cmd_buf in qty len
 */

static
void
flash_flush_command( MUInt len)
{
	uchar *p;

	p = cmd_buf;
	while( len-- )
		spi_send_byte( *p++ );
}

/*
 * 		Public functions
 */

/*
 * 	flash_init:
 * 		Inits whatever needed and
 * 		calls lower level to initialize
 */

void
flash_init( void )
{
	spihal_init();
}

/*
 * 	flash_send_command:
 *		This function is a generic function that setups
 *		the correct command to the Atmel flash part.
 *		First, the device is selected,
 *		Then the command is issued
 *		If, after the command, something must be read or written,
 *		CS is left active, otherwise the device is deselected
 */

void 
flash_send_command( MUInt dev_no, MUInt command, int page_address, int byte_address )
{
	OPC_T const *p;


#ifndef __RELEASE__
	if( command >= DF_NUM_OPCODES )
		dataflash_fatal( BAD_OPCODE );
#endif	

	p = &opc_actions[ command ];

	spi_select_channel( dev_no );
	if( page_address != DFNULL )
		flash_compose_address( dev_no, page_address, byte_address );
	*cmd_buf = p->opcode;
	flash_flush_command( p->num_bytes );
	if( p->additional )
		flash_flush_command( p->additional );
	if( !p->read_write )
		spi_deselect_channel( dev_no );
}

/*
 * 	flash_read_status:
 * 		Flash command STATUS_READ is issued
 * 		and status is returned
 */

MUInt
flash_read_status( MUInt dev_no )
{
	MUInt status;

#ifndef __RELEASE__
	if( dev_no >= NUM_FLASH_DEVICES )
		dataflash_fatal( BAD_DEVICE );
#endif	

	spi_select_channel( dev_no );
	*cmd_buf = opc_actions[ STATUS_READ ].opcode;
	flash_flush_command( 1 );
	status = spi_get_byte();
	spi_deselect_channel( dev_no );
	return status;
}

/*
 * 	flash_wait_ready:
 * 		Waits till flash is ready
 */

void
flash_wait_ready( MUInt dev_no )
{
	ushort wait_resp_count;
	MUInt status;

	wait_resp_count = MAX_STATUS_WAIT_RESP;
	while(1)
	{
		kick_cop_wdog();
		status = flash_read_status( dev_no );
		if( ( --wait_resp_count == 0 )	||
			( status == ERROR0_MEMORY ) ||
			( status == ERROR1_MEMORY ) ||
			( status & RDY )
		  )
			break;
	}
}

/*
 * 	flash_read:
 * 		Reads 'qty' bytes from SPI into 'prx'
 * 		Then deselects device
 */

void
flash_read( MUInt dev_no, uchar *prx, uint qty )
{
	while( qty-- )
		*prx++ = (uchar)spi_get_byte();
	spi_deselect_channel( dev_no );
}

/*
 * 	flash_write:
 * 		Writes 'qty' bytes into SPI from 'ptx'
 * 		Then deselects device
 */

void
flash_write( MUInt dev_no, uchar *ptx, uint qty )
{
	while( qty-- )
		spi_send_byte( *ptx++ );
	spi_deselect_channel( dev_no );
}

