#ifndef __REQUEST
#define __REQUEST

#include <windows.h>
#include "object.h"
#include "context.h"

#define CMD_READ 	0x0
#define CMD_WRITE	0xff
#define CMD_EXEC	0x88
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
	DWORD subject_request(DWORD, );
	DWORD open_request(DWORD, DWORD, LPCTSTR);
	DWORD access_request(DWORD, HANDLE, DWORD);
}

#endif