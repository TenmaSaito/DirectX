//================================================================================================================
//
// DirectXのチュートリアル処理 [tutorial.cpp]
// Author : TENMA
//
//================================================================================================================
#include "main.h"
#include "game.h"
#include "fade.h"
#include "stage.h"
#include "tutorial.h"
#include "block.h"
#include "player.h"
#include "placeChar.h"
#include "fade_stage.h"
#include "input.h"
#include "item.h"
#include "gauge.h"
#include "heart.h"
#include "sound.h"
#include "tutorial_gate.h"

// マクロ定義
#define MAX_TURN		(3)				// 草原ステージのターン数
#define TUTORIAL_TITLESTATE		(150)	// チュートリアルの各タイトルの表示する時間
#define TUTORIAL_TITLEWAIT		(250)	// チュートリアルの各タイトルの待機する時間
#define TUTORIAL_BRIEFSTATE		(10000000)	// チュートリアルの各説明の表示する時間(∞)
#define SUCCESS_STATE			(50)	// 成功判定の表示時間
#define SUCCESS_WAIT			(200)	// 成功判定の待機する時間
#define TUTORIAL_TITLEPOS		D3DXVECTOR3(640.0f, 540.0f, 0.0f)		// 各タイトルの位置
#define TUTORIAL_BRIEFPOS		D3DXVECTOR3(640.0f, 600.0f, 0.0f)		// 各タイトルの位置
#define GATE_FILENAME			"data\\STAGE\\TUTORIAL\\GATE.txt"		// ゲートのアドレス

// チュートリアルの列挙型定義
typedef enum
{
	TUTORIAL_NONE = 0,
	TUTORIAL_START,
	TUTORIAL_STEP_MOVE,
	TUTORIAL_STEP_SHOT,
	TUTORIAL_STEP_CHARGE,
	TUTORIAL_STEP_SP,
	TUTORIAL_STEP_BARRIAR,
	TUTORIAL_STEP_ENEMY,
	TUTORIAL_STEP_ITEM,
	TUTORIAL_STEP_COMPLETE,
	TUTORIAL_STEP_MAX
}TUTORIAL;

// プロトタイプ宣言
void UniteFileName(const char* pFileName, const char* pFiletype, char* pResult);
void NextTutorial(TUTORIAL current);

// グローバル変数
LPDIRECT3DTEXTURE9		g_pTextureTutorial = NULL;	// テクスチャへのポインタ
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffTutorial = NULL;				// 頂点バッファのポインタ
D3DXVECTOR3 g_posTutorial;										// ステージの現在位置
TUTORIAL g_TutorialExac;										// 現在のチュートリアル段階
CHARTEX g_CharTexExac;											// 表示中のchartex
float g_fLengthTutorial;										// ステージの対角線の長さ
float g_fAngleTutorial;											// ステージの対角線の角度
int g_nCounterTutorial;											// 汎用カウンター
int g_nCounterTutorialState;									// チュートリアルの状態カウンター

//================================================================================================================
// ステージの初期化処理
//================================================================================================================
void InitTutorial(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();			// デバイスのポインタ
	
	// テクスチャの読み込み
	D3DXCreateTextureFromFile(pDevice,
						    "data\\TEXTURE\\STAGE\\TUTORIAL_STAGE.png",
							&g_pTextureTutorial);

	g_posTutorial.x = SCREEN_WIDTH * 0.5f;
	g_posTutorial.y = SCREEN_HEIGHT * 0.5f;

	g_TutorialExac = TUTORIAL_START;
	g_nCounterTutorial = 0;
	g_nCounterTutorialState = 0;

	// 対角線の長さを取得
	g_fLengthTutorial = sqrtf(STAGE_WIDTH * STAGE_WIDTH + STAGE_HEIGHT * STAGE_HEIGHT) * 0.5f;

	// 対角線の角度を算出
	g_fAngleTutorial = atan2f(STAGE_WIDTH, STAGE_HEIGHT);

	g_posTutorial.x = SCREEN_WIDTH * 0.5f;
	g_posTutorial.y = SCREEN_HEIGHT * 0.5f;

	// 頂点バッファの生成
	pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * 4,			// sizeofの後必ず * 頂点数 を書くこと！
								D3DUSAGE_WRITEONLY,
								FVF_VERTEX_2D,
								D3DPOOL_MANAGED,
								&g_pVtxBuffTutorial,
								NULL);

	VERTEX_2D* pVtx;					// 頂点情報へのポインタ

	// 頂点バッファをロックし、頂点情報へのポインタを取得
	g_pVtxBuffTutorial->Lock(0, 0, (void**)&pVtx, 0);

	// 頂点座標の設定(座標設定は必ず右回りで！！！)
	pVtx[0].pos.x = g_posTutorial.x + sinf(D3DX_PI + g_fAngleTutorial) * g_fLengthTutorial;
	pVtx[0].pos.y = g_posTutorial.y + cosf(D3DX_PI + g_fAngleTutorial) * g_fLengthTutorial;
	pVtx[0].pos.z = 0.0f;
	
	pVtx[1].pos.x = g_posTutorial.x + sinf(D3DX_PI - g_fAngleTutorial) * g_fLengthTutorial;
	pVtx[1].pos.y = g_posTutorial.y + cosf(D3DX_PI - g_fAngleTutorial) * g_fLengthTutorial;
	pVtx[1].pos.z = 0.0f;
	
	pVtx[2].pos.x = g_posTutorial.x + sinf(-g_fAngleTutorial) * g_fLengthTutorial;
	pVtx[2].pos.y = g_posTutorial.y + cosf(-g_fAngleTutorial) * g_fLengthTutorial;
	pVtx[2].pos.z = 0.0f;
	
	pVtx[3].pos.x = g_posTutorial.x + sinf(g_fAngleTutorial) * g_fLengthTutorial;
	pVtx[3].pos.y = g_posTutorial.y + cosf(g_fAngleTutorial) * g_fLengthTutorial;
	pVtx[3].pos.z = 0.0f;
	
	// rhwの設定
	pVtx[0].rhw = 1.0f;
	pVtx[1].rhw = 1.0f;
	pVtx[2].rhw = 1.0f;
	pVtx[3].rhw = 1.0f;
	
	// 頂点カラー設定
	pVtx[0].col = D3DCOLOR_RGBA(255, 255, 255, 255);
	pVtx[1].col = D3DCOLOR_RGBA(255, 255, 255, 255);
	pVtx[2].col = D3DCOLOR_RGBA(255, 255, 255, 255);
	pVtx[3].col = D3DCOLOR_RGBA(255, 255, 255, 255);
	
	// テクスチャ座標の設定
	pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
	pVtx[1].tex = D3DXVECTOR2(1.0f, 0.0f);
	pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
	pVtx[3].tex = D3DXVECTOR2(1.0f, 1.0f);
	
	// 頂点バッファをアンロックする
	g_pVtxBuffTutorial->Unlock();
}

//================================================================================================================
// ステージの終了処理
//================================================================================================================
void UninitTutorial(void)
{
	// テクスチャの破棄(必ず行うこと！！！)

	if (g_pTextureTutorial != NULL)
	{
		g_pTextureTutorial->Release();
		g_pTextureTutorial = NULL;
	}

	// 頂点バッファの破棄(必ず行うこと！！！)
	if (g_pVtxBuffTutorial != NULL)
	{
		g_pVtxBuffTutorial->Release();
		g_pVtxBuffTutorial = NULL;
	}
}

//================================================================================================================
// ステージの更新処理
//================================================================================================================
void UpdateTutorial(void)
{
	VERTEX_2D* pVtx;					// 頂点情報へのポインタ
	PLAYER* pPlayer = GetPlayer();
	Gauge* pGauge = GetGauge();
	static int nCnt;
	char aStr[STRING_MAX] = {};

	D3DXVECTOR3 pos = pPlayer->moveposPlayer;		// プレイヤーの移動量を代入

	if ((GetKeyboardTrigger(DIK_M) == true
		|| GetJoypadTrigger(JOYKEY_BACK) == true)
		&& g_TutorialExac != TUTORIAL_STEP_COMPLETE)
	{
		DeleteChar(g_CharTexExac);
		NextTutorial(TUTORIAL_STEP_ITEM);
	}

	switch (g_TutorialExac)
	{
	case TUTORIAL_START:

		if (g_nCounterTutorialState == TUTORIAL_TITLEWAIT)
		{
			SetPlaceChar(TUTORIAL_TITLEPOS, POLY_SIZE(1000.0f, 200.0f), CHARTEX_TUTORIAL_START, 100);
			g_CharTexExac = CHARTEX_TUTORIAL_START;
			g_nCounterTutorialState = 200;
		}
		else if (g_nCounterTutorialState == 1)
		{
			SetPlaceChar(TUTORIAL_TITLEPOS, POLY_SIZE(1000.0f, 200.0f), CHARTEX_TUTORIAL_START_BRIEF, 100);
			g_CharTexExac = CHARTEX_TUTORIAL_START_BRIEF;
		}

		if (g_nCounterTutorialState > 0)
		{
			g_nCounterTutorialState--;
			if (g_nCounterTutorialState <= 0)
			{
				g_nCounterTutorialState = 0;
			}
		}
		else if (g_nCounterTutorialState == 0 && g_nCounterTutorial == 0)
		{
			g_nCounterTutorial = 250;
		}

		if (g_nCounterTutorial > 0)
		{
			g_nCounterTutorial--;
			if (g_nCounterTutorial <= 0)
			{
				NextTutorial(g_TutorialExac);
			}
		}

		break;

	case TUTORIAL_STEP_MOVE:
		if (g_nCounterTutorialState == TUTORIAL_TITLEWAIT)
		{
			SetPlaceChar(TUTORIAL_TITLEPOS, POLY_SIZE(1000.0f, 200.0f), CHARTEX_STEP1, TUTORIAL_TITLESTATE);
			g_CharTexExac = CHARTEX_STEP1;
			g_nCounterTutorialState = TUTORIAL_TITLEWAIT;
		}
		else if (g_nCounterTutorialState == 1)
		{
			SetPlaceChar(TUTORIAL_BRIEFPOS, POLY_SIZE(1000.0f, 200.0f), CHARTEX_STEP1_BRIEF, TUTORIAL_BRIEFSTATE);
			g_CharTexExac = CHARTEX_STEP1_BRIEF;
			SetPlayerDo(PLAYERDO_MOVE, true);
		}

		if (g_nCounterTutorialState > 0)
		{
			g_nCounterTutorialState--;
			if (g_nCounterTutorialState <= 0)
			{
				g_nCounterTutorialState = 0;
			}
		}
		else if (g_nCounterTutorialState == 0 && g_nCounterTutorial == 0)
		{
			if ((pPlayer->posPlayer.x <= (PLAYER_SPOWN_X - 150.0f)
				|| pPlayer->posPlayer.x >= (PLAYER_SPOWN_X + 150.0f))
				&& pPlayer->posPlayer.y <= (PLAYER_SPOWN_Y - 150.0f)
				|| pPlayer->posPlayer.y >= (PLAYER_SPOWN_Y + 150.0f))
			{
				DeleteChar(CHARTEX_STEP1_BRIEF);
				SetPlaceChar(TUTORIAL_BRIEFPOS, POLY_SIZE(1200.0f, 200.0f), CHARTEX_SUCCESS, SUCCESS_STATE);
				g_CharTexExac = CHARTEX_SUCCESS;
				g_nCounterTutorial = SUCCESS_WAIT;
			}
		}

		if (g_nCounterTutorial > 0)
		{
			g_nCounterTutorial--;
			if (g_nCounterTutorial <= 0)
			{
				NextTutorial(g_TutorialExac);
			}
		}

		break;

	case TUTORIAL_STEP_SHOT:
		if (g_nCounterTutorialState == TUTORIAL_TITLEWAIT)
		{
			SetPlaceChar(TUTORIAL_TITLEPOS, POLY_SIZE(1000.0f, 200.0f), CHARTEX_STEP2, TUTORIAL_TITLESTATE);
			g_CharTexExac = CHARTEX_STEP2;
			g_nCounterTutorialState = TUTORIAL_TITLEWAIT;
		}
		else if (g_nCounterTutorialState == 1)
		{
			SetPlaceChar(TUTORIAL_BRIEFPOS, POLY_SIZE(1200.0f, 200.0f), CHARTEX_STEP2_BRIEF, TUTORIAL_BRIEFSTATE);
			g_CharTexExac = CHARTEX_STEP2_BRIEF;
			SetPlayerDo(PLAYERDO_SHOT, true);
		}

		if (g_nCounterTutorialState > 0)
		{
			g_nCounterTutorialState--;
			if (g_nCounterTutorialState <= 0)
			{
				g_nCounterTutorialState = 0;
			}
		}
		else if (g_nCounterTutorialState == 0 && g_nCounterTutorial == 0)
		{
			if ((GetKeyboardTrigger(DIK_SPACE) == true || GetJoypadTrigger(JOYKEY_A) == true))
			{
				nCnt++;
				if (nCnt >= 10)
				{
					DeleteChar(CHARTEX_STEP2_BRIEF);
					SetPlaceChar(TUTORIAL_BRIEFPOS, POLY_SIZE(1200.0f, 200.0f), CHARTEX_SUCCESS, SUCCESS_STATE);
					g_CharTexExac = CHARTEX_SUCCESS;
					g_nCounterTutorial = SUCCESS_WAIT;
				}
			}
		}

		if (g_nCounterTutorial > 0)
		{
			g_nCounterTutorial--;
			if (g_nCounterTutorial <= 0)
			{
				NextTutorial(g_TutorialExac);
			}
		}

		break;


	case TUTORIAL_STEP_CHARGE:
		if (g_nCounterTutorialState == TUTORIAL_TITLEWAIT)
		{
			SetPlaceChar(TUTORIAL_TITLEPOS, POLY_SIZE(1000.0f, 200.0f), CHARTEX_STEP3, TUTORIAL_TITLESTATE);
			g_CharTexExac = CHARTEX_STEP3;
			g_nCounterTutorialState = TUTORIAL_TITLEWAIT;
		}
		else if (g_nCounterTutorialState == 1)
		{
			SetPlaceChar(TUTORIAL_BRIEFPOS, POLY_SIZE(1000.0f, 200.0f), CHARTEX_STEP3_BRIEF, TUTORIAL_BRIEFSTATE);
			g_CharTexExac = CHARTEX_STEP3_BRIEF;
			SetPlayerDo(PLAYERDO_CHARGE, true);
		}

		if (g_nCounterTutorialState > 0)
		{
			g_nCounterTutorialState--;
			if (g_nCounterTutorialState <= 0)
			{
				g_nCounterTutorialState = 0;
			}
		}
		else if (g_nCounterTutorialState == 0 && g_nCounterTutorial == 0)
		{
			if (pPlayer->bCharge == true)
			{
				DeleteChar(CHARTEX_STEP3_BRIEF);
				SetPlaceChar(TUTORIAL_BRIEFPOS, POLY_SIZE(1200.0f, 200.0f), CHARTEX_SUCCESS, SUCCESS_STATE);
				g_CharTexExac = CHARTEX_SUCCESS;
				g_nCounterTutorial = SUCCESS_WAIT;
			}
		}

		if (g_nCounterTutorial > 0)
		{
			g_nCounterTutorial--;
			if (g_nCounterTutorial <= 0)
			{
				NextTutorial(g_TutorialExac);
			}
		}

		break;

	case TUTORIAL_STEP_SP:
		if (g_nCounterTutorialState == TUTORIAL_TITLEWAIT)
		{
			SetPlaceChar(TUTORIAL_TITLEPOS, POLY_SIZE(1000.0f, 200.0f), CHARTEX_STEP4, TUTORIAL_TITLESTATE);
			g_CharTexExac = CHARTEX_STEP4;
			g_nCounterTutorialState = TUTORIAL_TITLEWAIT;
		}
		else if (g_nCounterTutorialState == 1)
		{
			SetPlaceChar(TUTORIAL_BRIEFPOS, POLY_SIZE(1000.0f, 200.0f), CHARTEX_STEP4_BRIEF, TUTORIAL_BRIEFSTATE);
			g_CharTexExac = CHARTEX_STEP4_BRIEF;
			SetPlayerDo(PLAYERDO_SP, true);
		}

		if (g_nCounterTutorialState > 0)
		{
			g_nCounterTutorialState--;
			if (g_nCounterTutorialState <= 0)
			{
				g_nCounterTutorialState = 0;
			}
		}
		else if (g_nCounterTutorialState == 0 && g_nCounterTutorial == 0)
		{
			if (pPlayer->bCharge == true && (GetKeyboardRelease(DIK_SPACE) == true || GetJoypadRelease(JOYKEY_A) == true))
			{
				DeleteChar(CHARTEX_STEP4_BRIEF);
				SetPlaceChar(TUTORIAL_BRIEFPOS, POLY_SIZE(1200.0f, 200.0f), CHARTEX_SUCCESS, SUCCESS_STATE);
				g_CharTexExac = CHARTEX_SUCCESS;
				g_nCounterTutorial = SUCCESS_WAIT;
			}
		}

		if (g_nCounterTutorial > 0)
		{
			g_nCounterTutorial--;
			if (g_nCounterTutorial <= 0)
			{
				NextTutorial(g_TutorialExac);
			}
		}

		break;

	case TUTORIAL_STEP_BARRIAR:
		if (g_nCounterTutorialState == TUTORIAL_TITLEWAIT)
		{
			SetPlaceChar(TUTORIAL_TITLEPOS, POLY_SIZE(1000.0f, 200.0f), CHARTEX_STEP5, TUTORIAL_TITLESTATE);
			g_CharTexExac = CHARTEX_STEP5;
			g_nCounterTutorialState = TUTORIAL_TITLEWAIT;
		}
		else if (g_nCounterTutorialState == 1)
		{
			SetPlaceChar(TUTORIAL_BRIEFPOS, POLY_SIZE(1000.0f, 200.0f), CHARTEX_STEP5_BRIEF, TUTORIAL_BRIEFSTATE);
			g_CharTexExac = CHARTEX_STEP5_BRIEF;
			SetPlayerDo(PLAYERDO_BARRIAR, true);
		}

		if (g_nCounterTutorialState > 0)
		{
			g_nCounterTutorialState--;
			if (g_nCounterTutorialState <= 0)
			{
				g_nCounterTutorialState = 0;
			}
		}
		else if (g_nCounterTutorialState == 0 && g_nCounterTutorial == 0)
		{
			if (pPlayer->state == PLAYERSTATE_BARRIER)
			{
				DeleteChar(CHARTEX_STEP5_BRIEF);
				SetPlaceChar(TUTORIAL_BRIEFPOS, POLY_SIZE(1200.0f, 200.0f), CHARTEX_SUCCESS, SUCCESS_STATE);
				g_CharTexExac = CHARTEX_SUCCESS;
				g_nCounterTutorial = SUCCESS_WAIT;
			}
		}

		if (g_nCounterTutorial > 0)
		{
			g_nCounterTutorial--;
			if (g_nCounterTutorial <= 0)
			{
				NextTutorial(g_TutorialExac);
			}
		}

		break;

	case TUTORIAL_STEP_ENEMY:
		if (g_nCounterTutorialState == TUTORIAL_TITLEWAIT)
		{
			SetPlaceChar(TUTORIAL_TITLEPOS, POLY_SIZE(1000.0f, 200.0f), CHARTEX_STEP6, TUTORIAL_TITLESTATE);
			g_CharTexExac = CHARTEX_STEP6;
			g_nCounterTutorialState = TUTORIAL_TITLEWAIT;
		}
		else if (g_nCounterTutorialState == 1)
		{
			SetPlaceChar(TUTORIAL_BRIEFPOS, POLY_SIZE(1000.0f, 200.0f), CHARTEX_STEP6_BRIEF, TUTORIAL_BRIEFSTATE);
			g_CharTexExac = CHARTEX_STEP6_BRIEF;
			// 敵を配置
			UniteFileName(TUTORIAL_FILENAME, ENEMY_FILETYPE_NORMAL, &aStr[0]);
			LoadEnemy(&aStr[0], 0);
			pPlayer->state = PLAYERSTATE_BARRIER;
			pPlayer->nCounterBarrierTime = 1000000;
		}

		if (g_nCounterTutorialState > 0)
		{
			g_nCounterTutorialState--;
			if (g_nCounterTutorialState <= 0)
			{
				g_nCounterTutorialState = 0;
			}
		}
		else if (g_nCounterTutorialState == 0 && g_nCounterTutorial == 0)
		{
			if (GetTotalEnemy() == 0)
			{
				DeleteChar(CHARTEX_STEP6_BRIEF);
				SetPlaceChar(TUTORIAL_BRIEFPOS, POLY_SIZE(1200.0f, 200.0f), CHARTEX_SUCCESS, SUCCESS_STATE);
				g_CharTexExac = CHARTEX_SUCCESS;
				g_nCounterTutorial = SUCCESS_WAIT;
			}
		}

		if (g_nCounterTutorial > 0)
		{
			g_nCounterTutorial--;
			if (g_nCounterTutorial <= 0)
			{
				NextTutorial(g_TutorialExac);
			}
		}

		break;

	case TUTORIAL_STEP_ITEM:
		if (g_nCounterTutorialState == TUTORIAL_TITLEWAIT)
		{
			SetPlaceChar(TUTORIAL_TITLEPOS, POLY_SIZE(1000.0f, 200.0f), CHARTEX_STEP7, TUTORIAL_TITLESTATE);
			g_CharTexExac = CHARTEX_STEP7;
			g_nCounterTutorialState = TUTORIAL_TITLEWAIT;
		}
		else if (g_nCounterTutorialState == 1)
		{
			SetPlaceChar(TUTORIAL_BRIEFPOS, POLY_SIZE(1000.0f, 200.0f), CHARTEX_STEP7_BRIEF, TUTORIAL_BRIEFSTATE);
			g_CharTexExac = CHARTEX_STEP7_BRIEF;
			// アイテムを配置
			UniteFileName(TUTORIAL_FILENAME, BLOCK_FILETYPE, &aStr[0]);
			SetItem(ITEMTYPE_HEAL, D3DXVECTOR3(640.0f, 360.0f, 0.0f));
			pPlayer->nCounterBarrierTime = 0;
			pPlayer->state = PLAYERSTATE_NORMAL;
			AddGauge(-pGauge[pPlayer->nBarrierNo].nPercentGauge, pPlayer->nBarrierNo);
			HitPlayer(1);
			nCnt = pPlayer->nLife;
		}
		
		if (g_nCounterTutorialState > 0)
		{
			g_nCounterTutorialState--;
			if (g_nCounterTutorialState <= 0)
			{
				g_nCounterTutorialState = 0;
			}
		}
		else if (g_nCounterTutorialState == 0 && g_nCounterTutorial == 0)
		{
			if (pPlayer->nLife > nCnt)
			{
				DeleteChar(CHARTEX_STEP7_BRIEF);
				SetPlaceChar(TUTORIAL_BRIEFPOS, POLY_SIZE(1200.0f, 200.0f), CHARTEX_SUCCESS, SUCCESS_STATE);
				g_CharTexExac = CHARTEX_SUCCESS;
				g_nCounterTutorial = SUCCESS_WAIT;
			}
		}

		if (g_nCounterTutorial > 0)
		{
			g_nCounterTutorial--;
			if (g_nCounterTutorial <= 0)
			{
				NextTutorial(g_TutorialExac);
			}
		}

		break;

	case TUTORIAL_STEP_COMPLETE:

		if (g_nCounterTutorialState == TUTORIAL_TITLEWAIT)
		{
			SetPlaceChar(TUTORIAL_TITLEPOS, POLY_SIZE(1000.0f, 200.0f), CHARTEX_TUTORIAL_COMPLETE, 20000);
			g_CharTexExac = CHARTEX_TUTORIAL_COMPLETE;
			g_nCounterTutorialState = TUTORIAL_TITLEWAIT;
		}
		else if (g_nCounterTutorialState == 1)
		{
			if (pPlayer->nLife > nCnt)
			{
				DeleteChar(CHARTEX_TUTORIAL_COMPLETE);
				pPlayer->nCounterState = 0;
				pPlayer->state = PLAYERSTATE_NORMAL;
				pPlayer->nLife = MAX_LIFE;
				SetHeart();
			}

			SetGameDifficulty(GAMEDIFFICULTY_NORMAL);

			char aStr[STRING_MAX] = GATE_FILENAME;
			LoadTutorialGate(&aStr[0]);
		}

		if (g_nCounterTutorialState > 0)
		{
			g_nCounterTutorialState--;
			if (g_nCounterTutorialState <= 0)
			{
				g_nCounterTutorialState = 0;
			}
		}

		break;
	}

	// 頂点バッファをロックし、頂点情報へのポインタを取得
	g_pVtxBuffTutorial->Lock(0, 0, (void**)&pVtx, 0);

	// 頂点座標の設定(座標設定は必ず右回りで！！！)
	pVtx[0].pos.x = (g_posTutorial.x + pos.x) + sinf(D3DX_PI + g_fAngleTutorial) * g_fLengthTutorial;
	pVtx[0].pos.y = (g_posTutorial.y + pos.y) + cosf(D3DX_PI + g_fAngleTutorial) * g_fLengthTutorial;
	pVtx[0].pos.z = 0.0f;

	pVtx[1].pos.x = (g_posTutorial.x + pos.x) + sinf(D3DX_PI - g_fAngleTutorial) * g_fLengthTutorial;
	pVtx[1].pos.y = (g_posTutorial.y + pos.y) + cosf(D3DX_PI - g_fAngleTutorial) * g_fLengthTutorial;
	pVtx[1].pos.z = 0.0f;

	pVtx[2].pos.x = (g_posTutorial.x + pos.x) + sinf(-g_fAngleTutorial) * g_fLengthTutorial;
	pVtx[2].pos.y = (g_posTutorial.y + pos.y) + cosf(-g_fAngleTutorial) * g_fLengthTutorial;
	pVtx[2].pos.z = 0.0f;

	pVtx[3].pos.x = (g_posTutorial.x + pos.x) + sinf(g_fAngleTutorial) * g_fLengthTutorial;
	pVtx[3].pos.y = (g_posTutorial.y + pos.y) + cosf(g_fAngleTutorial) * g_fLengthTutorial;
	pVtx[3].pos.z = 0.0f;

	// 頂点バッファをアンロックする
	g_pVtxBuffTutorial->Unlock();
}

//================================================================================================================
// ステージの描画処理
//================================================================================================================
void DrawTutorial(void)
{
	LPDIRECT3DDEVICE9 pDevice;			// デバイスのポインタ

	// デバイスの取得
	pDevice = GetDevice();

	//頂点バッファをデータストリームに設定
	pDevice->SetStreamSource(0, g_pVtxBuffTutorial, 0, sizeof(VERTEX_2D));

	// 頂点フォーマットの設定
	pDevice->SetFVF(FVF_VERTEX_2D);

	// テクスチャの設定
	pDevice->SetTexture(0, g_pTextureTutorial);

	// ポリゴンの描画
	pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP,		// プリミティブの種類
					       0,						// 描画する最初の頂点インデックス
						   2);						// 描画するプリミティブの数
}

//================================================================================================================
// ステージの設定
//================================================================================================================
void SetTutorial(void)
{
	char aFileName[STRING_MAX] = {};
	PLAYER* pPlayer = GetPlayer();

	// 現在配置されているブロックをすべて消去
	ResetBlock();

	UniteFileName(FRAME_FILENAME, FRAME_LEFT_FILETYPE, &aFileName[0]);
	LoadBlock(&aFileName[0]);

	UniteFileName(FRAME_FILENAME, FRAME_RIGHT_FILETYPE, &aFileName[0]);
	LoadBlock(&aFileName[0]);

	UniteFileName(FRAME_FILENAME, FRAME_UP_FILETYPE, &aFileName[0]);
	LoadBlock(&aFileName[0]);

	UniteFileName(FRAME_FILENAME, FRAME_DOWN_FILETYPE, &aFileName[0]);
	LoadBlock(&aFileName[0]);

	// チュートリアルをリセット
	g_TutorialExac = TUTORIAL_START;
	g_nCounterTutorialState = TUTORIAL_TITLEWAIT;

	for (int nCnt = 0; nCnt < 5; nCnt++)
	{
		pPlayer->aCouldDo[nCnt] = true;
	}
}

// ファイルアドレスの合体
void UniteFileName(const char* pFileName, const char* pFiletype, char *pResult)
{
	char aStr[STRING_MAX] = {};

	strcat(&aStr[0], pFileName);

	strcat(&aStr[0], pFiletype);

	strcpy(pResult, &aStr[0]);
}

// チュートリアルの進行及び待機時間の初期化
void NextTutorial(TUTORIAL current)
{
	int n = current;

	n++;
	if (n > TUTORIAL_STEP_MAX)
	{
		n = TUTORIAL_STEP_MAX;
	}

	g_TutorialExac = (TUTORIAL)n;
	g_nCounterTutorialState = TUTORIAL_TITLEWAIT;
	g_nCounterTutorial = 0;
}