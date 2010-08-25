/*
 * pwrctrl.c
 */

#include "news.h"
#include "mparser.h"
#include "mpdata.h"
#include "pwract.h"
#include "pwrctrl.h"
#include <stdlib.h>

static const TRAN trans_extchrg[] =
{
	BATT_REM,		&ext,			batt2nbatt,
	PWR_FAIL,		&backup,		ext2backup,
	TOUT_PWR,		&extnotchrg,	chrg2nchrg,
	TOUT_FSYNC,		&extchrg,		flash_sync,
	UNKNOWN,		&extchrg,		NULL,
	MPANY,			&extchrg,		NULL
};

static const TRAN trans_extnotchrg[] =
{
	PWR_FAIL,		&backup,		ext2backup,
	TOUT_PWR,		&extnotchrg,	checkbatt,
	TOUT_FSYNC,		&extnotchrg,	flash_sync,
	BATT_REM,		&ext,			batt2nbatt,
	BATT_OK,		&extchrg,		nchrg2chrg,	
	UNKNOWN,		&extnotchrg,	NULL,
	MPANY,			&extnotchrg,	NULL
};

static const TRAN trans_ext[] =
{
	TOUT_PWR,		&ext,		ext2ext,
	BATT_OK,		&extchrg,	nchrg2chrg,	
	TOUT_FSYNC,		&ext,		flash_sync,
	UNKNOWN,		&ext,		NULL,
	MPANY,			&ext,		NULL
};

static const TRAN trans_backup[] =
{
	PWR_OK,			&extchrg,		backup2ext,
	TOUT_PWR,		&backup,		back_bmes,
	SHTDWN,			&backup,		shtdwn,
	UNKNOWN,		&backup,		NULL,
	MPANY,			&backup,		NULL
};

const STATE extchrg =
{
	trans_extchrg,		pwrctrl_ppro
};

const STATE extnotchrg =
{
	trans_extnotchrg,	pwrctrl_ppro
};

const STATE ext =
{
	trans_ext,	pwrctrl_ppro
};

const STATE backup =
{
	trans_backup,		pwrctrl_ppro
};
