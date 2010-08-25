/*
 * settings.c
 */

#include "settings.h"

/*
 * Firmware and hardware version string
 */

#pragma CONST_SEG 			BOOTROM
const char version_number[ VER_STR_LEN ] =  { VERSION_STRING };
#pragma CONST_SEG 			DEFAULT

