##ifndef __OBJECT
#define __OBJECT

#include <windows.h>

struct object
{
	HANDLE hObj;
	int type;
};

#endif