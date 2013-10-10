#ifndef __REQUEST
#define __REQUEST

#include <windows.h>
#include "object.h"
#include "subject.h"

#define CMD_READ 	0x0
#define CMD_WRITE	0xff
#define CMD_EXEC	0x88
#define CMD_ANY		0x55
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
	DWORD request_subject(PSUBJECT, LPCTSTR);
	DWORD request_open(PSUBJECT, LPCTSTR);
	DWORD request_access(PSUBJECT, HANDLE, DWORD);
}

#endif