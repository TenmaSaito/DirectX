//================================================================================================================
//
// DirectXのポーズ処理 [pause.cpp]
// Author : TENMA
//
//================================================================================================================
#include "main.h"
#include "pause.h"
#include "input.h"
#include "sound.h"
#include "block.h"
#include "fade.h"
#include "game.h"
#include "settings.h"

// グローバル変数
LPDIRECT3DTEXTURE9		g_apTexturePause[SELECTTYPE_MAX] = {};	// テクスチャへのポインタ
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffPause = NULL;					// 頂点バッファのポインタ
PAUSE g_Pause;													// フェードの種類
SELECTTYPE g_Select;											// 現在選択されている選択肢
D3DXCOLOR g_colorPause;											// ポリゴンの色
int g_aVibration[2];											// 現在の左右のバイブレーションの値
XINPUT_VIBRATION* g_pVibration;									// 現在のバイブレーションの値
bool g_bUse;													// ポーズ画面が使われているかどうか							

//================================================================================================================
// 背景の初期化処理
//================================================================================================================
void InitPause(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();			// デバイスのポインタ

	// 各変数の初期化
	g_Pause = PAUSE_NONE;
	g_Select = SELECTTYPE_NONE;
	g_colorPause.a = 0.0f;
	g_bUse = false;
	g_aVibration[0] = 0;
	g_aVibration[1] = 0;

	// テクスチャの読み込み
	D3DXCreateTextureFromFile(pDevice,
		"data\\TEXTURE\\CHARACTER\\pause_continue.png",
		&g_apTexturePause[SELECTTYPE_CONTINUE]);

	D3DXCreateTextureFromFile(pDevice,
		"data\\TEXTURE\\CHARACTER\\pause_settings.png",
		&g_apTexturePause[SELECTTYPE_SETTINGS]);

	D3DXCreateTextureFromFile(pDevice,
		"data\\TEXTURE\\CHARACTER\\pause_restart.png",
		&g_apTexturePause[SELECTTYPE_RESTART]);

	D3DXCreateTextureFromFile(pDevice,
		"data\\TEXTURE\\CHARACTER\\pause_exit.png",
		&g_apTexturePause[SELECTTYPE_EXIT]);

	D3DXCreateTextureFromFile(pDevice,
		"data\\TEXTURE\\CHARACTER\\pause.png",
		&g_apTexturePause[SELECTTYPE_NONE]);

	// 頂点バッファの生成
	pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * 4,			// sizeofの後必ず * 頂点数 を書くこと！
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_2D,
		D3DPOOL_MANAGED,
		&g_pVtxBuffPause,
		NULL);

	VERTEX_2D* pVtx;					// 頂点情報へのポインタ

	// 頂点バッファをロックし、頂点情報へのポインタを取得
	g_pVtxBuffPause->Lock(0, 0, (void**)&pVtx, 0);

	// 頂点座標の設定(座標設定は必ず右回りで！！！)
	pVtx[0].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	pVtx[1].pos = D3DXVECTOR3(SCREEN_WIDTH, 0.0f, 0.0f);
	pVtx[2].pos = D3DXVECTOR3(0.0f, SCREEN_HEIGHT, 0.0f);
	pVtx[3].pos = D3DXVECTOR3(SCREEN_WIDTH, SCREEN_HEIGHT, 0.0f);

	// rhwの設定
	pVtx[0].rhw = 1.0f;
	pVtx[1].rhw = 1.0f;
	pVtx[2].rhw = 1.0f;
	pVtx[3].rhw = 1.0f;

	// 頂点カラーの設定
	pVtx[0].col = D3DXCOLOR(0.0f, 0.0f, 0.0f, g_colorPause.a);
	pVtx[1].col = D3DXCOLOR(0.0f, 0.0f, 0.0f, g_colorPause.a);
	pVtx[2].col = D3DXCOLOR(0.0f, 0.0f, 0.0f, g_colorPause.a);
	pVtx[3].col = D3DXCOLOR(0.0f, 0.0f, 0.0f, g_colorPause.a);

	// テクスチャ座標の設定
	pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
	pVtx[1].tex = D3DXVECTOR2(1.0f, 0.0f);
	pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
	pVtx[3].tex = D3DXVECTOR2(1.0f, 1.0f);

	// 頂点バッファをアンロックする
	g_pVtxBuffPause->Unlock();

}

//================================================================================================================
// 背景の終了処理
//================================================================================================================
void UninitPause(void)
{
	for (int nCntPause = 0; nCntPause < SELECTTYPE_MAX; nCntPause++)
	{
		if (g_apTexturePause[nCntPause] != NULL)
		{
			g_apTexturePause[nCntPause]->Release();
			g_apTexturePause[nCntPause] = NULL;
		}
	}

	// 頂点バッファの破棄(必ず行うこと！！！)
	if (g_pVtxBuffPause != NULL)
	{
		g_pVtxBuffPause->Release();
		g_pVtxBuffPause = NULL;
	}
}

//================================================================================================================
// 背景の更新処理
//================================================================================================================
void UpdatePause(void)
{
	bool pPause = GetGamePause();
	XINPUT_STATE *pState = GetJoypadState();

	if (g_Pause != PAUSE_NONE)
	{
		if (g_Pause == PAUSE_IN)
		{// フェードイン
			g_colorPause.a += 0.175f;
			if (g_colorPause.a >= 1.0f)
			{// フェードイン終了
				g_colorPause.a = 1.0f;
				g_Pause = PAUSE_NONE;
			}
		}
		else if (g_Pause == PAUSE_OUT)
		{// フェードアウト
			g_colorPause.a -= 0.175f;
			if (g_colorPause.a <= 0.0f)
			{// フェードアウト終了
				g_colorPause.a = 0.0f;
				g_Pause = PAUSE_NONE;

				g_pVibration->wLeftMotorSpeed = g_aVibration[0];
				g_pVibration->wRightMotorSpeed = g_aVibration[1];

				XInputSetState(0, g_pVibration);
			}
		}

		VERTEX_2D* pVtx;

		// 頂点バッファをロックし、頂点情報へのポインタを取得
		g_pVtxBuffPause->Lock(0, 0, (void**)&pVtx, 0);

		// 頂点カラーの設定
		pVtx[0].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, g_colorPause.a);
		pVtx[1].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, g_colorPause.a);
		pVtx[2].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, g_colorPause.a);
		pVtx[3].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, g_colorPause.a);

		// 頂点バッファをアンロックする
		g_pVtxBuffPause->Unlock();
	}
	else if(pPause == true)
	{
		if (GetKeyboardRepeat(DIK_W) == true
			|| GetKeyboardRepeat(DIK_UP) == true
			|| GetJoypadRepeat(JOYKEY_UP) == true
			|| (GetJoyThumbSlow(JOYTHUMB_LY_UP) == true && pState->Gamepad.sThumbLY >= 1000.0f))
		{
			switch (g_Select)
			{
			case SELECTTYPE_CONTINUE:

				g_Select = SELECTTYPE_EXIT;

				break;
#if 0
			case SELECTTYPE_SETTINGS:

				g_Select = SELECTTYPE_CONTINUE;

				break;
#endif
			case SELECTTYPE_RESTART:

				g_Select = SELECTTYPE_CONTINUE;

				break;

			case SELECTTYPE_EXIT:

				g_Select = SELECTTYPE_RESTART;

				break;

			case SELECTTYPE_NONE:

				break;
			}

			PlaySound(SOUND_LABEL_SE_SELECT);
		}
		else
		{
			if (GetKeyboardRepeat(DIK_S) == true
				|| GetKeyboardRepeat(DIK_DOWN) == true
				|| GetJoypadRepeat(JOYKEY_DOWN) == true
				|| (GetJoyThumbSlow(JOYTHUMB_LY_DOWN) == true && pState->Gamepad.sThumbLY <= -15000.0f))
			{
				switch (g_Select)
				{
				case SELECTTYPE_CONTINUE:

					g_Select = SELECTTYPE_RESTART;

					break;
#if 0
				case SELECTTYPE_SETTINGS:

					g_Select = SELECTTYPE_RESTART;

					break;
#endif
				case SELECTTYPE_RESTART:

					g_Select = SELECTTYPE_EXIT;

					break;

				case SELECTTYPE_EXIT:

					g_Select = SELECTTYPE_CONTINUE;

					break;

				case SELECTTYPE_NONE:

					break;
				}

				PlaySound(SOUND_LABEL_SE_SELECT);
			}
		}

		if (GetKeyboardTrigger(DIK_RETURN) == true
			|| GetJoypadTrigger(JOYKEY_A) == true)
		{
			switch (g_Select)
			{
			case SELECTTYPE_CONTINUE:

				g_Pause = PAUSE_OUT;

				SetGamePause(false);

				break;
#if 0
			case SELECTTYPE_SETTINGS:

				SetSettings();
				g_Select = SELECTTYPE_NONE;

				break;
#endif
			case SELECTTYPE_RESTART:

				FadeSound(SOUND_LABEL_GAME_NORMAL);
				SetFade(MODE_GAME, FADE_TYPE_NORMAL);
				g_Select = SELECTTYPE_NONE;

				break;

			case SELECTTYPE_EXIT:

				FadeSound(SOUND_LABEL_TITLE);
				SetFade(MODE_TITLE, FADE_TYPE_NORMAL);
				g_Select = SELECTTYPE_NONE;

				break;

			case SELECTTYPE_NONE:

				break;
			}

			PlaySound(SOUND_LABEL_SE_ENTER);
		}

		if (GetJoypadTrigger(JOYKEY_START) == true)
		{
			g_Pause = PAUSE_OUT;

			SetGamePause(false);
		}
	}
}

//================================================================================================================
// 背景の描画処理
//================================================================================================================
void DrawPause(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();			// デバイスのポインタ

	//頂点バッファをデータストリームに設定
	pDevice->SetStreamSource(0, g_pVtxBuffPause, 0, sizeof(VERTEX_2D));

	// 頂点フォーマットの設定
	pDevice->SetFVF(FVF_VERTEX_2D);
	
	// テクスチャの設定(使わないならNULLを入れる！！！！)
	pDevice->SetTexture(0, g_apTexturePause[g_Select]);

	// ポリゴンの描画
	pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP,		// プリミティブの種類
		0,											// 描画する最初の頂点インデックス
		2);											// 描画するプリミティブの数
	
}

// フェードの設定
void SetPause(bool bUse)
{
	g_bUse = bUse;
	if (g_bUse == true)
	{
		g_Select = SELECTTYPE_CONTINUE;
		g_Pause = PAUSE_IN;
		g_bUse = true;
		g_pVibration = GetJoyVibration();
		

		g_aVibration[0] = g_pVibration->wLeftMotorSpeed;
		g_aVibration[1] = g_pVibration->wRightMotorSpeed;

		g_pVibration->wLeftMotorSpeed = 0;
		g_pVibration->wRightMotorSpeed = 0;

		XInputSetState(0, g_pVibration);
	}
	else
	{
		g_Pause = PAUSE_OUT;
		
		g_bUse = false;
	}
}

// フェードの取得
PAUSE GetPause(void)
{
	return g_Pause;
}

SELECTTYPE *GetSelect(void)
{
	return &g_Select;
}