

#include <stdio.h>
#include <tchar.h>
#include <locale.h>
#include <windows.h> 
#include <tlhelp32.h>
#include <string>

#define STR_LEN    256
#define CMD_TOKEN_NUM  10

TCHAR ERROR_CMD[]
= _T("'%s'은(는) 실행할 수 있는 프로그램이 아닙니다. \n");

TCHAR cmdString[STR_LEN];
TCHAR cmdTokenList[CMD_TOKEN_NUM][STR_LEN];
TCHAR seps[] = _T(" ,\t\n");

int CmdReadTokenize(void);
int CmdProcessing(int);
TCHAR * StrLower(TCHAR *);
void ShowFilesInDirectory(TCHAR *filePath);
void ListProcessInfo();
void KillProcess();

int _tmain(int argc, TCHAR * argv[])
{
	// 한글 입력을 가능케 하기 위해.
	_tsetlocale(LC_ALL, _T("Korean"));

	if (argc > 2)  // 매개변수 전달인자가 있는 경우.
	{
		for (int i = 1; i<argc; i++)
			_tcscpy_s(cmdTokenList[i - 1], argv[i]);

		CmdProcessing(argc - 1);
	}

	DWORD isExit = NULL;
	while (1)
	{
		int tokenNum = CmdReadTokenize();
		if (tokenNum == 0) //Enter 입력시 처리를 위해...
			continue;

		isExit = CmdProcessing(tokenNum);
		if (isExit == TRUE)
		{
			_fputts(_T("명령어 처리를 종료합니다. \n"), stdout);
			break;
		}
	}

	return 0;
}

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

int CmdReadTokenize(void)
{
	TCHAR * token;
	TCHAR * nextToken;

	_fputts(_T("Best command prompt(type 'h' for help(>> "), stdout);
	_getts_s(cmdString);

	token = _tcstok_s(cmdString, seps, &nextToken);

	int tokenNum = 0;

	while (token != NULL)
	{
		_tcscpy_s(cmdTokenList[tokenNum++], StrLower(token));
		token = _tcstok_s(NULL, seps, &nextToken);
	}

	return tokenNum;
}

int CmdProcessing(int tokenNum)
{
	BOOL isRun;
	STARTUPINFO si = { 0, };
	PROCESS_INFORMATION pi;

	TCHAR cmdStringWithOptions[STR_LEN] = { 0, };
	TCHAR optString[STR_LEN] = { 0, };

	si.cb = sizeof(si);
	if (!_tcscmp(cmdTokenList[0], _T("exit")))
	{
		return TRUE;
	}
	else if (!_tcscmp(cmdTokenList[0], _T("start")))
	{
		if (tokenNum >1)
		{
			for (int i = 1; i<tokenNum; i++)
				_stprintf_s(optString, _T("%s %s"), optString, cmdTokenList[i]);

			_stprintf_s(cmdStringWithOptions, _T("%s %s"), _T("MyShell.exe"), optString);
		}
		else
			_stprintf_s(cmdStringWithOptions, _T("%s"), _T("MyShell.exe"));

		isRun = CreateProcess(NULL, cmdStringWithOptions, NULL, NULL, TRUE, CREATE_NEW_CONSOLE, NULL, NULL, &si, &pi);

		CloseHandle(pi.hProcess);
		CloseHandle(pi.hThread);
	}
	else if (!_tcscmp(cmdTokenList[0], _T("echo")))
	{
		for (int i = 1; i<tokenNum; i++)
			_stprintf_s(optString, _T("%s %s"), optString, cmdTokenList[i]);

		_tprintf(_T("echo message:%s \n"), optString);
	}
	else if (!_tcscmp(cmdTokenList[0], _T("pwd")))
	{
		_fputts(_T("Input password to show current directory:>"), stdout);
		TCHAR inputStr[STR_LEN];
		_getts_s(inputStr);

		if (!_tcscmp(inputStr, _T("mypwd")))
		{
			ShowFilesInDirectory(_T("."));
		}
		else
		{
			printf_s("\nWrong Password! Correct password is 'mypwd'\n");
		}

	}
	else if (!_tcscmp(cmdTokenList[0], _T("dir")))
	{
		int paramLen = _tcslen(cmdTokenList[1]);

		if (paramLen == 0)
			ShowFilesInDirectory(_T("."));
		else
			ShowFilesInDirectory(cmdTokenList[1]);
	}
	else if (!_tcscmp(cmdTokenList[0], _T("help"))
		|| !_tcscmp(cmdTokenList[0], _T("?"))
		|| !_tcscmp(cmdTokenList[0], _T("h"))
		)
	{
		printf_s("\n<commands>\nkillprocess(kp) explorer.exe \nlistproc(lp) \necho \nexplorer c:\\  \ndir \ndir .. \ndir C:\\ \nhelp \npwd \nexit \nstart \n\n");
	}
	else if (!_tcscmp(cmdTokenList[0], _T("listproc"))
		|| !_tcscmp(cmdTokenList[0], _T("lp")))
	{
		ListProcessInfo();
	}
	else if (!_tcscmp(cmdTokenList[0], _T("killproc"))
		|| !_tcscmp(cmdTokenList[0], _T("kp")))
	{
		KillProcess();
	}
	else
	{
		_tcscpy_s(cmdStringWithOptions, cmdTokenList[0]);

		for (int i = 1; i<tokenNum; i++)
			_stprintf_s(cmdStringWithOptions, _T("%s %s"), cmdStringWithOptions, cmdTokenList[i]);

		isRun = CreateProcess(NULL, cmdStringWithOptions, NULL, NULL, TRUE, 0, NULL, NULL, &si, &pi);

		CloseHandle(pi.hProcess);
		CloseHandle(pi.hThread);

		if (isRun == FALSE)
			_tprintf(ERROR_CMD, cmdTokenList[0]);
	}

	return 0;
}


TCHAR * StrLower(TCHAR *pStr)
{
	TCHAR *ret = pStr;

	while (*pStr)
	{
		if (_istupper(*pStr))
			*pStr = _totlower(*pStr);

		pStr++;
	}

	return ret;
}

void ShowFilesInDirectory(TCHAR *filePath)
{
	WIN32_FIND_DATA FindFileData;
	HANDLE hFind;

	std::wstring targetStr = filePath;
	targetStr += _T("\\*.*");

	hFind = FindFirstFile(targetStr.c_str(), &FindFileData);
	if (hFind == INVALID_HANDLE_VALUE)
	{
		printf_s("Error in finding files in current directory\n");
		return;
	}

	_tprintf(TEXT("\n<Displaying contents of [%s]>\n\n"), targetStr.c_str());
	do
	{
		_tprintf(TEXT("%s\n"),
			FindFileData.cFileName);

	} while (FindNextFile(hFind, &FindFileData));
	printf_s("\n");

	FindClose(hFind);

}
