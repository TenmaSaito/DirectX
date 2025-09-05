//================================================================================================================
//
// DirectXの各種ゲーム内設定処理 [settings.cpp]
// Author : TENMA
//
//================================================================================================================
#include "main.h"
#include "settings.h"
#include "input.h"
#include "sound.h"
#include "block.h"
#include "fade.h"
#include "game.h"
#include "volume.h"
#include "pause.h"

// グローバル変数
LPDIRECT3DTEXTURE9		g_apTextureSettings[SETTINGTYPE_MAX] = {};	// テクスチャへのポインタ
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffSettings = NULL;					// 頂点バッファのポインタ
FADE g_FadeSettings;												// フェードの種類
SETTINGTYPE g_Setting;												// 現在選択されている選択肢
D3DXCOLOR g_colorSettings;											// ポリゴンの色
bool g_bUseSettings;												// 設定画面が使われているかどうか							

//================================================================================================================
// 背景の初期化処理
//================================================================================================================
void InitSettings(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();			// デバイスのポインタ

	// 各変数の初期化
	g_FadeSettings = FADE_NONE;
	g_Setting = SETTINGTYPE_NONE;
	g_colorSettings.a = 0.0f;
	g_bUseSettings = false;

	// テクスチャの読み込み
	D3DXCreateTextureFromFile(pDevice,
		"data\\TEXTURE\\CHARACTER\\PauseToSettings.png",
		&g_apTextureSettings[SETTINGTYPE_NONE]);

	D3DXCreateTextureFromFile(pDevice,
		"data\\TEXTURE\\CHARACTER\\PauseToSettings_GameSound.png",
		&g_apTextureSettings[SETTINGTYPE_GAME_SOUND]);

	D3DXCreateTextureFromFile(pDevice,
		"data\\TEXTURE\\CHARACTER\\PauseToSettings_SE.png",
		&g_apTextureSettings[SETTINGTYPE_SE_SOUND]);

	D3DXCreateTextureFromFile(pDevice,
		"data\\TEXTURE\\CHARACTER\\PauseToSettings_exit.png",
		&g_apTextureSettings[SETTINGTYPE_EXIT]);

	// 頂点バッファの生成
	pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * 4,			// sizeofの後必ず * 頂点数 を書くこと！
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_2D,
		D3DPOOL_MANAGED,
		&g_pVtxBuffSettings,
		NULL);

	VERTEX_2D* pVtx;					// 頂点情報へのポインタ

	// 頂点バッファをロックし、頂点情報へのポインタを取得
	g_pVtxBuffSettings->Lock(0, 0, (void**)&pVtx, 0);

	// 頂点座標の設定(座標設定は必ず右回りで！！！)
	pVtx[0].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	pVtx[1].pos = D3DXVECTOR3(1280.0f, 0.0f, 0.0f);
	pVtx[2].pos = D3DXVECTOR3(0.0f, 720.0f, 0.0f);
	pVtx[3].pos = D3DXVECTOR3(1280.0f, 720.0f, 0.0f);

	// rhwの設定
	pVtx[0].rhw = 1.0f;
	pVtx[1].rhw = 1.0f;
	pVtx[2].rhw = 1.0f;
	pVtx[3].rhw = 1.0f;

	// 頂点カラーの設定
	pVtx[0].col = D3DXCOLOR(0.0f, 0.0f, 0.0f, g_colorSettings.a);
	pVtx[1].col = D3DXCOLOR(0.0f, 0.0f, 0.0f, g_colorSettings.a);
	pVtx[2].col = D3DXCOLOR(0.0f, 0.0f, 0.0f, g_colorSettings.a);
	pVtx[3].col = D3DXCOLOR(0.0f, 0.0f, 0.0f, g_colorSettings.a);

	// テクスチャ座標の設定
	pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
	pVtx[1].tex = D3DXVECTOR2(1.0f, 0.0f);
	pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
	pVtx[3].tex = D3DXVECTOR2(1.0f, 1.0f);

	// 頂点バッファをアンロックする
	g_pVtxBuffSettings->Unlock();
}

//================================================================================================================
// 背景の終了処理
//================================================================================================================
void UninitSettings(void)
{
	for (int nCntSettings = 0; nCntSettings < SETTINGTYPE_MAX; nCntSettings++)
	{
		if (g_apTextureSettings[nCntSettings] != NULL)
		{
			g_apTextureSettings[nCntSettings]->Release();
			g_apTextureSettings[nCntSettings] = NULL;
		}
	}

	// 頂点バッファの破棄(必ず行うこと！！！)
	if (g_pVtxBuffSettings != NULL)
	{
		g_pVtxBuffSettings->Release();
		g_pVtxBuffSettings = NULL;
	}
}

//================================================================================================================
// 背景の更新処理
//================================================================================================================
void UpdateSettings(void)
{
	bool pSettings = GetGamePause();
	XINPUT_STATE *pState = GetJoypadState();

	if (g_FadeSettings != FADE_NONE)
	{
		if (g_FadeSettings == FADE_IN)
		{// フェードイン
			g_colorSettings.a += 0.175f;
			if (g_colorSettings.a >= 1.0f)
			{// フェードイン終了
				g_colorSettings.a = 1.0f;
				g_FadeSettings = FADE_NONE;
				SetVolume();
			}
		}
		else if (g_FadeSettings == FADE_OUT)
		{// フェードアウト
			g_colorSettings.a -= 0.175f;
			if (g_colorSettings.a <= 0.0f)
			{// フェードアウト終了
				g_colorSettings.a = 0.0f;
				g_FadeSettings = FADE_NONE;
				g_bUseSettings = false;
				DeleteVolume();
				SELECTTYPE* pSelect = GetSelect();
				*pSelect = SELECTTYPE_SETTINGS;
			}
		}

		VERTEX_2D* pVtx;

		// 頂点バッファをロックし、頂点情報へのポインタを取得
		g_pVtxBuffSettings->Lock(0, 0, (void**)&pVtx, 0);

		// 頂点カラーの設定
		pVtx[0].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, g_colorSettings.a);
		pVtx[1].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, g_colorSettings.a);
		pVtx[2].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, g_colorSettings.a);
		pVtx[3].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, g_colorSettings.a);

		// 頂点バッファをアンロックする
		g_pVtxBuffSettings->Unlock();
	}
	else if(g_bUseSettings == true)
	{
		if (GetKeyboardRepeat(DIK_W) == true
			|| GetKeyboardRepeat(DIK_UP) == true
			|| GetJoypadRepeat(JOYKEY_UP) == true
			|| (GetJoyThumbSlow(JOYTHUMB_LY_UP) == true && pState->Gamepad.sThumbLY >= 25000.0f))
		{
			switch (g_Setting)
			{
			case SETTINGTYPE_GAME_SOUND:

				g_Setting = SETTINGTYPE_EXIT;

				break;

			case SETTINGTYPE_SE_SOUND:

				g_Setting = SETTINGTYPE_GAME_SOUND;

				break;

			case SETTINGTYPE_EXIT:

				g_Setting = SETTINGTYPE_SE_SOUND;

				break;
			}
		}
		else
		{
			if (GetKeyboardRepeat(DIK_S) == true
				|| GetKeyboardRepeat(DIK_DOWN) == true
				|| GetJoypadRepeat(JOYKEY_DOWN) == true
				|| (GetJoyThumbSlow(JOYTHUMB_LY_DOWN) == true && pState->Gamepad.sThumbLY <= -25000.0f))
			{
				switch (g_Setting)
				{
				case SETTINGTYPE_GAME_SOUND:

					g_Setting = SETTINGTYPE_SE_SOUND;

					break;

				case SETTINGTYPE_SE_SOUND:

					g_Setting = SETTINGTYPE_EXIT;

					break;

				case SETTINGTYPE_EXIT:

					g_Setting = SETTINGTYPE_GAME_SOUND;

					break;
				}

			}
		}

		if ((GetJoypadTrigger(JOYKEY_A) == true && g_Setting == SETTINGTYPE_EXIT)
			|| GetJoypadTrigger(JOYKEY_B) == true
			|| GetKeyboardTrigger(DIK_RETURN) == true)
		{// 設定画面を閉じる
			g_FadeSettings = FADE_OUT;
		}

		if (GetKeyboardRepeat(DIK_D) == true
			|| GetJoypadRepeat(JOYKEY_RIGHT) == true
			|| pState->Gamepad.sThumbLX >= 10000)
		{
			switch (g_Setting)
			{
			case SETTINGTYPE_GAME_SOUND:

				AddVolume(1, SETSOUND_GAME);

				break;

			case SETTINGTYPE_SE_SOUND:

				AddVolume(1, SETSOUND_MENU_SE);

				break;
			}
		}
		else if (GetKeyboardRepeat(DIK_A) == true
			|| GetJoypadRepeat(JOYKEY_LEFT) == true
			|| pState->Gamepad.sThumbLX <= -10000)
		{
			switch (g_Setting)
			{
			case SETTINGTYPE_GAME_SOUND:

				AddVolume(-1, SETSOUND_GAME);

				break;

			case SETTINGTYPE_SE_SOUND:

				AddVolume(-1, SETSOUND_MENU_SE);

				break;
			}
		}
	}
}

//================================================================================================================
// 背景の描画処理
//================================================================================================================
void DrawSettings(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();			// デバイスのポインタ

	//頂点バッファをデータストリームに設定
	pDevice->SetStreamSource(0, g_pVtxBuffSettings, 0, sizeof(VERTEX_2D));

	// 頂点フォーマットの設定
	pDevice->SetFVF(FVF_VERTEX_2D);
	
	// テクスチャの設定(使わないならNULLを入れる！！！！)
	pDevice->SetTexture(0, g_apTextureSettings[g_Setting]);

	if (g_bUseSettings == true)
	{
		// ポリゴンの描画
		pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP,		// プリミティブの種類
			0,											// 描画する最初の頂点インデックス
			2);											// 描画するプリミティブの数
	}
}

// フェードの設定
void SetSettings(void)
{
	g_bUseSettings = true;

	g_Setting = SETTINGTYPE_GAME_SOUND;
	g_FadeSettings = FADE_IN;
}

// フェードの取得
FADE GetSettings(void)
{
	return g_FadeSettings;
}