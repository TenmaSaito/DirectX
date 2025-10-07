//================================================================================================================
//
// DirectXのランキング処理 [ranking.cpp]
// Author : TENMA
//
//================================================================================================================
#include "ranking.h"
#include "score.h"
#include "game.h"
#include "input.h"

// マクロ定義
#define NUM_PLACE			(8)				// スコアの桁数
#define NUM_WIDTH			(25)			// 一桁の横幅
#define NUM_HEIGHT			(35)			// 一桁の縦幅
#define MAX_RANKING			(5)				// 表示する順位の数
#define FIRSTMOVE_RANKING	(4)				// 最初に右から出て来る順位
#define MOVE_SPD			D3DXVECTOR3(15.0f,0.0f,0.0f)								// 移動する速さ
#define GOAL_POS			D3DXVECTOR3(700.0f, 120.0f * (nCntRanking + 1), 0.0f)		// 最終的なスコアの位置
#define FILENAME_RANKING_EASY	"data\\RANKING\\Ranking_E.bin"			// ランキング保存用ファイル名(EASY)
#define FILENAME_RANKING_NORMAL	"data\\RANKING\\Ranking_N.bin"			// ランキング保存用ファイル名(NORMAL)
#define FILENAME_RANKING_HARD	"data\\RANKING\\Ranking_H.bin"			// ランキング保存用ファイル名(HARD)
#define FILENAME_RESET		"data\\RANKING\\Ranking_Reset.bin"			// リセット用バイナリファイル名
#define RESET_COMMAND_NUM	(7)				// スコアリセット時に入力するキーの合計数

// ランキング構造体の定義
typedef struct
{
	D3DXVECTOR3 pos;						// スコア表示の位置
	D3DXCOLOR col;							// 色
	int nScore;								// スコアの値
	int nRanking;							// 順位
	bool bAdd;								// 追加されたスコアか
	bool bUse;								// 使われているか
	bool bMove;								// 画面内に入ってくるか
}RANKING, *PRANKING;

// プロトタイプ宣言
void CheckRanking(PRANKING pRanking);
int Compare(const void* a, const void* b);
void ResetRanking(void);

// グローバル変数
LPDIRECT3DTEXTURE9		g_pTextureRanking = NULL;						// テクスチャへのポインタ
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffRanking = NULL;						// 頂点バッファのポインタ
RANKING g_aRanking[GAMEDIFFICULTY_MAX][MAX_RANKING];					// ランキング構造体の宣言
int g_nScoreCurrent;													// 今回のスコア
int g_nCounterRanking;													// 汎用カウンター
int g_nCurrentMoveRanking;												// 現在動いている順位
char g_aRankingFileName[STRING_MAX];									// ランキングのファイル名
bool g_aTrigger[RESET_COMMAND_NUM];										// 指定のキーが入力されたか
	
//================================================================================================================
// ランキングの初期化処理
//================================================================================================================
void InitRanking(void)
{
	LPDIRECT3DDEVICE9 pDevice;			// デバイスのポインタ
	int nCntRanking;
	PRANKING pRanking = &g_aRanking[GetGameDifficulty()][0];

	// デバイスの取得
	pDevice = GetDevice();

	// 構造体の初期化
	for (nCntRanking = 0; nCntRanking < MAX_RANKING; nCntRanking++, pRanking++)
	{
		pRanking->pos = D3DXVECTOR3(1380.0f, 120.0f * (nCntRanking + 1), 0.0f);
		pRanking->col = D3DXCOLOR_NULL;
		pRanking->nScore = 0;
		pRanking->nRanking = 1;
		pRanking->bAdd = false;
		pRanking->bUse = false;
	}

	switch (GetGameDifficulty())
	{
	case GAMEDIFFICULTY_EASY:

		strcpy(g_aRankingFileName, FILENAME_RANKING_EASY);

		break;

	case GAMEDIFFICULTY_NORMAL:

		strcpy(g_aRankingFileName, FILENAME_RANKING_NORMAL);

		break;

	case GAMEDIFFICULTY_HARD:

		strcpy(g_aRankingFileName, FILENAME_RANKING_HARD);

		break;

	default:

		strcpy(g_aRankingFileName, FILENAME_RANKING_NORMAL);

		break;
	}

	g_nCounterRanking = 0;
	g_nCurrentMoveRanking = FIRSTMOVE_RANKING;

	// テクスチャの読み込み
	D3DXCreateTextureFromFile(pDevice,
		"data\\TEXTURE\\CHARACTER\\number003.png",
		&g_pTextureRanking);

	// 頂点バッファの生成
	pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * 4 * (NUM_PLACE * MAX_RANKING),			// NUM_PLACE * MAX_RANKING分の頂点を作成
	D3DUSAGE_WRITEONLY,
	FVF_VERTEX_2D,
	D3DPOOL_MANAGED,
	& g_pVtxBuffRanking,
	NULL);

	VERTEX_2D* pVtx;					// 頂点情報へのポインタ

	// 頂点バッファをロックし、頂点情報へのポインタを取得
	g_pVtxBuffRanking->Lock(0, 0, (void**)&pVtx, 0);

	pRanking = &g_aRanking[GetGameDifficulty()][0];

	for (nCntRanking = 0; nCntRanking < MAX_RANKING; nCntRanking++, pRanking++)
	{
		for (int nCntScore = 0; nCntScore < NUM_PLACE; nCntScore++)
		{
			// 頂点座標の設定(座標設定は必ず右回りで！！！)
			pVtx[0].pos.x = pRanking->pos.x + ((NUM_WIDTH * 2) * nCntScore) - NUM_WIDTH;
			pVtx[0].pos.y = pRanking->pos.y - NUM_HEIGHT;
			pVtx[0].pos.z = 0.0f;
	
			pVtx[1].pos.x = pRanking->pos.x + ((NUM_WIDTH * 2) * nCntScore) + NUM_WIDTH;
			pVtx[1].pos.y = pRanking->pos.y - NUM_HEIGHT;
			pVtx[1].pos.z = 0.0f;
	
			pVtx[2].pos.x = pRanking->pos.x + ((NUM_WIDTH * 2) * nCntScore) - NUM_WIDTH;
			pVtx[2].pos.y = pRanking->pos.y + NUM_HEIGHT;
			pVtx[2].pos.z = 0.0f;
	
			pVtx[3].pos.x = pRanking->pos.x + ((NUM_WIDTH * 2) * nCntScore) + NUM_WIDTH;
			pVtx[3].pos.y = pRanking->pos.y + NUM_HEIGHT;
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
			pVtx[1].tex = D3DXVECTOR2(0.1f, 0.0f);
			pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
			pVtx[3].tex = D3DXVECTOR2(0.1f, 1.0f);
	
			pVtx += 4;
		}
	}
	
	g_pVtxBuffRanking->Unlock();
	
	if (GetModeExac() == MODE_GAMECLEAR || GetModeExac() == MODE_GAMEOVER)
	{
		AddRanking(GetScore());
	}
	else if (GetModeExac() == MODE_TITLE)
	{
		CheckRanking(&g_aRanking[GetGameDifficulty()][0]);
	}
}

//================================================================================================================
// ランキングの終了処理
//================================================================================================================
void UninitRanking(void)
{
	// テクスチャの破棄(必ず行うこと！！！)
	if (g_pTextureRanking != NULL)
	{
		g_pTextureRanking->Release();
		g_pTextureRanking = NULL;
	}

	// 頂点バッファの破棄(必ず行うこと！！！)
	if (g_pVtxBuffRanking != NULL)
	{
		g_pVtxBuffRanking->Release();
		g_pVtxBuffRanking = NULL;
	}
}

//================================================================================================================
// ランキングの更新処理
//================================================================================================================
void UpdateRanking(void)
{
	// 後で書きたきゃ書け
	PRANKING pRanking = &g_aRanking[GetGameDifficulty()][0];;
	VERTEX_2D* pVtx;

	if ((GetKeyboardPress(DIK_R) == true
		&& GetKeyboardPress(DIK_RSHIFT) == true)
		||(GetJoypadPress(JOYKEY_A) == true
			&& GetJoypadPress(JOYKEY_B) == true
			&& GetJoypadPress(JOYKEY_X) == true
			&& GetJoypadPress(JOYKEY_Y) == true))
	{
		ResetRanking();
	}

	if (GetKeyboardTrigger(DIK_RETURN) == true
		|| GetJoypadTrigger(JOYKEY_A) == true
		|| GetJoypadTrigger(JOYKEY_START) == true)
	{
		for (int nCntRanking = 0; nCntRanking < MAX_RANKING; nCntRanking++, pRanking++)
		{
			pRanking->pos = GOAL_POS;
		}

		g_nCurrentMoveRanking = -1;
		pRanking = &g_aRanking[GetGameDifficulty()][0];
	}

	// 頂点バッファをロックし、頂点情報へのポインタを取得
	g_pVtxBuffRanking->Lock(0, 0, (void**)&pVtx, 0);

	for (int nCntRanking = 0; nCntRanking < MAX_RANKING; nCntRanking++, pRanking++)
	{
		if (g_nCurrentMoveRanking == nCntRanking)
		{
			pRanking->pos -= MOVE_SPD;
			if (pRanking->pos.x <= 700.0f)
			{
				pRanking->pos.x = 700.0f;
				g_nCurrentMoveRanking--;
			}
		}

		if (pRanking->bAdd == true)
		{
			if ((g_nCounterRanking % 15) <= 7)
			{
				pRanking->col.a -= 0.08f;
				if (pRanking->col.a < 0.5f)
				{
					pRanking->col.a = 0.5f;
				}
			}
			else
			{
				pRanking->col.a += 0.08f;
				if (pRanking->col.a > 1.0f)
				{
					pRanking->col.a = 1.0f;
				}
			}
		}

		for (int nCntCol = 0; nCntCol < NUM_PLACE; nCntCol++)
		{
			// 頂点座標の設定(座標設定は必ず右回りで！！！)
			pVtx[0].pos.x = pRanking->pos.x + ((NUM_WIDTH * 2) * nCntCol) - NUM_WIDTH;
			pVtx[0].pos.y = pRanking->pos.y - NUM_HEIGHT;
			pVtx[0].pos.z = 0.0f;

			pVtx[1].pos.x = pRanking->pos.x + ((NUM_WIDTH * 2) * nCntCol) + NUM_WIDTH;
			pVtx[1].pos.y = pRanking->pos.y - NUM_HEIGHT;
			pVtx[1].pos.z = 0.0f;

			pVtx[2].pos.x = pRanking->pos.x + ((NUM_WIDTH * 2) * nCntCol) - NUM_WIDTH;
			pVtx[2].pos.y = pRanking->pos.y + NUM_HEIGHT;
			pVtx[2].pos.z = 0.0f;

			pVtx[3].pos.x = pRanking->pos.x + ((NUM_WIDTH * 2) * nCntCol) + NUM_WIDTH;
			pVtx[3].pos.y = pRanking->pos.y + NUM_HEIGHT;
			pVtx[3].pos.z = 0.0f;

			// 頂点カラーの設定
			pVtx[0].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, pRanking->col.a);
			pVtx[1].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, pRanking->col.a);
			pVtx[2].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, pRanking->col.a);
			pVtx[3].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, pRanking->col.a);

			pVtx += 4;
		}

		g_pVtxBuffRanking->Unlock();
	}

	g_nCounterRanking++;
}

//================================================================================================================
// ランキングの描画処理
//================================================================================================================
void DrawRanking(void)
{
	LPDIRECT3DDEVICE9 pDevice;			// デバイスのポインタ
	int nCntRanking;

	// デバイスの取得
	pDevice = GetDevice();

	//頂点バッファをデータストリームに設定
	pDevice->SetStreamSource(0, g_pVtxBuffRanking, 0, sizeof(VERTEX_2D));

	// 頂点フォーマットの設定
	pDevice->SetFVF(FVF_VERTEX_2D);

	// テクスチャの設定
	pDevice->SetTexture(0, g_pTextureRanking);

	for (nCntRanking = 0; nCntRanking < (NUM_PLACE * MAX_RANKING); nCntRanking++)
	{
		// ポリゴンの描画
		pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP,		// プリミティブの種類
			4 * nCntRanking,							// 描画する最初の頂点インデックス
			2);											// 描画するプリミティブの数
	}
}

//================================================================================================================
// ランキングの表示処理
//================================================================================================================
void SetRanking(void)
{
	VERTEX_2D* pVtx;					// 頂点情報へのポインタ
	PRANKING pRanking = &g_aRanking[GetGameDifficulty()][0];
	int aTexU[MAX_RANKING][NUM_PLACE];				//各桁の数字を収納

	for (int nCntRanking = 0; nCntRanking < MAX_RANKING; nCntRanking++,pRanking++)
	{
		for (int nCntScore = 0; nCntScore < NUM_PLACE; nCntScore++)
		{
			aTexU[nCntRanking][nCntScore] = pRanking->nScore % (int)powf(10.0f, (float)(NUM_PLACE - nCntScore)) / (int)powf(10.0f, (float)(NUM_PLACE - nCntScore) - 1.0f);
		}
	}

	// 頂点バッファをロックし、頂点情報へのポインタを取得
	g_pVtxBuffRanking->Lock(0, 0, (void**)&pVtx, 0);

	pRanking = &g_aRanking[GetGameDifficulty()][0];

	for (int nCntRanking = 0; nCntRanking < MAX_RANKING; nCntRanking++, pRanking++)
	{
		for (int nCntScore = 0; nCntScore < NUM_PLACE; nCntScore++)
		{
			// テクスチャ座標の設定
			pVtx[0].tex = D3DXVECTOR2((0.1f * aTexU[nCntRanking][nCntScore]), 0.0f);
			pVtx[1].tex = D3DXVECTOR2((0.1f * aTexU[nCntRanking][nCntScore]) + 0.1f, 0.0f);
			pVtx[2].tex = D3DXVECTOR2((0.1f * aTexU[nCntRanking][nCntScore]), 1.0f);
			pVtx[3].tex = D3DXVECTOR2((0.1f * aTexU[nCntRanking][nCntScore]) + 0.1f, 1.0f);

			pVtx += 4;
		}
	}

	g_pVtxBuffRanking->Unlock();
}

//================================================================================================================
// ランキングの取得
//================================================================================================================
int GetRanking(void)
{
	return g_nScoreCurrent;
}

//================================================================================================================
// ランキングの追加処理
//================================================================================================================
void AddRanking(int nScore)
{
	PRANKING pRanking = &g_aRanking[GetGameDifficulty()][0];
	FILE* pFile;
	HWND hWnd = GetActiveWindow();
	int aScore[MAX_RANKING + 1] = {};
	int nCntRanking;

	g_nScoreCurrent = nScore;

	CheckRanking(pRanking);
	for (nCntRanking = 0; nCntRanking < MAX_RANKING; nCntRanking++, pRanking++)
	{
		aScore[nCntRanking] = pRanking->nScore;
	}

	aScore[nCntRanking] = g_nScoreCurrent;

	qsort(&aScore[0], MAX_RANKING + 1, sizeof(int), Compare);

	pRanking = &g_aRanking[GetGameDifficulty()][0];

	for (nCntRanking = 0; nCntRanking < MAX_RANKING; nCntRanking++, pRanking++)
	{
		pRanking->nRanking = nCntRanking;
		pRanking->nScore = aScore[nCntRanking];
	}

	pRanking = &g_aRanking[GetGameDifficulty()][0];

	for (nCntRanking = 0; nCntRanking < MAX_RANKING; nCntRanking++, pRanking++)
	{
		if (pRanking->nScore == g_nScoreCurrent)
		{
			pRanking->bAdd = true;
			break;
		}
	}

	pFile = fopen(&g_aRankingFileName[0], "wb");
	if (pFile == NULL) 
	{
		MessageBox(hWnd, "ランキングデータの書き出しに失敗！", "警告", MB_ICONWARNING);
		return;
	}

	fwrite(&aScore[0], sizeof(int), MAX_RANKING, pFile);

	fclose(pFile);
}

//================================================================================================================
// ランキングの読み込み処理
//================================================================================================================
void CheckRanking(PRANKING pRanking)
{
	FILE* pFile;
	HWND hWnd = NULL;
	int aBuffer[MAX_RANKING];		// スコアの一時保管場所

	pFile = fopen(&g_aRankingFileName[0], "rb");
	if (pFile == NULL) 
	{ 
		// ファイルの読み込みに失敗した場合、既定のランキングデータを代入
		for (int nCntRanking = 0; nCntRanking < MAX_RANKING; nCntRanking++, pRanking++)
		{
			pRanking->nRanking = nCntRanking;
			pRanking->nScore = 155000 * (MAX_RANKING - nCntRanking);
			if (pRanking->nScore == 0) pRanking->bUse = false;
		}

		return;
	}

	fread(aBuffer, sizeof(int), MAX_RANKING, pFile);
	fclose(pFile);

	// 取得したランキングデータを代入
	for (int nCntRanking = 0; nCntRanking < MAX_RANKING; nCntRanking++, pRanking++)
	{
		pRanking->nRanking = nCntRanking;
		pRanking->nScore = aBuffer[nCntRanking];
		if (pRanking->nScore == 0) pRanking->bUse = false;
	}
}

//================================================================================================================
// 比較関数
//================================================================================================================
int Compare(const void* a, const void* b)
{
	int ret;
	const int* p_int1 = (int *)a;
	const int* p_int2 = (int *)b;

	if (*p_int1 < *p_int2) {
		ret = 1;
	}
	else if (*p_int1 > *p_int2) {
		ret = -1;
	}
	else {
		ret = 0;
	}

	return ret;
}

//================================================================================================================
// ランキングのリセット処理
//================================================================================================================
void ResetRanking(void)
{
	HWND hWnd; 
	BOOL BSUCCEED;
	switch (GetGameDifficulty())
	{
	case GAMEDIFFICULTY_EASY:

		BSUCCEED = CopyFileA(FILENAME_RESET, FILENAME_RANKING_EASY, FALSE);		// リセット用のバイナリファイルで現在のスコアファイルを上書き

		break;

	case GAMEDIFFICULTY_NORMAL:

		BSUCCEED = CopyFileA(FILENAME_RESET, FILENAME_RANKING_EASY, FALSE);		// リセット用のバイナリファイルで現在のスコアファイルを上書き

		break;

	case GAMEDIFFICULTY_HARD:

		BSUCCEED = CopyFileA(FILENAME_RESET, FILENAME_RANKING_EASY, FALSE);		// リセット用のバイナリファイルで現在のスコアファイルを上書き

		break;

	default:

		BSUCCEED = FALSE;

		break;
	}

	GetHandleWindow(&hWnd);
#ifdef _DEBUG
	if (BSUCCEED == FALSE)
	{
		MessageBox(hWnd, "FAILED", "ERORR", MB_ICONERROR);
	}
#endif
}