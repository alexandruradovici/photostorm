
#ifndef _PS_SYSTEM__H
#define _PS_SYSTEM__H

#include "data.h"

#ifdef WIN32
	#include "ps_win32.h"
#endif

#ifdef LINUX
	#include "ps_linux.h"
#endif

#endif
