/*
 * error.h
 */

#include "mytypes.h"

enum
{
	INVALID_PINNUMBER,
	INVALID_PORTNUMBER,
	PIN_MUSTOUT,
	MUST_IO,
	MUST_ADC,
	MIN_PIN,
	MIN_PORT,
	INVALID_OPT,
	ERRORS_NUM
};

void log_error( MUInt error_num );
