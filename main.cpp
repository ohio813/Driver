#include "main.h"
#include "hook.h"

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

	pShare = (PSHARE)ExAllocatePool(NonPagedPool, sizeof(PSHARE));
	pMdl = IoAllocateMdl(pShare, sizeof(PSHARE), FALSE, FALSE, NULL);
	MmBuildMdlForNonPagedPool(pMdl);
	DbgPrint("Share Memory Address:0x%08X\n", (ULONG)pShare);
	DbgPrint("SSDT count:%d\n", KeServiceDescriptorTable->NumberOfService);
	pShare -> Code = CODE_ALLOW;

	DeviceObject->Flags |= DO_DIRECT_IO;
	DeviceObject->AlignmentRequirement = FILE_WORD_ALIGNMENT;
	status = IoCreateSymbolicLink(&Win32Device, &DeviceName);

	DeviceObject->Flags &= ~DO_DEVICE_INITIALIZING;

	return STATUS_SUCCESS;
}

void HookUnload(IN PDRIVER_OBJECT DriverObject)
{
	DbgPrint("Free Share Memory\n");
	IoFreeMdl(pMdl);
	ExFreePool(pShare);

	if (hooking)
		ModifyProcAddress(SERVICE_INDEX(ZwWriteFile), OldAddress);

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
			DbgPrint("Hook On of service %d\n", SERVICE_INDEX(ZwWriteFile));
			NewAddress = (ULONG)HookZwWriteFile;
			OldAddress = ModifyProcAddress(SERVICE_INDEX(ZwWriteFile), NewAddress);
			hooking = true;
		}
		break;
	}
	case HOOK_OFF:
	{
		if (hooking)
		{
			DbgPrint("Hook Off\n");
			ModifyProcAddress(SERVICE_INDEX(ZwWriteFile), OldAddress);
			hooking = false;
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