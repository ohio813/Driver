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

void db_read_buffer(PBUFFER buffer, PFILE file)
{
	fread(buffer -> hash, sizeof(unsigned char), HASH_LEN, file);
	fread(&buffer -> length, sizeof(int), 1, file);
	buffer -> mask = new MASK[length];
	fread(buffer -> mask, sizeof(MASK), length, file);
}

void db_write_buffer(PBUFFER buffer, PFILE file)
{
	fwrite(buffer -> hash, sizeof(unsigned char), HASH_LEN, file);
	fwrite(&buffer -> length, sizeof(int), 1, file);
	fwrite(buffer -> mask, sizeof(MASK), buffer -> length, file);
}

PBUFFER db_fetch(PFILE file)
{
	int count;
	fread(&count, sizeof(int), 1, file);
	PBUFFER ret = new PBUFFER[count];
	for (int i = 0; i < count; ++i)
		db_read_buffer(ret + i, file);
	return ret;
}

DWORD db_query_exe_role(LPCTSTR file)
{
	LPCTSTR db_get_path(file);
	PFILE input = fopen(file, "rb");
	if (input)
	{
		
	} else {
		//TODO
		return ROLE_NULL;
	}
}
