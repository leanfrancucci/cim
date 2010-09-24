/*
 * lockctrl.c
 */

#include "news.h"
#include "mparser.h"
#include "mpdata.h"
#include "lockact.h"
#include "lockctrl.h"
#include <stdlib.h>

static const TRAN trans_lockidle[] =
{
	UNLOCK_DOOR,	&wkeysw,		chk_keysw,
	UNKNOWN,		&lockidle,		NULL,
	MPANY,			&lockidle,		NULL
};

static const TRAN trans_wkeysw[] =
{
	OPEN_KEYS,		&powered,		feed_lock,
	LOCK_DOOR,		&lockidle,		release_lock,
	TOUT_ULE,		&lockidle,		release_lock,
	UNKNOWN,		&wkeysw,		NULL,
	MPANY,			&wkeysw,		NULL
};

static const TRAN trans_powered[] =
{
	LOCK_DOOR,		&lockidle,		release_lock,
	TOUT_DOOR,		&lockidle,		release_lock,
	UNKNOWN,		&powered,		NULL,
	MPANY,			&powered,		NULL
};

const STATE lockidle =
{
	trans_lockidle,	lock_ppro
};

const STATE wkeysw =
{
	trans_wkeysw,	lock_ppro
};

const STATE powered =
{
	trans_powered,	lock_ppro
};
