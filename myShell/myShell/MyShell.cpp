#define _CRT_SECURE_NO_WARNINGS

#include <stdio.h>
#include <stdlib.h>
#include <tchar.h>
#include <locale.h>
#include <windows.h> 
#include <string>
#include <vector>
#include <iostream>

#define STR_LEN			256
#define CMD_TOKEN_NUM	10

TCHAR ERROR_CMD[] 
 = _T("'%s'은(는) 실행할 수 있는 프로그램이 아닙니다. \n");

int CmdProcessing(void);
TCHAR * StrLower(TCHAR *);
void ShowFilesInDirectory(TCHAR *filePath);

int main(int agrc, TCHAR * argv[])
{
	
	// 한글 입력을 가능케 하기 위해.
	_tsetlocale(LC_ALL, _T("Korean")); 
	
	DWORD isExit;
	while(1)
	{
		isExit = CmdProcessing();
		if(isExit == TRUE)
		{
			_fputts(_T("명령어 처리를 종료합니다. \n"), stdout );
			break;
		}
	}

	return 0; 
}


TCHAR cmdString[STR_LEN];
TCHAR cmdTokenList[CMD_TOKEN_NUM][STR_LEN];
TCHAR seps[]   = _T(" ,\t\n");



int CmdProcessing(void)
{
	_fputts( _T("Best command prompt(type 'h' for help(>> "), stdout );
	_getts(cmdString);

	TCHAR * token = _tcstok(cmdString, seps);

	int tokenNum = 0;
	while(token != NULL)
	{
		_tcscpy(
			cmdTokenList[tokenNum++], StrLower(token)
		);
		token = _tcstok(NULL, seps);
	}
	
	if( !_tcscmp(cmdTokenList[0],_T("exit")) )
	{
		return TRUE;
	}
	else if (!_tcscmp(cmdTokenList[0], _T("pwd")))
	{
		_fputts(_T("Input password to show current directory:>"), stdout);
		TCHAR inputStr[STR_LEN];
		_getts(inputStr);

		if (!_tcscmp(inputStr, _T("mypwd")))
		{
			ShowFilesInDirectory(_T("."));
		}
		else
		{
			printf_s("\nWrong Password! Correct password is 'mypwd'\n");
		}

	}
	else if ( !_tcscmp(cmdTokenList[0],_T("dir")) )
	{
		int paramLen = _tcslen(cmdTokenList[1]);

		if (paramLen == 0)
			ShowFilesInDirectory(_T("."));
		else
			ShowFilesInDirectory(cmdTokenList[1]);
	}
	else if ( !_tcscmp(cmdTokenList[0],_T("help"))
		|| !_tcscmp(cmdTokenList[0], _T("?"))
		|| !_tcscmp(cmdTokenList[0], _T("h"))
		)
	{
		printf_s("\n<commands> \ndir \ndir .. \ndir C:\\ \nhelp \npwd \nexit \n\n");
	}
	else
	{	
		STARTUPINFO si={0,};
		PROCESS_INFORMATION pi;	
		si.cb=sizeof(si);
		BOOL isRun = CreateProcess(NULL, cmdTokenList[0], NULL, NULL, TRUE, 0, NULL, NULL, &si, &pi);

		if(isRun == FALSE)
			_tprintf(ERROR_CMD, cmdTokenList[0]);
	}

	return 0;
}


TCHAR * StrLower(TCHAR *pStr)
{
	TCHAR *ret = pStr;

	while(*pStr)
	{
		if(_istupper(*pStr))
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
