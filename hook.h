#ifndef __HOOK
#define __HOOK

#include "main.h"

NTSYSAPI
NTSTATUS
NTAPI
HookZwOpenProcess (
    __out PHANDLE ProcessHandle,
    __in ACCESS_MASK DesiredAccess,
    __in POBJECT_ATTRIBUTES ObjectAttributes,
    __in_opt PCLIENT_ID ClientId
);

NTSTATUS
NTAPI
HookZwWriteFile(
	__in HANDLE FileHandle,
	__in HANDLE Event OPTIONAL,
	__in PIO_APC_R__OUT__inE ApcRoutine OPTIONAL,
	__in PVOID ApcContext OPTIONAL,
	__out PIO_STATUS_BLOCK IoStatusBlock,
	__in PVOID Buffer,
	__in ULONG Length,
	__in PLARGE___inTEGER ByteOffset OPTIONAL,
	__in PULONG Key OPTIONAL
);

#endif