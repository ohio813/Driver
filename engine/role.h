#ifndef __ROLE
#define __ROLE

#include <windows.h>
#include <set>

typedef set<DWORD> ROLE, *PROLE

extern "C" 
{
	bool role_check(PROLE, DWORD, DWORD);
	void role_attach(PROLE, DWORD);
	void role_attach(PROLE, PROLE);
}

#endif