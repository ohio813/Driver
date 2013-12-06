#include "subject.h"

subject_map subjects;

PROLE subject_query_role(DWORD subject)
{
	subject_iter ret = subjects.find(subject);
	if (ret == subjects.end())
		return ROLE_NULL;
	else
		return ret -> second.role;
}

LPCTSTR subject_query_path(DWORD subject)
{
	subject_iter ret = subjects.find(subject);
	if (ret == subjects.end())
		return NULL;
	else
		return ret -> second.path;
}

bool subject_clear(DWORD subject)
{
	subject_iter iter = subjects.find(subject);
	if (iter == subjects.end())
		return false;
	delete iter -> second.role;
	subjects.erase(subject);
	return true;
}

bool subject_store(DWORD parent, DWORD pid, LPCTSTR file)
{
	bool ret = true;
	subject_data data;
	data.path = file;
	data.role = new ROLE();
	if (!(ret = role_attach_role(data.role, db_query_exe_role(file))))
		return ret;
	PROLE par = subject_query_role(parent);
	if (!par == ROLE_NULL)
		ret = role_attach_parent(data.role, par);
	return ret;
}
