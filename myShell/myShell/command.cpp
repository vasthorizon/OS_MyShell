#include <tchar.h>
#include <stdio.h>
#include <Windows.h>

#include "global.h"
#include "command.h"
#include "dir.h"
#include "process.h"

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