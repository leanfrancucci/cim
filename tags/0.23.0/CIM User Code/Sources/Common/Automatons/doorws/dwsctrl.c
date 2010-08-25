/*
 * doorctrl.c
 */

#include "news.h"
#include "mparser.h"
#include "mpdata.h"
#include "dwsact.h"
#include "dwsctrl.h"
#include <stdlib.h>

static const TRAN trans_idle_ws[] =
{
	TO_LOCKED,		&locked_ws,			NULL,
	TO_UNLOCKED,	&unlocked_ws,		NULL,
	UNKNOWN,		&idle_ws,			NULL,
	MPANY,			&idle_ws,			NULL
};

static const TRAN trans_locked_ws[] =
{
	OPEN_DOOR,		&locked_ws,			inform_open_ws,
	CLOSE_DOOR,		&locked_ws,			inform_close_ws,
	UNLOCK_DOOR,	&pre_unlocking_ws,	lked2pulking_ws,
	UNLOCKED,		&unlocked_ws,		lked2unlked_ws,
	UNKNOWN,		&locked_ws,			NULL,
	MPANY,			&locked_ws,			NULL
};

static const TRAN trans_pre_unlocking_ws[] =
{
	OPEN_DOOR,		&pre_unlocking_ws,		inform_open_ws,
	CLOSE_DOOR,		&pre_unlocking_ws,		inform_close_ws,
	OPEN_KEYS,		&unlocking_ws,			pulking2ulking_ws,
	LOCK_DOOR,		&locked_ws,				pulking2lked_ws,
	UNLOCKED,		&unlocked_ws,			pulking2ulked_ws,
	TOUT_ULE,		&locked_ws,				pulking2lked_ws,
	UNKNOWN,		&pre_unlocking_ws,		NULL,
	MPANY,			&pre_unlocking_ws,		NULL
};

static const TRAN trans_unlocking_ws[] =
{
	OPEN_DOOR,		&unlocking_ws,		inform_open_ws,
	CLOSE_DOOR,		&unlocking_ws,		inform_close_ws,
	UNLOCKED,		&unlocked_ws,		ulking2ulked_ws,
#if 0
	LOCK_DOOR,		&locked_ws,			ulking2lked_ws,
	TOUT_DOOR,		&locked_ws,			ulking2lked_ws,
#else
	LOCK_DOOR,		&wpunlock_ws,		lock_and_wait,
	TOUT_DOOR,		&wpunlock_ws,		lock_and_wait,
#endif
	UNKNOWN,		&unlocking_ws,		NULL,
	MPANY,			&unlocking_ws,		NULL
};

static const TRAN trans_wpunlock_ws[] =
{
	OPEN_DOOR,		&wpunlock_ws,		inform_open_ws,
	CLOSE_DOOR,		&wpunlock_ws,		inform_close_ws,
	UNLOCKED,		&unlocked_ws,		ulking2ulked_ws,
	TOUT_DOOR,		&locked_ws,			ulking2lked_ws,
	UNKNOWN,		&unlocking_ws,		NULL,
	MPANY,			&unlocking_ws,		NULL
};

static const TRAN trans_unlocked_ws[] =
{
	OPEN_DOOR,		&unlocked_ws,		inform_open_ws,
	CLOSE_DOOR,		&unlocked_ws,		inform_close_ws,
#if 0
	UNLOCK_DOOR,	&unlocked_ws,		NULL,
#else
	UNLOCK_DOOR,	&pre_unlocking_ws,	lked2pulking_ws,
#endif
	LOCK_DOOR,		&locking_ws,		ulked2lking_ws,
	LOCKED,			&locked_ws,			ulked2lked_ws,
	TOUT_DOOR,		&locking_ws,		ulked2lking_ws,
	UNKNOWN,		&unlocked_ws,		NULL,
	MPANY,			&unlocked_ws,		NULL
};

static const TRAN trans_locking_ws[] =
{
	OPEN_DOOR,		&locking_ws,		inform_open_ws,
	CLOSE_DOOR,		&locking_ws,		inform_close_ws,
#if 0
	UNLOCK_DOOR,	&locking_ws,		NULL,
#else
	UNLOCK_DOOR,	&pre_unlocking_ws,	lked2pulking_ws,
#endif
	LOCKED,			&locked_ws,			lking2lked_ws,
	UNKNOWN,		&locking_ws,		NULL,
	MPANY,			&locking_ws,		NULL
};

const STATE idle_ws =
{
	trans_idle_ws,	dws_ppro
};

const STATE locked_ws =
{
	trans_locked_ws,	dws_ppro
};

const STATE pre_unlocking_ws =
{
	trans_pre_unlocking_ws,	dws_ppro
};

const STATE unlocking_ws =
{
	trans_unlocking_ws,	dws_ppro
};

const STATE wpunlock_ws =
{
	trans_wpunlock_ws,	dws_ppro
};

const STATE unlocked_ws =
{
	trans_unlocked_ws,	dws_ppro
};

const STATE locking_ws =
{
	trans_locking_ws,	dws_ppro
};
