#ifndef __ROLE
#define __ROLE

#include <windows.h>
#include <set>

typedef set<DWORD> ROLE, *PROLE

extern "C" 
{
	bool role_check(PROLE, HANDLE, DWORD);
	bool role_check(PROLE, LPCTSTR, DWORD);
	bool role_attach(PROLE, DWORD);
	bool role_attach(PROLE, PROLE);
}

#endif