#include <tchar.h>
#include <Windows.h>
#include <locale>
#include <stdio.h>

#include "dir.h"

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