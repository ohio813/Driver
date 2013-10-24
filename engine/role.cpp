#include "role.h"

bool role_check(PROLE role, HANDLE object, DWORD cmd)
{
	bool ans = true;
	for (ROLE::iterator iter = role -> begin(); iter != role -> end(); iter++)
	{
		role_iter info = roles.find(*iter);
		if (role_iter == roles.end())
		{
			ans = false;
		} else {
			switch (cmd)
			case CMD_READ:
				ans = ans && role_iter -> second -> matrix.read;
				break;
			case CMD_WRITE:
				ans = ans && role_iter -> second -> matrix.write;
				break;
			case CMD_EXEC:
				ans = ans && role_iter -> second -> matrix.exec;
				break;
			case CMD_ANY:
				ans = ans &&   (role_iter -> second -> matrix.read ||
								role_iter -> second -> matrix.write ||
								role_iter -> second -> matrix.exec);
				break;
			default:
				ans = false;
		}
	}
	return ans;
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
			iter -> second -> ref_count++;
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
				if ((--riter -> second -> ref_count) < 0)
					role_map.erase(riter);
			}
		}
		delete role;
		return true;
	} else {
		return false;
	}
}