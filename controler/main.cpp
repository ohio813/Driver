#include "main.h"

HANDLE hEvent, hCallBack;
PSHARE pShare;
bool bExit = false;
DWORD nothing;
HANDLE hThread[THREADS];

void hook(HANDLE hDevice)
{
	DeviceIoControl(hDevice,
					GET_ADD,
					NULL,
					0,
					&pShare,
					sizeof(PSHARE),
					&nothing,
					0);
	IO_PACKAGE package;
	hEvent = package.hEvent = CreateEvent(NULL, FALSE, FALSE, NULL);
	hCallBack = package.hCallBack = CreateEvent(NULL, TRUE, FALSE, NULL);
	DeviceIoControl(hDevice,
					HOOK_ON,
					&package,
					sizeof(IO_PACKAGE),
					NULL,
					0,
					&nothing,
					0);
}

void unhook(HANDLE hDevice)
{
	DeviceIoControl(hDevice,
					HOOK_OFF,
					NULL,
					0,
					NULL,
					0,
					&nothing,
					0);
}

DWORD WINAPI work (LPVOID lpParam)
{
	while (true)
	{
		WaitForSingleObject(hEvent, INFINITE);
		if (bExit)
			return 0;
		pShare -> Code = CODE_ALLOW;
		PulseEvent(hCallBack);
		printf("Event hit: %d\n", pShare -> id);
		if (pShare -> id == ID_ZwCreateFile)
			printf("%S\n", pShare -> Str);
		printf("Allowed\n");
	}
}

int main(int argc, char** argv)
{
	HANDLE hDevice = CreateFile("\\\\.\\Hook",
								GENERIC_READ|GENERIC_WRITE,
								0,
								NULL,
								OPEN_EXISTING,
								0,
								NULL);
	if (hDevice == INVALID_HANDLE_VALUE)
	{
		printf("Error\n");
		return 0;
	}
	if (argc == 1)
	{
		hook(hDevice);
		DWORD dwThread;
		for (int i = 0; i < THREADS; ++i)
			hThread[i] = CreateThread(NULL, 0, work, NULL, 0, &dwThread);
		while (true)
		{
			char c = getchar();
			if (c == 'q')
				break;
		}
		bExit = true;
		for (int i = 0; i < THREADS; ++i)
			SetEvent(hEvent);
		for (int i = 0; i < THREADS; ++i)
			CloseHandle(hThread[i]);
		unhook(hDevice);
		CloseHandle(hEvent);
		CloseHandle(hCallBack);
	} else {
		unhook(hDevice);
	}
	return 0;
}
