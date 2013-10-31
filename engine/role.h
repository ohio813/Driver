#ifndef __ROLE
#define __ROLE

#include <windows.h>
#include <set>
#include <map>

#define ROLE_NULL	0
#define CMD_READ 	0x0
#define CMD_WRITE	0xff
#define CMD_EXEC	0x88
#define CMD_ANY		0x55

typedef unsigned char MASK, *PMASK;

#define CHECK_CMD(mask, offset)	((mask << offset) % 2)
#define CHECK_ANY(mask)			(mask)

typedef std::map<HANDLE, MASK> MATRIX, *PMATRIX;
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

extern role_map roles;

extern "C" 
{
	bool role_check_handle(PROLE, HANDLE, DWORD);
	bool role_check_file(PROLE, LPCTSTR, DWORD);
	bool role_attach_role(PROLE, DWORD);
	bool role_attach_parent(PROLE, PROLE);
	bool role_dispose(PROLE);
}

#endif
