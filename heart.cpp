//================================================================================================================
//
// DirectXのハート設置のcppファイル [heart.cpp]
// Author : TENMA
//
//================================================================================================================
#include "heart.h"
#include "player.h"
#include "playerframe.h"

// マクロ定義
#define HEART_WIDTH		(30.0f)					// ハートの横幅
#define HEART_HEIGHT	(30.0f)					// ハートの縦幅

// グローバル変数
LPDIRECT3DTEXTURE9		g_pTextureHeart = NULL;	// テクスチャへのポインタ
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffHeart = NULL;	// 頂点バッファのポインタ
D3DXVECTOR3 g_posHeart;							// ライフを表示する位置
int g_nLife;									// ライフの値

//================================================================================================================
// ハートの初期化処理
//================================================================================================================
void InitHeart(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();			// デバイスのポインタ

	// テクスチャの読み込み
	D3DXCreateTextureFromFile(pDevice,
							"data\\TEXTURE\\CHARACTER\\BLOCK_HEART.png",
							&g_pTextureHeart);

	// 頂点バッファの生成
	pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * 4 * MAX_LIFE,			// sizeofの後必ず * 頂点数 を書くこと！
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_2D,
		D3DPOOL_MANAGED,
		&g_pVtxBuffHeart,
		NULL);

	g_posHeart = D3DXVECTOR3(50, 50, 0);			// 位置を初期化
	g_nLife = 0;									// 値を初期化

	AddFunctionLog("END : VertexBuffer Create");

	VERTEX_2D* pVtx;					// 頂点情報へのポインタ

	// 頂点バッファをロックし、頂点情報へのポインタを取得
	g_pVtxBuffHeart->Lock(0, 0, (void**)&pVtx, 0);

	AddFunctionLog("START : VertexBuffer Setting");

	for (int nCntHeart = 0; nCntHeart < MAX_LIFE; nCntHeart++)
	{
		// 頂点座標の設定(座標設定は必ず右回りで！！！)
		pVtx[0].pos.x = g_posHeart.x + ((HEART_WIDTH * 1.5f) * nCntHeart) - HEART_WIDTH;
		pVtx[0].pos.y = g_posHeart.y - HEART_HEIGHT;
		pVtx[0].pos.z = 0.0f;

		pVtx[1].pos.x = g_posHeart.x + ((HEART_WIDTH * 1.5f) * nCntHeart) + HEART_WIDTH;
		pVtx[1].pos.y = g_posHeart.y - HEART_HEIGHT;
		pVtx[1].pos.z = 0.0f;

		pVtx[2].pos.x = g_posHeart.x + ((HEART_WIDTH * 1.5f) * nCntHeart) - HEART_WIDTH;
		pVtx[2].pos.y = g_posHeart.y + HEART_HEIGHT;
		pVtx[2].pos.z = 0.0f;

		pVtx[3].pos.x = g_posHeart.x + ((HEART_WIDTH * 1.5f) * nCntHeart) + HEART_WIDTH;
		pVtx[3].pos.y = g_posHeart.y + HEART_HEIGHT;
		pVtx[3].pos.z = 0.0f;

		// rhwの設定
		pVtx[0].rhw = 1.0f;
		pVtx[1].rhw = 1.0f;
		pVtx[2].rhw = 1.0f;
		pVtx[3].rhw = 1.0f;

		// 頂点カラーの設定
		pVtx[0].col = D3DXCOLOR(0.0f, 0.0f, 0.0f, 1.0f);
		pVtx[1].col = D3DXCOLOR(0.0f, 0.0f, 0.0f, 1.0f);
		pVtx[2].col = D3DXCOLOR(0.0f, 0.0f, 0.0f, 1.0f);
		pVtx[3].col = D3DXCOLOR(0.0f, 0.0f, 0.0f, 1.0f);

		// テクスチャ座標の設定
		pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
		pVtx[1].tex = D3DXVECTOR2(1.0f, 0.0f);
		pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
		pVtx[3].tex = D3DXVECTOR2(1.0f, 1.0f);

		pVtx += 4;
	}

	// 頂点バッファをアンロックする
	g_pVtxBuffHeart->Unlock();

	AddFunctionLog("END : VertexBuffer Create");
}

//================================================================================================================
// ハートの終了処理
//================================================================================================================
void UninitHeart(void)
{
	// テクスチャの破棄(必ず行うこと！！！)

	if (g_pTextureHeart != NULL)
	{
		g_pTextureHeart->Release();
		g_pTextureHeart = NULL;
	}
	

	// 頂点バッファの破棄(必ず行うこと！！！)
	if (g_pVtxBuffHeart != NULL)
	{
		g_pVtxBuffHeart->Release();
		g_pVtxBuffHeart = NULL;
	}
}

//================================================================================================================
// ハートの更新処理
//================================================================================================================
void UpdateHeart(void)
{
	
}

//================================================================================================================
// ハートの描画処理
//================================================================================================================
void DrawHeart(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();			// デバイスのポインタ

	//頂点バッファをデータストリームに設定
	pDevice->SetStreamSource(0, g_pVtxBuffHeart, 0, sizeof(VERTEX_2D));

	// 頂点フォーマットの設定
	pDevice->SetFVF(FVF_VERTEX_2D);

	// テクスチャの設定(使わないならNULLを入れる！！！！)
	pDevice->SetTexture(0, g_pTextureHeart);

	for (int nCntBlock = 0; nCntBlock < MAX_LIFE; nCntBlock++)
	{

		// ポリゴンの描画
		pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP,		// プリミティブの種類
			nCntBlock * 4,								// 描画する最初の頂点インデックス
			2);											// 描画するプリミティブの数
	}
}

//================================================================================================================
// ハートの設置
//================================================================================================================
void SetHeart(void)
{
	VERTEX_2D* pVtx;

	AddFunctionLog("START : Block Set");

	// 頂点バッファをロックし、頂点情報へのポインタを取得
	g_pVtxBuffHeart->Lock(0, 0, (void**)&pVtx, 0);

	for (int nCntHeart = 0; nCntHeart < MAX_LIFE; nCntHeart++)
	{
		// 頂点カラーの設定
		pVtx[0].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
		pVtx[1].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
		pVtx[2].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
		pVtx[3].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);

		pVtx += 4;
	}

	// 頂点バッファをアンロックする
	g_pVtxBuffHeart->Unlock();

	g_nLife = MAX_LIFE;
}

//================================================================================================================
// ハートの追加、減少
//================================================================================================================
void AddHeart(int nValue)
{
	VERTEX_2D* pVtx;

	if (nValue >= 1 && (g_nLife - 1) < MAX_LIFE)
	{ // 回復した場合
		g_nLife++;

		ChangeModeFrame(FRAMESTATE_HEAL);

		// 頂点バッファをロックし、頂点情報へのポインタを取得
		g_pVtxBuffHeart->Lock(0, 0, (void**)&pVtx, 0);

		pVtx += 4 * (g_nLife - 1);

		// 頂点カラーの設定
		pVtx[0].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
		pVtx[1].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
		pVtx[2].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
		pVtx[3].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);

		// 頂点バッファをアンロックする
		g_pVtxBuffHeart->Unlock();
	}
	else if (nValue <= -1 && (g_nLife - 1) > -1)
	{ // ダメージを食らった場合
		// 頂点バッファをロックし、頂点情報へのポインタを取得
		g_pVtxBuffHeart->Lock(0, 0, (void**)&pVtx, 0);

		pVtx += 4 * (g_nLife - 1);

		// 頂点カラーの設定
		pVtx[0].col = D3DXCOLOR(0.0f, 0.0f, 0.0f, 1.0f);
		pVtx[1].col = D3DXCOLOR(0.0f, 0.0f, 0.0f, 1.0f);
		pVtx[2].col = D3DXCOLOR(0.0f, 0.0f, 0.0f, 1.0f);
		pVtx[3].col = D3DXCOLOR(0.0f, 0.0f, 0.0f, 1.0f);

		// 頂点バッファをアンロックする
		g_pVtxBuffHeart->Unlock();

		g_nLife--;

		if (g_nLife > 1)
		{
			ChangeModeFrame(FRAMESTATE_DAMAGE);
		}
		else if (g_nLife == 1)
		{
			ChangeModeFrame(FRAMESTATE_DANGER);
		}
	}
	else
	{ // 0により現在体力にて何かを判定したい場合
		if (g_nLife > 1)
		{
			ChangeModeFrame(FRAMESTATE_NORMAL);
		}
		else
		{
			ChangeModeFrame(FRAMESTATE_DANGER);
		}
	}
}