#include "main.h"

HANDLE hEvent, hCallBack;
PSHARE pShare;
bool bExit = false;

bool hook(HANDLE hDevice)
{
	DWORD nothing;
	if (DeviceIoControl(hDevice,
						GET_ADD,
						NULL,
						0,
						&pShare,
						sizeof(PSHARE),
						&nothing,
						0) == 0);
	{
		return false;
	}
	IO_PACKAGE package;
	hEvent = package.hEvent = CreateEvent(NULL, FALSE, FALSE, NULL);
	hCallBack = package.hCallBack = CreateEvent(NULL, FALSE, FALSE, NULL);
	if (DeviceIoControl(hDevice,
						HOOK_ON,
						&package,
						sizeof(IO_PACKAGE),
						NULL,
						0,
						&nothing,
						0) == 0);
	{
		CloseHandle(hEvent);
		CloseHandle(hCallBack);
		return false;
	}
	return true;
}

DWORD WINAPI work (LPVOID lpParam)
{
	while (true)
	{
		WaitForSingleObject(hCallBack, INFINITE);
		if (bExit)
			return 0;
		pShare -> Code = CODE_ALLOW;
		SetEvent(hCallBack);
		printf("Event hit: NtOpenProcess\n");
		printf("Allowed\n");
	}
}

int main()
{
	HANDLE hDevice = CreateFile("\\\\.\\Hook",
								GENERIC_READ|GENERIC_WRITE,
								0,
								NULL,
								OPEN_EXISTING,
								0,
								NULL);
	if (hDevice == INVALID_HANDLE_VALUE || !hook(hDevice))
	{
		printf("Error\n");
		return 0;
	}
	DWORD dwThread;
	HANDLE hThread = CreateThread(NULL, 0, work, NULL, 0, &dwThread);
	while (true)
	{
		char str[20];
		scanf("%s", str);
		if (strcmp(str, "stop"))
		{
			bExit = true;
			DWORD nothing;
			if (DeviceIoControl(hDevice,
								HOOK_OFF,
								NULL,
								0,
								NULL,
								0,
								&nothing,
								0) == 0)
			{
				printf("Hook off error!\n");
			} else {
				bExit = true;
				SetEvent(hCallBack);
				CloseHandle(hEvent);
				CloseHandle(hCallBack);
				CloseHandle(hThread);
				return 0;
			}
		}
	}
}
