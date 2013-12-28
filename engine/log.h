#ifndef __LOG
#define __LOG

#include "database.h"
#include <map>

#define LOG_SIZE 256

typedef std::map<HASH, DWORD> LOG, *PLOG;
typedef LOG::iterator LOG_iter;

#include "subject.h"

extern "C"
{
	void log_query_file(LPCTSTR, LOG);
}

#endif
