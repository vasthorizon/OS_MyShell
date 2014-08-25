#include <stdio.h>
#include <tchar.h>
#include <locale.h>
#include <windows.h> 
#include <tlhelp32.h>
#include <string>

#include "global.h"
#include "process.h"

void ListProcessInfo()
{
	HANDLE hProcessHand = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
	if (hProcessHand == INVALID_HANDLE_VALUE)
	{
		_tprintf_s(_T("Error in CreateToolhelp32Snapshot \n"));
		return;
	}

	PROCESSENTRY32 pe32;
	pe32.dwSize = sizeof(PROCESSENTRY32);

	if (!Process32First(hProcessHand, &pe32))
	{
		_tprintf_s(_T("Error in getting first process! \n"));
		CloseHandle(hProcessHand);
		return;
	}

	do
	{
		_tprintf(_T("%25s %5d \n"), pe32.szExeFile, pe32.th32ProcessID);
	} while (Process32Next(hProcessHand, &pe32));

	CloseHandle(hProcessHand);
}

// kills the process in command token list [1]
void KillProcess()
{
	HANDLE hProcessHand = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
	if (hProcessHand == INVALID_HANDLE_VALUE)
	{
		_tprintf_s(_T("Error in CreateToolhelp32Snapshot \n"));
		return;
	}

	PROCESSENTRY32 pe32;
	pe32.dwSize = sizeof(PROCESSENTRY32);

	if (!Process32First(hProcessHand, &pe32))
	{
		_tprintf_s(_T("Error in getting first process! \n"));
		CloseHandle(hProcessHand);
		return;
	}

	HANDLE hProcess = nullptr;
	BOOL isKilled = FALSE;

	do
	{
		if (_tcscmp(pe32.szExeFile, cmdTokenList[1]) == 0)
		{
			hProcess = OpenProcess(PROCESS_ALL_ACCESS, FALSE, pe32.th32ProcessID);

			if (hProcess != NULL)
			{
				TerminateProcess(hProcess, -1);
				isKilled = true;
			}

			CloseHandle(hProcess);
			break;
		}
	} while (Process32Next(hProcessHand, &pe32));

	CloseHandle(hProcessHand);

	if (isKilled == FALSE)
		_tprintf_s(_T("Yo! Kill Process failed! \n"));

}