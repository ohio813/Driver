#include "role.h"

role_map roles;

bool role_check_handle(PROLE role, HANDLE object, DWORD cmd)
{
	bool ans = true;
	for (ROLE::iterator iter = role -> begin(); iter != role -> end(); iter++)
	{
		role_iter info = roles.find(*iter);
		if (info == roles.end())
		{
			ans = false;
		} else {
			MATRIX_iter mask = info -> second -> matrix.find(object);
			switch (cmd)
			{
			case CMD_READ:
				ans = ans && CHECK_CMD(mask -> second, 2);
				break;
			case CMD_WRITE:
				ans = ans && CHECK_CMD(mask -> second, 1);
				break;
			case CMD_EXEC:
				ans = ans && CHECK_CMD(mask -> second, 0);
				break;
			case CMD_ANY:
				ans = ans && CHECK_ANY(mask);
				break;
			default:
				ans = false;
			}
		}
	}
	return ans;
}

bool role_check_file(PROLE role, LPCTSTR file, DWORD cmd)
{
	//TODO
	return false;
}

bool role_attach_role(PROLE role, DWORD access)
{
	if (role -> find(access) == role -> end())
	{
		role -> insert(access);
		role_iter iter = roles.find(access);
		if (iter == roles.end())
		{
			roles.insert(std::make_pair(access, new ROLE_INFO()));
		} else {
			iter -> second -> ref_count++;
		}
		return true;
	} else return false;
}

bool role_attach_parent(PROLE role, PROLE parent)
{
	if (parent -> empty())
		return false;
	for (ROLE::iterator iter = parent -> begin(); iter != parent -> end(); iter++)
		role_attach_role(role, *iter);
	return true;
}

bool role_dispose(PROLE role)
{
	if (role)
	{
		for (ROLE::iterator iter = role -> begin(); iter != role -> end(); iter++)
		{
			role_iter riter = roles.find(*iter);
			if (riter != roles.end())
			{
				if ((--riter -> second -> ref_count) < 0)
					roles.erase(riter);
			}
		}
		delete role;
		return true;
	} else {
		return false;
	}
}
