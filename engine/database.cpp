#include "database.h"

LPCTSTR db_get_path(LPCTSTR src, LPCTSTR name)
{
	int src_size = _tcslen(src);
	int size = _tcslen(name);
	TCHAR* ret = new TCHAR[(src_size + size + 1) * sizeof(TCHAR)];
	int k = src_size;
	for (const TCHAR* pos = src + src_size; pos != ret && *pos != '/' && *pos != '\\'; --pos)
		--k;
	for (int i = 0; i < k; ++i)
		ret[i] = src[i];
	for (int i = 0; i < size; ++i)
		ret[k++] = name[i];
	ret[k] = 0;
	return ret;
}

void db_read_buffer(PBUFFER buffer, PFILE file)
{
	fread(&buffer -> hash, sizeof(HASH), 1, file);
	fread(&buffer -> role, sizeof(DWORD), 1, file);
	fread(&buffer -> length, sizeof(int), 1, file);
	buffer -> row = new DM[buffer -> length];
	fread(buffer -> row, sizeof(DM), buffer -> length, file);
}

void db_write_buffer(PBUFFER buffer, PFILE file)
{
	fwrite(&buffer -> hash, sizeof(HASH), 1, file);
	fwrite(&buffer -> role, sizeof(DWORD), 1, file);
	fwrite(&buffer -> length, sizeof(int), 1, file);
	fwrite(buffer -> row, sizeof(DM), buffer -> length, file);
}

void db_release_buffer(PBUFFER buffer, int cnt)
{
	for (int i = 0; i < cnt; ++i)
		if (buffer[i].row)
			delete[] buffer[i].row;
	delete[] buffer;
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

bool db_compare_hash(HASH hash1, HASH hash2)
{
	for (int i = 0; i < HASH_LEN; ++i)
		if (hash1.data[i] != hash2.data[i])
			return false;
	return true;
}

HASH db_get_hash(LPCTSTR file)
{
	HASH ret;
	MD5_CTX context;
	unsigned int length = _tcslen(file);
	MD5Init(&context);
	MD5Update(&context, (unsigned char*) file, length);
	MD5Final(ret.data, &context);
	return ret;
}

DWORD db_query_exe_role(LPCTSTR file)
{
	DWORD ret = ROLE_NULL;
	LPCTSTR path = db_get_path(file, DATA_FILE);
	PFILE input = fopen(path, "rb");
	if (input)
	{
		HASH hash = db_get_hash(file);
		std::pair<PBUFFER, int> buffer = db_fetch(input);
		for (int i = 0; i < buffer.second; ++i)
			if (db_compare_hash(buffer.first[i].hash, hash))
			{
				ret = buffer.first[i].role;
				break;
			}
		db_release_buffer(buffer.first, buffer.second);
	} else {
		//TODO
	}
	return ret;
}

MASK db_query_file(LPCTSTR file, DWORD role)
{
	MASK ret = MASK_NULL;
	LPCTSTR path = db_get_path(file, DATA_FILE);
	PFILE input = fopen(path, "rb");
	if (input)
	{
		HASH hash = db_get_hash(file);
		std::pair<PBUFFER, int> buffer = db_fetch(input);
		for (int i = 0; i < buffer.second; ++i)
			if (db_compare_hash(buffer.first[i].hash, hash))
			{
				for (int j = 0; j < buffer.first[i].length; ++j)
					if (buffer.first[i].row[j].first == role)
					{
						ret = buffer.first[i].row[j].second;
						break;
					}
				break;
			}
		db_release_buffer(buffer.first, buffer.second);
	} else {
		//TODO
	}
	return ret;
}
