/*
 * 		dfrefrsh.h
 * 			Module to make data flash refresh
 * 			as stated in first method of AN-4
 * 			of Atmel Corp.
 *
 * 			Eduardo A. Martinez
 * 			DelsatGroup S.A.
 *
 * 			First editiom: 2/23/2008
 */

#include "mytypes.h"

enum
{
	NO_REFRESH = -1
};

/*
 * 	must_refresh:
 * 		This function only tests if refresh
 * 		must be done in 'page' of 'device'
 * 		If so, returns page to be refreshed,
 * 		else returns negative number
 */

int must_refresh( MUInt device, int page );

