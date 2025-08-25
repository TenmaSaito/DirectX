//================================================================================================================
//
// DirectXのステージ処理 [stage.cpp]
// Author : TENMA
//
//================================================================================================================
#include "main.h"
#include "stage.h"
#include "player.h"
#include "enemy.h"
#include "block.h"

// マクロ定義
#define ALREADY_CLEARED	(-1)			// ステージがクリアされた場合のターン
#define GRASS_TURN		(3)				// 草原ステージのターン数
#define SET_EXIT_LEFT(x)	(SetBlock(x, D3DXVECTOR3(STAGE_LEFT + BLOCK_WIDTH, STAGE_FLOOR * 0.4f, 0.0f)))		// ステージの左側の出口(x:BLOCKTYPE)
#define SET_EXIT_RIGHT(x)	(SetBlock(x, D3DXVECTOR3(STAGE_RIGHT - BLOCK_WIDTH, STAGE_FLOOR * 0.4f, 0.0f)))		// ステージの右側の出口(x:BLOCKTYPE)
#define SET_EXIT_UPPER(x)	(SetBlock(x, D3DXVECTOR3(STAGE_RIGHT * 0.435f - BLOCK_WIDTH, STAGE_CEILING, 0.0f)))	// ステージの上側の出口(x:BLOCKTYPE)
#define SET_EXIT_UNDER(x)	(SetBlock(x, D3DXVECTOR3(STAGE_RIGHT * 0.435f - BLOCK_WIDTH, STAGE_FLOOR, 0.0f)))	// ステージの下側の出口(x:BLOCKTYPE)

// グローバル変数
LPDIRECT3DTEXTURE9		g_apTextureStage[STAGE_MAX] = {};	// テクスチャへのポインタ
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffStage = NULL;				// 頂点バッファのポインタ
D3DXVECTOR3 g_posStage;										// ステージの現在位置
STAGE g_stageExac;											// 現在のステージ
int g_aTurn[STAGE_MAX];										// 各ステージの現在のターン
float g_fLengthStage;										// ステージの対角線の長さ
float g_fAngleStage;										// ステージの対角線の角度

//================================================================================================================
// ステージの初期化処理
//================================================================================================================
void InitStage(void)
{
	LPDIRECT3DDEVICE9 pDevice;			// デバイスのポインタ

	// デバイスの取得
	pDevice = GetDevice();

	for (int nCntStage = 0; nCntStage < STAGE_MAX; nCntStage++)
	{
		g_aTurn[nCntStage] = 0;
	}

	// テクスチャの読み込み
	D3DXCreateTextureFromFile(pDevice,
						    "data\\TEXTURE\\STAGE\\STAGE_GRASS.png",
							&g_apTextureStage[STAGE_GRASS]);

	D3DXCreateTextureFromFile(pDevice,
							"data\\TEXTURE\\STAGE\\STAGE_DESERT.png",
							&g_apTextureStage[STAGE_DESERT]);

	D3DXCreateTextureFromFile(pDevice,
							"data\\TEXTURE\\STAGE\\STAGE_ICE.png",
							&g_apTextureStage[STAGE_ICE]);

	D3DXCreateTextureFromFile(pDevice,
							"data\\TEXTURE\\STAGE\\STAGE_FOREST.png",
							&g_apTextureStage[STAGE_FOREST]);

	D3DXCreateTextureFromFile(pDevice,
							"data\\TEXTURE\\STAGE\\STAGE_VOLCANO.png",
							&g_apTextureStage[STAGE_VOLCANO]);

	D3DXCreateTextureFromFile(pDevice,
							"data\\TEXTURE\\STAGE\\STAGE_SEA.png",
							&g_apTextureStage[STAGE_SEA]);

	// 対角線の長さを取得
	g_fLengthStage = sqrtf(STAGE_WIDTH * STAGE_WIDTH + STAGE_HEIGHT * STAGE_HEIGHT) * 0.5f;

	// 対角線の角度を算出
	g_fAngleStage = atan2f(STAGE_WIDTH, STAGE_HEIGHT);

	g_posStage.x = SCREEN_WIDTH * 0.5f;
	g_posStage.y = SCREEN_HEIGHT * 0.5f;

	// 頂点バッファの生成
	pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * 4 * STAGE_MAX,			// sizeofの後必ず * 頂点数 を書くこと！
								D3DUSAGE_WRITEONLY,
								FVF_VERTEX_2D,
								D3DPOOL_MANAGED,
								&g_pVtxBuffStage,
								NULL);

	VERTEX_2D* pVtx;					// 頂点情報へのポインタ

	// 頂点バッファをロックし、頂点情報へのポインタを取得
	g_pVtxBuffStage->Lock(0, 0, (void**)&pVtx, 0);

	// 頂点座標の設定(座標設定は必ず右回りで！！！)
	pVtx[0].pos.x = g_posStage.x + sinf(D3DX_PI + g_fAngleStage) * g_fLengthStage;
	pVtx[0].pos.y = g_posStage.y + cosf(D3DX_PI + g_fAngleStage) * g_fLengthStage;
	pVtx[0].pos.z = 0.0f;

	pVtx[1].pos.x = g_posStage.x + sinf(D3DX_PI - g_fAngleStage) * g_fLengthStage;
	pVtx[1].pos.y = g_posStage.y + cosf(D3DX_PI - g_fAngleStage) * g_fLengthStage;
	pVtx[1].pos.z = 0.0f;

	pVtx[2].pos.x = g_posStage.x + sinf(-g_fAngleStage) * g_fLengthStage;
	pVtx[2].pos.y = g_posStage.y + cosf(-g_fAngleStage) * g_fLengthStage;
	pVtx[2].pos.z = 0.0f;

	pVtx[3].pos.x = g_posStage.x + sinf(g_fAngleStage) * g_fLengthStage;
	pVtx[3].pos.y = g_posStage.y + cosf(g_fAngleStage) * g_fLengthStage;
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
	g_pVtxBuffStage->Unlock();
}

//================================================================================================================
// ステージの終了処理
//================================================================================================================
void UninitStage(void)
{
	// テクスチャの破棄(必ず行うこと！！！)
	for (int nCntStage = 0; nCntStage < STAGE_MAX; nCntStage++)
	{
		if (g_apTextureStage[nCntStage] != NULL)
		{
			g_apTextureStage[nCntStage]->Release();
			g_apTextureStage[nCntStage] = NULL;
		}
	}

	// 頂点バッファの破棄(必ず行うこと！！！)
	if (g_pVtxBuffStage != NULL)
	{
		g_pVtxBuffStage->Release();
		g_pVtxBuffStage = NULL;
	}
}

//================================================================================================================
// ステージの更新処理
//================================================================================================================
void UpdateStage(void)
{
	//後で書く
	VERTEX_2D *pVtx;					// 頂点情報へのポインタ
	PLAYER *pPlayer = GetPlayer();		// プレイヤーへのポインタ

	D3DXVECTOR3 pos = pPlayer->moveposPlayer;		// プレイヤーの移動量を代入

	switch (g_stageExac)
	{
	case STAGE_GRASS:

		if (GetTotalEnemy() == 0)
		{
			if (g_aTurn[g_stageExac] != ALREADY_CLEARED)
			{
				// ステージのターンにより処理を変更
				switch (g_aTurn[g_stageExac])
				{
				case 0:
					// 最初のターン

					// 敵を配置
					SetEnemy(D3DXVECTOR3(300.0f, 200.0f, 0.0f), ENEMYTYPE_HOMING, ENEMYTEX_SLIME, 1.0f);
					SetEnemy(D3DXVECTOR3(500.0f, 200.0f, 0.0f), ENEMYTYPE_HOMING, ENEMYTEX_SLIME, 1.0f);
					SetEnemy(D3DXVECTOR3(700.0f, 200.0f, 0.0f), ENEMYTYPE_HOMING, ENEMYTEX_SLIME, 1.0f);
					SetEnemy(D3DXVECTOR3(900.0f, 200.0f, 0.0f), ENEMYTYPE_HOMING, ENEMYTEX_SLIME, 1.0f);
					SetEnemy(D3DXVECTOR3(600.0f, 500.0f, 0.0f), ENEMYTYPE_HOMING, ENEMYTEX_SLIME, 1.0f);
					SetEnemy(D3DXVECTOR3(100.0f, 700.0f, 0.0f), ENEMYTYPE_HOMING, ENEMYTEX_SLIME, 1.0f);
					SetEnemy(D3DXVECTOR3(1500.0f, 900.0f, 0.0f), ENEMYTYPE_HOMING, ENEMYTEX_SLIME, 1.0f);

					break;

				case 1:

					// 敵を配置
					SetEnemy(D3DXVECTOR3(STAGE_RIGHT * 0.25f, STAGE_FLOOR * 0.35f, 0.0f), ENEMYTYPE_FIX, ENEMYTEX_3, 0.0f);
					SetEnemy(D3DXVECTOR3(STAGE_RIGHT * 0.65f, STAGE_FLOOR * 0.35f, 0.0f), ENEMYTYPE_FIX, ENEMYTEX_3, 0.0f);
					SetEnemy(D3DXVECTOR3(STAGE_RIGHT * 0.25f, STAGE_FLOOR * 0.65f, 0.0f), ENEMYTYPE_FIX, ENEMYTEX_3, 0.0f);
					SetEnemy(D3DXVECTOR3(STAGE_RIGHT * 0.65f, STAGE_FLOOR * 0.65f, 0.0f), ENEMYTYPE_FIX, ENEMYTEX_3, 0.0f);

					break;

				case 2:

					// ボスを配置
					SetEnemy(D3DXVECTOR3(STAGE_RIGHT * 0.75f, STAGE_HEIGHT * 0.25f, 0.0f), ENEMYTYPE_BOSS, ENEMYTEX_LASTBOSS, 4.0f);

					break;

				case GRASS_TURN:

					g_aTurn[g_stageExac] = ALREADY_CLEARED;

					break;

				default:
					break;
				}

				if (g_aTurn[g_stageExac] != ALREADY_CLEARED)
				{
					g_aTurn[g_stageExac]++;
				}
			}
		}

		break;

	default:

		break;
	}

	// 頂点バッファをロックし、頂点情報へのポインタを取得
	g_pVtxBuffStage->Lock(0, 0, (void**)&pVtx, 0);

	// 頂点座標の設定(座標設定は必ず右回りで！！！)
	pVtx[0].pos.x = (g_posStage.x + pos.x) + sinf(D3DX_PI + g_fAngleStage) * g_fLengthStage;
	pVtx[0].pos.y = (g_posStage.y + pos.y) + cosf(D3DX_PI + g_fAngleStage) * g_fLengthStage;
	pVtx[0].pos.z = 0.0f;

	pVtx[1].pos.x = (g_posStage.x + pos.x) + sinf(D3DX_PI - g_fAngleStage) * g_fLengthStage;
	pVtx[1].pos.y = (g_posStage.y + pos.y) + cosf(D3DX_PI - g_fAngleStage) * g_fLengthStage;
	pVtx[1].pos.z = 0.0f;

	pVtx[2].pos.x = (g_posStage.x + pos.x) + sinf(-g_fAngleStage) * g_fLengthStage;
	pVtx[2].pos.y = (g_posStage.y + pos.y) + cosf(-g_fAngleStage) * g_fLengthStage;
	pVtx[2].pos.z = 0.0f;

	pVtx[3].pos.x = (g_posStage.x + pos.x) + sinf(g_fAngleStage) * g_fLengthStage;
	pVtx[3].pos.y = (g_posStage.y + pos.y) + cosf(g_fAngleStage) * g_fLengthStage;
	pVtx[3].pos.z = 0.0f;

	// 頂点バッファをアンロックする
	g_pVtxBuffStage->Unlock();
}

//================================================================================================================
// ステージの描画処理
//================================================================================================================
void DrawStage(void)
{
	LPDIRECT3DDEVICE9 pDevice;			// デバイスのポインタ

	// デバイスの取得
	pDevice = GetDevice();

	//頂点バッファをデータストリームに設定
	pDevice->SetStreamSource(0, g_pVtxBuffStage, 0, sizeof(VERTEX_2D));

	// 頂点フォーマットの設定
	pDevice->SetFVF(FVF_VERTEX_2D);

	// テクスチャの設定
	pDevice->SetTexture(0, g_apTextureStage[g_stageExac]);

	// ポリゴンの描画
	pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP,		// プリミティブの種類
					       0,						// 描画する最初の頂点インデックス
						   2);						// 描画するプリミティブの数
}

//================================================================================================================
// ステージの設定
//================================================================================================================
void SetStage(STAGE stage)
{
	PLAYER *pPlayer = GetPlayer();
	g_stageExac = stage;

	// 指定されたステージへ移行
	switch (g_stageExac)
	{
	case STAGE_GRASS:
		// 現在配置されているブロックをすべて消去
		ResetBlock();

		// 各出入口を配置
		SET_EXIT_LEFT(BLOCKTYPE_EXIT_A);
		SET_EXIT_UPPER(BLOCKTYPE_EXIT_B);
		SET_EXIT_RIGHT(BLOCKTYPE_EXIT_C);

		// もしクリア済みでなければ
		if (g_aTurn[g_stageExac] != ALREADY_CLEARED)
		{// ターンを初期化
			g_aTurn[g_stageExac] = 0;
		}

		break;

	case STAGE_DESERT:

		ResetBlock();

		SetEnemy(D3DXVECTOR3(300.0f, 200.0f, 0.0f), ENEMYTYPE_FIX, ENEMYTEX_1, 1.0f);
		SetEnemy(D3DXVECTOR3(500.0f, 200.0f, 0.0f), ENEMYTYPE_FIX, ENEMYTEX_1, 1.0f);
		SetEnemy(D3DXVECTOR3(700.0f, 200.0f, 0.0f), ENEMYTYPE_FIX, ENEMYTEX_1, 1.0f);

		break;

	case STAGE_ICE:

		ResetBlock();

		SetEnemy(D3DXVECTOR3(100.0f, 100.0f, 0.0f), ENEMYTYPE_MOVE, ENEMYTEX_2, 1.0f);

		break;

	case STAGE_FOREST:

		ResetBlock();

		SetEnemy(D3DXVECTOR3(100.0f, 100.0f, 0.0f), ENEMYTYPE_MOVE, ENEMYTEX_2, 1.0f);

		SetEnemy(D3DXVECTOR3(SCREEN_WIDTH * 0.5f, SCREEN_HEIGHT * 0.25f, 0.0f), ENEMYTYPE_BOSS, ENEMYTEX_LASTBOSS, 0.0f);

		break;

	case STAGE_VOLCANO:

		ResetBlock();

		SET_EXIT_RIGHT(BLOCKTYPE_EXIT_A);
		SET_EXIT_UPPER(BLOCKTYPE_EXIT_D);

		SetEnemy(D3DXVECTOR3(100.0f, 100.0f, 0.0f), ENEMYTYPE_MOVE, ENEMYTEX_2, 1.0f);

		break;

	case STAGE_SEA:

		ResetBlock();

		SetEnemy(D3DXVECTOR3(100.0f, 100.0f, 0.0f), ENEMYTYPE_MOVE, ENEMYTEX_2, 1.0f);

		break;

	default:

		break;
	}
}

// 現在のステージの取得
STAGE GetStage(void)
{
	return g_stageExac;
}

// 現在のターンを取得
int GetTurn(void)
{
	return g_aTurn[g_stageExac];
}