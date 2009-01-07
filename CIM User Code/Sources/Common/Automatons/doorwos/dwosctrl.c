/*
 * dwosctrl.c
 */

#include "news.h"
#include "mparser.h"
#include "mpdata.h"
#include "dwosact.h"
#include "dwosctrl.h"
#include <stdlib.h>

static const TRAN trans_idle_wos[] =
{
	TO_LOCKED,		&locked_wos,		NULL,
	TO_UNLOCKED,	&unlocked_wos,		NULL,
	UNKNOWN,		&idle_wos,			NULL,
	MPANY,			&idle_wos,			NULL
};

static const TRAN trans_locked_wos[] =
{
	OPEN_DOOR,		&unlocked_wos,		lked2ulked_wos,
	CLOSE_DOOR,		&locked_wos,		lked2lked_wos,
//	OPEN_KEYS,		&locked_wos,		NULL,
//	CLOSE_KEYS,		&locked_wos,		NULL,
	UNLOCK_DOOR,	&pre_unlocking_wos,	lked2pulking_wos,
//	LOCK_DOOR,		&locked_wos,		NULL,
//	TOUT_DOOR,		&locked_wos,		NULL,
	UNKNOWN,		&locked_wos,		NULL,
	MPANY,			&locked_wos,		NULL
};

static const TRAN trans_pre_unlocking_wos[] =
{
	OPEN_DOOR,		&unlocked_wos,			pulking2ulked_wos,
	CLOSE_DOOR,		&locked_wos,			ulked2lked_wos,
	OPEN_KEYS,		&unlocking_wos,			pulking2ulking_wos,
//	CLOSE_KEYS,		&pre_unlocking_wos,		NULL,
//	UNLOCK_DOOR,	&pre_unlocking_wos,		NULL,
	LOCK_DOOR,		&locked_wos,			pulking2lked_wos,
	TOUT_ULE,		&locked_wos,			pulking2lked_wos,
	UNKNOWN,		&pre_unlocking_wos,		NULL,
	MPANY,			&pre_unlocking_wos,		NULL
};

static const TRAN trans_unlocking_wos[] =
{
	OPEN_DOOR,		&unlocked_wos,		ulking2ulked_wos,
	CLOSE_DOOR,		&locked_wos,		ulked2lked_wos,
//	OPEN_KEYS,		&unlocking_wos,		NULL,
//	CLOSE_KEYS,		&unlocking_wos,		NULL,
//	UNLOCK_DOOR,	&unlocking_wos,		NULL,
#if 0
	LOCK_DOOR,		&locked_wos,		ulking2lked_wos,
	TOUT_DOOR,		&locked_wos,		ulking2lked_wos,
#else
	LOCK_DOOR,		&wpunlock_wos,		lock_and_wait_wos,
	TOUT_DOOR,		&wpunlock_wos,		lock_and_wait_wos,
#endif
	UNKNOWN,		&unlocking_wos,		NULL,
	MPANY,			&unlocking_wos,		NULL
};

static const TRAN trans_wpunlock_wos[] =
{
	OPEN_DOOR,		&unlocked_wos,		ulking2ulked_wos,
	CLOSE_DOOR,		&locked_wos,		ulked2lked_wos,
	TOUT_DOOR,		&locked_wos,		ulking2lked_wos,
	UNKNOWN,		&unlocking_wos,		NULL,
	MPANY,			&unlocking_wos,		NULL
};

static const TRAN trans_unlocked_wos[] =
{
//	OPEN_DOOR,		&unlocked_wos,		NULL,
	CLOSE_DOOR,		&locked_wos,		ulked2lked_wos,
//	OPEN_KEYS,		&unlocked_wos,		NULL,
//	CLOSE_KEYS,		&unlocked_wos,		NULL,
	UNLOCK_DOOR,	&pre_unlocking_wos,	lked2pulking_wos,
	LOCK_DOOR,		&locking_wos,		ulked2lking_wos,
	TOUT_DOOR,		&locking_wos,		ulked2lking_wos,
	UNKNOWN,		&unlocked_wos,		NULL,
	MPANY,			&unlocked_wos,		NULL
};

static const TRAN trans_locking_wos[] =
{
//	OPEN_DOOR,		&locking_wos,		NULL,
	CLOSE_DOOR,		&locked_wos,		lking2lked_wos,
//	OPEN_KEYS,		&locking_wos,		NULL,
//	CLOSE_KEYS,		&locking_wos,		NULL,
	UNLOCK_DOOR,	&pre_unlocking_wos,	lked2pulking_wos,
//	LOCK_DOOR,		&locking_wos,		NULL,
//	TOUT_DOOR,		&locking_wos,		NULL,
	UNKNOWN,		&locking_wos,		NULL,
	MPANY,			&locking_wos,		NULL
};

const STATE idle_wos =
{
	trans_idle_wos,	dwos_ppro
};

const STATE locked_wos =
{
	trans_locked_wos,		dwos_ppro
};

const STATE pre_unlocking_wos =
{
	trans_pre_unlocking_wos,	dwos_ppro
};

const STATE unlocking_wos =
{
	trans_unlocking_wos,	dwos_ppro
};

const STATE wpunlock_wos =
{
	trans_wpunlock_wos,	dwos_ppro
};

const STATE unlocked_wos =
{
	trans_unlocked_wos,		dwos_ppro
};

const STATE locking_wos =
{
	trans_locking_wos,		dwos_ppro
};
