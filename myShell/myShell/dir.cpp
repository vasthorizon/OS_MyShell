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

void CreateNewDirectory(TCHAR *directoryName)
{
	BOOL result = CreateDirectory(directoryName, NULL);
	if (result == TRUE)
		_tprintf_s(_T("Succeeded in creating directory %s \n"), directoryName);
	else
		_tprintf_s(_T("Failed in creating directory %s \n"), directoryName);
}

void RemoveExistingDirectory(TCHAR *directoryName)
{
	BOOL result = RemoveDirectory(directoryName);
	if (result == TRUE)
		_tprintf_s(_T("Succeeded in removing directory %s \n"), directoryName);
	else
		_tprintf_s(_T("Failed in removing directory %s \n"), directoryName);
}

void RenameExistingFile(TCHAR *originalName, TCHAR *newName)
{
	BOOL result = MoveFile(originalName, newName);
	if (result == TRUE)
		_tprintf_s(_T("Succeeded in renaming file %s \n"), originalName);
	else
		_tprintf_s(_T("Failed in renaming file %s \n"), originalName);
}

void RemoveExistingFile(TCHAR *fileName)
{
	BOOL result = DeleteFile(fileName);
	if (result == TRUE)
		_tprintf_s(_T("Succeeded in deleting file %s \n"), fileName);
	else
		_tprintf_s(_T("Failed in deleting file %s \n"), fileName);
}