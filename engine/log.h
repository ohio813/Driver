#ifndef __LOG
#define __LOG

#include "subject.h"
#include "database.h"
#include <map>

#define LOG_SIZE 256

typedef std::map<HASH, DWORD> LOG;
typedef LOG::iterator LOG_iter;

extern "C"
{
	void log_query_file(LPCTSTR, LOG);
}

#endif