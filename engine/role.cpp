#include "role.h"

role_map roles;

bool role_check_mask(MASK mask, DWORD cmd)
{
	if (mask == MASK_NULL)
		return false;
	bool ans = true;
	switch (cmd)
	{
		case CMD_READ:
			ans = ans && CHECK_CMD(mask, 2);
			break;
		case CMD_WRITE:
			ans = ans && CHECK_CMD(mask, 1);
			break;
		case CMD_EXEC:
			ans = ans && CHECK_CMD(mask, 0);
			break;
		case CMD_ANY:
			ans = ans && CHECK_ANY(mask);
			break;
		default:
			ans = false;
	}
	return ans;
}

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
			if (mask != info -> second -> matrix.end())
				ans = ans && role_check_mask(mask -> second, cmd);
		}
	}
	return ans;
}

bool role_check_file(PROLE role, LPCTSTR file, DWORD cmd)
{
	bool ans = true;
	for (ROLE::iterator iter = role -> begin(); iter != role -> end(); iter++)
	{
		MASK mask = db_query_file(file, *iter);
		ans = ans && role_check_mask(mask, cmd);
	}
	return ans;
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
