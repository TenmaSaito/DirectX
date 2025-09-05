//================================================================================================================
//
// DirectXのエディタのcppファイル [editer.h]
// Author : TENMA
//
//================================================================================================================
#include "editer.h"
#include "input.h"
#include "enemy.h"
#include "block.h"
#include "stage.h"
#include "fade_stage.h"

// マクロ定義
#define EDITER_SPD			(50)		// エディタブロックの速度
#define FILE_POS			"POS = "	// ファイルでのPOS
#define FILE_TYPE			"TYPE = "	// ファイルでのTYPE
#define FILE_BULLET			"BULLET = "	// ファイルでのBULLET
#define FILE_TEX			"TEX = "	// ファイルでのTEX
#define FILE_MOVE			"MOVE = "	// ファイルでのMOVE
#define FILE_LIFE			"LIFE = "	// ファイルでのLIFE	

// エディタ構造体の定義
typedef struct
{
	D3DXVECTOR3 pos[MAX_EDITER_BLOCK];		// 設置位置
	BLOCKTYPE typeBlock[MAX_EDITER_BLOCK];	// ブロックのタイプ
	ENEMYTYPE typeEnemy[MAX_EDITER_BLOCK];	// 敵の種類
	ENEMYTEX texEnemy[MAX_EDITER_BLOCK];	// 敵のテクスチャの種類
	D3DXVECTOR3 move[MAX_EDITER_BLOCK];		// 移動量
	int nTurn[MAX_EDITER_BLOCK];			// 設置するターン
	bool bUseBlock[MAX_EDITER_BLOCK];		// ブロックが設置されているか(false = enemy)
	int nCounterBlock;		// ブロックを設置した数
	int nCounterEnemy;		// 敵を設置した数
}EDITER;

// グローバル変数
int g_nID;					// エディタブロックのID
int g_nCounterEditer;		// エディタブロックの配置した数
bool g_bUseEditer;
EDITER g_Editer;
LPD3DXFONT g_pFontEditer = NULL;							// フォントへのポインタ


//================================================================================================================
// ステージエディタの初期化処理
//================================================================================================================
void InitEditer(void)
{ 
	EDITER* pEditer = &g_Editer;

	for (int nCntEditer = 0; nCntEditer < MAX_EDITER_BLOCK; nCntEditer++)
	{
		pEditer->pos[nCntEditer] = D3DXVECTOR3_NULL;
		pEditer->typeBlock[nCntEditer] = BLOCKTYPE_WALL;
		pEditer->typeEnemy[nCntEditer] = ENEMYTYPE_FIX;
		pEditer->texEnemy[nCntEditer] = ENEMYTEX_SLIME;
		pEditer->move[nCntEditer] = D3DXVECTOR3_NULL;
		pEditer->bUseBlock[nCntEditer] = false;
		pEditer->nTurn[nCntEditer] = 0;
	}

	pEditer->nCounterBlock = 0;
	pEditer->nCounterEnemy = 0;
	g_nCounterEditer = 0;

	// デバッグ用フォントの生成
	D3DXCreateFont(GetDevice(),
		30,
		0,
		0,
		0,
		FALSE,
		SHIFTJIS_CHARSET,
		OUT_DEFAULT_PRECIS,
		DEFAULT_QUALITY,
		DEFAULT_PITCH,
		"terminal",
		&g_pFontEditer);
}

//================================================================================================================
// ステージエディタの終了処理
//================================================================================================================
void UninitEditer(void)
{ // 無し

}

//================================================================================================================
// ステージエディタの更新処理
//================================================================================================================
void UpdateEditer(void)
{
	BLOCK *pBlock;
	EDITER* pEditer = &g_Editer;
	HWND hWnd;

	if (g_bUseEditer == true)
	{
		pBlock = GetBlock(g_nID);
		hWnd = GetActiveWindow();

		if (pBlock->type == BLOCKTYPE_EDITER)
		{// ステージエディタブロック
			if (GetKeyboardRepeat(DIK_TAB) != true)
			{
				// キーボード入力の各処理
				if (GetKeyboardRepeat(DIK_UP) == true)
				{// Wを押したとき
					// 追加入力
					if (GetKeyboardRepeat(DIK_LEFT) == true)
					{// 左上移動
						// エディタブロックの移動量を更新
						pBlock->pos.x += sinf(D3DX_PI * -0.75f) * EDITER_SPD;
						pBlock->pos.y += cosf(D3DX_PI * -0.75f) * EDITER_SPD;
					}
					else if (GetKeyboardRepeat(DIK_RIGHT) == true)
					{// 右上移動
						// エディタブロックの移動量を更新
						pBlock->pos.x += sinf(D3DX_PI * 0.75f) * EDITER_SPD;
						pBlock->pos.y += cosf(D3DX_PI * 0.75f) * EDITER_SPD;
					}
					else
					{// 上へ移動
						// エディタブロックの移動量を更新
						pBlock->pos.y -= EDITER_SPD;
					}
				}
				else if (GetKeyboardRepeat(DIK_DOWN) == true)
				{// Sを押したとき
					// 追加入力
					if (GetKeyboardRepeat(DIK_LEFT) == true)
					{// 左下移動
						// エディタブロックの移動量を更新
						pBlock->pos.x += sinf(D3DX_PI * -0.25f) * EDITER_SPD;
						pBlock->pos.y += cosf(D3DX_PI * -0.25f) * EDITER_SPD;
					}
					else if (GetKeyboardRepeat(DIK_RIGHT) == true)
					{// 右下移動
						// エディタブロックの移動量を更新
						pBlock->pos.x += sinf(D3DX_PI * 0.25f) * EDITER_SPD;
						pBlock->pos.y += cosf(D3DX_PI * 0.25f) * EDITER_SPD;
					}
					else
					{// 下へ移動
						// エディタブロックの移動量を更新
						pBlock->pos.y += EDITER_SPD;
					}
				}
				else if (GetKeyboardRepeat(DIK_LEFT) == true)
				{// Aを押したとき

					// エディタブロックの移動量を更新
					pBlock->pos.x -= EDITER_SPD;
				}
				else if (GetKeyboardRepeat(DIK_RIGHT) == true)
				{// Dを押したとき
					// エディタブロックの移動量を更新
					pBlock->pos.x += EDITER_SPD;
				}
			}
			else
			{
				// キーボード入力の各処理
				if (GetKeyboardRepeat(DIK_UP) == true)
				{// Wを押したとき
					// 追加入力
					if (GetKeyboardRepeat(DIK_LEFT) == true)
					{// 左上移動
						// エディタブロックの移動量を更新
						pBlock->pos.x += sinf(D3DX_PI * -0.75f) * 1;
						pBlock->pos.y += cosf(D3DX_PI * -0.75f) * 1;
					}
					else if (GetKeyboardRepeat(DIK_RIGHT) == true)
					{// 右上移動
						// エディタブロックの移動量を更新
						pBlock->pos.x += sinf(D3DX_PI * 0.75f) * 1;
						pBlock->pos.y += cosf(D3DX_PI * 0.75f) * 1;
					}
					else
					{// 上へ移動
						// エディタブロックの移動量を更新
						pBlock->pos.y -= 1;
					}
				}
				else if (GetKeyboardRepeat(DIK_DOWN) == true)
				{// Sを押したとき
					// 追加入力
					if (GetKeyboardRepeat(DIK_LEFT) == true)
					{// 左下移動
						// エディタブロックの移動量を更新
						pBlock->pos.x += sinf(D3DX_PI * -0.25f) * 1;
						pBlock->pos.y += cosf(D3DX_PI * -0.25f) * 1;
					}
					else if (GetKeyboardRepeat(DIK_RIGHT) == true)
					{// 右下移動
						// エディタブロックの移動量を更新
						pBlock->pos.x += sinf(D3DX_PI * 0.25f) * 1;
						pBlock->pos.y += cosf(D3DX_PI * 0.25f) * 1;
					}
					else
					{// 下へ移動
						// エディタブロックの移動量を更新
						pBlock->pos.y += 1;
					}
				}
				else if (GetKeyboardRepeat(DIK_LEFT) == true)
				{// Aを押したとき

					// エディタブロックの移動量を更新
					pBlock->pos.x -= 1;
				}
				else if (GetKeyboardRepeat(DIK_RIGHT) == true)
				{// Dを押したとき
					// エディタブロックの移動量を更新
					pBlock->pos.x += 1;
				}
			}
		}

		if (GetKeyboardTrigger(DIK_O) == true)
		{
			int nType = pEditer->typeBlock[g_nCounterEditer];
			int nTypeEnemy = pEditer->typeEnemy[g_nCounterEditer];

			nTypeEnemy++;
			nType++;
			if (nType >= BLOCKTYPE_MAX)
			{
				nType = BLOCKTYPE_WALL;
			}

			if (nTypeEnemy >= ENEMYTYPE_MAX)
			{
				nTypeEnemy = ENEMYTYPE_FIX;
			}

			pEditer->typeBlock[g_nCounterEditer] = (BLOCKTYPE)nType;
			pEditer->typeEnemy[g_nCounterEditer] = (ENEMYTYPE)nTypeEnemy;
		}
		else if (GetKeyboardTrigger(DIK_P) == true)
		{
			int nType = pEditer->typeBlock[g_nCounterEditer];
			int nTypeEnemy = pEditer->typeEnemy[g_nCounterEditer];

			nTypeEnemy--;
			nType--;
			if (nType < BLOCKTYPE_WALL)
			{
				nType = BLOCKTYPE_MAX - 1;
			}

			if (nTypeEnemy < ENEMYTYPE_FIX)
			{
				nTypeEnemy = ENEMYTYPE_MAX - 1;
			}

			pEditer->typeBlock[g_nCounterEditer] = (BLOCKTYPE)nType;
			pEditer->typeEnemy[g_nCounterEditer] = (ENEMYTYPE)nTypeEnemy;
		}

		if (GetKeyboardTrigger(DIK_K) == true)
		{
			int nTexEnemy = pEditer->texEnemy[g_nCounterEditer];

			nTexEnemy++;

			if (nTexEnemy >= ENEMYTEX_MAX)
			{
				nTexEnemy = ENEMYTEX_SLIME;
			}


			pEditer->texEnemy[g_nCounterEditer] = (ENEMYTEX)nTexEnemy;
		}
		else if (GetKeyboardTrigger(DIK_L) == true)
		{
			int nTexEnemy = pEditer->texEnemy[g_nCounterEditer];

			nTexEnemy--;

			if (nTexEnemy < ENEMYTEX_SLIME)
			{
				nTexEnemy = ENEMYTEX_MAX - 1;
			}


			pEditer->texEnemy[g_nCounterEditer] = (ENEMYTEX)nTexEnemy;
		}

		if (GetKeyboardTrigger(DIK_COMMA) == true)
		{
			pEditer->nTurn[g_nCounterEditer]++;
		}
		else if (GetKeyboardTrigger(DIK_COLON) == true)
		{
			pEditer->nTurn[g_nCounterEditer]--;
		}

		if (GetKeyboardTrigger(DIK_N) == true)
		{
			pEditer->move[g_nCounterEditer].z++;
		}
		else if (GetKeyboardTrigger(DIK_M) == true)
		{
			pEditer->move[g_nCounterEditer].z--;
		}

		if (g_nCounterEditer < MAX_EDITER_BLOCK)
		{
			if (GetKeyboardTrigger(DIK_RSHIFT) == true)
			{
				if (g_nCounterEditer >= MAX_EDITER_BLOCK)
				{
					MessageBox(hWnd, "ブロックの配置可能上限数を超えています！", "警告！", MB_ICONWARNING);
					return;
				}

				SetBlock(BLOCKTYPE_EDITER_WALL, pBlock->pos, BLOCK_WIDTH, BLOCK_HEIGHT);
				pEditer->nCounterBlock++;
				pEditer->pos[g_nCounterEditer] = pBlock->pos;
				pEditer->bUseBlock[g_nCounterEditer] = true;
				g_nCounterEditer++;
			}
			else if (GetKeyboardTrigger(DIK_LSHIFT) == true)
			{
				if (g_nCounterEditer >= MAX_EDITER_BLOCK)
				{
					MessageBox(hWnd, "ブロックの配置可能上限数を超えています！", "警告！", MB_ICONWARNING);
					return;
				}

				SetBlock(BLOCKTYPE_EDITER_ENEMY, pBlock->pos, BLOCK_WIDTH, BLOCK_HEIGHT);
				pEditer->nCounterEnemy++;
				pEditer->pos[g_nCounterEditer] = pBlock->pos;
				pEditer->bUseBlock[g_nCounterEditer] = false;
				g_nCounterEditer++;
			}
		}

		if (GetKeyboardTrigger(DIK_F5) == true && g_nCounterEditer != 0)
		{
			char aStr[10][STRING_MAX] = {};

			if (SUCCEEDED(SaveBlock()) && SUCCEEDED(SaveEnemy()))
			{
				strcat(&aStr[0][0], "セーブ結果");

				wsprintf(&aStr[1][0], "配置したエディタブロックの総数 : %d\n", g_nCounterEditer);
				wsprintf(&aStr[2][0], "配置したブロックの総数 : %d\n", pEditer->nCounterBlock);
				wsprintf(&aStr[3][0], "配置した敵の総数 : %d\n", pEditer->nCounterEnemy);

				strcat(&aStr[0][0], &aStr[1][0]);
				strcat(&aStr[0][0], &aStr[2][0]);
				strcat(&aStr[0][0], &aStr[3][0]);

				MessageBox(hWnd, &aStr[0][0], "結果", MB_ICONINFORMATION);
			}
			else
			{
				MessageBox(hWnd, "正常にセーブされませんでした。", "警告！", MB_ICONWARNING);
			}

			g_nCounterEditer = 0;
			g_bUseEditer = false;
			DeleteBlock(g_nID);

			SetFadeStage(STAGE_GRASS, FADESTAGE_OUT);
		}

		if ((pBlock->pos.x) <= STAGE_LEFT + (BLOCK_WIDTH))
		{
			pBlock->pos.x = STAGE_LEFT + (BLOCK_WIDTH);
		}

		if (pBlock->pos.x >= STAGE_RIGHT - (BLOCK_WIDTH))
		{
			pBlock->pos.x = (float)(STAGE_RIGHT - (BLOCK_WIDTH));
		}

		if (pBlock->pos.y <= STAGE_CEILING + (BLOCK_HEIGHT))
		{
			pBlock->pos.y = STAGE_CEILING + (BLOCK_HEIGHT);
		}

		if (pBlock->pos.y > STAGE_FLOOR - (BLOCK_HEIGHT))
		{
			pBlock->pos.y = (float)(STAGE_FLOOR - (BLOCK_HEIGHT));
		}
	}
}

//================================================================================================================
// ステージエディタの描画処理
//================================================================================================================
void DrawEditer(void)
{ // 無し
	EDITER* pEditer = &g_Editer;
	RECT rect = { 0,0,SCREEN_WIDTH,SCREEN_HEIGHT };			// 画面サイズ
	char aStr[15][256];										// 画面に表示する文字列

	// 文字列に代入
	wsprintf(aStr[0], "設置したエディタブロックの数 : %d\n" ,g_nCounterEditer);

	wsprintf(aStr[1], "配置したブロックの総数 : %d\n", pEditer->nCounterBlock);

	wsprintf(aStr[2], "配置した敵の総数 : %d\n", pEditer->nCounterEnemy);

	wsprintf(aStr[3], "現在のブロックの種類 : %d\n", (int)pEditer->typeBlock[g_nCounterEditer]);

	wsprintf(aStr[4], "現在の敵の種類 : %d\n", (int)pEditer->typeEnemy[g_nCounterEditer]);

	wsprintf(aStr[5], "現在の敵のテクスチャの種類 : %d\n", (int)pEditer->texEnemy[g_nCounterEditer]);

	wsprintf(aStr[6], "現在のターン : %d\n", (int)pEditer->nTurn[g_nCounterEditer]);

	// 文字列を結合
	for (int nCntDebug = 0; nCntDebug < 6; nCntDebug++)
	{
		strcat(&aStr[0][0], &aStr[nCntDebug + 1][0]);
	}

	// テキストの描画
	//g_pFontEditer->DrawText(NULL, &aStr[0][0], -1, &rect, DT_RIGHT, D3DCOLOR_RGBA(0, 0, 0, 255));

}

//================================================================================================================
// ステージエディタの有効化
//================================================================================================================
void SetEnableEditer(bool bUse, int nID)
{
	g_bUseEditer = bUse;
	g_nID = nID;
}

//================================================================================================================
// ブロックの書き出し処理
//================================================================================================================
HRESULT SaveBlock(void)
{
	char aStr[STRING_MAX] = {};
	char aNum[STRING_MAX] = {};
	int nBlock = 0;
	FILE* pFile;
	D3DXVECTOR3 pos;
	EDITER* pEditer = &g_Editer;
	HWND hWnd = GetActiveWindow();

	pFile = fopen("data\\STAGE\\DEBUG\\BLOCK.txt", "w");
	if (pFile != NULL)
	{
		AddFunctionLog("START : BLOCK SAVE");

		fprintf(pFile, "START_SCRIPT\n\n");
		fprintf(pFile, "#========================================\n");
		fprintf(pFile, "# ブロックの配置\n");
		fprintf(pFile, "#========================================\n");

		for (int nCntBlock = 0; nCntBlock < g_nCounterEditer; nCntBlock++)
		{
			if (pEditer->bUseBlock[nCntBlock] == false)
			{
				continue;
			}

			fprintf(pFile, "#========================================\n");
			fprintf(pFile, "# %d 番目のブロック\n", nBlock + 1);
			fprintf(pFile, "#========================================\n");
			fprintf(pFile, "START_SETBLOCK\n\n");

			fprintf(pFile, FILE_POS);
			fprintf(pFile, "%.1f %.1f %.1f\n", pEditer->pos[nCntBlock].x, pEditer->pos[nCntBlock].y, pEditer->pos[nCntBlock].z);

			fprintf(pFile, FILE_TYPE);
			fprintf(pFile, "%d\n\n", pEditer->typeBlock[nCntBlock]);

			fprintf(pFile, "END_SETBLOCK\n");
			fprintf(pFile, "#========================================\n\n");

			nBlock++;
		}

		fprintf(pFile, "END_SCRIPT");

		fclose(pFile);

		MessageBox(hWnd, "ファイルデータの書き出しに成功", "成功", MB_ICONINFORMATION);

		return S_OK;
	}
	else
	{
		MessageBox(hWnd, "ファイルデータの書き出しに失敗！(BLOCK)", "警告！", MB_ICONWARNING);

		return E_FAIL;
	}
}

//================================================================================================================
// 敵の書き出し処理
//================================================================================================================
HRESULT SaveEnemy(void)
{
	char aStr[STRING_MAX] = {};
	char aNum[STRING_MAX] = {};
	int nEnemy = 0;
	FILE* pFile;
	D3DXVECTOR3 pos;
	EDITER* pEditer = &g_Editer;
	HWND hWnd = GetActiveWindow();

	pFile = fopen("data\\STAGE\\DEBUG\\ENEMY.txt", "w");
	if (pFile != NULL)
	{
		fprintf(pFile, "START_SCRIPT\n\n");

		for (int nCntTurn = 0; nCntTurn < 3; nCntTurn++)
		{

			fprintf(pFile, "#========================================\n");
			fprintf(pFile, "# ターン%d\n", nCntTurn);
			fprintf(pFile, "#========================================\n");

			fprintf(pFile, "START_TURN%d\n\n",nCntTurn);

			fprintf(pFile, "#========================================\n");
			fprintf(pFile, "# 敵の配置\n");
			fprintf(pFile, "#========================================\n");

			for (int nCntEnemy = 0; nCntEnemy < g_nCounterEditer; nCntEnemy++)
			{
				if (pEditer->bUseBlock[nCntEnemy] != false || pEditer->nTurn[nCntEnemy] != nCntTurn)
				{
					continue;
				}

				fprintf(pFile, "#========================================\n");
				fprintf(pFile, "# %d 番目の敵\n", nEnemy);
				fprintf(pFile, "#========================================\n");
				fprintf(pFile, "START_SETENEMY\n\n");

				fprintf(pFile, FILE_POS);
				fprintf(pFile, "%.1f %.1f %.1f\n", pEditer->pos[nCntEnemy].x, pEditer->pos[nCntEnemy].y, pEditer->pos[nCntEnemy].z);

				fprintf(pFile, FILE_TYPE);
				fprintf(pFile, "%d\n", pEditer->typeEnemy[nCntEnemy]);

				fprintf(pFile, FILE_BULLET);
				fprintf(pFile, "0\n");

				fprintf(pFile, FILE_TEX);
				fprintf(pFile, "%d\n", pEditer->texEnemy[nCntEnemy]);

				fprintf(pFile, FILE_MOVE);
				fprintf(pFile, "%.1f %.1f %.1f\n", pEditer->move[nCntEnemy].x, pEditer->move[nCntEnemy].y, pEditer->move[nCntEnemy].z);

				fprintf(pFile, FILE_LIFE);
				fprintf(pFile, "0\n\n");

				fprintf(pFile, "END_SETENEMY\n");
				fprintf(pFile, "#========================================\n\n");

				nEnemy++;
			}

			fprintf(pFile, "END_TURN%d\n\n", nCntTurn);
		}

		fprintf(pFile, "END_SCRIPT");

		fclose(pFile);

		MessageBox(hWnd, "ファイルデータの書き出しに成功", "成功", MB_ICONINFORMATION);

		return S_OK;
	}
	else
	{
		MessageBox(hWnd, "ファイルデータの書き出しに失敗！(BLOCK)", "警告！", MB_ICONWARNING);

		return E_FAIL;
	}
}