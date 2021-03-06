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

PLOG subject_query_log(DWORD subject)
{
	subject_iter ret = subjects.find(subject);
	if (ret == subjects.end())
		return NULL;
	else
		return &(ret -> second.log);
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
	log_query_file(file, data.log);
	data.role = new ROLE();
	if (!(ret = role_attach_role(data.role, db_query_exe_role(file))))
		return ret;
	PROLE par = subject_query_role(parent);
	if (!par == ROLE_NULL)
		ret = role_attach_parent(data.role, par);
	return ret;
}

double subject_compare(DWORD subject1, DWORD subject2)
{
	DWORD sum1 = 0;
	DWORD sum2 = 0;
	PLOG log1 = subject_query_log(subject1);
	PLOG log2 = subject_query_log(subject2);
	for (LOG_iter i = log1 -> begin(); i != log1 -> end(); ++i)
		sum1 += i -> second;
	for (LOG_iter i = log2 -> begin(); i != log2 -> end(); ++i)
		sum2 += i -> second;
	double ret = .0;
	for (LOG_iter i = log1 -> begin(); i != log1 -> end(); ++i)
	{
		LOG_iter j = log2 -> find(i -> first);
		double k1 = (i -> second + .0) / (sum1 + .0);
		double k2 = .0;
		if (j != log2 -> end())
			k2 = (j -> second + .0) / (sum1 + .0);
		ret += (k1 - k2) * (k1 - k2);
	}
	for (LOG_iter i = log2 -> begin(); i != log2 -> end(); ++i)
	{
		LOG_iter j = log1 -> find(i -> first);
		double k1 = (i -> second + .0) / (sum1 + .0);
		double k2 = .0;
		if (j != log1 -> end())
			k2 = (j -> second + .0) / (sum1 + .0);
		ret += (k1 - k2) * (k1 - k2);
	}
	return ret;
}
