#ifndef _DIR_H_
#define _DIR_H_

void ShowFilesInDirectory(TCHAR *filePath);
void CreateNewDirectory(TCHAR *directoryName);
void RemoveExistingDirectory(TCHAR *directoryName);
void RenameExistingFile(TCHAR *originalName, TCHAR *newName);
void RemoveExistingFile(TCHAR *fileName);

#endif