#ifndef _GLOBAL_
#define _GLOBAL_


#define STR_LEN    256
#define CMD_TOKEN_NUM  10

extern TCHAR ERROR_CMD[];
extern TCHAR cmdString[STR_LEN];
extern TCHAR cmdTokenList[CMD_TOKEN_NUM][STR_LEN];
extern TCHAR seps[];

#endif