//=============================================================================
//
// サウンド処理 [sound.cpp]
// Author : TENMA SAITO (SPACIAL THANKS : AKIRA TANAKA)
//
//=============================================================================
#include "sound.h"
#include "fade.h"
#include "fade_stage.h"

//*****************************************************************************
// サウンド情報の構造体定義
//*****************************************************************************
typedef struct
{
	const char *pFilename;	// ファイル名
	int nCntLoop;			// ループカウント
} SOUNDINFO;

//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************
HRESULT CheckChunk(HANDLE hFile, DWORD format, DWORD *pChunkSize, DWORD *pChunkDataPosition);
HRESULT ReadChunkData(HANDLE hFile, void *pBuffer, DWORD dwBuffersize, DWORD dwBufferoffset);

//*****************************************************************************
// グローバル変数
//*****************************************************************************
IXAudio2 *g_pXAudio2 = NULL;								// XAudio2オブジェクトへのインターフェイス
IXAudio2MasteringVoice *g_pMasteringVoice = NULL;			// マスターボイス
IXAudio2SourceVoice *g_apSourceVoice[SOUND_LABEL_MAX] = {};	// ソースボイス
XAUDIO2_VOICE_DETAILS g_MasterVoiceDetails = {};						// マスターボイスの音声情報
XAUDIO2_VOICE_DETAILS g_aSourceVoiceDetails[SOUND_LABEL_MAX] = {};		// ソースボイスの音声情報
BYTE *g_apDataAudio[SOUND_LABEL_MAX] = {};					// オーディオデータ
DWORD g_aSizeAudio[SOUND_LABEL_MAX] = {};					// オーディオデータサイズ
bool g_aPlayAudio[SOUND_LABEL_MAX] = {};					// 再生状況
float g_fVolumeAudio;										// 現在の音量
SOUND_LABEL g_PlaySound;									// 再生中の音源
SOUND_LABEL	g_FadelabelExac;								// フェードアウト時に再生されている音源
SOUND_LABEL g_Fadelabel;									// フェードイン時に再生する音源
FADE g_CurrentFadeSound;									// 現在のフェード状態
int g_nCounterSound;										// 汎用カウンター

// サウンドの情報(sound.hのLABELに追加したらここにも追加する！)
SOUNDINFO g_aSoundInfo[SOUND_LABEL_MAX] =
{
	{"data/BGM/BGM_TITLE.wav", -1},					// タイトル画面のBGM
	{"data/BGM/BGM_GAME_NORMAL.wav", -1},			// ゲームプレイ時の草原BGM
	{"data/BGM/BGM_GAME_DESERT.wav", -1},			// ゲームプレイ時の砂漠BGM
	{"data/BGM/BGM_GAME_ICE.wav", -1},				// ゲームプレイ時の氷河BGM
	{"data/BGM/BGM_GAME_FOREST.wav", -1},			// ゲームプレイ時の森林BGM
	{"data/BGM/BGM_GAME_VOLCANO.wav", -1},			// ゲームプレイ時の火山BGM
	{"data/BGM/BGM_GAME_SEA.wav", -1},				// ゲームプレイ時の海洋BGM
	{"data/BGM/BGM_GAME_NOTIME.wav", -1},		// 制限時間が迫った時のBGM
	{"data/BGM/BGM_GAMECLEAR.wav",-1},				// ゲームクリア時のBGM
	{"data/BGM/BGM_GAMEOVER.wav",-1},				// ゲームオーバー時のBGM
	{"data/BGM/BGM_GAME_TUTORIAL.wav",-1},			// チュートリアル時のBGM
	{"data/BGM/BGM_CREDIT.wav",-1},					// クレジット時のBGM
	{"data/SE/shot000.wav", 0},			// 弾発射音
	{"data/SE/hit000.wav", 0},			// ヒット音
	{"data/SE/explosion000.wav", 0},	// 爆発音
	{"data/SE/Enter.wav", 0},			// 決定音
	{"data/SE/BarriarSE.wav", 0},		// バリア音
	{"data/SE/CoinSE.wav", 0},			// ノーマルコイン音
	{"data/SE/Ultimate_CoinSE.wav", 0},	// 凄いコイン音
	{"data/SE/GetKeySE.wav", 0},		// 鍵の取得音
	{"data/SE/OpenDoorSE.wav", 0},		// ドアを開ける音
	{"data/SE/SelectSE.wav", 0},		// 選択音
	{"data/SE/ChargeSE.wav", 0},		// チャージ中の音
	{"data/SE/FullChargeSE.wav", -1},	// チャージマックス音
	{"data/SE/ShotSE.wav", 0},			// ショット音
	{"data/SE/ChargeShotSE.wav", 0},	// チャージ技の射撃音
	{"data/SE/HEAL_SE.wav", 0},			// 回復の取得音
	{"data/SE/POWERITEM_SE.wav", 0},	// チャージ技の取得音
	{"data/SE/TELEPORT_SE.wav", 0},		// ステージ移動音
	{"data/SE/MirrorSE.wav", 0},		// 反射音
};

//=============================================================================
// 初期化処理
//=============================================================================
HRESULT InitSound(HWND hWnd)
{
	HRESULT hr;

	// COMライブラリの初期化
	hr = CoInitializeEx(NULL, COINIT_MULTITHREADED);
	if(FAILED(hr))
	{
		MessageBox(hWnd, "COMライブラリの初期化に失敗！", "警告！", MB_ICONWARNING);

		return E_FAIL;
	}

	// XAudio2オブジェクトの作成
	hr = XAudio2Create(&g_pXAudio2, 0);
	if(FAILED(hr))
	{
		MessageBox(hWnd, "XAudio2オブジェクトの作成に失敗！", "警告！", MB_ICONWARNING);

		// COMライブラリの終了処理
		CoUninitialize();

		return E_FAIL;
	}
	
	// マスターボイスの生成
	hr = g_pXAudio2->CreateMasteringVoice(&g_pMasteringVoice);
	if(FAILED(hr))
	{
		MessageBox(hWnd, "マスターボイスの生成に失敗！", "警告！", MB_ICONWARNING);

		if(g_pXAudio2 != NULL)
		{
			// XAudio2オブジェクトの開放
			g_pXAudio2->Release();
			g_pXAudio2 = NULL;
		}

		// COMライブラリの終了処理
		CoUninitialize();

		return E_FAIL;
	}

	// サウンドデータの初期化
	for(int nCntSound = 0; nCntSound < SOUND_LABEL_MAX; nCntSound++)
	{
		HANDLE hFile;
		DWORD dwChunkSize = 0;
		DWORD dwChunkPosition = 0;
		DWORD dwFiletype;
		WAVEFORMATEXTENSIBLE wfx;
		XAUDIO2_BUFFER buffer;

		// バッファのクリア
		memset(&wfx, 0, sizeof(WAVEFORMATEXTENSIBLE));
		memset(&buffer, 0, sizeof(XAUDIO2_BUFFER));

		// サウンドデータファイルの生成
		hFile = CreateFile(g_aSoundInfo[nCntSound].pFilename, GENERIC_READ, FILE_SHARE_READ, NULL, OPEN_EXISTING, 0, NULL);
		if(hFile == INVALID_HANDLE_VALUE)
		{
			MessageBox(hWnd, "サウンドデータファイルの生成に失敗！(1)", "警告！", MB_ICONWARNING);

			return HRESULT_FROM_WIN32(GetLastError());
		}

		if(SetFilePointer(hFile, 0, NULL, FILE_BEGIN) == INVALID_SET_FILE_POINTER)
		{// ファイルポインタを先頭に移動
			MessageBox(hWnd, "サウンドデータファイルの生成に失敗！(2)", "警告！", MB_ICONWARNING);

			return HRESULT_FROM_WIN32(GetLastError());
		}
	
		// WAVEファイルのチェック
		hr = CheckChunk(hFile, 'FFIR', &dwChunkSize, &dwChunkPosition);
		if(FAILED(hr))
		{
			MessageBox(hWnd, "WAVEファイルのチェックに失敗！(1)", "警告！", MB_ICONWARNING);

			return S_FALSE;
		}

		// チャンクデータの読み込み
		hr = ReadChunkData(hFile, &dwFiletype, sizeof(DWORD), dwChunkPosition);
		if(FAILED(hr))
		{
			MessageBox(hWnd, "WAVEファイルのチェックに失敗！(2)", "警告！", MB_ICONWARNING);

			return S_FALSE;
		}

		if(dwFiletype != 'EVAW')
		{
			MessageBox(hWnd, "WAVEファイルのチェックに失敗！(3)", "警告！", MB_ICONWARNING);

			return S_FALSE;
		}
	
		// フォーマットチェック
		hr = CheckChunk(hFile, ' tmf', &dwChunkSize, &dwChunkPosition);
		if(FAILED(hr))
		{
			MessageBox(hWnd, "フォーマットチェックに失敗！(1)", "警告！", MB_ICONWARNING);

			return S_FALSE;
		}

		// チャンクデータの読み込み
		hr = ReadChunkData(hFile, &wfx, dwChunkSize, dwChunkPosition);
		if(FAILED(hr))
		{
			MessageBox(hWnd, "フォーマットチェックに失敗！(2)", "警告！", MB_ICONWARNING);

			return S_FALSE;
		}

		// データチェック
		hr = CheckChunk(hFile, 'atad', &g_aSizeAudio[nCntSound], &dwChunkPosition);
		if(FAILED(hr))
		{
			MessageBox(hWnd, "オーディオデータ読み込みに失敗！(1)", "警告！", MB_ICONWARNING);

			return S_FALSE;
		}

		// チャンクデータの読み込み
		g_apDataAudio[nCntSound] = (BYTE*)malloc(g_aSizeAudio[nCntSound]);
		hr = ReadChunkData(hFile, g_apDataAudio[nCntSound], g_aSizeAudio[nCntSound], dwChunkPosition);
		if(FAILED(hr))
		{
			MessageBox(hWnd, "オーディオデータ読み込みに失敗！(2)", "警告！", MB_ICONWARNING);

			return S_FALSE;
		}
	
		// ソースボイスの生成
		hr = g_pXAudio2->CreateSourceVoice(&g_apSourceVoice[nCntSound], &(wfx.Format));
		if(FAILED(hr))
		{
			MessageBox(hWnd, "ソースボイスの生成に失敗！", "警告！", MB_ICONWARNING);

			return S_FALSE;
		}

		// バッファの値設定
		memset(&buffer, 0, sizeof(XAUDIO2_BUFFER));
		memset(g_aSourceVoiceDetails, 0, sizeof(XAUDIO2_VOICE_DETAILS));
		buffer.AudioBytes = g_aSizeAudio[nCntSound];
		buffer.pAudioData = g_apDataAudio[nCntSound];
		buffer.Flags      = XAUDIO2_END_OF_STREAM;
		buffer.LoopCount  = g_aSoundInfo[nCntSound].nCntLoop;

		// オーディオバッファの登録
		g_apSourceVoice[nCntSound]->SubmitSourceBuffer(&buffer);

		g_apSourceVoice[nCntSound]->GetVoiceDetails(&g_aSourceVoiceDetails[nCntSound]);

		// ファイルをクローズ
		CloseHandle(hFile);
	}

	g_pMasteringVoice->GetVoiceDetails(&g_MasterVoiceDetails);

	for (int nCntSound = 0; nCntSound < SOUND_LABEL_MAX; nCntSound++)
	{
		g_aPlayAudio[nCntSound] = false;
	}

	for (int nCntSound = 0; nCntSound < SOUND_LABEL_MAX; nCntSound++)
	{
		if (g_apSourceVoice[nCntSound] != NULL)
		{
			// 音量を設定
			g_apSourceVoice[nCntSound]->SetVolume(0.0f, XAUDIO2_COMMIT_NOW);
		}
	}

	// 初期音量を代入
	g_fVolumeAudio = 0.0f;

	// フェードイン時の音源を設定(初期)
	g_Fadelabel = SOUND_LABEL_TITLE;

	// フェードアウト時の音源を設定(初期)
	g_FadelabelExac = SOUND_LABEL_TITLE;

	// 再生中の音源の初期化
	g_PlaySound = SOUND_LABEL_TITLE;

	// フェードインに設定
	g_CurrentFadeSound = FADE_IN;

	// カウンターを初期化
	g_nCounterSound = 0;

	return S_OK;
}

//=============================================================================
// 終了処理
//=============================================================================
void UninitSound(void)
{
	// 一時停止
	for(int nCntSound = 0; nCntSound < SOUND_LABEL_MAX; nCntSound++)
	{
		if(g_apSourceVoice[nCntSound] != NULL)
		{
			// 一時停止
			g_apSourceVoice[nCntSound]->Stop(0);
	
			// ソースボイスの破棄
			g_apSourceVoice[nCntSound]->DestroyVoice();
			g_apSourceVoice[nCntSound] = NULL;
			g_aSourceVoiceDetails[nCntSound] = {};
	
			// オーディオデータの開放
			free(g_apDataAudio[nCntSound]);
			g_apDataAudio[nCntSound] = NULL;
		}
	}
	
	// マスターボイスの破棄
	g_pMasteringVoice->DestroyVoice();
	g_pMasteringVoice = NULL;
	g_MasterVoiceDetails = {};
	
	if(g_pXAudio2 != NULL)
	{
		// XAudio2オブジェクトの開放
		g_pXAudio2->Release();
		g_pXAudio2 = NULL;
	}
	
	// COMライブラリの終了処理
	CoUninitialize();
}

//=============================================================================
// 更新処理
//=============================================================================
void UpdateSound(void)
{
	if (g_CurrentFadeSound != FADE_NONE)
	{
		if (g_CurrentFadeSound == FADE_IN && (GetFade() == FADE_IN || GetFadeStage() == FADESTAGE_IN))
		{
			g_fVolumeAudio += 0.01f;
			if (g_nCounterSound == 0)
			{
				PlaySound(g_Fadelabel);
				g_nCounterSound--;
			}
			else
			{
				g_nCounterSound--;
			}

			for (int nCntSound = 0; nCntSound < SOUND_LABEL_MAX; nCntSound++)
			{
				if (g_apSourceVoice[nCntSound] != NULL)
				{
					// 音量を設定
					g_apSourceVoice[nCntSound]->SetVolume(g_fVolumeAudio, XAUDIO2_COMMIT_NOW);
				}
			}

			if (g_fVolumeAudio >= SOUND_STANDARD)
			{
				g_CurrentFadeSound = FADE_NONE;
				g_fVolumeAudio = SOUND_STANDARD;
				for (int nCntSound = 0; nCntSound < SOUND_LABEL_MAX; nCntSound++)
				{
					if (g_apSourceVoice[nCntSound] != NULL)
					{
						// 音量を設定
						g_apSourceVoice[nCntSound]->SetVolume(g_fVolumeAudio, XAUDIO2_COMMIT_NOW);
					}
				}
			}
		}
		else if (g_CurrentFadeSound == FADE_OUT)
		{
			g_fVolumeAudio -= 0.01f;
			for (int nCntSound = 0; nCntSound < SOUND_LABEL_MAX; nCntSound++)
			{
				if (g_apSourceVoice[nCntSound] != NULL)
				{
					// 音量を設定
					g_apSourceVoice[nCntSound]->SetVolume(g_fVolumeAudio, XAUDIO2_COMMIT_NOW);
				}
			}

			if (g_fVolumeAudio <= 0.0f)
			{
				g_CurrentFadeSound = FADE_IN;
				g_fVolumeAudio = 0.0f;
				for (int nCntSound = 0; nCntSound < SOUND_LABEL_MAX; nCntSound++)
				{
					if (g_apSourceVoice[nCntSound] != NULL)
					{
						// 音量を設定
						g_apSourceVoice[nCntSound]->SetVolume(g_fVolumeAudio, XAUDIO2_COMMIT_NOW);
					}
					//float af[12] = { 0.0f,1.0f,0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,0.0,0.0f,0.0f };
					//g_apSourceVoice[nCntSound]->GetVoiceDetails(&g_apSourceVoiceDetails[nCntSound]);
					//float f[51] = {};

					/* Set,GetOutputMatrixの引数(1 : SourceVoiceの宛先(MasterVoice)。複数のMasterVoiceが存在する場合のみ使用。一つのみの場合はNULL
												   2 : SourceVoiceのチャンネル数。初期化時にg_apSourceVoiceDetailsに取得済み。InputChannelsを参照する。
												   3 : MasterVoiceのチャンネル数。初期化時にg_pMasterVoiceDetailsに取得済み。InputChannelsを参照する。複数のMasterVoiceが存在する場合、宛先のMasterVoiceDetailsを使用すること。
												   4 : 各チャンネルの音量。浮動小数の配列のポインタを渡す。数は12だが、オーバーフロー防止に24にしている。
													   配列の各番号がスピーカーの場所を示す(例 : 0 = 左側, 3 = 右側)。Getの場合は配列に現在の音量の状態が代入される。
													   Setの場合は、配列の値を基にSourceVoiceの音量が調整される。)
					*/

					//g_apSourceVoice[nCntSound]->GetOutputMatrix(NULL, g_aSourceVoiceDetails[nCntSound].InputChannels, g_MasterVoiceDetails.InputChannels, &f[0]);
					//g_apSourceVoice[nCntSound]->SetOutputMatrix(g_aSourceVoice[nCntSound], g_aSourceVoiceDetails[nCntSound].InputChannels, g_MasterVoiceDetails.InputChannels, &af[0]);
				}

				g_nCounterSound = 6;

				StopSound();
			}
		}
	}
}

//=============================================================================
// セグメント再生(再生中なら停止)
//=============================================================================
HRESULT PlaySound(SOUND_LABEL label)
{
	XAUDIO2_VOICE_STATE xa2state;
	XAUDIO2_BUFFER buffer;

	// バッファの値設定
	memset(&buffer, 0, sizeof(XAUDIO2_BUFFER));
	buffer.AudioBytes = g_aSizeAudio[label];
	buffer.pAudioData = g_apDataAudio[label];
	buffer.Flags      = XAUDIO2_END_OF_STREAM;
	buffer.LoopCount  = g_aSoundInfo[label].nCntLoop;

	// 状態取得
	g_apSourceVoice[label]->GetState(&xa2state);
	if(xa2state.BuffersQueued != 0)
	{// 再生中
		// 一時停止
		g_apSourceVoice[label]->Stop(0);

		// オーディオバッファの削除
		g_apSourceVoice[label]->FlushSourceBuffers();
	}

	// オーディオバッファの登録
	g_apSourceVoice[label]->SubmitSourceBuffer(&buffer);

	// 再生
	g_apSourceVoice[label]->Start(0);

	// 再生状況をtrueに
	g_aPlayAudio[label] = true;

	if (label >= SOUND_BGMSTART && label < SOUND_SESTART)
	{// 再生するのがBGMだった場合
		g_PlaySound = label;
	}

	return S_OK;
}

//=============================================================================
// セグメント停止(ラベル指定)
//=============================================================================
void StopSound(SOUND_LABEL label)
{
	XAUDIO2_VOICE_STATE xa2state;

	// 状態取得
	g_apSourceVoice[label]->GetState(&xa2state);
	if(xa2state.BuffersQueued != 0)
	{// 再生中
		// 一時停止
		g_apSourceVoice[label]->Stop(0);

		// オーディオバッファの削除
		g_apSourceVoice[label]->FlushSourceBuffers();

		// 再生状況をfalseに
		g_aPlayAudio[label] = false;
	}
}

//=============================================================================
// セグメント停止(全て)
//=============================================================================
void StopSound(void)
{
	// 一時停止
	for(int nCntSound = 0; nCntSound < SOUND_LABEL_MAX; nCntSound++)
	{
		if(g_apSourceVoice[nCntSound] != NULL)
		{
			// 一時停止
			g_apSourceVoice[nCntSound]->Stop(0);
			// 再生状況をfalseに
			g_aPlayAudio[nCntSound] = false;
		}
	}
}

//=============================================================================
// チャンクのチェック
//=============================================================================
HRESULT CheckChunk(HANDLE hFile, DWORD format, DWORD *pChunkSize, DWORD *pChunkDataPosition)
{
	HRESULT hr = S_OK;
	DWORD dwRead;
	DWORD dwChunkType;
	DWORD dwChunkDataSize;
	DWORD dwRIFFDataSize = 0;
	DWORD dwFileType;
	DWORD dwBytesRead = 0;
	DWORD dwOffset = 0;
	
	if(SetFilePointer(hFile, 0, NULL, FILE_BEGIN) == INVALID_SET_FILE_POINTER)
	{// ファイルポインタを先頭に移動
		return HRESULT_FROM_WIN32(GetLastError());
	}
	
	while(hr == S_OK)
	{
		if(ReadFile(hFile, &dwChunkType, sizeof(DWORD), &dwRead, NULL) == 0)
		{// チャンクの読み込み
			hr = HRESULT_FROM_WIN32(GetLastError());
		}

		if(ReadFile(hFile, &dwChunkDataSize, sizeof(DWORD), &dwRead, NULL) == 0)
		{// チャンクデータの読み込み
			hr = HRESULT_FROM_WIN32(GetLastError());
		}

		switch(dwChunkType)
		{
		case 'FFIR':
			dwRIFFDataSize  = dwChunkDataSize;
			dwChunkDataSize = 4;
			if(ReadFile(hFile, &dwFileType, sizeof(DWORD), &dwRead, NULL) == 0)
			{// ファイルタイプの読み込み
				hr = HRESULT_FROM_WIN32(GetLastError());
			}
			break;

		default:
			if(SetFilePointer(hFile, dwChunkDataSize, NULL, FILE_CURRENT) == INVALID_SET_FILE_POINTER)
			{// ファイルポインタをチャンクデータ分移動
				return HRESULT_FROM_WIN32(GetLastError());
			}
		}

		dwOffset += sizeof(DWORD) * 2;
		if(dwChunkType == format)
		{
			*pChunkSize         = dwChunkDataSize;
			*pChunkDataPosition = dwOffset;

			return S_OK;
		}

		dwOffset += dwChunkDataSize;
		if(dwBytesRead >= dwRIFFDataSize)
		{
			return S_FALSE;
		}
	}
	
	return S_OK;
}

//=============================================================================
// チャンクデータの読み込み
//=============================================================================
HRESULT ReadChunkData(HANDLE hFile, void *pBuffer, DWORD dwBuffersize, DWORD dwBufferoffset)
{
	DWORD dwRead;
	
	if(SetFilePointer(hFile, dwBufferoffset, NULL, FILE_BEGIN) == INVALID_SET_FILE_POINTER)
	{// ファイルポインタを指定位置まで移動
		return HRESULT_FROM_WIN32(GetLastError());
	}

	if(ReadFile(hFile, pBuffer, dwBuffersize, &dwRead, NULL) == 0)
	{// データの読み込み
		return HRESULT_FROM_WIN32(GetLastError());
	}
	
	return S_OK;
}

//=============================================================================
// 音量の設定
//=============================================================================
void SetVolume(float volume, SETSOUND sound)
{
	switch (sound)
	{

	case SETSOUND_GAME:

		for (int nCntSound = SOUND_BGMSTART; nCntSound < SOUND_MENUSTART; nCntSound++)
		{
			if (g_apSourceVoice[nCntSound] != NULL)
			{
				// 音量を設定
				g_apSourceVoice[nCntSound]->SetVolume(volume, XAUDIO2_COMMIT_NOW);
			}
		}

		break;

	case SETSOUND_MENU_SE:

		for (int nCntSound = SOUND_MENUSTART; nCntSound < SOUND_LABEL_MAX; nCntSound++)
		{
			if (g_apSourceVoice[nCntSound] != NULL)
			{
				// 音量を設定
				g_apSourceVoice[nCntSound]->SetVolume(volume, XAUDIO2_COMMIT_NOW);
			}
		}

		break;

	case SETSOUND_MAX:

		for (int nCntSound = 0; nCntSound < SOUND_LABEL_MAX; nCntSound++)
		{
			if (g_apSourceVoice[nCntSound] != NULL)
			{
				// 音量を設定
				g_apSourceVoice[nCntSound]->SetVolume(volume, XAUDIO2_COMMIT_NOW);
			}
		}

		break;

	default:
		break;
	}

	g_fVolumeAudio = volume;
}

//=============================================================================
// 音量の取得
//=============================================================================
float GetVolume(SETSOUND sound)
{
	float fVolume = 0.0f;

	if (sound == SETSOUND_GAME)
	{
		g_apSourceVoice[SOUND_BGMSTART]->GetVolume(&fVolume);
	}
	else if (sound == SETSOUND_MENU_SE)
	{
		g_apSourceVoice[SOUND_MENUSTART]->GetVolume(&fVolume);
	}
	
	return fVolume;
}

//=============================================================================
// 音楽のフェード処理
//=============================================================================
void FadeSound(SOUND_LABEL label)
{
	g_CurrentFadeSound = FADE_OUT;			// フェードアウト状態に
	g_Fadelabel = label;					// フェードイン時のBGMを指定
	g_FadelabelExac = g_PlaySound;			// フェードアウト時のBGMを取得
}

//=============================================================================
// 現在再生中のBGMの取得
//=============================================================================
SOUND_LABEL GetPlaySound(void)
{
	return g_PlaySound;
}