/*
 * superv.c
 */

#include "mytypes.h"
#include "ioports.h"
#include "superv.h"
#include "reset.h"

typedef struct
{
	short	timer;
	uchar	last_in;
}WDOG_SUP_ST;

static WDOG_SUP_ST wdogsup[ NUM_WDOG_SUPS ] =
{
	{	WDOG_TOUT, 0 },
	{			0, 0 },
	{ 			0, 0 }
};

static WDOG_SUP_ST *p;
static MUInt sup_enable;
static MUInt isjump = 0;

void	
supervisor_init( void )
{
	WDOG_OUT = 1;
	sleep( ON_RESET_TIME );
	WDOG_OUT = 0;
	VRLY_OUT = 1;
	VLOCK_OUT = 1;
	sleep( POST_RESET_TIME );

	p = wdogsup;
	p->timer = WDOG_TOUT;
	p->last_in = ~WDOG_IN;
	++p;
	p->timer = VLOCK_TOUT;
	p->last_in = VRLY_IN;
	++p;
	p->timer = VRLY_TOUT;
	p->last_in = VLOCK_IN;
	
	TP8 = 0;
	sup_enable = 1;
}

static
void	
wdog_act( void )
{
	if( p->last_in != WDOG_IN )
		p->timer = WDOG_TOUT;
	
	p->last_in = WDOG_IN;

	if( p->timer <= 0 )
	{
		WDOG_OUT = 1;
		reset_now();
		return;
	}

	WDOG_OUT = 0;
	--p->timer; 
}

static
void	
vrly_act( void )
{
	if( p->last_in != VRLY_IN )
		p->timer = VRLY_TOUT;
	
	p->last_in = VRLY_IN;

	if( p->timer <= 0 )
	{
		VRLY_OUT = 0;
		return;
	}

	VRLY_OUT = 1;
	--p->timer; 
}

static
void	
vlock_act( void )
{
	if( p->last_in != VLOCK_IN )
		p->timer = VLOCK_TOUT;
	
	p->last_in = VLOCK_IN;

	if( p->timer <= 0 )
	{
		VLOCK_OUT = 0;
		return;
	}

	VLOCK_OUT = 1;
	--p->timer; 
}

static
MUInt
tst_jump_superv( void )
{
	if( (isjump == 1) && (TP6 == 1) )
	{
		p = wdogsup;
		p->timer = WDOG_TOUT;
		p->last_in = ~WDOG_IN;
		++p;
		p->timer = VRLY_TOUT;
		p->last_in = VRLY_IN;
		++p;
		p->timer = VLOCK_TOUT;
		p->last_in = VLOCK_IN;
		isjump = 0;
		return 0;
	}
	if( TP6 == 0 )
	{
		WDOG_OUT = 0;
		VRLY_OUT = 1;
		VLOCK_OUT = 1;
		isjump = 1;
		return 1;
	}
	return 0;
}

void
do_supervisor( void )
{
	if( !sup_enable )
		return;

	if( tst_jump_superv() )
		return;

	p = &wdogsup[WDOG_SUP];
	wdog_act();
	++p;
	vrly_act();
	++p;
	vlock_act();


}
