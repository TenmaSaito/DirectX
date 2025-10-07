//================================================================================================================
//
// DirectXのブロックファイル [block.cpp]
// Author : TENMA
//
//================================================================================================================
#include "tutorial_gate.h"
#include "game.h"
#include "sound.h"
#include "player.h"
#include "fade.h"
#include "showDifficulty.h"

// マクロ定義
#define MAX_GATE		(10)			// ゲートの最大数

// プロトタイプ宣言
void CollisionPlayer(GATE *pGate);

// グローバル変数
LPDIRECT3DTEXTURE9		g_pTextureTutorialGate = NULL;	// テクスチャへのポインタ
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffTutorialGate = NULL;	// 頂点バッファのポインタ
GATE g_aTutorialGate[MAX_GATE];							// ゲート構造体
int g_nAlphaGate;										// α地変同様変数

//================================================================================================================
// ブロックの初期化処理
//================================================================================================================
void InitTutorialGate(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();			// デバイスのポインタ
	GATE *pTutorialGate = &g_aTutorialGate[0];
	int nCntTutorialGate;

	for (nCntTutorialGate = 0; nCntTutorialGate < MAX_GATE; nCntTutorialGate++,pTutorialGate++)
	{
		pTutorialGate->pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		pTutorialGate->col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
		pTutorialGate->size = POLY_SIZE(GATE_WIDTH, GATE_HEIGHT);
		pTutorialGate->difficulty = GAMEDIFFICULTY_EASY;
		pTutorialGate->type = GATETYPE_EXIT_EASY;
		pTutorialGate->bUse = false;
	}

	g_nAlphaGate = 1;

	// テクスチャの読み込み
	D3DXCreateTextureFromFile(pDevice,
							"data\\TEXTURE\\CHARACTER\\TITLE\\TITLE_MAGE.png",
							&g_pTextureTutorialGate);

	// 頂点バッファの生成
	pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * 4 * MAX_GATE,			// sizeofの後必ず * 頂点数 を書くこと！
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_2D,
		D3DPOOL_MANAGED,
		&g_pVtxBuffTutorialGate,
		NULL);

	AddFunctionLog("END : VertexBuffer Create");

	VERTEX_2D* pVtx = NULL;					// 頂点情報へのポインタ

	// 頂点バッファをロックし、頂点情報へのポインタを取得
	g_pVtxBuffTutorialGate->Lock(0, 0, (void**)&pVtx, 0);

	AddFunctionLog("START : VertexBuffer Setting");

	pTutorialGate = &g_aTutorialGate[0];

	for (nCntTutorialGate = 0; nCntTutorialGate < MAX_GATE; nCntTutorialGate++,pTutorialGate++)
	{
		// 頂点座標の設定(座標設定は必ず右回りで！！！)
		pVtx[0].pos.x = pTutorialGate->pos.x - pTutorialGate->size.hx;
		pVtx[0].pos.y = pTutorialGate->pos.y - pTutorialGate->size.hy;
		pVtx[0].pos.z = 0.0f;

		pVtx[1].pos.x = pTutorialGate->pos.x + pTutorialGate->size.hx;
		pVtx[1].pos.y = pTutorialGate->pos.y - pTutorialGate->size.hy;
		pVtx[1].pos.z = 0.0f;

		pVtx[2].pos.x = pTutorialGate->pos.x - pTutorialGate->size.hx;
		pVtx[2].pos.y = pTutorialGate->pos.y + pTutorialGate->size.hy;
		pVtx[2].pos.z = 0.0f;

		pVtx[3].pos.x = pTutorialGate->pos.x + pTutorialGate->size.hx;
		pVtx[3].pos.y = pTutorialGate->pos.y + pTutorialGate->size.hy;
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
		pVtx[0].tex = D3DXVECTOR2(0.0f, 0.5f);
		pVtx[1].tex = D3DXVECTOR2(1.0f, 0.5f);
		pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
		pVtx[3].tex = D3DXVECTOR2(1.0f, 1.0f);

		pVtx += 4;
	}

	// 頂点バッファをアンロックする
	g_pVtxBuffTutorialGate->Unlock();

	AddFunctionLog("END : VertexBuffer Create");
}

//================================================================================================================
// ブロックの終了処理
//================================================================================================================
void UninitTutorialGate(void)
{
	// テクスチャの破棄(必ず行うこと！！！)
	if (g_pTextureTutorialGate != NULL)
	{
		g_pTextureTutorialGate->Release();
		g_pTextureTutorialGate = NULL;
	}
	

	// 頂点バッファの破棄(必ず行うこと！！！)
	if (g_pVtxBuffTutorialGate != NULL)
	{
		g_pVtxBuffTutorialGate->Release();
		g_pVtxBuffTutorialGate = NULL;
	}
}

//================================================================================================================
// ブロックの更新処理
//================================================================================================================
void UpdateTutorialGate(void)
{
	VERTEX_2D *pVtx = NULL;
	GATE *pTutorialGate = &g_aTutorialGate[0];
	PLAYER *pPlayer = GetPlayer();
	
	// 頂点バッファをロックし、頂点情報へのポインタを取得
	g_pVtxBuffTutorialGate->Lock(0, 0, (void**)&pVtx, 0);

	for (int nCntTutorialGate = 0; nCntTutorialGate < MAX_GATE; nCntTutorialGate++, pTutorialGate++)
	{
		if (pTutorialGate->bUse == true)
		{
			D3DXVECTOR3 pos = D3DXVECTOR3(0.0f,0.0f,0.0f);

			pTutorialGate->col.a += 0.01f * g_nAlphaGate;
			if (pTutorialGate->col.a >= 1.0f || pTutorialGate->col.a <= 0.1f)
			{
				g_nAlphaGate *= -1;
			}

			// 当たり判定
			CollisionPlayer(pTutorialGate);

			pos = pPlayer->moveposPlayer;

			// 頂点座標の設定(座標設定は必ず右回りで！！！)
			pVtx[0].pos.x = (pTutorialGate->pos.x + pos.x) - pTutorialGate->size.hx;
			pVtx[0].pos.y = (pTutorialGate->pos.y + pos.y) - pTutorialGate->size.hy;
			pVtx[0].pos.z = 0.0f;

			pVtx[1].pos.x = (pTutorialGate->pos.x + pos.x) + pTutorialGate->size.hx;
			pVtx[1].pos.y = (pTutorialGate->pos.y + pos.y) - pTutorialGate->size.hy;
			pVtx[1].pos.z = 0.0f;

			pVtx[2].pos.x = (pTutorialGate->pos.x + pos.x) - pTutorialGate->size.hx;
			pVtx[2].pos.y = (pTutorialGate->pos.y + pos.y) + pTutorialGate->size.hy;
			pVtx[2].pos.z = 0.0f;

			pVtx[3].pos.x = (pTutorialGate->pos.x + pos.x) + pTutorialGate->size.hx;
			pVtx[3].pos.y = (pTutorialGate->pos.y + pos.y) + pTutorialGate->size.hy;
			pVtx[3].pos.z = 0.0f;

			// rhwの設定
			pVtx[0].rhw = 1.0f;
			pVtx[1].rhw = 1.0f;
			pVtx[2].rhw = 1.0f;
			pVtx[3].rhw = 1.0f;

			// 頂点カラーの設定
			pVtx[0].col = pTutorialGate->col;
			pVtx[1].col = pTutorialGate->col;
			pVtx[2].col = pTutorialGate->col;
			pVtx[3].col = pTutorialGate->col;

			pVtx[0].tex = D3DXVECTOR2(0.0f, 0.5f);
			pVtx[1].tex = D3DXVECTOR2(1.0f, 0.5f);
			pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
			pVtx[3].tex = D3DXVECTOR2(1.0f, 1.0f);
		}

		pVtx += 4;				// 頂点データのポインタを4つ分進める
	}

	// 頂点バッファをアンロックする
	g_pVtxBuffTutorialGate->Unlock();
}

//================================================================================================================
// ブロックの描画処理
//================================================================================================================
void DrawTutorialGate(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();			// デバイスのポインタ
	GATE *pTutorialGate = &g_aTutorialGate[0];
	HWND hWnd = NULL;

	//頂点バッファをデータストリームに設定
	pDevice->SetStreamSource(0, g_pVtxBuffTutorialGate, 0, sizeof(VERTEX_2D));

	// 頂点フォーマットの設定
	pDevice->SetFVF(FVF_VERTEX_2D);

	for (int nCntTutorialGate = 0; nCntTutorialGate < MAX_GATE; nCntTutorialGate++,pTutorialGate++)
	{
		if (pTutorialGate->bUse == true)
		{
			// テクスチャの設定(使わないならNULLを入れる！！！！)
			pDevice->SetTexture(0, g_pTextureTutorialGate);

			// ポリゴンの描画
			pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP,		// プリミティブの種類
				nCntTutorialGate * 4,						// 描画する最初の頂点インデックス
				2);											// 描画するプリミティブの数
		}
	}
}

//================================================================================================================
// ブロックの読み込み処理
//================================================================================================================
void LoadTutorialGate(char* pFileName)
{
	char aStr[256];
	char aTrash[3];
	FILE *pFile;
	D3DXVECTOR3 pos;
	HWND hWnd = GetActiveWindow();
	GATE lGate = {};

	pFile = fopen(pFileName, "r");
	if (pFile != NULL)
	{
		(void)fscanf(pFile, "%s", &aStr[0]);
		if (strcmp(&aStr[0], "START_SCRIPT") == 0)
		{
			while (1)
			{
				(void)fscanf(pFile, "%s", &aStr[0]);
				if (strcmp(&aStr[0], "START_SETGATE") == 0)
				{
					while (1)
					{
						(void)fscanf(pFile, "%s", &aStr[0]);
 						if (strcmp(&aStr[0], "POS") == 0)
						{
							fread(&aTrash[0], 1, sizeof(aTrash), pFile);
							(void)fscanf(pFile, "%f %f %f", &lGate.pos.x, &lGate.pos.y, &lGate.pos.z);
						}
						else if (strcmp(&aStr[0], "TYPE") == 0)
						{
							fread(&aTrash[0], 1, sizeof(aTrash), pFile);
							(void)fscanf(pFile, "%d", &lGate.type);
						}
						else if (strcmp(&aStr[0], "SIZE") == 0)
						{
							fread(&aTrash[0], 1, sizeof(aTrash), pFile);
							(void)fscanf(pFile, "%f %f", &lGate.size.x, &lGate.size.y);
							lGate.size = POLY_SIZE(lGate.size.x, lGate.size.y);
						}

						if (strcmp(&aStr[0], "END_SETGATE") == 0)
						{
							SetTutorialGate(lGate.type, lGate.pos, lGate.size);
							break;
						}
					}
				}
				
				if (strcmp(&aStr[0], "END_SCRIPT") == 0)
				{
					break;
				}
			}
		}

		fclose(pFile);
		
	}
	else
	{
		MessageBox(hWnd, "ファイルデータの読み込みに失敗！(GATE)", "警告！", MB_ICONWARNING);
	}
}

//================================================================================================================
// ブロックの設置
//================================================================================================================
int SetTutorialGate(GATETYPE type, D3DXVECTOR3 pos, POLY_SIZE size)
{
	GATE* pTutorialGate = &g_aTutorialGate[0];
	PLAYER* pPlayer = GetPlayer();
	D3DXVECTOR3 Camerapos;
	VERTEX_2D* pVtx;
	HWND hWnd = NULL;
	int nCntTutorialGate;

	if (type < GATETYPE_EXIT_EASY || type >= GATETYPE_MAX)
	{
		if (SUCCEEDED(GetHandleWindow(&hWnd)))
		{ // ブロックの種類が万が一規定外の値だった場合、処理を中断
#ifdef  _DEBUG
			MessageBox(hWnd, "ヤバイ", "え？", MB_ICONWARNING);
#endif //  _DEBUG
			return 0;
		}
	}

	AddFunctionLog("START : TutorialGate Set");

	for (nCntTutorialGate = 0; nCntTutorialGate < MAX_GATE; nCntTutorialGate++,pTutorialGate++)
	{
		if (pTutorialGate->bUse == false)
		{
			Camerapos = pPlayer->moveposPlayer;

			pTutorialGate->pos = pos;
			pTutorialGate->type = type;
			pTutorialGate->difficulty = (GAMEDIFFICULTY)type;
			pTutorialGate->col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.8f);
			pTutorialGate->size = size;
			
			// 頂点バッファをロックし、頂点情報へのポインタを取得
			g_pVtxBuffTutorialGate->Lock(0, 0, (void**)&pVtx, 0);

			pVtx += 4 * nCntTutorialGate;

			// 頂点座標の設定(座標設定は必ず右回りで！！！)
			pVtx[0].pos.x = (pTutorialGate->pos.x + Camerapos.x) - pTutorialGate->size.hx;
			pVtx[0].pos.y = (pTutorialGate->pos.y + Camerapos.y) - pTutorialGate->size.hy;
			pVtx[0].pos.z = 0.0f;

			pVtx[1].pos.x = (pTutorialGate->pos.x + Camerapos.x) + pTutorialGate->size.hx;
			pVtx[1].pos.y = (pTutorialGate->pos.y + Camerapos.y) - pTutorialGate->size.hy;
			pVtx[1].pos.z = 0.0f;

			pVtx[2].pos.x = (pTutorialGate->pos.x + Camerapos.x) - pTutorialGate->size.hx;
			pVtx[2].pos.y = (pTutorialGate->pos.y + Camerapos.y) + pTutorialGate->size.hy;
			pVtx[2].pos.z = 0.0f;

			pVtx[3].pos.x = (pTutorialGate->pos.x + Camerapos.x) + pTutorialGate->size.hx;
			pVtx[3].pos.y = (pTutorialGate->pos.y + Camerapos.y) + pTutorialGate->size.hy;
			pVtx[3].pos.z = 0.0f;

			pVtx[0].tex = D3DXVECTOR2(0.0f, 0.5f);
			pVtx[1].tex = D3DXVECTOR2(1.0f, 0.5f);
			pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
			pVtx[3].tex = D3DXVECTOR2(1.0f, 1.0f);

			// 頂点カラーの設定
			pVtx[0].col = pTutorialGate->col;
			pVtx[1].col = pTutorialGate->col;
			pVtx[2].col = pTutorialGate->col;
			pVtx[3].col = pTutorialGate->col;

			// 頂点バッファをアンロックする
			g_pVtxBuffTutorialGate->Unlock();

			pTutorialGate->bUse = true;

			SetShowDifficulty(D3DXVECTOR3(pTutorialGate->pos.x, pTutorialGate->pos.y + 70.0f, pTutorialGate->pos.z), pTutorialGate->difficulty);

			break;
		}
	}

	return nCntTutorialGate;
}

//================================================================================================================
// ブロックの消去(全て)
//================================================================================================================
void DestroyTutorialGate(void)
{
	GATE *pTutorialGate = &g_aTutorialGate[0];

	for (int nCntTutorialGate = 0; nCntTutorialGate < MAX_GATE; nCntTutorialGate++, pTutorialGate++)
	{
		if (pTutorialGate->bUse == true)
		{
			pTutorialGate->bUse = false;
		}
	}
}

//================================================================================================================
// ブロックの当たり判定(Player)
//================================================================================================================
void CollisionPlayer(GATE* pTutorialGate)
{
	PLAYER* pPlayer = GetPlayer();

	if (pPlayer->posPlayer.x >= pTutorialGate->pos.x - pTutorialGate->size.hx - (PLAYER_SIZE * 0.25f)
		&& pPlayer->posPlayer.x <= pTutorialGate->pos.x + pTutorialGate->size.hx + (PLAYER_SIZE * 0.25f)
		&& pPlayer->posPlayer.y >= pTutorialGate->pos.y - pTutorialGate->size.hy - (PLAYER_SIZE * 0.5f)
		&& pPlayer->posPlayer.y <= pTutorialGate->pos.y + pTutorialGate->size.hy + (PLAYER_SIZE * 0.5f))
	{
		switch (pTutorialGate->type)
		{
		case GATETYPE_EXIT_EASY:

			pPlayer->movePlayer.x = 0.0f;
			pPlayer->movePlayer.y = 0.0f;

			pPlayer->state = PLAYERSTATE_UNMOVE;

			if (GetFade() == FADE_NONE)
			{
				SetGameTutorial(false);
				PlaySound(SOUND_LABEL_SE_TELEPORT);
				SetGameDifficulty(GAMEDIFFICULTY_EASY);
				SetFade(MODE_GAME, FADE_TYPE_TEXTURE);
				FadeSound(SOUND_LABEL_GAME_NORMAL);
			}

			break;

		case GATETYPE_EXIT_NORMAL:

			pPlayer->movePlayer.x = 0.0f;
			pPlayer->movePlayer.y = 0.0f;

			pPlayer->state = PLAYERSTATE_UNMOVE;

			if (GetFade() == FADE_NONE)
			{
				SetGameTutorial(false);
				PlaySound(SOUND_LABEL_SE_TELEPORT);
				SetGameDifficulty(GAMEDIFFICULTY_NORMAL);
				SetFade(MODE_GAME, FADE_TYPE_TEXTURE);
				FadeSound(SOUND_LABEL_GAME_NORMAL);
			}

			break;

		case GATETYPE_EXIT_HARD:

			pPlayer->movePlayer.x = 0.0f;
			pPlayer->movePlayer.y = 0.0f;

			pPlayer->state = PLAYERSTATE_UNMOVE;

			if (GetFade() == FADE_NONE)
			{
				SetGameTutorial(false);
				PlaySound(SOUND_LABEL_SE_TELEPORT);
				SetGameDifficulty(GAMEDIFFICULTY_HARD);
				SetFade(MODE_GAME, FADE_TYPE_TEXTURE);
				FadeSound(SOUND_LABEL_GAME_NORMAL);
			}

			break;
		}
	}
}
