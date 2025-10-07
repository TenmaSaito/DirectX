//================================================================================================================
//
// DirectXのメイン処理 [main.cpp]
// Author : TENMA
//
//================================================================================================================
#include "main.h"
#include "input.h"
#include "sound.h"
#include "title.h"
#include "game.h"
#include "result.h"
#include "enemy.h"
#include "player.h"
#include "fade.h"
#include "gauge.h"
#include "gameover.h"
#include "gameclear.h"
#include "credit.h"
#include "block.h"
#include "bullet.h"
#include "resource.h"
#include <crtdbg.h>

// マクロ定義
#define CLASS_NAME		"WindowClass"				// ウィンドウクラスの名前
#define WINDOW_NAME		"MapMaker"					// キャプションに表示される名前(提出する前に変えること！！！)

// プロトタイプ宣言
LRESULT CALLBACK WindowProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
HRESULT Init(HINSTANCE hInstance, HWND hWnd, BOOL bWindow);
void Uninit(void);
void Update(void);
void Draw(void);
void DrawDebug(void);
void ToggleFullscreen(HWND hWnd);

// グローバル変数
LPDIRECT3D9				g_pD3D = NULL;					// Direct3Dオブジェクトへのポインタ
LPDIRECT3DDEVICE9		g_pD3DDevice = NULL;			// Direct3Dデバイスへのポインタ
MODE g_mode = MODE_TITLE;								// 現在の画面
MODE g_modeExac = MODE_TITLE;							// ひとつ前の過去の画面
LPD3DXFONT g_pFont = NULL;								// フォントへのポインタ
HWND g_hWnd = NULL;										// 獲得したウィンドウハンドル
int g_nCountFPS = 0;									// FPSカウンタ
bool g_isFullscreen = false;							// フルスクリーンの使用状況
RECT g_windowRect;										// ウィンドウサイズ
GAMEDIFFICULTY g_Difficulty = GAMEDIFFICULTY_NORMAL;	// ゲームの難易度

//================================================================================================================
// メイン関数
//================================================================================================================
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hInstancePrev, LPSTR lpCmdLine, int nCmdShow)
{

#ifdef _DEBUG
	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF | _CRTDBG_CHECK_ALWAYS_DF);			// メモリリーク検知用のフラグ
#endif // _DEBUG

	HWND hWnd;									// ウィンドウハンドル
	MSG msg;									// メッセージを格納する変数
	DWORD dwCurrentTime;						// 現在時刻
	DWORD dwExecLastTime;						// 最後に処理した時刻
	DWORD dwFrameCount;							// フレームカウント
	DWORD dwFPSLastTime;						// 最後にFPSを計測した時刻
	RECT rect = { 0,0,SCREEN_WIDTH,SCREEN_HEIGHT };

	// ログの初期化処理
	InitLog();

	WNDCLASSEX wcex =
	{
		sizeof(WNDCLASSEX),						// ウィンドウクラスのメモリサイズ
		CS_CLASSDC,								// ウィンドウのスタイル
		WindowProc,								// ウィンドウプロシージャ
		0,										// 0
		0,										// 0
		hInstance,								// インスタンスハンドル
		LoadIcon(hInstance,(LPCSTR)IDI_ICON2),	// タスクバーのアイコン
		LoadCursor(NULL,IDC_ARROW),				// マウスカーソル
		(HBRUSH)(COLOR_WINDOW + 1),				// クライアント領域の背景色
		NULL,									// メニューバー
		CLASS_NAME,								// ウィンドウクラスの名前
		LoadIcon(hInstance,(LPCSTR)IDI_ICON2)	// ファイルのアイコン
	}; 

	AddFunctionLog("END : WindowClass Setting");

	// ウィンドウクラスの登録
	RegisterClassEx(&wcex);

	AddFunctionLog("END : WindowClass Register");

	// クライアント領域を指定のサイズに調整
	AdjustWindowRect(&rect, WS_OVERLAPPEDWINDOW, FALSE);

	AddFunctionLog("END : WindowRect Setting");

	// ウィンドウの生成
	hWnd = CreateWindowEx(
		0,										// 拡張ウィンドウスタイル
		CLASS_NAME,								// ウィンドウクラスの名前
		WINDOW_NAME,							// ウィンドウの名前
		WS_OVERLAPPEDWINDOW,					// ウィンドウスタイル
		CW_USEDEFAULT,							// ウィンドウの左上X座標
		CW_USEDEFAULT,							// ウィンドウの左上Y座標
		(rect.right - rect.left),				// ウィンドウの幅
		(rect.bottom - rect.top),				// ウィンドウの高さ
		NULL,									// 親ウィンドウのハンドル
		NULL,									// メニュー(もしくは子ウィンドウ)ハンドル
		hInstance,								// インスタンスハンドル
		NULL);									// ウィンドウ作成データ

	AddFunctionLog("END : Window Create");

	if(hWnd == NULL)
	{
		MessageBox(hWnd, "ウィンドウハンドルが確保されていません!", "警告！", MB_ICONWARNING);
		return -1;
	}

	// 初期化処理
	if (FAILED(Init(hInstance, hWnd, TRUE)))
	{// 初期化処理が失敗した場合
		return -1;
	}

	AddFunctionLog("CLEAR : DirectX Init");

	//分解能を設定
	timeBeginPeriod(1);

	AddFunctionLog("END : timeBegin Setting");

	dwCurrentTime = 0;							// 初期化
	dwExecLastTime = timeGetTime();				// 現在時刻を取得
	dwFrameCount = 0;
	dwFPSLastTime = timeGetTime();

	// ウィンドウの表示
	ShowWindow(hWnd, nCmdShow);					// ウィンドウの表示状態を設定

	AddFunctionLog("END : Window Show");

	UpdateWindow(hWnd);							// クライアント領域を更新

	AddFunctionLog("END : Window Update");
	
	g_hWnd = hWnd;

	// メッセージループ
	while (1)
	{
		if (PeekMessage(&msg, NULL, 0, 0,PM_REMOVE) != 0)
		{// windowsの処理
			if (msg.message == WM_QUIT)
			{// WM_QUITメッセージを受けると、メッセージループを抜ける
				break;
			}
			else
			{
				// メッセージの設定
				TranslateMessage(&msg);					// 仮想キーメッセージを文字メッセージへ変換

				DispatchMessage(&msg);					// ウィンドウプロシージャへメッセージを送出
			}
		}
		else
		{// DirectXの処理
			dwCurrentTime = timeGetTime();				// 現在時刻を取得

			if ((dwCurrentTime - dwFPSLastTime) >= 500)
			{// 0.5秒経過
				// FPSを計測
				g_nCountFPS = (dwFrameCount * 1000) / (dwCurrentTime - dwFPSLastTime);

				dwFPSLastTime = dwCurrentTime;			// FPSを計測した時刻を取得
				dwFrameCount = 0;						// フレームカウントをクリア
			}

			if ((dwCurrentTime - dwExecLastTime) >= (1000 / 60))
			{//60分の1秒経過

				dwExecLastTime = dwCurrentTime;			//処理開始時刻[現在時刻]を保存

				// 更新処理
				Update();

				// 描画処理
				Draw();

				dwFrameCount++;							// フレームカウントを加算
			}
		}
		
	}

	// 終了処理
	Uninit();

	AddFunctionLog("END : DirextX Uninit");

	// ウィンドウクラスの登録解除
	UnregisterClass(CLASS_NAME, wcex.hInstance); 

	AddFunctionLog("END : WindowClass Unregister");

	// ログの終了処理
	UninitLog();

	return (int)msg.wParam;
}

//================================================================================================================
// ウィンドウプロシージャ
//================================================================================================================
LRESULT CALLBACK WindowProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	int nID;

	switch (uMsg)
	{
	case WM_DESTROY:		// ウィンドウ破棄のメッセージ
		// WM_QUITメッセージを送る
		PostQuitMessage(0);
		break;

	// キー押下のメッセージ
	case WM_KEYDOWN:
		// 押されたキーの判定
		switch (wParam)
		{
		case VK_ESCAPE:
			// 終了確認
			nID = MessageBox(hWnd, "終了しますか？", "終了確認メッセージ", (MB_YESNO | MB_ICONINFORMATION));
			if (nID == IDYES)
			{// もしYESだった場合
				// ウィンドウを破棄する(WM_DESTROYメッセージを送る)
				DestroyWindow(hWnd);
			}

			break;

		case VK_F11:
			ToggleFullscreen(hWnd);// F11でフルスクリーン
			break;
		}
		break;

	case WM_CLOSE:		// 閉じるボタン押下のメッセージ
		
		// ウィンドウを破棄する(WM_DESTROYメッセージを送る)
		DestroyWindow(hWnd);
		
		break;
	}

	return DefWindowProc(hWnd, uMsg, wParam, lParam);
}

//================================================================================================================
//初期化処理
//================================================================================================================
HRESULT Init(HINSTANCE hInstance, HWND hWnd, BOOL bWindow)
{
	D3DDISPLAYMODE d3ddm;			// ディスプレイモード
	D3DPRESENT_PARAMETERS d3dpp;	// プレゼンテーションパラメータ		

	// Direct3Dオブジェクトの作成
	g_pD3D = Direct3DCreate9(D3D_SDK_VERSION);
	if (g_pD3D == NULL)
	{
		return E_FAIL;
	}

	AddFunctionLog("CLEAR : Direct3DObject Create");

	// 現在のディスプレイモードを取得
	if (FAILED(g_pD3D->GetAdapterDisplayMode(D3DADAPTER_DEFAULT,
		&d3ddm)))
	{
		return E_FAIL;
	}

	AddFunctionLog("CLEAR : DisplayMode Get");

	// デバイスのプレゼンテーションパラメータの設定
	ZeroMemory(&d3dpp, sizeof(d3dpp));			// パラメータのゼロクリア

	d3dpp.BackBufferWidth = SCREEN_WIDTH;		// ゲームの画面サイズ(横)
	d3dpp.BackBufferHeight = SCREEN_HEIGHT;		// ゲームの画面サイズ(高さ)
	d3dpp.BackBufferFormat = D3DFMT_UNKNOWN;		// バックバッファの形式
	d3dpp.BackBufferCount = 1;					// バックバッファの数
	d3dpp.SwapEffect = D3DSWAPEFFECT_DISCARD;	// ダブルバッファの切り替え(映像信号と同期)
	d3dpp.EnableAutoDepthStencil = TRUE;		// デプスバッファとステンシルバッファを作成
	d3dpp.AutoDepthStencilFormat = D3DFMT_D16;	// デプスバッファとして16bitを使う
	d3dpp.Windowed = bWindow;					// ウィンドウモード
	d3dpp.FullScreen_RefreshRateInHz = D3DPRESENT_RATE_DEFAULT;			// リフレッシュレート
	d3dpp.PresentationInterval = D3DPRESENT_INTERVAL_DEFAULT;			// インターバル

	// Direct3Dデバイスの作成
	if (FAILED(g_pD3D->CreateDevice(D3DADAPTER_DEFAULT,
		D3DDEVTYPE_HAL,
		hWnd,
		D3DCREATE_HARDWARE_VERTEXPROCESSING,
		&d3dpp,
		&g_pD3DDevice)))
	{
		if (FAILED(g_pD3D->CreateDevice(D3DADAPTER_DEFAULT,
			D3DDEVTYPE_HAL,
			hWnd,
			D3DCREATE_SOFTWARE_VERTEXPROCESSING,
			&d3dpp,
			&g_pD3DDevice)))
		{
			if (FAILED(g_pD3D->CreateDevice(D3DADAPTER_DEFAULT,
				D3DDEVTYPE_REF,
				hWnd,
				D3DCREATE_SOFTWARE_VERTEXPROCESSING,
				&d3dpp,
				&g_pD3DDevice)))
			{
				return E_FAIL;
			}
			else
			{
				AddFunctionLog("CLEAR : Device->SOFTWARE Create");
			}
		}
		else
		{
			AddFunctionLog("CLEAR : Device->SOFTWARE HALF Create");
		}
	}
	else
	{
		AddFunctionLog("CLEAR : Device->HARDWARE Create");
	}

	// レンダーステートの設定(消さないこと！ALPHA値の設定を適用する為の設定！)
	g_pD3DDevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);
	g_pD3DDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);
	g_pD3DDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	g_pD3DDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);

	// サンプラーステートの設定
	g_pD3DDevice->SetSamplerState(0, D3DSAMP_MINFILTER, D3DTEXF_LINEAR);		// テクスチャの拡縮補間の設定
	g_pD3DDevice->SetSamplerState(0, D3DSAMP_MAGFILTER, D3DTEXF_LINEAR);
	g_pD3DDevice->SetSamplerState(0, D3DSAMP_ADDRESSU, D3DTADDRESS_WRAP);		// テクスチャの繰り返しの設定
	g_pD3DDevice->SetSamplerState(0, D3DSAMP_ADDRESSV, D3DTADDRESS_WRAP);

	// テクスチャステージステートの設定(テクスチャのアルファブレンドの設定[テクスチャとポリゴンのALPHA値を混ぜる！])
	g_pD3DDevice->SetTextureStageState(0, D3DTSS_ALPHAOP, D3DTOP_MODULATE);
	g_pD3DDevice->SetTextureStageState(0, D3DTSS_ALPHAARG1, D3DTA_TEXTURE);
	g_pD3DDevice->SetTextureStageState(0, D3DTSS_ALPHAARG2, D3DTA_CURRENT);

	// デバッグ用フォントの生成
	D3DXCreateFont(g_pD3DDevice,
		30,
		0,
		0,
		0,
		FALSE,
		SHIFTJIS_CHARSET,
		OUT_DEFAULT_PRECIS,
		DEFAULT_QUALITY,
		DEFAULT_PITCH,
		"PixelMplus12",
		&g_pFont);

	AddFunctionLog("END : D3DXCreateFont Create");

	// キーボードの初期化処理
	if (FAILED(InitKeyboard(hInstance, hWnd)))
	{
		return E_FAIL;
	}

	AddFunctionLog("CLEAR : Keyboard Init");

	// ジョイパッドの初期化処理
	if (FAILED(InitJoypad()))
	{
		return E_FAIL;
	}

	AddFunctionLog("CLEAR : Joypad Init");

	// マウスの初期化処理
	if (FAILED(InitMouse(hWnd)))
	{
		return E_FAIL;
	}

	AddFunctionLog("CLEAR : Mouse Init");

	// サウンドの初期化処理
	if (FAILED(InitSound(hWnd)))
	{
		return E_FAIL;
	}

	AddFunctionLog("CLEAR : Sound Init");

	SetMode(g_mode);

	AddFunctionLog("END : Mode Set");

	InitFade(g_mode);

	AddFunctionLog("END : Fade Init");

	SetEnablePlayerFullburst(false);

	return S_OK;
}

//================================================================================================================
// 終了処理
//================================================================================================================
void Uninit(void)
{
	// タイトル画面の終了処理
	UninitTitle();

	AddFunctionLog("END : Title Uninit");

	// ゲーム画面の終了処理
	UninitGame();

	AddFunctionLog("END : Game Uninit");

	// リザルト画面の終了処理
	UninitResult();

	AddFunctionLog("END : Result Uninit");

	// ゲームオーバー画面の終了処理
	UninitGameover();

	AddFunctionLog("END : Gameover Uninit");

	// フェードの終了処理
	UninitFade();

	AddFunctionLog("END : Fade Uninit");

	// キーボードの終了処理
	UninitKeyboard();

	AddFunctionLog("END : Keyboard Uninit");

	// ジョイパッドの終了処理
	UninitJoypad();

	AddFunctionLog("END : Joypad Uninit");

	// マウスの終了処理
	UninitMouse();

	AddFunctionLog("END : Mouse Uninit");

	// デバッグ表示用フォントの破棄
	if (g_pFont != NULL)
	{
		g_pFont->Release();
		g_pFont = NULL;

		AddFunctionLog("END : Font Release");
	}

	// サウンドの終了処理
	UninitSound();

	AddFunctionLog("END : Sound Uninit");

	// Direct3Dデバイスの破棄
	if (g_pD3DDevice != NULL)
	{
		g_pD3DDevice->Release();

		g_pD3DDevice = NULL;

		AddFunctionLog("END : DirectXDevice Release");
	}

	// Direct3Dオブジェクトの破棄
	if (g_pD3D != NULL)
	{
		g_pD3D->Release();

		g_pD3D = NULL;

		AddFunctionLog("END : DirextXObject Release");
	}
}

//================================================================================================================
// 更新処理
//================================================================================================================
void Update(void)
{
	//キーボードの更新処理(これより上に書くな)
	UpdateKeyboard();

	//ジョイパッドの更新処理
	UpdateJoypad();

	// マウスの更新処理
	UpdateMouse();

	// 現在の画面(モード)の更新処理
	switch (g_mode)
	{
	case MODE_TITLE:
		UpdateTitle();

		break;

	case MODE_GAME:
		UpdateGame();

		break;

	case MODE_RESULT:
		UpdateResult();

		break;

	case MODE_GAMEOVER:
		UpdateGameover();

		break;

	case MODE_GAMECLEAR:
		UpdateGameclear();

		break;

	case MODE_CREDIT:
		UpdateCredit();

		break;
	}

	// フェードの更新処理
	UpdateFade();

	// サウンドの更新処理
	UpdateSound();

	// ログの更新処理
	UpdateLog();
}

//================================================================================================================
// 描画処理
//================================================================================================================
void Draw(void)
{
	// 画面クリア(バックバッファとZバッファのクリア)
	g_pD3DDevice->Clear(0, NULL,
		(D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER),
		D3DCOLOR_RGBA(0, 0, 0, 0), 1.0f, 0);

	// 描画開始
	if (SUCCEEDED(g_pD3DDevice->BeginScene()))
	{// 描画開始が成功した場合

		// 現在の画面(モード)の描画処理
		switch (g_mode)
		{
		case MODE_TITLE:
			DrawTitle();
			break;

		case MODE_GAME:
			DrawGame();
			break;

		case MODE_RESULT:
			DrawResult();
			break;

		case MODE_GAMEOVER:
			DrawGameover();
			break;

		case MODE_GAMECLEAR:
			DrawGameclear();
			break;

		case MODE_CREDIT:
			DrawCredit();
			break;
		}

		// フェードの描画処理
		DrawFade();

#ifdef _DEBUG

		// デバッグ表示
		DrawDebug();

#endif // _DEBUG

		//DrawTutorial();

		// ログの初期化処理
		DrawLog();

		// 描画終了
		g_pD3DDevice->EndScene();
	}

	// バックバッファとフロントバッファの入れ替え
   	g_pD3DDevice->Present(NULL, NULL, NULL, NULL);
}

// デバイスの取得
LPDIRECT3DDEVICE9 GetDevice(void)
{
	return g_pD3DDevice;
}

// 画面の設定
void SetMode(MODE mode)
{
	// 現在の画面(モード)の終了
	switch (g_mode)
	{
	case MODE_TITLE:
		UninitTitle();
		AddFunctionLog("END : Title Uninit");
		break;

	case MODE_GAME:
		UninitGame();
		AddFunctionLog("END : Game Uninit");
		break;

	case MODE_RESULT:
		UninitResult();
		AddFunctionLog("END : Result Uninit");
		break;

	case MODE_GAMEOVER:
		UninitGameover();
		AddFunctionLog("END : Gameover Uninit");
		break;

	case MODE_GAMECLEAR:
		UninitGameclear();
		break;

	case MODE_CREDIT:
		UninitCredit();
		break;
	}

	// 現在のモードを保存
	g_modeExac = g_mode;

	// 指定の画面(モード)の初期化処理
	switch (mode)
	{
	case MODE_TITLE:
		InitTitle();
		AddFunctionLog("END : Title Init");
		break;

	case MODE_GAME:
		InitGame(g_Difficulty);
		AddFunctionLog("END : Game Init");
		break;

	case MODE_RESULT:
		InitResult();
		AddFunctionLog("END : Result Init");
		break;

	case MODE_GAMEOVER:
		InitGameover();
		AddFunctionLog("END : Gameover Init");
		break;

	case MODE_GAMECLEAR:
		InitGameclear();
		break;

	case MODE_CREDIT:
		InitCredit();
		break;
	}

	AddFunctionLog("END : Mode Set");

	// モードを保存
	g_mode = mode;

	AddFunctionLog("END : Mode Save");
}

// 現在のモードを取得
MODE GetMode(void)
{
	AddFunctionLog("END : Mode Get");

	return g_mode;
}

// 直前のモードを取得
MODE GetModeExac(void)
{
	return g_modeExac;
}

// 獲得済みウィンドウの取得
HRESULT GetHandleWindow(HWND* phWnd)
{
	if (g_hWnd != NULL)
	{
		*phWnd = g_hWnd;
		return S_OK;
	}
	else
	{
		MessageBox(g_hWnd, "ウィンドウハンドルが確保されていません!", "警告！", MB_ICONWARNING);
		return E_FAIL;
	}
}

// デバッグ表示
void DrawDebug(void)
{
	RECT rect = { 0,0,SCREEN_WIDTH,SCREEN_HEIGHT };			// 画面サイズ
	char aStr[16][256];										// 画面に表示する文字列
	int nCntEnemy = GetTotalEnemy();						// 敵の総数
	PLAYER* pPlayer = GetPlayer();							// プレイヤーの情報
	XINPUT_VIBRATION *pVibration = GetJoyVibration();		// バイブレーションの情報
	XINPUT_STATE *pState = GetJoypadState();				// ジョイパッドの情報
	Gauge *pGauge = GetGauge();								// ゲージの情報
	POINT pos = GetMousePos();								// マウスの位置情報
	int nBullet = GetBulletAll();

	// 文字列に代入
	wsprintf(&aStr[0][0], "FPS:%d\n", g_nCountFPS);

	wsprintf(&aStr[1][0], "EnemyAll:%d\n", nCntEnemy);

	wsprintf(&aStr[2][0], "PlayerPos:%d / %d\n", (int)pPlayer->posPlayer.x, (int)pPlayer->posPlayer.y);

	wsprintf(&aStr[3][0], "Left:%d\n", pVibration->wLeftMotorSpeed);

	wsprintf(&aStr[4][0], "Right:%d\n", pVibration->wRightMotorSpeed);

	wsprintf(&aStr[5][0], "Key:%d\n", pState->Gamepad.wButtons);

	wsprintf(&aStr[6][0], "Gauge:%d\n", pGauge[pPlayer->nGaugeNo].nPercentGauge);

	wsprintf(&aStr[7][0], "Count:%d\n", pPlayer->nCounterBulletCharge);

	wsprintf(&aStr[8][0], "X:%d\n", pos.x);

	wsprintf(&aStr[9][0], "Y:%d\n", pos.y);

	wsprintf(&aStr[10][0], "Volume:%d\n", (int)(GetVolume(SETSOUND_GAME) * 100.0f));


	// 文字列を結合
	for (int nCntDebug = 0; nCntDebug < 10; nCntDebug++)
	{
		strcat(&aStr[0][0], &aStr[nCntDebug + 1][0]);
	}

	// テキストの描画
    g_pFont->DrawText(NULL, &aStr[0][0], -1, &rect, DT_LEFT, D3DCOLOR_RGBA(255, 255, 255, 255));
}

//================================================
// ウィンドウフルスクリーン処理
//================================================
void ToggleFullscreen(HWND hWnd)
{
	// 現在のウィンドウスタイルを取得
	DWORD dwStyle = GetWindowLong(hWnd, GWL_STYLE);
	if (g_isFullscreen)
	{
		// ウィンドウモードに切り替え
		SetWindowLong(hWnd, GWL_STYLE, dwStyle | WS_OVERLAPPEDWINDOW);
		SetWindowPos(hWnd, HWND_TOP, g_windowRect.left, g_windowRect.top,
			g_windowRect.right - g_windowRect.left, g_windowRect.bottom - g_windowRect.top,
			SWP_FRAMECHANGED | SWP_NOACTIVATE);
		ShowWindow(hWnd, SW_NORMAL);
	}
	else
	{
		// フルスクリーンモードに切り替え
		GetWindowRect(hWnd, &g_windowRect);
		SetWindowLong(hWnd, GWL_STYLE, dwStyle & ~WS_OVERLAPPEDWINDOW);
		SetWindowPos(hWnd, HWND_TOP, 0, 0, GetSystemMetrics(SM_CXSCREEN), GetSystemMetrics(SM_CYSCREEN),
			SWP_FRAMECHANGED | SWP_NOACTIVATE);
		ShowWindow(hWnd, SW_MAXIMIZE);
	}

	g_isFullscreen = !g_isFullscreen;
}

//================================================
// ゲームの難易度設定処理
//================================================
void SetGameDifficulty(GAMEDIFFICULTY difficulty)
{
	g_Difficulty = difficulty;
}

//================================================
// ゲームの難易度設定処理
//================================================
GAMEDIFFICULTY GetGameDifficulty(void)
{
	return g_Difficulty;
}