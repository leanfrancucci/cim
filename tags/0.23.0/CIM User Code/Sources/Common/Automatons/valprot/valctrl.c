/*
 * valctrl.c
 */

#include "news.h"
#include "mparser.h"
#include "mpdata.h"
#include "valact.h"
#include "valctrl.h"

#include <stdlib.h>

static const TRAN trans_vblocked[] =
{
	VALUNBLOCK,		&idle_val,		set_start_rcv,
	UNKNOWN_DATA,	&vblocked,		NULL,	
	MPANY,			&vblocked,		NULL	
};

static const TRAN trans_idle_val[] =
{
	DATA_VALID,		&in_frame,		init_vframe,
	VALBLOCK,		&vblocked,		drop_vframe,	
	TOUT_VAL,		&vblocked,		drop_vframe,
	UNKNOWN_DATA,	&idle_val,		NULL,	
	MPANY,			&idle_val,		NULL	
};

static const TRAN trans_in_frame[] =
{
	DATA_VALID,		&in_frame,		store_vdata,
	TOUT_VAL,		&vblocked,		rec_vframe,
	VALBLOCK,		&vblocked,		drop_vframe,
	INVALID_DATA,	&vblocked,		rec_vframe,
	UNKNOWN_DATA,	&in_frame,		NULL,
	MPANY,			&in_frame,		NULL
};

const STATE vblocked =
{
	trans_vblocked,		vblocked_ppro	
};

const STATE idle_val =
{
	trans_idle_val,		idle_val_ppro
};

const STATE in_frame =
{
	trans_in_frame,		in_frame_ppro
};
