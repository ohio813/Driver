extern "C" 
{
	#include <ntddk.h>
	#include <ntddstor.h>
	#include <mountdev.h>
	#include <ntddvol.h>
}

#define HOOK_INDEX 				0x860 
#define HOOK_ON					CTL_CODE(FILE_DEVICE_UNKNOWN,HOOK_INDEX + 0,METHOD_BUFFERED,FILE_ANY_ACCESS)
#define HOOK_OFF				CTL_CODE(FILE_DEVICE_UNKNOWN,HOOK_INDEX + 1,METHOD_BUFFERED,FILE_ANY_ACCESS)
#define GET_ADD					CTL_CODE(FILE_DEVICE_UNKNOWN,HOOK_INDEX + 2,METHOD_BUFFERED,FILE_ANY_ACCESS)
#define SERVICE_INDEX(FuncName)	(*(PULONG)((PUCHAR)FuncName+1))
#define CODE_ALLOW				0xff
#define CODE_DENY				0

//Driver Functions
extern "C" NTSTATUS DriverEntry(IN PDRIVER_OBJECT DriverObject, IN PUNICODE_STRING  RegistryPath);
void HookUnload(IN PDRIVER_OBJECT DriverObject);
NTSTATUS HookCreateClose(IN PDEVICE_OBJECT DeviceObject, IN PIRP Irp);
NTSTATUS HookDefaultHandler(IN PDEVICE_OBJECT DeviceObject, IN PIRP Irp);
NTSTATUS HookIoControl(IN PDEVICE_OBJECT DeviceObject, IN PIRP Irp);

//Hook Functions
void OpenProtection(void);
void CloseProtection(void);
ULONG GetAddress(ULONG ServiceID);
ULONG GetProcAddress(ULONG ServiceID);
ULONG ModifyProcAddress(ULONG ServiceID, ULONG NewAddress);

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
);

//SSDT
typedef struct _SERVICE_DESCRIPTOR_TABLE
{
	PVOID ServiceTableBase;
	PULONG ServiceCounterTableBase;
	ULONG NumberOfService;
	ULONG ParamTableBase;
}SERVICE_DESCRIPTOR_TABLE, *PSERVICE_DESCRIPTOR_TABLE;
extern "C" PSERVICE_DESCRIPTOR_TABLE KeServiceDescriptorTable;

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