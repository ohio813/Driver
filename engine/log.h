#ifndef __LOG
#define __LOG

#include "database.h"
#include <map>

#define LOG_SIZE	256
#define LOG_FILE	"\\lc.log"

typedef std::map<HASH, DWORD> LOG, *PLOG;
typedef LOG::iterator LOG_iter;
typedef std::pair<HASH, DWORD> HD, *PHD;

typedef struct _log_buffer
{
	HD data[LOG_SIZE];
}BLOG, *PBLOG;

#include "subject.h"

extern "C"
{
	void log_query_file(LPCTSTR, LOG);
}

#endif
