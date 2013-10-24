#ifndef __ROLE
#define __ROLE

#include <windows.h>
#include <set>
#include <map>

typedef struct _mask
{
	bool read;
	bool write;
	bool exec;
} MASK, *PMASK;

typedef std::map<DWORD, MASK> MATRIX, *PMATRIX;
typedef MATRIX::iterator MATRIX_iter;

typedef struct _role_info
{
	MATRIX matrix;
	int ref_count;
	_role_info()
	{
		ref_count = 1;
	}
} ROLE_INFO, *PROLE_INFO;

typedef std::map<DWORD, PROLE_INFO> role_map;
typedef role_map::iterator role_iter;

typedef std::set<DWORD> ROLE, *PROLE;

extern role_map roles

extern "C" 
{
	bool role_check(PROLE, HANDLE, DWORD);
	bool role_check(PROLE, LPCTSTR, DWORD);
	bool role_attach(PROLE, DWORD);
	bool role_attach(PROLE, PROLE);
	bool role_dispose(PROLE);
}

#endif