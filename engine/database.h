#ifndef __DATABASE
#define __DATABASE

#include "role.h"
#include "MD5lib.h"
#include <cstring>
#include <tchar.h>
#include <cstdio>
#include <map>

#define HASH_LEN	16

typedef FILE *PFILE;

typedef pair<DWORD, MASK> DM, *PDM;

typedef struct _buffer
{
	unsigned char hash[HASH_LEN];
	DWORD role;
	int length;
	PDM row;
}BUFFER, *PBUFFER;

#define DATA_FILE	"\\lc.lc"
#define LOG_FILE	"\\lc.log"

extern "C" {
	LPCTSTR db_get_path(LPCTSTR, LPCTSTR);
	void db_read_buffer(PBUFFER, PFILE);
	void db_write_buffer(PBUFFER, PFILE);
	std::pair<PBUFFER, int> db_fetch(PFILE file);
	DWORD db_query_exe_role(LPCTSTR);
	MASK db_query_file(LPCTSTR, DWORD);
}

#endif
