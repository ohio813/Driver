#include "main.h"

static ULONG OldAddress[SERVICE_COUNT];
static ULONG NewAddress[SERVICE_COUNT];
static bool hooking = false;
static PSHARE pShare = NULL;
static PMDL pMdl = NULL;
static PKEVENT pEvent = NULL;
static PKEVENT pCallBack = NULL;

void OpenProtection()
{
	__asm
	{					
		cli
		push eax
		mov eax,cr0
		and eax,not 10000h
		mov cr0,eax
		pop eax		
	}
	return;
}

void CloseProtection()
{
	__asm
	{			
		push eax		
		mov eax,cr0		
		or eax,10000h
		mov cr0,eax
		pop eax
		sti
	}
	return;
}

ULONG GetAddress(ULONG ServiceID)
{
	ULONG Address = (ULONG)KeServiceDescriptorTable->ServiceTableBase + ServiceID*4;
	return Address;
}

ULONG GetProcAddress(ULONG ServiceID)
{
	OpenProtection();

	ULONG Address = GetAddress(ServiceID);
	ULONG ProcAddress = *((ULONG*)Address);

	CloseProtection();

	return ProcAddress;
}

ULONG ModifyProcAddress(ULONG ServiceID, ULONG NewAddress)
{

	OpenProtection();
	ULONG Address = GetAddress(ServiceID);

	ULONG old = *((ULONG*)Address);
	*((ULONG*)Address) = (ULONG) NewAddress;

	CloseProtection();

	return old;
}

void HookService(PVOID Service, bool hook, bool debug)
{
	ULONG id = SERVICE_INDEX(Service);
	if (hook)
	{
		if (debug)
			DbgPrint("Hook on Service: %d", id);
		OldAddress[id] = ModifyProcAddress(id, NewAddress[id]);
	} else {
		if (debug)
			DbgPrint("Hook off Service: %d", id);
		ModifyProcAddress(id, OldAddress[id]);
	}
}

void LoadAddress(void)
{
	NewAddress[SERVICE_INDEX(ZwWriteFile)] = (ULONG)HookZwWriteFile;
	NewAddress[SERVICE_INDEX(ZwCreateFile)] = (ULONG)HookZwCreateFile;
}

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
	HookService(ZwWriteFile, false, false);
	DbgPrint("Hit: %s", "ZwWriteFile");
	
	LARGE_INTEGER timeout;
	timeout.QuadPart = TIMEOUT;

	bool bExit = false;

	NTSTATUS wait = KeWaitForSingleObject(pCallBack, Executive, UserMode, FALSE, &timeout);
	if (wait == STATUS_TIMEOUT || !hooking)
		bExit = true;

	if (!bExit)
	{
		KeClearEvent(pCallBack);

		pShare -> id = SERVICE_INDEX(ZwWriteFile);

		KeSetEvent(pEvent, IO_NO_INCREMENT, TRUE);
	}

	int code = CODE_ALLOW;
	while (!bExit)
	{

		NTSTATUS wait = KeWaitForSingleObject(pCallBack, Executive, UserMode, FALSE, &timeout);
		int id = pShare -> id;
		code = pShare -> Code;
		if (wait == STATUS_TIMEOUT || !hooking)
		{
			DbgPrint("Timeout or exited. Allowed");
			code = CODE_ALLOW;
			break;
		} else {
			if (id == SERVICE_INDEX(ZwWriteFile))
				break;
		}
	}

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
	if (hooking)
		HookService(ZwWriteFile, true, false);
	return ret;
}

NTSTATUS
NTAPI
HookZwCreateFile (
	OUT PHANDLE FileHandle,
	IN ACCESS_MASK DesiredAccess,
	IN POBJECT_ATTRIBUTES ObjectAttributes,
	OUT PIO_STATUS_BLOCK IoStatusBlock,
	IN PLARGE_INTEGER AllocationSize OPTIONAL,
	IN ULONG FileAttributes,
	IN ULONG ShareAccess,
	IN ULONG CreateDisposition,
	IN ULONG CreateOptions,
	IN PVOID EaBuffer OPTIONAL,
	IN ULONG EaLength
)
{
	HookService(ZwCreateFile, false, false);
	DbgPrint("Hit: %s", "ZwCreateFile");

	LARGE_INTEGER timeout;
	timeout.QuadPart = TIMEOUT;

	bool bExit = false;

	NTSTATUS wait = KeWaitForSingleObject(pCallBack, Executive, UserMode, FALSE, &timeout);
	if (wait == STATUS_TIMEOUT || !hooking)
		bExit = true;

	if (!bExit)
	{
		KeClearEvent(pCallBack);

		pShare -> id = SERVICE_INDEX(ZwCreateFile);

		PUNICODE_STRING pStr = ObjectAttributes -> ObjectName;
		int length = pStr -> Length / sizeof(WCHAR);
		DbgPrint("FileName: %wZ\n", pStr);
		if (pStr -> Length < STR_SIZE)
			RtlCopyMemory(pShare -> Str, pStr -> Buffer, pStr -> Length);
		pShare -> Str[length] = 0;

		KeSetEvent(pEvent, IO_NO_INCREMENT, TRUE);
	}

	int code = CODE_ALLOW;
	while (!bExit)
	{
		NTSTATUS wait = KeWaitForSingleObject(pCallBack, Executive, UserMode, FALSE, &timeout);
		int id = pShare -> id;
		code = pShare -> Code;
		if (wait == STATUS_TIMEOUT || !hooking)
		{
			DbgPrint("Timeout or exited. Allowed");
			break;
		} else {
			if (id == SERVICE_INDEX(ZwCreateFile))
				break;
		}
	}
	
	NTSTATUS ret;
	switch (code)
	{
	case CODE_ALLOW:
		DbgPrint("Allowed\n");
		ret = ZwCreateFile(
			FileHandle,
			DesiredAccess,
			ObjectAttributes,
			IoStatusBlock,
			AllocationSize,
			FileAttributes,
			ShareAccess,
			CreateDisposition,
			CreateOptions,
			EaBuffer,
			EaLength
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
	if (hooking)
		HookService(ZwCreateFile, true, false);
	return ret;
}

NTSTATUS DriverEntry(IN PDRIVER_OBJECT DriverObject, IN PUNICODE_STRING  RegistryPath)
{
	UNICODE_STRING DeviceName,Win32Device;
	PDEVICE_OBJECT DeviceObject = NULL;
	NTSTATUS status;
	unsigned i;

	RtlInitUnicodeString(&DeviceName,L"\\Device\\Hook");
	RtlInitUnicodeString(&Win32Device,L"\\DosDevices\\Hook");

	for (i = 0; i <= IRP_MJ_MAXIMUM_FUNCTION; i++)
		DriverObject->MajorFunction[i] = HookDefaultHandler;

	DriverObject->MajorFunction[IRP_MJ_CREATE] = HookCreateClose;
	DriverObject->MajorFunction[IRP_MJ_CLOSE] = HookCreateClose;
	DriverObject->MajorFunction[IRP_MJ_DEVICE_CONTROL] = HookIoControl;
	
	DriverObject->DriverUnload = HookUnload;

	status = IoCreateDevice(DriverObject,
							0,
							&DeviceName,
							FILE_DEVICE_UNKNOWN,
							0,
							FALSE,
							&DeviceObject);
	if (!NT_SUCCESS(status))
		return status;
	if (!DeviceObject)
		return STATUS_UNEXPECTED_IO_ERROR;

	pShare = (PSHARE)ExAllocatePool(NonPagedPool, sizeof(SHARE));
	pMdl = IoAllocateMdl(pShare, sizeof(PSHARE), FALSE, FALSE, NULL);
	MmBuildMdlForNonPagedPool(pMdl);
	DbgPrint("Share Memory Address:0x%08X\n", (ULONG)pShare);
	pShare -> Code = CODE_ALLOW;

	DeviceObject->Flags |= DO_DIRECT_IO;
	DeviceObject->AlignmentRequirement = FILE_WORD_ALIGNMENT;
	status = IoCreateSymbolicLink(&Win32Device, &DeviceName);

	DeviceObject->Flags &= ~DO_DEVICE_INITIALIZING;

	LoadAddress();

	return STATUS_SUCCESS;
}

void HookUnload(IN PDRIVER_OBJECT DriverObject)
{
	if (hooking)
	{
		HookService(ZwWriteFile, false, true);
		HookService(ZwCreateFile, false, true);
		if (pCallBack)
		{
			KeSetEvent(pCallBack, IO_NO_INCREMENT, TRUE);
			KeWaitForSingleObject(pCallBack, Executive, UserMode, FALSE, NULL);
		}
		hooking = false;
	}

	DbgPrint("Free Share Memory\n");
	IoFreeMdl(pMdl);
	ExFreePool(pShare);

	UNICODE_STRING Win32Device;
	RtlInitUnicodeString(&Win32Device,L"\\DosDevices\\Hook");
	IoDeleteSymbolicLink(&Win32Device);
	IoDeleteDevice(DriverObject->DeviceObject);
}

NTSTATUS HookCreateClose(IN PDEVICE_OBJECT DeviceObject, IN PIRP Irp)
{
	Irp->IoStatus.Status = STATUS_SUCCESS;
	Irp->IoStatus.Information = 0;
	IoCompleteRequest(Irp, IO_NO_INCREMENT);
	return STATUS_SUCCESS;
}

NTSTATUS HookDefaultHandler(IN PDEVICE_OBJECT DeviceObject, IN PIRP Irp)
{
	Irp->IoStatus.Status = STATUS_NOT_SUPPORTED;
	Irp->IoStatus.Information = 0;
	IoCompleteRequest(Irp, IO_NO_INCREMENT);
	return Irp->IoStatus.Status;
}

NTSTATUS HookIoControl(IN PDEVICE_OBJECT DeviceObject, IN PIRP Irp)
{
	DbgPrint("Device Control Detected\n");
	Irp->IoStatus.Status = STATUS_SUCCESS;
	Irp->IoStatus.Information = 0;
	PIO_STACK_LOCATION IrpStack = IoGetCurrentIrpStackLocation(Irp);
	switch (IrpStack->Parameters.DeviceIoControl.IoControlCode)
	{
	case HOOK_ON:
	{
		if (hooking)
			DbgPrint("Already Hooking!\n");
		else
		{
			PIO_PACKAGE pBuffer = (PIO_PACKAGE)Irp->AssociatedIrp.SystemBuffer;
			ULONG size = IrpStack->Parameters.DeviceIoControl.InputBufferLength; 
			if (pBuffer == NULL || size < sizeof(IO_PACKAGE))
			{
				DbgPrint("Buffer Error\n");
				Irp->IoStatus.Status = STATUS_UNEXPECTED_IO_ERROR;
				break;
			}
			NTSTATUS status1 = ObReferenceObjectByHandle(pBuffer->hEvent,
        												SYNCHRONIZE,
        												*ExEventObjectType,
        												KernelMode,
         												(PVOID *)&pEvent,
         												NULL);
			NTSTATUS status2 = ObReferenceObjectByHandle(pBuffer->hCallBack,
        												SYNCHRONIZE,
        												*ExEventObjectType,
        												KernelMode,
         												(PVOID *)&pCallBack,
         												NULL);
			if (!NT_SUCCESS(status1) || ! NT_SUCCESS(status2))
			{
				DbgPrint("Reference Error\n");
				Irp->IoStatus.Status = STATUS_UNEXPECTED_IO_ERROR;
				break;
			}
			HookService(ZwWriteFile, true, true);
			HookService(ZwCreateFile, true, true);
			hooking = true;
		}
		break;
	}
	case HOOK_OFF:
	{
		if (hooking)
		{
			HookService(ZwWriteFile, false, true);
			HookService(ZwCreateFile, false, true);
			hooking = false;
			if (pCallBack)
			{
				KeSetEvent(pCallBack, IO_NO_INCREMENT, TRUE);
				KeWaitForSingleObject(pCallBack, Executive, UserMode, FALSE, NULL);
			}
		}
		else
			DbgPrint("Not Hooking\n");
		break;
	}
	case GET_ADD:
	{
		PVOID * pBuffer = (PVOID *)Irp->UserBuffer;
		ULONG size = IrpStack->Parameters.DeviceIoControl.OutputBufferLength;
		if (pBuffer == NULL || size < sizeof(PSHARE))
		{
			DbgPrint("Buffer Error\n");
			Irp->IoStatus.Status = STATUS_UNEXPECTED_IO_ERROR;
			break;
		}
		PVOID UserAdd = MmMapLockedPages(pMdl, UserMode);
		*pBuffer = UserAdd;
		DbgPrint("UserMode Address:0x%08X\n", UserAdd);
		break;
	}
	default:
		DbgPrint("Unknown Control\n");
		Irp->IoStatus.Status = STATUS_NOT_SUPPORTED;
	}
	NTSTATUS ret = Irp->IoStatus.Status;
	IoCompleteRequest(Irp, IO_NO_INCREMENT);
	return ret;
}
