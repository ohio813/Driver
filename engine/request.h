#ifndef __REQUEST
#define __REQUEST

#include <windows.h>
#include "object.h"
#include "subject.h"

#define RES_ALLOW	0xff
#define RES_DENY	0x0

struct REQUEST
{
	DWORD subject;
	HANDLE object;
	DWORD cmd;
};

extern "C"
{
	DWORD request_subject(DWORD, LPCTSTR);
	DWORD request_open(DWORD, LPCTSTR);
	DWORD request_access(DWORD, HANDLE, DWORD);
}

#endif
