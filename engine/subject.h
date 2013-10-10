#ifndef __SUBJECT
#define __SUBJECT

#include <map>
#include <windows.h>
#include "rule.h"
#include "database.h"

#define ROLE_NULL	NULL

typedef struct _subject
{
	PROLE role;
}subject_data;

typedef std::map<DWORD, subject_data> subject_map;
typedef subject_map::iterator subject_iter;

extern subject_map subjects;

extern "C" {
	PROLE subject_query_role(DWORD subject);
	bool subject_store(DWORD parent, DWORD pid, LPCTSTR file);
	bool subject_clear(DWORD subject);
}

#endif