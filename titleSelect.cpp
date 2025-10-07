//================================================================================================================
//
// DirectXのタイトルセレクト処理 [titleSelect.cpp]
// Author : TENMA
//
//================================================================================================================
#include "main.h"
#include "title.h"
#include "titleSelect.h"
#include "input.h"
#include "sound.h"
#include "block.h"
#include "fade.h"
#include "game.h"
#include "settings.h"
#include "title_Mage.h"
#include "titlelogo.h"

// マクロ定義
#define SELECTWAIT_STATE		(100)							// 選択後の待機時間
#define SHADOW_ALPHA			(0.05f)							// 影の濃さ
#define SELECT_NORMAL			D3DXVECTOR3(SCREEN_WIDTH * 0.3f, SCREEN_HEIGHT * 0.3f, 0.0f)			// 選択肢が選ばれているときの位置
#define SELECT_UNSELECT			D3DXVECTOR3(SCREEN_WIDTH * 0.3f, SCREEN_HEIGHT * 0.3f + 200.0f, 0.0f)	// 選択肢が選ばれていない時の位置
#define SELECT_UPPER			(SELECT_NORMAL.y)				// 選択肢の浮かんでいるときの位置
#define SELECT_LOWER			(SELECT_UPPER - 200.0f)	// 選択肢の選んでいない状態の高さ(透明)
#define SELECTSHADOW_NORMAL		D3DXVECTOR3(SCREEN_WIDTH * 0.3f, 416.0f, 0.0f)		// 影の初期位置
#define SELECT_MOVE_X				(0.95f)							// 横に動く幅
#define SELECT_MOVE_Y				(1.5f)							// 縦に動く幅
#define SELECT_MOVE_X_MAX			(15.0f)							// 横に動く幅の上下限
#define SELECT_MOVE_Y_MAX			(10.0f)							// 縦に動く幅の上下限
#define SELECT_BOOK_NORMAL		D3DXVECTOR3(SCREEN_WIDTH * 0.7f, SCREEN_HEIGHT * 0.4f + (50.0f * (nCntTitleSelect + 1)), 0.0f)		// 本の選択肢の位置

// タイトルセレクト構造体
typedef struct
{
	D3DXVECTOR3 pos;			// 選択肢の位置
	D3DXCOLOR col;				// 選択肢の色
	TITLESELECTTYPE type;		// 選択肢の種類
	bool bSelect;				// 選択されているか
}TITLESELECT;

// グローバル変数
LPDIRECT3DTEXTURE9		g_pTextureTitleSelect = NULL;	// テクスチャへのポインタ
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffTitleSelect = NULL;						// 頂点バッファのポインタ
TITLESELECT g_aTitleSelect[TITLESELECTTYPE_MAX];							// タイトルセレクト構造体の宣言
int g_nTitleSelect;															// 現在選ばれている選択肢
int g_nCounterSelectState;													// 選択後の待機時間
int g_nCounterSelect;														// セレクト画面の汎用カウンター
bool g_bUseSelect;															// ポーズ画面が使われているかどうか							
bool g_bMoveSelectUpper;													// 上方向に動くかどうか
bool g_bMoveSelectLeft;														// 左方向に動くかどうか
bool g_bMoveSelect[2];															// 選択肢が動いてるかどうか

//================================================================================================================
// 背景の初期化処理
//================================================================================================================
void InitTitleSelect(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();			// デバイスのポインタ
	TITLESELECT *pTitleSelect = &g_aTitleSelect[0];
	int nCntTitleSelect;

	// 各変数の初期化
	for (nCntTitleSelect = 0; nCntTitleSelect < TITLESELECTTYPE_BOOKSTART; nCntTitleSelect++, pTitleSelect++)
	{
		pTitleSelect->pos = D3DXVECTOR3(SCREEN_WIDTH * 0.3f, SCREEN_HEIGHT * 0.3f, 0.0f);
		pTitleSelect->col = D3DXCOLOR_NULL;
		pTitleSelect->type = (TITLESELECTTYPE)nCntTitleSelect;
		pTitleSelect->bSelect = false;
	}

	for (nCntTitleSelect = TITLESELECTTYPE_BOOKSTART; nCntTitleSelect < TITLESELECTTYPE_START_SHADOW; nCntTitleSelect++, pTitleSelect++)
	{
		pTitleSelect->pos = D3DXVECTOR3(SCREEN_WIDTH * 0.7f, SCREEN_HEIGHT * 0.4f + (50.0f * (nCntTitleSelect + 1)), 0.0f);
		pTitleSelect->col = D3DXCOLOR_NULL;
		pTitleSelect->type = (TITLESELECTTYPE)nCntTitleSelect;
		pTitleSelect->bSelect = false;
	}

	for (nCntTitleSelect = TITLESELECTTYPE_START_SHADOW; nCntTitleSelect < TITLESELECTTYPE_MAX; nCntTitleSelect++, pTitleSelect++)
	{
		pTitleSelect->pos = D3DXVECTOR3(SCREEN_WIDTH * 0.3f, 416.0f, 0.0f);
		pTitleSelect->col = D3DXCOLOR(0.0f,0.0f,0.0f, SHADOW_ALPHA);
		pTitleSelect->type = (TITLESELECTTYPE)nCntTitleSelect;
		pTitleSelect->bSelect = false;
	}


	D3DXCreateTextureFromFile(pDevice,
		TITLE_FILENAME,
		&g_pTextureTitleSelect);

	// 頂点バッファの生成
	pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * 4 * TITLESELECTTYPE_MAX,			// sizeofの後必ず * 頂点数 を書くこと！
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_2D,
		D3DPOOL_MANAGED,
		&g_pVtxBuffTitleSelect,
		NULL);

	g_nTitleSelect = TITLESELECTTYPE_START;
	g_nCounterSelectState = SELECTWAIT_STATE;
	g_nCounterSelect = 0;
	g_aTitleSelect[g_nTitleSelect].bSelect = true;
	g_aTitleSelect[g_nTitleSelect + TITLESELECTTYPE_BOOKSTART].bSelect = true;
	g_aTitleSelect[g_nTitleSelect + TITLESELECTTYPE_START_SHADOW].bSelect = true;
	g_bMoveSelectUpper = true;
	g_bMoveSelectLeft = true;
	g_bUseSelect = false;
	g_bMoveSelect[0] = false;
	g_bMoveSelect[1] = false;

	VERTEX_2D* pVtx = NULL;					// 頂点情報へのポインタ

	// 頂点バッファをロックし、頂点情報へのポインタを取得
	g_pVtxBuffTitleSelect->Lock(0, 0, (void**)&pVtx, 0);

	pTitleSelect = &g_aTitleSelect[0];

	for (nCntTitleSelect = 0; nCntTitleSelect < TITLESELECTTYPE_MAX; nCntTitleSelect++)
	{
		// 頂点座標の設定(座標設定は必ず右回りで！！！)
		pVtx[0].pos.x = pTitleSelect->pos.x - (SELECTSIZE_WIDTH * 0.5f);
		pVtx[0].pos.y = pTitleSelect->pos.y - (SELECTSIZE_HEIGHT * 0.5f);
		pVtx[0].pos.z = 0.0f;

		pVtx[1].pos.x = pTitleSelect->pos.x + (SELECTSIZE_WIDTH * 0.5f);
		pVtx[1].pos.y = pTitleSelect->pos.y - (SELECTSIZE_HEIGHT * 0.5f);
		pVtx[1].pos.z = 0.0f;

		pVtx[2].pos.x = pTitleSelect->pos.x - (SELECTSIZE_WIDTH * 0.5f);
		pVtx[2].pos.y = pTitleSelect->pos.y + (SELECTSIZE_HEIGHT * 0.5f);
		pVtx[2].pos.z = 0.0f;

		pVtx[3].pos.x = pTitleSelect->pos.x + (SELECTSIZE_WIDTH * 0.5f);
		pVtx[3].pos.y = pTitleSelect->pos.y + (SELECTSIZE_HEIGHT * 0.5f);
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
		pVtx[0].tex = D3DXVECTOR2(0.0f, 0.125f * (nCntTitleSelect % TITLESELECTTYPE_START_SHADOW));
		pVtx[1].tex = D3DXVECTOR2(1.0f, 0.125f * (nCntTitleSelect % TITLESELECTTYPE_START_SHADOW));
		pVtx[2].tex = D3DXVECTOR2(0.0f, 0.125f * (nCntTitleSelect % TITLESELECTTYPE_START_SHADOW) + 0.125f);
		pVtx[3].tex = D3DXVECTOR2(1.0f, 0.125f * (nCntTitleSelect % TITLESELECTTYPE_START_SHADOW) + 0.125f);

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
	
	if (g_pTextureTitleSelect != NULL)
	{
		g_pTextureTitleSelect->Release();
		g_pTextureTitleSelect = NULL;
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
		if (g_nCounterSelectState != SELECTWAIT_STATE || GetFade() != FADE_NONE) continue;
		if (pTitleSelect->type >= TITLESELECTTYPE_START && pTitleSelect->type < TITLESELECTTYPE_BOOKSTART)
		{
			if (pTitleSelect->bSelect == true)
			{
				if (pTitleSelect->col.a >= 1.0f) { g_bMoveSelect[0] = false; continue; }
				pTitleSelect->col.a += 0.05f;
				pTitleSelect->pos.y -= 10.0f;
			}
			else
			{
				if (pTitleSelect->col.a <= 0.0f) { g_bMoveSelect[1] = false; continue; }
				pTitleSelect->col.a -= 0.05f;
				pTitleSelect->pos.y += 10.0f;
			}
		}
		else if (pTitleSelect->type >= TITLESELECTTYPE_BOOKSTART && pTitleSelect->type < TITLESELECTTYPE_START_SHADOW)
		{
			if (pTitleSelect->bSelect == false)
			{
				if (pTitleSelect->col.a <= 0.3f) 
				{ 
					continue; 
				}

				pTitleSelect->col.a -= 0.05f;
			}
			else
			{
				if (pTitleSelect->col.a >= 1.0f) continue;
				pTitleSelect->col.a += 0.05f;
			}
		}
		else if (pTitleSelect->type >= TITLESELECTTYPE_START_SHADOW && pTitleSelect->type < TITLESELECTTYPE_MAX)
		{
			if (pTitleSelect->bSelect == true)
			{
				if (pTitleSelect->col.a >= SHADOW_ALPHA) { continue; }
				pTitleSelect->col.a += 0.05f;
			}
			else
			{
				if (pTitleSelect->col.a <= 0.0f) continue;
				pTitleSelect->col.a -= 0.025f;
			}
		}
	}

	if (g_nTitleSelect != TITLESELECTTYPE_MAX 
		&& g_nCounterSelectState == SELECTWAIT_STATE 
		&& GetEnableTitleSelect() == true
		&& g_bMoveSelect[0] == false
		&& g_bMoveSelect[1] == false
		&& GetFade() == FADE_NONE)
	{
		if (GetKeyboardRepeat(DIK_S) == true
			|| GetJoypadRepeat(JOYKEY_DOWN) == true
			|| GetJoyThumbRepeat(JOYTHUMB_LY_DOWN) == true)
		{
			PlaySound(SOUND_LABEL_SE_SELECT);

			// 選択中だった初期化し、未選択状態に
			g_aTitleSelect[g_nTitleSelect].bSelect = false;
			g_aTitleSelect[g_nTitleSelect].pos = SELECT_NORMAL;
			g_aTitleSelect[g_nTitleSelect + TITLESELECTTYPE_BOOKSTART].bSelect = false;
			g_aTitleSelect[g_nTitleSelect + TITLESELECTTYPE_START_SHADOW].bSelect = false;
			g_aTitleSelect[g_nTitleSelect + TITLESELECTTYPE_START_SHADOW].pos = SELECTSHADOW_NORMAL;

			g_nTitleSelect++;
			if (g_nTitleSelect >= TITLESELECTTYPE_BOOKSTART)
			{
				g_nTitleSelect = TITLESELECTTYPE_START;
			}

			g_aTitleSelect[g_nTitleSelect].pos = SELECT_UNSELECT;
			SetTitleMage(D3DXVECTOR3(g_aTitleSelect[g_nTitleSelect + TITLESELECTTYPE_BOOKSTART].pos.x - 150.0f, g_aTitleSelect[g_nTitleSelect + TITLESELECTTYPE_BOOKSTART].pos.y, 0.0f), TITLE_MAGETYPE_SELECT);
			g_aTitleSelect[g_nTitleSelect].bSelect = true;
			g_aTitleSelect[g_nTitleSelect + TITLESELECTTYPE_BOOKSTART].bSelect = true;
			g_aTitleSelect[g_nTitleSelect + TITLESELECTTYPE_START_SHADOW].bSelect = true;
			SetTitleMage(SELECTSHADOW_NORMAL, TITLE_MAGETYPE_TEXT);

			g_bMoveSelect[0] = true;
			g_bMoveSelect[1] = true;
		}
		else if (GetKeyboardRepeat(DIK_W) == true
			|| GetJoypadRepeat(JOYKEY_UP) == true
			|| GetJoyThumbRepeat(JOYTHUMB_LY_UP) == true)
		{
			PlaySound(SOUND_LABEL_SE_SELECT);

			// 選択中だった初期化し、未選択状態に
			g_aTitleSelect[g_nTitleSelect].bSelect = false;
			g_aTitleSelect[g_nTitleSelect].pos = SELECT_NORMAL;
			g_aTitleSelect[g_nTitleSelect + TITLESELECTTYPE_BOOKSTART].bSelect = false;
			g_aTitleSelect[g_nTitleSelect + TITLESELECTTYPE_START_SHADOW].bSelect = false;
			g_aTitleSelect[g_nTitleSelect + TITLESELECTTYPE_START_SHADOW].pos = SELECTSHADOW_NORMAL;

			g_nTitleSelect--;
			if (g_nTitleSelect < TITLESELECTTYPE_START)
			{
				g_nTitleSelect = TITLESELECTTYPE_EXIT;
			}

			g_aTitleSelect[g_nTitleSelect].pos = SELECT_UNSELECT;
			SetTitleMage(D3DXVECTOR3(g_aTitleSelect[g_nTitleSelect + TITLESELECTTYPE_BOOKSTART].pos.x - 150.0f, g_aTitleSelect[g_nTitleSelect + TITLESELECTTYPE_BOOKSTART].pos.y, 0.0f), TITLE_MAGETYPE_SELECT);
			g_aTitleSelect[g_nTitleSelect].bSelect = true;
			g_aTitleSelect[g_nTitleSelect + TITLESELECTTYPE_BOOKSTART].bSelect = true;
			g_aTitleSelect[g_nTitleSelect + TITLESELECTTYPE_START_SHADOW].bSelect = true;
			SetTitleMage(SELECTSHADOW_NORMAL, TITLE_MAGETYPE_TEXT);

			g_bMoveSelect[0] = true;
			g_bMoveSelect[1] = true;
		}

		if (GetKeyboardTrigger(DIK_RETURN) == true
			|| GetJoypadTrigger(JOYKEY_A) == true
			|| GetJoypadTrigger(JOYKEY_START) == true)
		{
			g_nCounterSelectState--;
			PlaySound(SOUND_LABEL_SE_ENTER);
			LightUpTitleMage(TITLE_MAGETYPE_TEXT);
		}
	}
	else if ((GetKeyboardTrigger(DIK_RETURN) == true
		|| GetJoypadTrigger(JOYKEY_A) == true
		|| GetJoypadTrigger(JOYKEY_START) == true)
		&& GetEnableTitleSelect() == false)
	{
		for (int nCntSelect = (g_nTitleSelect + 1); nCntSelect < TITLESELECTTYPE_BOOKSTART; nCntSelect++)
		{
			g_aTitleSelect[(g_nTitleSelect + nCntSelect) % TITLESELECTTYPE_BOOKSTART].pos.y = SELECT_LOWER;
			g_aTitleSelect[(g_nTitleSelect + nCntSelect) % TITLESELECTTYPE_BOOKSTART].col.a = 0.0f;
		}

		for (int nCntTitle = TITLESELECTTYPE_BOOKSTART; nCntTitle < TITLESELECTTYPE_START_SHADOW; nCntTitle++)
		{
			g_aTitleSelect[nCntTitle].col.a = 0.3f;
		}

		g_bMoveSelect[0] = false;
		g_bMoveSelect[1] = false;
	}

	if(g_nCounterSelectState < SELECTWAIT_STATE)
	{
		// 選択している項目によって処理を変更
		switch (g_nTitleSelect)
		{
		case TITLESELECTTYPE_START:
			if (g_nCounterSelectState <= 0)
			{
				if (GetFade() == FADE_NONE)
				{ // ゲームへ進む
#ifdef SELECTTYPETUTORIAL_TRUE
					SetGameTutorial(false);
#else
					SetGameTutorial(true);
#endif
					SetFade(MODE_GAME, FADE_TYPE_TEXTURE);
					FadeSound(SOUND_LABEL_GAME_TUTORIAL);
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

#ifdef SELECTTYPETUTORIAL_TRUE
		case TITLESELECTTYPE_TUTORIAL:

			if (g_nCounterSelectState <= 0)
			{
				if (GetFade() == FADE_NONE)
				{ // チュートリアルへ進む
					SetGameTutorial(true);
					SetFade(MODE_GAME, FADE_TYPE_TEXTURE);
					FadeSound(SOUND_LABEL_GAME_TUTORIAL);
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
#endif

		case TITLESELECTTYPE_CREDIT:

			if (g_nCounterSelectState <= 0)
			{
				if (GetFade() == FADE_NONE)
				{ // クレジットへ進む
					SetFade(MODE_CREDIT, FADE_TYPE_TEXTURE);
					FadeSound(SOUND_LABEL_CREDIT);
				}
			}
			else
			{
				if (g_nCounterSelectState % 20 <= 9)
				{
					g_aTitleSelect[TITLESELECTTYPE_CREDIT].col.a = 0.0f;
				}
				else
				{
					g_aTitleSelect[TITLESELECTTYPE_CREDIT].col.a = 1.0f;
				}
			}

			g_nCounterSelectState--;

			break;

		case TITLESELECTTYPE_EXIT:
			if (SUCCEEDED(GetHandleWindow(&hWnd)))
			{ // ゲームを終了
				DestroyWindow(hWnd);
			}

			break;

		default:

			break;
		}
	}

	if (g_nCounterSelect % 5 == 0)
	{
		if (g_nTitleSelect >= TITLESELECTTYPE_START && g_nTitleSelect <= TITLESELECTTYPE_EXIT)
		{
			if (g_bMoveSelectUpper == true)
			{
				g_aTitleSelect[g_nTitleSelect].pos.y -= SELECT_MOVE_Y;
				g_aTitleSelect[g_nTitleSelect + TITLESELECTTYPE_START_SHADOW].pos.y -= SELECT_MOVE_Y;
				if (g_aTitleSelect[g_nTitleSelect].pos.y < SELECT_UPPER - SELECT_MOVE_Y_MAX) g_bMoveSelectUpper = false;
			}
			else
			{
				g_aTitleSelect[g_nTitleSelect].pos.y += SELECT_MOVE_Y;
				g_aTitleSelect[g_nTitleSelect + TITLESELECTTYPE_START_SHADOW].pos.y += SELECT_MOVE_Y;
				if (g_aTitleSelect[g_nTitleSelect].pos.y > SELECT_UPPER + SELECT_MOVE_Y_MAX) g_bMoveSelectUpper = true;
			}
		}
	}
	else if (g_nCounterSelect % 16 == 0)
	{
		if (g_nTitleSelect >= TITLESELECTTYPE_START && g_nTitleSelect <= TITLESELECTTYPE_EXIT)
		{
			if (g_bMoveSelectUpper == true)
			{
				g_aTitleSelect[g_nTitleSelect].pos.x -= SELECT_MOVE_X;
				g_aTitleSelect[g_nTitleSelect + TITLESELECTTYPE_START_SHADOW].pos.x -= SELECT_MOVE_X;
				if (g_aTitleSelect[g_nTitleSelect].pos.x < SELECT_NORMAL.x - SELECT_MOVE_X_MAX) g_bMoveSelectLeft = false;
			}
			else
			{
				g_aTitleSelect[g_nTitleSelect].pos.x += SELECT_MOVE_X;
				g_aTitleSelect[g_nTitleSelect + TITLESELECTTYPE_START_SHADOW].pos.x += SELECT_MOVE_X;
				if (g_aTitleSelect[g_nTitleSelect].pos.x > SELECT_NORMAL.x + SELECT_MOVE_X_MAX) g_bMoveSelectLeft = true;
			}
		}
	}

	// 頂点バッファをロックし、頂点情報へのポインタを取得
	g_pVtxBuffTitleSelect->Lock(0, 0, (void**)&pVtx, 0);

	pTitleSelect = &g_aTitleSelect[0];

	for (int nCntVtx = 0; nCntVtx < TITLESELECTTYPE_MAX; nCntVtx++,pTitleSelect++)
	{
		// 頂点座標の設定(座標設定は必ず右回りで！！！)
		pVtx[0].pos.x = pTitleSelect->pos.x - (SELECTSIZE_WIDTH * 0.5f);
		pVtx[0].pos.y = pTitleSelect->pos.y - (SELECTSIZE_HEIGHT * 0.5f);
		pVtx[0].pos.z = 0.0f;

		pVtx[1].pos.x = pTitleSelect->pos.x + (SELECTSIZE_WIDTH * 0.5f);
		pVtx[1].pos.y = pTitleSelect->pos.y - (SELECTSIZE_HEIGHT * 0.5f);
		pVtx[1].pos.z = 0.0f;

		pVtx[2].pos.x = pTitleSelect->pos.x - (SELECTSIZE_WIDTH * 0.5f);
		pVtx[2].pos.y = pTitleSelect->pos.y + (SELECTSIZE_HEIGHT * 0.5f);
		pVtx[2].pos.z = 0.0f;

		pVtx[3].pos.x = pTitleSelect->pos.x + (SELECTSIZE_WIDTH * 0.5f);
		pVtx[3].pos.y = pTitleSelect->pos.y + (SELECTSIZE_HEIGHT * 0.5f);
		pVtx[3].pos.z = 0.0f;

		// 頂点カラーの設定
		pVtx[0].col = pTitleSelect->col;
		pVtx[1].col = pTitleSelect->col;
		pVtx[2].col = pTitleSelect->col;
		pVtx[3].col = pTitleSelect->col;
	
		pVtx += 4;
	}

	// 頂点バッファをアンロックする
	g_pVtxBuffTitleSelect->Unlock();

	g_nCounterSelect++;
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
	
	for (int nCntTitleSelect = 0; nCntTitleSelect < TITLESELECTTYPE_MAX; nCntTitleSelect++, pSelect++)
	{
		if (g_bUseSelect == true)
		{
			// テクスチャの設定(使わないならNULLを入れる！！！！)
			pDevice->SetTexture(0, g_pTextureTitleSelect);

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

	SetTitleMage(D3DXVECTOR3(g_aTitleSelect[g_nTitleSelect + TITLESELECTTYPE_BOOKSTART].pos.x - 150.0f, g_aTitleSelect[g_nTitleSelect + TITLESELECTTYPE_BOOKSTART].pos.y, 0.0f), TITLE_MAGETYPE_SELECT);
	g_aTitleSelect[g_nTitleSelect].bSelect = true;
	g_aTitleSelect[g_nTitleSelect + TITLESELECTTYPE_BOOKSTART].bSelect = true;
	g_aTitleSelect[g_nTitleSelect + TITLESELECTTYPE_START_SHADOW].bSelect = true;
	SetTitleMage(SELECTSHADOW_NORMAL, TITLE_MAGETYPE_TEXT);
}