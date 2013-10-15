#ifndef __OBJECT
#define __OBJECT

#include <windows.h>

typedef struct _object
{
	int type;
	LPCTSTR path;
}OBJECT, *POBJECT;

typedef std::map<HANDLE, POBJECT> object_map;
typedef object_map::iterator object_iter;

extern object_map objects;

extern "C" {

} 

#endif