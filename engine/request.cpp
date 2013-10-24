#include "request.h"

DWORD request_subject(DWORD subject, LPCTSTR file)
{
	PROLE subject_role = subject_query_role(subject);
	if (role_check_file(subject_role, file, CMD_EXEC))
		return RES_ALLOW;
	else
		return RES_DENY;
}

DWORD request_open(DWORD subject, LPCTSTR file)
{
	PROLE subject_role = subject_query_role(subject);
	if (role_check_file(subject_role, file, CMD_ANY))
		return RES_ALLOW;
	else
		return RES_DENY;
}

DWORD request_access(DWORD subject, HANDLE object, DWORD cmd)
{
	PROLE subject_role = subject_query_role(subject);
	if (role_check_handle(subject_role, object, cmd))
		return RES_ALLOW;
	else
		return RES_DENY;
}
