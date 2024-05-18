#include <windows.h>
#include <iostream>
#include <ctime>
#include <stdlib.h>
#include "hidapi.h"
#pragma comment(lib,"hidapi.lib")
using namespace std;

// shared memory
static TCHAR szName[] = TEXT("sdvxrgb");
HANDLE hMapFile;
LPSTR pBuf;
const int DATA_SIZE = 1284;

// hid variables
hid_device* handle;
const int vid = 0x1234; // vendor id
const int pid = 0x1234; // product id

// program variables
int openFailedCounter;
const int timeout = 100; // waiting 100 seconds before exit.

static void Delay(int time)
{
	clock_t now = clock();
	while (clock() - now < time);
}

static int openSharedMemory()
{
	hMapFile = OpenFileMapping(FILE_MAP_READ, FALSE, szName);
	if (hMapFile == NULL) return 0;
	pBuf = (LPSTR)MapViewOfFile(hMapFile, FILE_MAP_READ, 0, 0, DATA_SIZE);
	if (pBuf == NULL) return 0;
	return 1;
}

static void closeSharedMemory()
{
	UnmapViewOfFile(pBuf);
	CloseHandle(hMapFile);
}

static void exitFunction()
{
	printf("[Exit]\nCleaning up...");
	if (handle) hid_close(handle);
	hid_exit();
	printf("Done");
	exit(0);
}

static BOOL WINAPI ctrlHandler(DWORD dwCtrlType)
{
	switch (dwCtrlType)
	{
	case CTRL_C_EVENT:
	case CTRL_BREAK_EVENT:
	case CTRL_CLOSE_EVENT:
	case CTRL_LOGOFF_EVENT:
	case CTRL_SHUTDOWN_EVENT:
		exitFunction();
		break;
	}
	return 0;
}

int main() {
	SetConsoleCtrlHandler(ctrlHandler, TRUE);
	// open hid device
	int res;
	res = hid_init(); // initialize the hidapi
	if (res)
	{
		printf("Unable initialize hidapi, code: %d\n", res);
		return 0;
	}
	handle = hid_open(vid, pid, NULL);
	if (!handle)
	{
		printf("Unable to open device\n");
		Delay(2000);
		return 0;
	}

	Delay(1000);
	while (1)
	{
		if (openSharedMemory())
		{
			// reset
			if (openFailedCounter)
			{
				system("cls");
				printf("Started reading shared memory...\n");
			}
			openFailedCounter = 0;

			// copy data
			uint8_t lightData[DATA_SIZE]{};
			memcpy(lightData, pBuf, DATA_SIZE);

			// send data to the device
			uint8_t buf[65]{};
			int sendSize = 63;
			for (int i = 0; i < 21; i++) // divide into 21 packets to send
			{
				if (i == 20) sendSize = 24; // last packet has 24 bytes of data
				buf[1] = i;
				memcpy(buf + 2, lightData + i * 63, sendSize);
				res = hid_write(handle, buf, 65);
				if (res == -1) printf("Unable to send data, code: %d\n", res);
			}
		}
		else
		{
			// reset
			if (!openFailedCounter)
			{
				system("cls");
				printf("Started waiting for shared memory...\n");
			}

			// wait for shared memory
			if (openFailedCounter > timeout) return 0; // exit after waiting some time
			printf("[ %03d ] Waiting for shared memory to be created.\n",openFailedCounter);
			openFailedCounter++;
			Delay(1000);
		}
		closeSharedMemory();
	}
	return 0;
}