#ifndef __HANDLE
#define __HANDLE

#include <map>

template <class OBJECT>
typedef typename std::map<DWORD, OBJECT*> HMAP<OBJECT>;

#endif