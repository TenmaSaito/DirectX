//================================================================================================================
//
// DirectXのクレジット画面表示処理 [credit.cpp]
// Author : TENMA
//
//================================================================================================================
#include "credit.h"
#include "input.h"
#include "sound.h"
#include "fade.h"
#include "creditBg.h"
#include "creditLogo.h"
#include "creditPhoto.h"
#include "creditChar.h"

//================================================================================================================
// ゲームオーバー画面の初期化処理
//================================================================================================================
void InitCredit(void)
{
	InitCreditBg();

	InitCreditChar();

	InitCreditLogo();

	InitCreditPhoto();
}

//================================================================================================================
// ゲームオーバー画面の終了処理
//================================================================================================================
void UninitCredit(void)
{
	UninitCreditBg();

	UninitCreditLogo();

	UninitCreditPhoto();

	UninitCreditChar();
}

//================================================================================================================
// ゲームオーバー画面の更新処理
//================================================================================================================
void UpdateCredit(void)
{
	// Enterでタイトル画面へ進む
	if ((GetJoypadTrigger(JOYKEY_START) == true 
		|| GetJoypadTrigger(JOYKEY_A) == true
		|| GetKeyboardTrigger(DIK_RETURN) == true)
		&& GetFade() == FADE_NONE)
	{
		PlaySound(SOUND_LABEL_SE_ENTER);
		FadeSound(SOUND_LABEL_TITLE);
		SetFade(MODE_TITLE, FADE_TYPE_NORMAL);
	}

	UpdateCreditBg();

	UpdateCreditChar();

	UpdateCreditLogo();

	UpdateCreditPhoto();
}

//================================================================================================================
// ゲームオーバー画面の描画処理
//================================================================================================================
void DrawCredit(void)
{
	DrawCreditBg();

	DrawCreditPhoto();

	DrawCreditChar();

	DrawCreditLogo();
}