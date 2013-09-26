#ifndef __SUBJECT
#define __SUBJECT

#include <map>
#include <windows.h>
#include "rule.h"
#include "database.h"

#define ROLE_NULL	0xff

typedef struct _subject
{
	DWORD role;
}subject_data;

typedef std::map<DWORD, subject_data> subject_map;
typedef subject_map::iterator subject_iter;

extern subject_map subjects;

extern "C" {
	DWORD subject_query_role(DWORD subject);
	DWORD subject_store(DWORD parent, DWORD pid, LPCTSTR file);
	DWORD subject_clear(DWORD subject);
}

#endif