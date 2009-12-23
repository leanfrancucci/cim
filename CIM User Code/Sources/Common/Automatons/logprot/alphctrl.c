/*
 * alphctrl.c
 */

#include "news.h"
#include "mparser.h"
#include "mpdata.h"
#include "alphact.h"
#include "alphctrl.h"

#include <stdlib.h>

static const TRAN trans_idle_prot[] =
{
	MARK,			&wait_dev,		init_frame,
	TINACTIVITY,	&idle_prot,		master_restart,
	UNKNOWN_DATA,	&idle_prot,		NULL,	
	MPANY,			&idle_prot,		NULL	
};

static const TRAN trans_wait_dev[] =
{
	DATA_VALID,		&wait_cmd,		store_dev,
	INVALID_DATA,	&idle_prot,		drop_frame,
	TBET_BYTE,		&idle_prot,		drop_frame,
	UNKNOWN_DATA,	&wait_dev,		NULL,
	MPANY,			&wait_dev,		NULL
};

static const TRAN trans_wait_cmd[] =
{
	DATA_VALID,		&wait_numdata,	store_cmd,
	INVALID_DATA,	&idle_prot,		drop_frame,
	TBET_BYTE,		&idle_prot,		drop_frame,
	UNKNOWN_DATA,	&wait_cmd,		NULL,
	MPANY,			&wait_cmd,		NULL
};

static const TRAN trans_wait_numdata[] =
{
	NDATA_VALID,	&wait_numdata,	in_numdata,
	DATA_VALID,		&in_data,		store_numdata,
	INVALID_DATA,	&idle_prot,		drop_frame,
	TBET_BYTE,		&idle_prot,		drop_frame,
	UNKNOWN_DATA,	&wait_numdata,	NULL,
	MPANY,			&wait_numdata,	NULL
};

static const TRAN trans_in_data[] =
{
	DATA_VALID,		&in_data,		store_data,
	END_PAYLOAD,	&pframe,		rec_frame,
	TBET_BYTE,		&idle_prot,		send_nack,
	UNKNOWN_DATA,	&in_data,		NULL,
	MPANY,			&in_data,		NULL
};

static const TRAN trans_pframe[] =
{
	VALSENT,		&wait_vresp,	NULL,
	TXMITCT0,		&idle_prot,		send_resp,
	TXMITCT1,		&idle_prot,		send_nack,
	UNKNOWN_DATA,	&pframe,		NULL,
	MPANY,			&pframe,		NULL
};

static const TRAN trans_wait_vresp[] =
{
	VALRESP,		&idle_prot,		toidle,
	MARK,			&wait_dev,		block_val,
	UNKNOWN_DATA,	&wait_vresp,	NULL,
	MPANY,			&wait_vresp,	NULL
};

const STATE idle_prot =
{
	trans_idle_prot,	idle_ppro
};

const STATE wait_dev =
{
	trans_wait_dev,		wait_dev_ppro
};

const STATE wait_cmd =
{
	trans_wait_cmd,		wait_cmd_ppro
};

const STATE wait_numdata =
{
	trans_wait_numdata,	wait_ndata_ppro
};

const STATE in_data =
{
	trans_in_data,		in_data_ppro
};

const STATE pframe =
{
	trans_pframe,		pframe_ppro
};

const STATE wait_vresp =
{
	trans_wait_vresp,	wait_vresp_ppro	
};
