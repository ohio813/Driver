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
		role_iter iter = role_map.find(access);
		if (iter == role_map.end())
		{
			role_map.insert(new ROLE_INFO());
		} else {
			iter -> second.ref_count++;
		}
		return true;
	} else return false;
}

bool role_attach(PROLE role, PROLE parent)
{
	if (parent -> empty())
		return false;
	for (ROLE::iterator iter = parent -> begin(); iter != parent -> end(); iter++)
		role_attach(role, *iter);
	return true;
}

bool role_dispose(PROLE role)
{
	if (role)
	{
		for (ROLE::iterator iter = role -> begin(); iter != parent -> end(); iter++)
		{
			role_iter riter = role_map.find(*iter);
			if (riter != role_map.end())
			{
				if ((--riter -> second.ref_count) < 0)
					role_map.erase(riter);
			}
		}
		delete role;
		return true;
	} else {
		return false;
	}
}