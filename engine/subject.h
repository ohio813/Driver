#ifndef __SUBJECT
#define __SUBJECT

#include <map>
#include <windows.h>
#include "role.h"
#include "database.h"
#include "log.h"

typedef struct _subject
{
	PROLE role;
	LPCTSTR path;
	LOG log;
}subject_data;

typedef std::map<DWORD, subject_data> subject_map;
typedef subject_map::iterator subject_iter;

extern subject_map subjects;

extern "C" {
	PROLE subject_query_role(DWORD);
	LPCTSTR subject_query_path(DWORD);
	bool subject_store(DWORD, DWORD, LPCTSTR);
	bool subject_clear(DWORD);
	DWORD subject_compare(DWORD);
}

#endif
