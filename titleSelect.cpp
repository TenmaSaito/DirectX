//================================================================================================================
//
// DirectXのタイトルセレクト処理 [titleSelect.cpp]
// Author : TENMA
//
//================================================================================================================
#include "main.h"
#include "titleSelect.h"
#include "input.h"
#include "sound.h"
#include "block.h"
#include "fade.h"
#include "game.h"
#include "settings.h"

// マクロ定義
#define SELECTSIZE_WIDTH		(500)		// 横のサイズ
#define SELECTSIZE_HEIGHT		(60)		// 縦のサイズ
#define SELECTWAIT_STATE		(100)		// 選択後の待機時間

// タイトルセレクト構造体
typedef struct
{
	D3DXVECTOR3 pos;			// 選択肢の位置
	D3DXCOLOR col;				// 選択肢の色
	TITLESELECTTYPE type;		// 選択肢の種類
	bool bSelect;				// 選択されているか
}TITLESELECT;

// グローバル変数
LPDIRECT3DTEXTURE9		g_apTextureTitleSelect[TITLESELECTTYPE_MAX] = {};	// テクスチャへのポインタ
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffTitleSelect = NULL;						// 頂点バッファのポインタ
TITLESELECT g_aTitleSelect[TITLESELECTTYPE_MAX];							// タイトルセレクト構造体の宣言
int g_nTitleSelect;
int g_nCounterSelectState;
bool g_bUseSelect;															// ポーズ画面が使われているかどうか							

const char* g_TITLESELECTTEX[TITLESELECTTYPE_MAX]
{
	"data\\TEXTURE\\CHARACTER\\CHARTEX\\GAME_START.png",
	"data\\TEXTURE\\CHARACTER\\CHARTEX\\TUTORIAL.png",
	"data\\TEXTURE\\CHARACTER\\CHARTEX\\SETTINGS.png",
	"data\\TEXTURE\\CHARACTER\\CHARTEX\\CREDIT.png",
	"data\\TEXTURE\\CHARACTER\\CHARTEX\\GAME_END.png",
	"data\\TEXTURE\\CHARACTER\\CHARTEX\\GAME_START.png"
};

//================================================================================================================
// 背景の初期化処理
//================================================================================================================
void InitTitleSelect(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();			// デバイスのポインタ
	TITLESELECT *pTitleSelect = &g_aTitleSelect[0];
	int nCntTitleSelect;

	// 各変数の初期化
	for (nCntTitleSelect = 0; nCntTitleSelect < TITLESELECTTYPE_MAX; nCntTitleSelect++, pTitleSelect++)
	{
		pTitleSelect->pos = D3DXVECTOR3(200.0f, 400.0f * (nCntTitleSelect + 1), 0.0f);
		pTitleSelect->col = D3DXCOLOR_NULL;
		pTitleSelect->type = (TITLESELECTTYPE)nCntTitleSelect;
		pTitleSelect->bSelect = false;

		D3DXCreateTextureFromFile(pDevice,
			g_TITLESELECTTEX[nCntTitleSelect],
			&g_apTextureTitleSelect[nCntTitleSelect]);
	}

	// 頂点バッファの生成
	pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * 4 * TITLESELECTTYPE_MAX,			// sizeofの後必ず * 頂点数 を書くこと！
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_2D,
		D3DPOOL_MANAGED,
		&g_pVtxBuffTitleSelect,
		NULL);

	g_nTitleSelect = TITLESELECTTYPE_START;
	g_nCounterSelectState = SELECTWAIT_STATE;
	g_aTitleSelect[g_nTitleSelect].bSelect = true;

	VERTEX_2D* pVtx = NULL;					// 頂点情報へのポインタ

	// 頂点バッファをロックし、頂点情報へのポインタを取得
	g_pVtxBuffTitleSelect->Lock(0, 0, (void**)&pVtx, 0);

	pTitleSelect = &g_aTitleSelect[0];

	for (nCntTitleSelect = 0; nCntTitleSelect < TITLESELECTTYPE_MAX; nCntTitleSelect++)
	{
		// 頂点座標の設定(座標設定は必ず右回りで！！！)
		pVtx[0].pos.x = pTitleSelect->pos.x - (SELECTSIZE_WIDTH * 0.5f);
		pVtx[0].pos.y = pTitleSelect->pos.y - (SELECTSIZE_HEIGHT * 0.5f) + ((SELECTSIZE_HEIGHT + 10.0f) * nCntTitleSelect);
		pVtx[0].pos.z = 0.0f;

		pVtx[1].pos.x = pTitleSelect->pos.x + (SELECTSIZE_WIDTH * 0.5f);
		pVtx[1].pos.y = pTitleSelect->pos.y - (SELECTSIZE_HEIGHT * 0.5f) + ((SELECTSIZE_HEIGHT + 10.0f) * nCntTitleSelect);
		pVtx[1].pos.z = 0.0f;

		pVtx[2].pos.x = pTitleSelect->pos.x - (SELECTSIZE_WIDTH * 0.5f);
		pVtx[2].pos.y = pTitleSelect->pos.y + (SELECTSIZE_HEIGHT * 0.5f) + ((SELECTSIZE_HEIGHT + 10.0f) * nCntTitleSelect);
		pVtx[2].pos.z = 0.0f;

		pVtx[3].pos.x = pTitleSelect->pos.x + (SELECTSIZE_WIDTH * 0.5f);
		pVtx[3].pos.y = pTitleSelect->pos.y + (SELECTSIZE_HEIGHT * 0.5f) + ((SELECTSIZE_HEIGHT + 10.0f) * nCntTitleSelect);
		pVtx[3].pos.z = 0.0f;

		// rhwの設定
		pVtx[0].rhw = 1.0f;
		pVtx[1].rhw = 1.0f;
		pVtx[2].rhw = 1.0f;
		pVtx[3].rhw = 1.0f;

		// 頂点カラーの設定
		pVtx[0].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
		pVtx[1].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
		pVtx[2].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
		pVtx[3].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);

		// テクスチャ座標の設定
		pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
		pVtx[1].tex = D3DXVECTOR2(1.0f, 0.0f);
		pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
		pVtx[3].tex = D3DXVECTOR2(1.0f, 1.0f);

		pVtx += 4;
	}

	// 頂点バッファをアンロックする
	g_pVtxBuffTitleSelect->Unlock();

}

//================================================================================================================
// 背景の終了処理
//================================================================================================================
void UninitTitleSelect(void)
{
	for (int nCntTitleSelect = 0; nCntTitleSelect < TITLESELECTTYPE_MAX; nCntTitleSelect++)
	{
		if (g_apTextureTitleSelect[nCntTitleSelect] != NULL)
		{
			g_apTextureTitleSelect[nCntTitleSelect]->Release();
			g_apTextureTitleSelect[nCntTitleSelect] = NULL;
		}
	}

	// 頂点バッファの破棄(必ず行うこと！！！)
	if (g_pVtxBuffTitleSelect != NULL)
	{
		g_pVtxBuffTitleSelect->Release();
		g_pVtxBuffTitleSelect = NULL;
	}
}

//================================================================================================================
// 背景の更新処理
//================================================================================================================
void UpdateTitleSelect(void)
{
	TITLESELECT *pTitleSelect = &g_aTitleSelect[0];
	VERTEX_2D *pVtx = NULL;
	HWND hWnd;

	for (int nCntSelect = 0; nCntSelect < TITLESELECTTYPE_MAX; nCntSelect++, pTitleSelect++)
	{
		if (pTitleSelect->bSelect == true)
		{
			pTitleSelect->col.a = 1.0f;
		}
		else
		{
			pTitleSelect->col.a = 0.3f;
		}
	}

	if (g_nTitleSelect != TITLESELECTTYPE_NONE && g_nCounterSelectState == SELECTWAIT_STATE && GetFade() == FADE_NONE)
	{
		if (GetKeyboardRepeat(DIK_S) == true
			|| GetJoypadRepeat(JOYKEY_DOWN) == true)
		{
			g_aTitleSelect[g_nTitleSelect].bSelect = false;
			g_nTitleSelect++;
			if (g_nTitleSelect >= TITLESELECTTYPE_NONE)
			{
				g_nTitleSelect = TITLESELECTTYPE_START;
			}
			g_aTitleSelect[g_nTitleSelect].bSelect = true;

			PlaySound(SOUND_LABEL_SE_SELECT);
		}
		else if (GetKeyboardRepeat(DIK_W) == true
			|| GetJoypadRepeat(JOYKEY_UP) == true)
		{
			g_aTitleSelect[g_nTitleSelect].bSelect = false;
			g_nTitleSelect--;
			if (g_nTitleSelect < TITLESELECTTYPE_START)
			{
				g_nTitleSelect = TITLESELECTTYPE_EXIT;
			}
			g_aTitleSelect[g_nTitleSelect].bSelect = true;

			PlaySound(SOUND_LABEL_SE_SELECT);
		}

		if (GetKeyboardTrigger(DIK_RETURN) == true
			|| GetJoypadTrigger(JOYKEY_A) == true
			|| GetJoypadTrigger(JOYKEY_START) == true)
		{
			g_nCounterSelectState--;
			PlaySound(SOUND_LABEL_SE_ENTER);
		}
	}
	
	if(g_nCounterSelectState < SELECTWAIT_STATE)
	{
		switch (g_nTitleSelect)
		{
		case TITLESELECTTYPE_START:
			if (g_nCounterSelectState <= 0)
			{
				if (GetFade() == FADE_NONE)
				{
					SetGameTutorial(false);
					SetFade(MODE_GAME);
				}
			}
			else
			{
				if (g_nCounterSelectState % 20 <= 9)
				{
					g_aTitleSelect[TITLESELECTTYPE_START].col.a = 0.0f;
				}
				else
				{
					g_aTitleSelect[TITLESELECTTYPE_START].col.a = 1.0f;
				}
			}

			g_nCounterSelectState--;

			break;

		case TITLESELECTTYPE_TUTORIAL:

			if (g_nCounterSelectState <= 0)
			{
				if (GetFade() == FADE_NONE)
				{
					SetGameTutorial(true);
					PlaySound(SOUND_LABEL_SE_ENTER);
					SetFade(MODE_GAME);
				}
			}
			else
			{
				if (g_nCounterSelectState % 20 <= 9)
				{
					g_aTitleSelect[TITLESELECTTYPE_TUTORIAL].col.a = 0.0f;
				}
				else
				{
					g_aTitleSelect[TITLESELECTTYPE_TUTORIAL].col.a = 1.0f;
				}
			}

			g_nCounterSelectState--;

			break;

		case TITLESELECTTYPE_SETTING:

			g_nCounterSelectState = SELECTWAIT_STATE;

			break;

		case TITLESELECTTYPE_CREDIT:

			g_nCounterSelectState = SELECTWAIT_STATE;

			break;

		case TITLESELECTTYPE_EXIT:
			if (SUCCEEDED(GetHandleWindow(&hWnd)))
			{
				DestroyWindow(hWnd);
			}

			break;
		}
	}

	// 頂点バッファをロックし、頂点情報へのポインタを取得
	g_pVtxBuffTitleSelect->Lock(0, 0, (void**)&pVtx, 0);

	pTitleSelect = &g_aTitleSelect[0];

	for (int nCntVtx = 0; nCntVtx < TITLESELECTTYPE_MAX; nCntVtx++,pTitleSelect++)
	{
		// 頂点カラーの設定
		pVtx[0].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, pTitleSelect->col.a);
		pVtx[1].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, pTitleSelect->col.a);
		pVtx[2].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, pTitleSelect->col.a);
		pVtx[3].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, pTitleSelect->col.a);
	
		pVtx += 4;
	}

	// 頂点バッファをアンロックする
	g_pVtxBuffTitleSelect->Unlock();
}

//================================================================================================================
// 背景の描画処理
//================================================================================================================
void DrawTitleSelect(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();			// デバイスのポインタ
	TITLESELECT* pSelect = &g_aTitleSelect[0];

	//頂点バッファをデータストリームに設定
	pDevice->SetStreamSource(0, g_pVtxBuffTitleSelect, 0, sizeof(VERTEX_2D));

	// 頂点フォーマットの設定
	pDevice->SetFVF(FVF_VERTEX_2D);
	
	for (int nCntTitleSelect = 0; nCntTitleSelect < TITLESELECTTYPE_NONE; nCntTitleSelect++, pSelect++)
	{
		if (g_bUseSelect == true)
		{
			// テクスチャの設定(使わないならNULLを入れる！！！！)
			pDevice->SetTexture(0, g_apTextureTitleSelect[pSelect->type]);

			// ポリゴンの描画
			pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP,		// プリミティブの種類
				4 * nCntTitleSelect,						// 描画する最初の頂点インデックス
				2);											// 描画するプリミティブの数
		}
	}
}

TITLESELECTTYPE GetTitleSelect(void)
{
	return (TITLESELECTTYPE)g_nTitleSelect;
}

void SetTitleSelect(bool bUse)
{
	g_bUseSelect = bUse;
}