/*
 * 	spihal.h
 * 		SPI Hardware Abstraction Layer
 */

#include "mytypes.h"
#include "sleep.h"

#define DFLASH_BASE				1
#define DF_BOUNCE_TIME 			(50/SLEEP_BASE)
#define DF_RELEASE_TIME 		(1000/DFLASH_BASE)

#define	DUMMY_DATA	0xFF

#define __MEM_RELAY__

void	spihal_init			( void );
void	spi_select_channel	( MUInt dev_no );
void	spi_deselect_channel( MUInt dev_no );
void	dflash_handler		( void );
void	spi_send_byte		( MUInt byte );
MUInt	spi_get_byte		( void );



