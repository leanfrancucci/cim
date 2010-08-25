/*
 * 	mtimetbl.c
 * 		Main TaBLe timer
 */

#include <stdio.h>

#include "mytypes.h"

#include "mtimedef.h"
#include "mtimetbl.h"
#include "watchdog.h"
#include "sleep.h"
#include "sensor.h"
#include "stimers.h"
#include "pwrhdl.h"
#include "wouts.h"

void
none( void )
{
}

static void(* const actions_2[])( void ) =
{
	check_watchdog, check_sleep, proc_sensors, NULL
};

static void(* const actions_20[])( void ) =
{
	handler_timers, proc_wouts, NULL
};

static void(* const actions_100[])( void ) =
{
	proc_pwrstatus_rti, NULL
};

const TIMER_CHAIN timer_chain[] =
{
	{ 2000/MAIN_TIMER_TIME,		actions_2 		},
	{ 20000/MAIN_TIMER_TIME, 	actions_20 		},
	{ 100000/MAIN_TIMER_TIME,	actions_100		}
};


