/*
 *	spi.c
 *
 *	Routines for the SPI peripherical management.
 *
 *	Programmer: Darío S. Baliña.
 *	Date : 26/05/06.
 */

#include "derivative.h"
#include "mytypes.h"
#include "spi.h"

void
spi_init(void)
{
	SPI1C1_MSTR = 1;
	SPI1C1_CPOL = 1;
	SPI1C1_CPHA = 1;
	SPI1C1_SSOE = 0;
	SPI1C1_LSBFE = 0;
	
	SPI1C2_MODFEN = 0;
	
	SPI1C1_SPE = 1;
}

#include "stack.h"
MUInt
spi_xfer( MUInt byte )
{
	test_sp();
	while( !SPI1S_SPTEF );

	SPI1D = byte;

	while( !SPI1S_SPRF );
	
	return SPI1D;
}


