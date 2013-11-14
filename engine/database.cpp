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
	fread(&buffer -> role, sizeof(DWORD), 1, file);
	fread(&buffer -> length, sizeof(int), 1, file);
	buffer -> mask = new MASK[buffer -> length];
	fread(buffer -> mask, sizeof(MASK), buffer -> length, file);
}

void db_write_buffer(PBUFFER buffer, PFILE file)
{
	fwrite(buffer -> hash, sizeof(unsigned char), HASH_LEN, file);
	fwrite(&buffer -> role, sizeof(DWORD), 1, file);
	fwrite(&buffer -> length, sizeof(int), 1, file);
	fwrite(buffer -> mask, sizeof(MASK), buffer -> length, file);
}

std::pair<PBUFFER, int> db_fetch(PFILE file)
{
	int count;
	fread(&count, sizeof(int), 1, file);
	PBUFFER ret = new BUFFER[count];
	for (int i = 0; i < count; ++i)
		db_read_buffer(ret + i, file);
	return std::make_pair(ret, count);
}

bool db_compare_hash(unsigned char* hash1, unsigned char* hash2)
{
	for (int i = 0; i < HASH_LEN; ++i)
		if (hash1[i] != hash2[i])
			return false;
	return true;
}

void db_get_hash(LPCTSTR file, unsigned char* ret)
{
	MD5_CTX context;
	unsigned int length = strlen(file);
	MD5Init(&context);
	MD5Update(&context, (unsigned char*) file, length);
	MD5Final(ret, &context);
}

DWORD db_query_exe_role(LPCTSTR file)
{
	DWORD ret = ROLE_NULL;
	LPCTSTR db_get_path(file);
	PFILE input = fopen(file, "rb");
	if (input)
	{
		unsigned char hash[HASH_LEN];
		db_get_hash(file, hash);
		std::pair<PBUFFER, int> buffer = db_fetch(input);
		for (int i = 0; i < buffer.second; ++i)
			if (db_compare_hash(buffer.first[i].hash, hash))
			{
				ret = buffer.first[i].role;
				break;
			}
		delete []buffer.first;
	} else {
		//TODO
	}
	return ret;
}
