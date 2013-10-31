#ifndef __DATABASE
#define __DATABASE

#include "role.h"
#include "MD5lib.h"
#include <cstring>
#include <tchar.h>
#include <cstdio>
#include <pair>
#include <vector>

#define HASH_LEN	16

typedef FILE *PFILE;

typedef struct _buffer
{
	unsigned char hash[HASH_LEN];
	int length;
	PMASK mask;
}BUFFER, *PBUFFER;

#define DATA_FILE	"\\lc.lc"

extern "C" {
	LPCTSTR db_get_path(LPCTSTR);
	void db_read_buffer(PBUFFER, PFILE);
	void db_write_buffer(PBUFFER, PFILE);
	PBUFFER db_fetch(PFILE);
	DWORD db_query_exe_role(LPCTSTR);
}

#endif
