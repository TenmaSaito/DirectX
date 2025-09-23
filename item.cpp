//================================================================================================================
//
// DirectXのアイテムファイル [item.cpp]
// Author : TENMA
//
//================================================================================================================
#include "item.h"
#include "player.h"
#include "score.h"
#include "heart.h"
#include "sound.h"
#include "stage.h"
#include "playerframe.h"

// マクロ定義
#define MAX_ITEM		(64)			// ブロックの最大数

// プロトタイプ宣言
void CollisionPlayer(ITEM *pItem);

// グローバル変数
LPDIRECT3DTEXTURE9		g_pTextureItem = NULL;	// テクスチャへのポインタ
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffItem = NULL;	// 頂点バッファのポインタ
ITEM g_aItem[MAX_ITEM];						// ブロック構造体

//================================================================================================================
// ブロックの初期化処理
//================================================================================================================
void InitItem(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();			// デバイスのポインタ
	ITEM *pItem = &g_aItem[0];
	int nCntItem;

	for (nCntItem = 0; nCntItem < MAX_ITEM; nCntItem++,pItem++)
	{
		pItem->pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		pItem->col = D3DXCOLOR_NULL;
		pItem->nAlphaItem = 1;
		pItem->type = ITEMTYPE_COIN;
		pItem->bUse = false;
	}

	// テクスチャの読み込み
	D3DXCreateTextureFromFile(pDevice,
			"data\\TEXTURE\\CHARACTER\\ITEM\\ITEM.png",
			&g_pTextureItem);

	// 頂点バッファの生成
	pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * 4 * MAX_ITEM,			// sizeofの後必ず * 頂点数 を書くこと！
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_2D,
		D3DPOOL_MANAGED,
		&g_pVtxBuffItem,
		NULL);

	AddFunctionLog("END : VertexBuffer Create");

	VERTEX_2D* pVtx = NULL;					// 頂点情報へのポインタ

	// 頂点バッファをロックし、頂点情報へのポインタを取得
	g_pVtxBuffItem->Lock(0, 0, (void**)&pVtx, 0);

	AddFunctionLog("START : VertexBuffer Setting");

	pItem = &g_aItem[0];

	for (nCntItem = 0; nCntItem < MAX_ITEM; nCntItem++,pItem++)
	{
		// 頂点座標の設定(座標設定は必ず右回りで！！！)
		pVtx[0].pos.x = pItem->pos.x - ITEM_WIDTH;
		pVtx[0].pos.y = pItem->pos.y - ITEM_HEIGHT;
		pVtx[0].pos.z = 0.0f;

		pVtx[1].pos.x = pItem->pos.x + ITEM_WIDTH;
		pVtx[1].pos.y = pItem->pos.y - ITEM_HEIGHT;
		pVtx[1].pos.z = 0.0f;

		pVtx[2].pos.x = pItem->pos.x - ITEM_WIDTH;
		pVtx[2].pos.y = pItem->pos.y + ITEM_HEIGHT;
		pVtx[2].pos.z = 0.0f;

		pVtx[3].pos.x = pItem->pos.x + ITEM_WIDTH;
		pVtx[3].pos.y = pItem->pos.y + ITEM_HEIGHT;
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
		pVtx[0].tex = D3DXVECTOR2(0.125f * pItem->type, 0.0f);
		pVtx[1].tex = D3DXVECTOR2(0.125f * pItem->type + 0.125f, 0.0f);
		pVtx[2].tex = D3DXVECTOR2(0.125f * pItem->type, 1.0f);
		pVtx[3].tex = D3DXVECTOR2(0.125f * pItem->type + 0.125f, 1.0f);

		pVtx += 4;
	}

	// 頂点バッファをアンロックする
	g_pVtxBuffItem->Unlock();

	AddFunctionLog("END : VertexBuffer Create");
}

//================================================================================================================
// ブロックの終了処理
//================================================================================================================
void UninitItem(void)
{
	// テクスチャの破棄(必ず行うこと！！！)
	for (int nCntItem = 0; nCntItem < ITEMTYPE_MAX; nCntItem++)
	{
		if (g_pTextureItem != NULL)
		{
			g_pTextureItem->Release();
			g_pTextureItem = NULL;
		}
	}

	// 頂点バッファの破棄(必ず行うこと！！！)
	if (g_pVtxBuffItem != NULL)
	{
		g_pVtxBuffItem->Release();
		g_pVtxBuffItem = NULL;
	}
}

//================================================================================================================
// ブロックの更新処理
//================================================================================================================
void UpdateItem(void)
{
	VERTEX_2D *pVtx = NULL;
	ITEM *pItem = &g_aItem[0];
	PLAYER *pPlayer = GetPlayer();
	
	D3DXVECTOR3 pos = D3DXVECTOR3_NULL;

	// 頂点バッファをロックし、頂点情報へのポインタを取得
	g_pVtxBuffItem->Lock(0, 0, (void**)&pVtx, 0);

	for (int nCntItem = 0; nCntItem < MAX_ITEM; nCntItem++, pItem++)
	{
		if (pItem->bUse == true)
		{
			// 当たり判定
			CollisionPlayer(pItem);

			pos = pPlayer->moveposPlayer;

			pItem->col.a += 0.01f * pItem->nAlphaItem;
			if (pItem->col.a >= 1.0f || pItem->col.a <= 0.2f)
			{
				pItem->nAlphaItem *= -1;
			}

			// 頂点座標の設定(座標設定は必ず右回りで！！！)
			pVtx[0].pos.x = (pItem->pos.x + pos.x) - ITEM_WIDTH;
			pVtx[0].pos.y = (pItem->pos.y + pos.y) - ITEM_HEIGHT;
			pVtx[0].pos.z = 0.0f;

			pVtx[1].pos.x = (pItem->pos.x + pos.x) + ITEM_WIDTH;
			pVtx[1].pos.y = (pItem->pos.y + pos.y) - ITEM_HEIGHT;
			pVtx[1].pos.z = 0.0f;

			pVtx[2].pos.x = (pItem->pos.x + pos.x) - ITEM_WIDTH;
			pVtx[2].pos.y = (pItem->pos.y + pos.y) + ITEM_HEIGHT;
			pVtx[2].pos.z = 0.0f;

			pVtx[3].pos.x = (pItem->pos.x + pos.x) + ITEM_WIDTH;
			pVtx[3].pos.y = (pItem->pos.y + pos.y) + ITEM_HEIGHT;
			pVtx[3].pos.z = 0.0f;

			// rhwの設定
			pVtx[0].rhw = 1.0f;
			pVtx[1].rhw = 1.0f;
			pVtx[2].rhw = 1.0f;
			pVtx[3].rhw = 1.0f;

			// 頂点カラーの設定
			pVtx[0].col = pItem->col;
			pVtx[1].col = pItem->col;
			pVtx[2].col = pItem->col;
			pVtx[3].col = pItem->col;

			// テクスチャ座標の設定
			pVtx[0].tex = D3DXVECTOR2(0.125f * pItem->type, 0.0f);
			pVtx[1].tex = D3DXVECTOR2(0.125f * pItem->type + 0.125f, 0.0f);
			pVtx[2].tex = D3DXVECTOR2(0.125f * pItem->type, 1.0f);
			pVtx[3].tex = D3DXVECTOR2(0.125f * pItem->type + 0.125f, 1.0f);
		}

		pVtx += 4;				// 頂点データのポインタを4つ分進める
	}

	// 頂点バッファをアンロックする
	g_pVtxBuffItem->Unlock();
}

//================================================================================================================
// ブロックの描画処理
//================================================================================================================
void DrawItem(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();			// デバイスのポインタ
	ITEM *pItem = &g_aItem[0];

	//頂点バッファをデータストリームに設定
	pDevice->SetStreamSource(0, g_pVtxBuffItem, 0, sizeof(VERTEX_2D));

	// 頂点フォーマットの設定
	pDevice->SetFVF(FVF_VERTEX_2D);

	for (int nCntItem = 0; nCntItem < MAX_ITEM; nCntItem++,pItem++)
	{
		if (pItem->bUse == true)
		{
			// テクスチャの設定(使わないならNULLを入れる！！！！)
			pDevice->SetTexture(0, g_pTextureItem);

			// ポリゴンの描画
			pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP,		// プリミティブの種類
				nCntItem * 4,								// 描画する最初の頂点インデックス
				2);											// 描画するプリミティブの数
		}
	}
}

//================================================================================================================
// ブロックの設置
//================================================================================================================
void SetItem(ITEMTYPE type, D3DXVECTOR3 pos)
{
	ITEM *pItem = &g_aItem[0];
	PLAYER* pPlayer = GetPlayer();
	D3DXVECTOR3 Camerapos = D3DXVECTOR3_NULL;
	VERTEX_2D* pVtx = NULL;
	int nCntItem = 0;

	AddFunctionLog("START : Item Set");

	// 頂点バッファをロックし、頂点情報へのポインタを取得
	g_pVtxBuffItem->Lock(0, 0, (void**)&pVtx, 0);

	for (nCntItem = 0; nCntItem < MAX_ITEM; nCntItem++,pItem++)
	{
		if (pItem->bUse == false)
		{
			Camerapos = pPlayer->moveposPlayer;

			pItem->pos = pos;
			pItem->col = D3DXCOLOR(1.0f,1.0f,1.0f,0.3f);
			pItem->nAlphaItem = 1;
			pItem->type = type;

			// 頂点座標の設定(座標設定は必ず右回りで！！！)
			pVtx[0].pos.x = (pItem->pos.x + Camerapos.x) - ITEM_WIDTH;
			pVtx[0].pos.y = (pItem->pos.y + Camerapos.y) - ITEM_HEIGHT;
			pVtx[0].pos.z = 0.0f;

			pVtx[1].pos.x = (pItem->pos.x + Camerapos.x) + ITEM_WIDTH;
			pVtx[1].pos.y = (pItem->pos.y + Camerapos.y) - ITEM_HEIGHT;
			pVtx[1].pos.z = 0.0f;

			pVtx[2].pos.x = (pItem->pos.x + Camerapos.x) - ITEM_WIDTH;
			pVtx[2].pos.y = (pItem->pos.y + Camerapos.y) + ITEM_HEIGHT;
			pVtx[2].pos.z = 0.0f;

			pVtx[3].pos.x = (pItem->pos.x + Camerapos.x) + ITEM_WIDTH;
			pVtx[3].pos.y = (pItem->pos.y + Camerapos.y) + ITEM_HEIGHT;
			pVtx[3].pos.z = 0.0f;

			// テクスチャ座標の設定
			pVtx[0].tex = D3DXVECTOR2(0.125f * pItem->type, 0.0f);
			pVtx[1].tex = D3DXVECTOR2(0.125f * pItem->type + 0.125f, 0.0f);
			pVtx[2].tex = D3DXVECTOR2(0.125f * pItem->type, 1.0f);
			pVtx[3].tex = D3DXVECTOR2(0.125f * pItem->type + 0.125f, 1.0f);

			// 頂点バッファをアンロックする
			g_pVtxBuffItem->Unlock();

			pItem->bUse = true;

			break;
		}

		pVtx += 4;
	}
}

//================================================================================================================
// ブロックの消去(全て)
//================================================================================================================
void DestroyItem(void)
{
	ITEM *pItem = &g_aItem[0];

	for (int nCntItem = 0; nCntItem < MAX_ITEM; nCntItem++, pItem++)
	{
		if (pItem->bUse == true)
		{
			pItem->bUse = false;
		}
	}
}

//================================================================================================================
// ブロックの当たり判定(Player)
//================================================================================================================
void CollisionPlayer(ITEM* pItem)
{
	PLAYER* pPlayer = GetPlayer();

	if (pPlayer->posPlayer.x >= pItem->pos.x - ITEM_WIDTH - (PLAYER_SIZE * 0.25f)
		&& pPlayer->posPlayer.x <= pItem->pos.x + ITEM_WIDTH + (PLAYER_SIZE * 0.25f)
		&& pPlayer->posPlayer.y >= pItem->pos.y - ITEM_HEIGHT - (PLAYER_SIZE * 0.5f)
		&& pPlayer->posPlayer.y <= pItem->pos.y + ITEM_HEIGHT + (PLAYER_SIZE * 0.5f))
	{
		switch (pItem->type)
		{
		case ITEMTYPE_COIN:
			// コイン
  			AddScore(1000);

			PlaySound(SOUND_LABEL_SE_GETCOIN);

			pItem->bUse = false;

			break;

		case ITEMTYPE_HEAL:

			PlaySound(SOUND_LABEL_SE_HEAL);
			
			if (pPlayer->nLife < MAX_LIFE)
			{	// ヒール
				AddHeart(1);

				pPlayer->nLife++;
			}

			pItem->bUse = false;

			break;

		case ITEMTYPE_HOMING:

			ChangeModeFrame(FRAMESTATE_POWER_ITEM);

			PlaySound(SOUND_LABEL_SE_POWER_CHANGE);

			SetPlayerShotType(SHOTTYPE_HOMING);

			pItem->bUse = false;

			break;

		case ITEMTYPE_BOMB:

			ChangeModeFrame(FRAMESTATE_POWER_ITEM);

			PlaySound(SOUND_LABEL_SE_POWER_CHANGE);

			SetPlayerShotType(SHOTTYPE_BOMB);

			pItem->bUse = false;

			break;

		case ITEMTYPE_LASER:

			ChangeModeFrame(FRAMESTATE_POWER_ITEM);

			PlaySound(SOUND_LABEL_SE_POWER_CHANGE);

			SetPlayerShotType(SHOTTYPE_LASER);

			pItem->bUse = false;

			break;

		case ITEMTYPE_KEY:

			pPlayer->bHaveKey = true;
			pItem->bUse = false;
			SetEnableHaveKeyExac(true);

			PlaySound(SOUND_LABEL_SE_GETKEY);

			break;

		case ITEMTYPE_ULTIMATE_COIN:

			// スーパーアルティメットコイン
			AddScore(100000);

			PlaySound(SOUND_LABEL_SE_GETULTIMATECOIN);

			pItem->bUse = false;

			break;

		case ITEMTYPE_SECRET:

			PlaySound(SOUND_LABEL_SE_GETULTIMATECOIN);

			SetEnableHaveSecret(true);

			pItem->bUse = false;

			break;
		}
	}
}