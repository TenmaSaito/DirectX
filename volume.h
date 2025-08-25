//================================================================================================================
//
// DirectXの音量調整ヘッダーファイル [volume.h]
// Author : TENMA
//
//================================================================================================================
#ifndef _VOLUME_H_
#define _VOLUME_H_

#include "main.h"
#include "sound.h"

// プロトタイプ宣言
void InitVolume(void);
void UninitVolume(void);
void UpdateVolume(void);
void DrawVolume(void);

void SetVolume(void);
void DeleteVolume(void);
void AddVolume(int nValue, SETSOUND type);

#endif
