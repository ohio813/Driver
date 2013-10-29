#include "database.h"

LPCTSTR db_get_path(LPCTSTR src)
{
	int src_size = _tcslen(src);
	int size = _tcslen(DATA_FILE);
	TCHAR* ret = new TCHAR[(src_size + size + 1) * sizeof(TCHAR)];
	int k = src_size;
	for (const TCHAR* pos = src + src_size; pos != ret && *pos != '/' && *pos != '\\'; --pos)
		--k;
	for (int i = 0; i < k; ++i)
		ret[i] = src[i];
	for (int i = 0; i < size; ++i)
		ret[k++] = DATA_FILE[i];
	ret[k] = 0;
	return ret;
}

DWORD db_query_exe_role(LPCTSTR file)
{
	LPCTSTR db_get_path(file);
	FILE* input = fopen(file, "rb");
	if (input)
	{
		
	} else {
		//TODO
		return ROLE_NULL;
	}
}
