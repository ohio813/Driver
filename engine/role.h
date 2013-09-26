#ifndef __ROLE
#define __ROLE

#include <windows.h>
#include <set>

extern "C" 
{
	bool role_check(DWORD, DWORD, DWORD);
	DWORD attach_role(DWORD, DWORD);
}

#endif