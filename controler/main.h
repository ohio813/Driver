#include <cstdlib>
#include <cstdio>
#include <windows.h>

#define HOOK_INDEX 				0x860 
#define HOOK_ON					CTL_CODE(FILE_DEVICE_UNKNOWN,HOOK_INDEX + 0,METHOD_BUFFERED,FILE_ANY_ACCESS)
#define HOOK_OFF				CTL_CODE(FILE_DEVICE_UNKNOWN,HOOK_INDEX + 1,METHOD_BUFFERED,FILE_ANY_ACCESS)
#define GET_ADD					CTL_CODE(FILE_DEVICE_UNKNOWN,HOOK_INDEX + 2,METHOD_BUFFERED,FILE_ANY_ACCESS)
#define CODE_ALLOW				0xff
#define CODE_DENY

//Share Memory
typedef struct _SHARE
{
	int Code;
}SHARE, *PSHARE;

//IO
typedef struct _IO_PACKAGE
{
	HANDLE hEvent;
	HANDLE hCallBack;
}IO_PACKAGE, *PIO_PACKAGE;