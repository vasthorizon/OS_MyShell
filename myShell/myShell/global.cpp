#include <Windows.h>
#include <tchar.h>
#include "global.h"

TCHAR ERROR_CMD[]
= _T("'%s'��(��) ������ �� �ִ� ���α׷��� �ƴմϴ�. \n");

TCHAR cmdString[STR_LEN];
TCHAR cmdTokenList[CMD_TOKEN_NUM][STR_LEN];
TCHAR seps[] = _T(" ,\t\n");