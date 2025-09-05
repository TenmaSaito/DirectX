//================================================================================================================
//
// DirectXの魔法陣設置のcppファイル [mage.cpp]
// Author : TENMA
//
//================================================================================================================
#include "mage.h"
#include "player.h"

// マクロ定義
#define MAGE_WIDTH		(80.0f)					// 横幅
#define MAGE_HEIGHT		(80.0f)					// 縦幅

// グローバル変数
LPDIRECT3DTEXTURE9		g_apTextureMage[PLAYERDIRECTION_MAX] = {};		// テクスチャへのポインタ
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffMage = NULL;	// 頂点バッファのポインタ
D3DXVECTOR3 g_posMage;							// ライフを表示する位置
PLAYERDIRECTION g_direction = PLAYERDIRECTION_DOWN;
bool g_bUseMage = false;

const char* g_apTEXTURE_MAGE[PLAYERDIRECTION_MAX]
{
	"data\\TEXTURE\\CHARACTER\\DIRECTION_UP.png",			// 上
	"data\\TEXTURE\\CHARACTER\\DIRECTION_DOWN.png",			// 下
	"data\\TEXTURE\\CHARACTER\\DIRECTION_LEFT.png",			// 左
	"data\\TEXTURE\\CHARACTER\\DIRECTION_RIGHT.png",		// 右
	"data\\TEXTURE\\CHARACTER\\DIRECTION_UPLEFT.png",		// 右上
	"data\\TEXTURE\\CHARACTER\\DIRECTION_UPRIGHT.png",		// 左上
	"data\\TEXTURE\\CHARACTER\\DIRECTION_DOWNLEFT.png",		// 右下
	"data\\TEXTURE\\CHARACTER\\DIRECTION_DOWNRIGHT.png"		// 左下
};

//================================================================================================================
// ハートの初期化処理
//================================================================================================================
void InitMage(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();			// デバイスのポインタ

	for (int nCntMage = 0; nCntMage < PLAYERDIRECTION_MAX; nCntMage++)
	{
		// テクスチャの読み込み
		D3DXCreateTextureFromFile(pDevice,
			g_apTEXTURE_MAGE[nCntMage],
			&g_apTextureMage[nCntMage]);
	}

	// 頂点バッファの生成
	pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * 4,			// sizeofの後必ず * 頂点数 を書くこと！
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_2D,
		D3DPOOL_MANAGED,
		&g_pVtxBuffMage,
		NULL);

	g_posMage = D3DXVECTOR3(PLAYER_SPOWN_X, PLAYER_SPOWN_Y, 0.0f);			// 位置を初期化
	g_bUseMage = false;
	
	AddFunctionLog("END : VertexBuffer Create");

	VERTEX_2D* pVtx;					// 頂点情報へのポインタ

	// 頂点バッファをロックし、頂点情報へのポインタを取得
	g_pVtxBuffMage->Lock(0, 0, (void**)&pVtx, 0);

	AddFunctionLog("START : VertexBuffer Setting");

	
	// 頂点座標の設定(座標設定は必ず右回りで！！！)
	pVtx[0].pos.x = g_posMage.x - MAGE_WIDTH;
	pVtx[0].pos.y = g_posMage.y - MAGE_HEIGHT;
	pVtx[0].pos.z = 0.0f;

	pVtx[1].pos.x = g_posMage.x + MAGE_WIDTH;
	pVtx[1].pos.y = g_posMage.y - MAGE_HEIGHT;
	pVtx[1].pos.z = 0.0f;

	pVtx[2].pos.x = g_posMage.x - MAGE_WIDTH;
	pVtx[2].pos.y = g_posMage.y + MAGE_HEIGHT;
	pVtx[2].pos.z = 0.0f;

	pVtx[3].pos.x = g_posMage.x + MAGE_WIDTH;
	pVtx[3].pos.y = g_posMage.y + MAGE_HEIGHT;
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

	// 頂点バッファをアンロックする
	g_pVtxBuffMage->Unlock();

	AddFunctionLog("END : VertexBuffer Create");
}

//================================================================================================================
// ハートの終了処理
//================================================================================================================
void UninitMage(void)
{
	// テクスチャの破棄(必ず行うこと！！！)
	for (int nCntMage = 0; nCntMage < PLAYERDIRECTION_MAX; nCntMage++)
	{
		if (g_apTextureMage[nCntMage] != NULL)
		{
			g_apTextureMage[nCntMage]->Release();
			g_apTextureMage[nCntMage] = NULL;
		}
	}
	
	// 頂点バッファの破棄(必ず行うこと！！！)
	if (g_pVtxBuffMage != NULL)
	{
		g_pVtxBuffMage->Release();
		g_pVtxBuffMage = NULL;
	}
}

//================================================================================================================
// ハートの更新処理
//================================================================================================================
void UpdateMage(void)
{
	if (g_bUseMage == true)
	{
		g_direction = GetPlayerDirection();
	}
}

//================================================================================================================
// ハートの描画処理
//================================================================================================================
void DrawMage(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();			// デバイスのポインタ

	//頂点バッファをデータストリームに設定
	pDevice->SetStreamSource(0, g_pVtxBuffMage, 0, sizeof(VERTEX_2D));

	// 頂点フォーマットの設定
	pDevice->SetFVF(FVF_VERTEX_2D);

	// テクスチャの設定(使わないならNULLを入れる！！！！)
	pDevice->SetTexture(0, g_apTextureMage[g_direction]);

	if (g_bUseMage == true)
	{
		// ポリゴンの描画
		pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP,		// プリミティブの種類
			0,											// 描画する最初の頂点インデックス
			2);											// 描画するプリミティブの数
	}
}

//================================================================================================================
// ハートの設置
//================================================================================================================
void SetMage(void)
{
	g_bUseMage = true;

	g_direction = PLAYERDIRECTION_DOWN;
}

void EnableMageDraw(bool bUse)
{
	g_bUseMage = bUse;
}