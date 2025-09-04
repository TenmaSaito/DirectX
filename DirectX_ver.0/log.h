//================================================================================================================
//
// DirectXのログ生成のヘッダファイル [log.h]
// Author : TENMA
//
//================================================================================================================
#ifndef _LOG_H_
#define _LOG_H_

#include "main.h"

void InitLog(void);
void UninitLog(void);
void UpdateLog(void);
void DrawLog(void);

void AddFunctionLog(const char *pBuffer);

#endif // !_LOG_H_

