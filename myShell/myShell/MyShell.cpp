

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
	// �ѱ� �Է��� ������ �ϱ� ����.
	_tsetlocale(LC_ALL, _T("Korean"));

	if (argc >= 2)  // �Ű����� �������ڰ� �ִ� ���.
	{
		for (int i = 1; i<argc; i++)
			_tcscpy_s(cmdTokenList[i - 1], argv[i]);

		CmdProcessing(argc - 1);
	}

	DWORD isExit = NULL;
	while (1)
	{
		int tokenNum = CmdReadTokenize();
		if (tokenNum == 0) //Enter �Է½� ó���� ����...
			continue;

		isExit = CmdProcessing(tokenNum);
		if (isExit == TRUE)
		{
			_fputts(_T("��ɾ� ó���� �����մϴ�. \n"), stdout);
			break;
		}
	}

	return 0;
}



