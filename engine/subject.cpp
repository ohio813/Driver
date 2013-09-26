#include "subject.h"

subject_map subjects;

DWORD subject_query_role(DWORD subject)
{
	subject_iter ret = subjects[subject].find(subject);
	if (ret == subjects.end())
		return ROLE_NULL;
	else
		return ret->second.role;
}

DWORD subject_clear(DWORD subject)
{
	subjects.erase(subject);
}

DWORD subject_store(DWORD pid, LPCTSTR file)
{
	
}