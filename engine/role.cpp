#include "rule.h"

bool role_check(PROLE role, HANDLE object, DWORD cmd)
{
	//TODO
}

bool role_check(PROLE role, LPCTSTR file, DWORD cmd)
{
	//TODO
}

bool role_attach(PROLE role, DWORD access)
{
	if (role -> find(access) == role -> end())
	{
		role -> insert(access);
	} else return false;
}

bool role_attach(PROLE role, PROLE parent)
{
	if (parent -> empty)
		return false;
	for (ROLE::iterator iter = parent -> begin(); iter != parent -> end(); iter++)
		role_attach(role, *iter);
	return true;
}