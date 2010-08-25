/*
 * alphctrl.h
 */

#ifndef __ALPHCTRL_H__
#define __ALPHCTRL_H__

#include "news.h"
#include "mparser.h"
#include "mpdata.h"
#include "alphact.h"


extern const STATE 	idle_prot,
					wait_dev, 
					wait_cmd, 
					wait_numdata, 
					in_data,
					pframe,
					wait_vresp;

#endif
