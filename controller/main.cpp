#include "main.h"

HANDLE hEvent[SHARE_COUNT];
HANDLE hCallback[SHARE_COUNT];
PSHARE pShare = NULL;
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

	for (int i = 0; i < SHARE_COUNT; ++i)
	{
		hEvent[i] = package.hEvent[i] = CreateEvent(NULL, FALSE, FALSE, NULL);
		hCallback[i] = package.hCallback[i] = CreateEvent(NULL, FALSE, FALSE, NULL);
	}

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
		DWORD dwShareID = WaitForMultipleObjects(SHARE_COUNT, hEvent, FALSE, INFINITE) - WAIT_OBJECT_0;
		if (bExit)
			return 0;
		PSHARE pShareBase = pShare + dwShareID;

		pShareBase -> Code = CODE_ALLOW;
		
		printf("Event hit: %d\n", pShareBase -> id);
		if (pShare -> id == ID_ZwCreateFile)
			printf("%S\n", pShareBase -> Str);
		printf("Allowed\n");

		SetEvent(hCallback[dwShareID]);
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
	for (int i = 0; i < SHARE_COUNT; ++i)
	{
		SetEvent(hEvent[i]);
		CloseHandle(hEvent[i]);
		CloseHandle(hCallback[i]);
	}
	for (int i = 0; i < THREADS; ++i)
		CloseHandle(hThread[i]);
	unhook(hDevice);

	return 0;
}
