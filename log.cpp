//================================================================================================================
//
// DirectXのログ生成のcppファイル [bg.h]
// Author : TENMA
//
//================================================================================================================
#include "log.h"
#include <time.h>

// プロトタイプ宣言

void InitLog(void)
{
	FILE* pFile = NULL;
	time_t timer;
	struct tm* local_time;
	char time_str[256];
	size_t ret;
	timer = time(NULL);
	local_time = localtime(&timer);
	ret = strftime(time_str, 256, "%Y/%m/%d %H:%M:%S", local_time);

	pFile = fopen("data\\LOG\\LOG.txt", "w");
	if (pFile == NULL)
	{
		return;
	}

	fseek(pFile, -2, SEEK_CUR);
	fprintf(pFile, "%s | <LOG START>\n", &time_str[0]);

	fclose(pFile);
}

void UninitLog(void)
{
	FILE* pFile = NULL;
	time_t timer;
	struct tm* local_time;
	char time_str[256];
	size_t ret;
	timer = time(NULL);
	local_time = localtime(&timer);
	ret = strftime(time_str, 256, "%Y/%m/%d %H:%M:%S", local_time);

	pFile = fopen("data\\LOG\\LOG.txt", "a");
	if (pFile == NULL)
	{
		return;
	}

	fseek(pFile, -2, SEEK_CUR);
	fprintf(pFile, "%s | <LOG END>", &time_str[0]);

	fclose(pFile);
}

void UpdateLog(void)
{

}

void DrawLog(void)
{

}

void AddFunctionLog(const char* pBuffer)
{
	FILE* pFile = NULL;
	time_t timer;
	struct tm* local_time;
	char time_str[256];
	size_t ret;
	timer = time(NULL);
	local_time = localtime(&timer);
	ret = strftime(time_str, 256, "%Y/%m/%d %H:%M:%S", local_time);

	pFile = fopen("data\\LOG\\LOG.txt", "a");
	if (pFile == NULL)
	{
		return;
	}

	fseek(pFile, -2, SEEK_CUR);
	fprintf(pFile, "%s | %s\n", &time_str[0],pBuffer);

	fclose(pFile);
}
