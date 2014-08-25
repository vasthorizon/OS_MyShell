

#include <stdio.h>
#include <tchar.h>
#include <locale.h>
#include <windows.h> 
#include <tlhelp32.h>
#include <string>

#include "global.h"
#include "process.h"
#include "dir.h"
#include "command.h"



int _tmain(int argc, TCHAR * argv[])
{
	// 한글 입력을 가능케 하기 위해.
	_tsetlocale(LC_ALL, _T("Korean"));

	if (argc >= 2)  // 매개변수 전달인자가 있는 경우.
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



