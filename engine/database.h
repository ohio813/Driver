#ifndef __DATABASE
#define __DATABASE

#include "role.h"
#include <string.h>
#include <tchar.h>
#include <stdio.h>

#define DATA_FILE	"\\lc.lc"

extern "C" {
	LPCTSTR db_get_path(LPCTSTR);
	DWORD db_query_exe_role(LPCTSTR);
}

#endif
