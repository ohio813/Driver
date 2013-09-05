#include "hook.h"

NTSTATUS
NTAPI
HookZwWriteFile(
	IN HANDLE FileHandle,
	IN HANDLE Event OPTIONAL,
	IN PIO_APC_ROUTINE ApcRoutine OPTIONAL,
	IN PVOID ApcContext OPTIONAL,
	OUT PIO_STATUS_BLOCK IoStatusBlock,
	IN PVOID Buffer,
	IN ULONG Length,
	IN PLARGE_INTEGER ByteOffset OPTIONAL,
	IN PULONG Key OPTIONAL
)
{
	ModifyProcAddress(SERVICE_INDEX(ZwWriteFile), OldAddress);
	DbgPrint("Hit ZwWriteFile");
	KeSetEvent(pEvent, IO_NO_INCREMENT, FALSE);
	KeWaitForSingleObject(pCallBack, Executive, UserMode, FALSE, NULL);

	int code = pShare -> Code;
	NTSTATUS ret;
	switch (code)
	{
	case CODE_ALLOW:
		DbgPrint("Allowed\n");
		ret = ZwWriteFile(
			FileHandle,
			Event,
			ApcRoutine,
			ApcContext,
			IoStatusBlock,
			Buffer,
			Length,
			ByteOffset,
			Key
			);
		break;
	case CODE_DENY:
		DbgPrint("Denied\n");
		ret = STATUS_ACCESS_DENIED;
		break;
	default:
		DbgPrint("UNEXPECTED\n");
		ret = STATUS_UNEXPECTED_IO_ERROR;
	}
	ModifyProcAddress(SERVICE_INDEX(ZwWriteFile), NewAddress);
	return ret;
}