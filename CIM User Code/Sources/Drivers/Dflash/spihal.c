/*
 * 	spihal.c
 * 		SPI implementation of
 * 		Hardware Abstraction Layer
 */

#include "mytypes.h"

#include "spihal.h"
#include "spi.h"
#include "ioports.h"
#include "wouts.h"
#include "flash.h"
#include "watchdog.h"
#include "enaint.h"
#include "vaultdef.h"
#include "watchdog.h"

enum
{
	RELAIS_OPEN, RELAIS_CLOSE
};

static uchar relais_state = RELAIS_OPEN;
static ushort dftimer;

#define kick_dftimer(x)	(dftimer=x)
#define kill_dftimer()	(dftimer=0)
#define visit_dftimer()	((dftimer!=0)&&(--dftimer==0))

static
void
set_memctrl( void )
{
	E_MEMCTRL = 1;
}

static
void
clear_memctrl( void )
{
	E_MEMCTRL = 0;
}

void
spihal_init( void )
{
	SPI_CS_M = 1;
	SPI_CS_PM = 1;
	relais_state = RELAIS_OPEN;

	spi_init();
#ifndef __MEM_RELAY__
	set_memctrl();
	sleep(500/SLEEP_BASE);
#endif
}

static
void
assert_cs( MUInt dev_no )
{
	if( dev_no == SECONDARY_DEVICE )
	{
		SPI1BR = SPI_PRESCAL_X3;
		SPI_CS_M = 1;
		SPI_CS_PM = 0;
	}
	else
	{
		SPI1BR = SPI_PRESCAL_X1;
		SPI_CS_PM = 1;
		SPI_CS_M = 0;
	}
}

static
void
deassert_cs( MUInt dev_no )
{
	if( dev_no == SECONDARY_DEVICE )
		SPI_CS_PM = 1;
	else
		SPI_CS_M = 1;
}

void
spi_select_channel( MUInt dev_no )
{
	kick_watchdog();
#ifdef __MEM_RELAY__
	if( dev_no == SECONDARY_DEVICE )
	{
		sem_disable();
		kill_dftimer();
		sem_enable();
		if( relais_state == RELAIS_OPEN )
		{
			set_memctrl();
			sleep( DF_BOUNCE_TIME );
			relais_state = RELAIS_CLOSE;
		}
	}
#endif
	assert_cs( dev_no ); 
}

void
spi_deselect_channel( MUInt dev_no )
{
	kick_watchdog();
	deassert_cs( dev_no );
#ifdef __MEM_RELAY__
	if( dev_no == SECONDARY_DEVICE )
	{
		sem_disable();
		kick_dftimer( DF_RELEASE_TIME );
		sem_enable();
	}
#endif
}

void
dflash_handler( void )
{
#ifdef __MEM_RELAY__
	if( visit_dftimer() )
	{
		relais_state = RELAIS_OPEN;
		clear_memctrl();
	}
#endif
}
	
void
spi_send_byte( MUInt byte )
{
	spi_xfer(byte);
}

MUInt
spi_get_byte( void )
{
	return spi_xfer(DUMMY_DATA);
}


