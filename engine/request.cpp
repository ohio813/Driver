#include "request.h"

DWORD request_subject(DWORD subject, LPCTSTR file)
{
	PROLE subject_role = subject_query_role(subject);
	DWORD object = object_query_name(file);
	if (role_check(subject_role, object, CMD_EXEC))
		return RES_ALLOW;
	else
		return RES_DENY;
}

DWORD request_open(DWORD subject, LPCTSTR object)
{
	PROLE subject_role = subject_query_role(subject);
	DWORD object = object_query_name(object);
	if (role_check(subject_role, object, CMD_ANY))
		return RES_ALLOW;
	else
		return RES_DENY;
}

DWORD request_access(DWORD subject, HANDLE object, DWORD cmd)
{
	PROLE subject_role = subject_query_role(subject);
	DWORD object = object_query_handle(object);
	if (role_check(subject_role, object, cmd))
		return RES_ALLOW;
	else
		return RES_DENY;
}