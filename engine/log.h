#ifndef __LOG
#define __LOG

#include "subject.h"
#include "database.h"
#include <map>

#define LOG_SIZE 256

typedef std::pair<DWORD, int> DI, *PDI;

typedef struct _log_buffer
{
	DI data[LOG_SIZE];
}LOG, *PLOG;

#endif