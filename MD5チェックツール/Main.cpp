// Main.cpp : アプリケーションのエントリ ポイントを定義します。
//
#include "stdafx.h"
#include <winioctl.h>
#include "..\shared\WindowSize.h"

// グローバル変数の定義します:
INITCOMMONCONTROLSEX InitCtrls;
MainWindow tagMainWindow1;
DWORD dwAppFrag = 0;
DWORD dwIsAppFrag = 0;
DWORD dwAppState = (DWORD)-1;
DWORD dwHashCpuMode = 0;
DWORD dwHashFileOutToCopyFolderMode = 0;
unsigned int OPENSSL_ia32cap_P1[4];
int nWindowX = 0;
int nWindowY = 0;
int nEditMarginX[] = { 0, 0, 0, 0 };
int nEditMarginY[] = { 0, 0, 0, 0 };
int nStringX[] = { 12, 16, 16 };
int nStringY[] = { 12, 16, 16 };
int nHashX[MAX_HASH_TYPE + 1] = { 288, 360, 504, 576, 432, 864, 1152, 504, 576, 504, 576, 864, 1152, 288, 288, 288, 288, 288 };
int nHashY[MAX_HASH_TYPE + 1] = { 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16 };
//int nHashX_YMode[MAX_HASH_TYPE + 1] = { 288, 360, 504, 576, 432, 864, 1152, 504, 576, 504, 576, 864, 1152, 288, 288, 288, 288, 288 };
int nHashY_YMode[MAX_HASH_TYPE + 1] = { 16, 32, 32, 32, 32, 48, 64, 32, 32, 32, 32, 48, 64, 16, 16, 16, 16, 16 };
int nHashY_Rate[MAX_HASH_TYPE + 1] = { 1, 2, 2, 2, 2, 3, 4, 2, 2, 2, 2, 3, 4, 1, 1, 1, 1, 1 };
int nMLModeDiff[4] = { 0 };

unsigned int(WINAPI* HashThread_MultiFile_Thread_Func)(void* lpThread) = HashThread_MultiFile_Thread;

#if _MSC_VER < 1500
BOOL WINAPI SetLayeredWindowAttributes_Func1(HWND hWnd, COLORREF crKey, BYTE bAlpha, DWORD dwFlags)
{
	UNREFERENCED_PARAMETER(hWnd);
	UNREFERENCED_PARAMETER(crKey);
	UNREFERENCED_PARAMETER(bAlpha);
	UNREFERENCED_PARAMETER(dwFlags);
	return 0;
}
BOOL(WINAPI* SetLayeredWindowAttributes_Func)(HWND, COLORREF, BYTE, DWORD) = SetLayeredWindowAttributes_Func1;
#else
BOOL(WINAPI* SetLayeredWindowAttributes_Func)(HWND, COLORREF, BYTE, DWORD) = SetLayeredWindowAttributes;
#endif /* _MSC_VER < 1500 */

// 外部ファイルの変数定義します。
extern FileListWindow tagFileListWindow1;
extern AboutWindow tagAboutWindow1;
extern OptionWindow tagOptionWindow1;
extern DebugWindow tagDebugWindow1;
extern BOOL g_darkModeSupported;
extern BOOL g_darkModeEnabled;
extern const DWORD cdwHashLen[MAX_HASH_TYPE + 1];


int GetStringWidth(HDC hDC)
{
	static TCHAR szHash[] = _T("bbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbb");
	HFONT hfonPrev[3] = { NULL };
	SIZE sz = { 0 };
	int i = 0;
#ifdef _DEBUG
	TCHAR szDebugText[1024];
#endif

	IF_UNLIKELY(hDC == NULL)
	{
		for (i = 0; i < SIZEOF_NUM(nStringX); i++)
		{
			nStringX[i] = 0;
			nStringY[i] = 0;
		}
		return FALSE;
	}

	hfonPrev[0] = (HFONT)SelectObject(hDC, tagMainWindow1.hFont[0]);
	GetTextExtentPoint32(hDC, szHash, 2, &sz);
	nStringX[0] = sz.cx;
	nStringY[0] = sz.cy;

	hfonPrev[1] = (HFONT)SelectObject(hDC, tagMainWindow1.hFont[1]);
	GetTextExtentPoint32(hDC, szHash, 2, &sz);
	nStringX[1] = sz.cx;
	nStringY[1] = sz.cy;

	hfonPrev[2] = (HFONT)SelectObject(hDC, tagMainWindow1.hFont[2]);
	GetTextExtentPoint32(hDC, szHash, 2, &sz);
	nStringX[2] = sz.cx;
	nStringY[2] = sz.cy;

	for (i = 0; i < MAX_HASH_TYPE; i++)
	{
		const DWORD dwHashLen = (cdwHashLen[i] >= 16) ? cdwHashLen[i] : 16;
		TCHAR* ptr = NULL;
		int n = 0;

		ptr = (size_t)dwHashLen * 2 + szHash;
		*ptr = '\0';
		GetTextExtentPoint32(hDC, szHash, dwHashLen * 2, &sz);
		nHashX[i] = sz.cx;
		nHashY[i] = sz.cy;
		n = INT_CEIL(dwHashLen, 16);
		nHashY_Rate[i] = n;
		nHashY_YMode[i] = nHashY[i] * n;
		*ptr = 'b';

#ifdef _DEBUG
		STPRINTF_FUNC(szDebugText, _T("nHashX[%d] = %d, nHashY[%d] = %d, nHashY_Rate[%d] = %d, nHashY_YMode[%d] = %d; // [%s]\r\n"), i, nHashX[i], i, nHashY[i], i, nHashY_Rate[i], i, nHashY_YMode[i], HashThread_GetHashName(i));
		OutputDebugString(szDebugText);
#endif

	}

	SelectObject(hDC, hfonPrev[2]);
	SelectObject(hDC, hfonPrev[1]);
	SelectObject(hDC, hfonPrev[0]);
	return TRUE;
}



#define DIALOG1_X 258
#define DIALOG1_Y 171

enum
{
	SHA_1_OPTION = CHAR_MAX + 1,
	SHA_224_OPTION,
	SHA_256_OPTION,
	SHA_256_192_OPTION,
	SHA_384_OPTION,
	SHA_512_OPTION,
	SHA_512_224_OPTION,
	SHA_512_256_OPTION,
	SHA3_224_OPTION,
	SHA3_256_OPTION,
	SHA3_384_OPTION,
	SHA3_512_OPTION,
	DEBUG_OPTION
};

static struct option const long_options[] = {
	{ _T("add"), no_argument, NULL, 'a'},
	{ _T("filelist"), no_argument, NULL, 'f'},
	{ _T("md5"), no_argument, NULL, 'm'},
	{ _T("sha"), required_argument, NULL, 's'},
	{ _T("hash"), required_argument, NULL, 'h'},
	{ NULL, 0, NULL, 0 }
};

int MainWindow_Free()
{
	int i = 0;

	free(tagMainWindow1.pBuf);
	tagMainWindow1.pBuf = NULL;

	free(tagMainWindow1.pStBuf);
	tagMainWindow1.pStBuf = NULL;

	free(tagMainWindow1.pTempBuf);
	tagMainWindow1.pTempBuf = NULL;

	_AlignedFree(tagMainWindow1.szIconRes_Load);
	tagMainWindow1.szIconRes_Load = NULL;

	free(tagMainWindow1.pTitle);
	tagMainWindow1.pTitle = NULL;

	free(tagMainWindow1.pWindowClass);
	tagMainWindow1.pWindowClass = NULL;

	free(tagMainWindow1.pVersion);
	tagMainWindow1.pVersion = NULL;

	free(tagMainWindow1.pCopyRight);
	tagMainWindow1.pCopyRight = NULL;

	free(tagMainWindow1.pStatusTitle);
	tagMainWindow1.pStatusTitle = NULL;

	free(tagMainWindow1.pFileNameEdit);
	tagMainWindow1.pFileNameEdit = NULL;

	free(tagMainWindow1.pClipboardString);
	tagMainWindow1.pClipboardString = NULL;

	free(tagMainWindow1.pInputtext);
	tagMainWindow1.pInputtext = NULL;

	free(tagMainWindow1.pFile);
	tagMainWindow1.pFile = NULL;

	free(tagMainWindow1.pHashFile);
	tagMainWindow1.pHashFile = NULL;

	free(tagMainWindow1.pINIFile);
	tagMainWindow1.pINIFile = NULL;

	free(tagMainWindow1.pFileExtension);
	tagMainWindow1.pFileExtension = NULL;

	free(tagMainWindow1.pCheckSumFile);
	tagMainWindow1.pCheckSumFile = NULL;

	free(tagMainWindow1.pNotFoundFile);
	tagMainWindow1.pNotFoundFile = NULL;

	free(tagMainWindow1.pEmptyFolder);
	tagMainWindow1.pEmptyFolder = NULL;

	free(tagMainWindow1.pHashNotFoundFile);
	tagMainWindow1.pHashNotFoundFile = NULL;

	free(tagMainWindow1.pTimeText);
	tagMainWindow1.pTimeText = NULL;

	free(tagMainWindow1.pTimeView1_TransPerSec);
	tagMainWindow1.pTimeView1_TransPerSec = NULL;

	for (i = 0; i < SIZEOF_NUM(tagMainWindow1.pTimeView1_Size); i++)
	{
		free(tagMainWindow1.pTimeView1_Size[i]);
		tagMainWindow1.pTimeView1_Size[i] = NULL;
	}

	free(tagMainWindow1.pTimeView1_Time);
	tagMainWindow1.pTimeView1_Time = NULL;

	free(tagMainWindow1.pOfn1Buf);
	tagMainWindow1.pOfn1Buf = NULL;

	for (i = 0; i < SIZEOF_NUM(tagMainWindow1.pWindowTitle); i++)
	{
		free(tagMainWindow1.pWindowTitle[i]);
		tagMainWindow1.pWindowTitle[i] = NULL;
	}

	for (i = 0; i < SIZEOF_NUM(tagMainWindow1.pStatusText); i++)
	{
		free(tagMainWindow1.pStatusText[i]);
		tagMainWindow1.pStatusText[i] = NULL;
	}

	for (i = 0; i < SIZEOF_NUM(tagMainWindow1.pButtonText1); i++)
	{
		free(tagMainWindow1.pButtonText1[i]);
		tagMainWindow1.pButtonText1[i] = NULL;
	}

	for (i = 0; i < SIZEOF_NUM(tagMainWindow1.pButtonText2); i++)
	{
		free(tagMainWindow1.pButtonText2[i]);
		tagMainWindow1.pButtonText2[i] = NULL;
	}

	for (i = 0; i < SIZEOF_NUM(tagMainWindow1.pToolTipText); i++)
	{
		free(tagMainWindow1.pToolTipText[i]);
		tagMainWindow1.pToolTipText[i] = NULL;
	}

	for (i = 0; i < SIZEOF_NUM(tagMainWindow1.pTimeText1); i++)
	{
		free(tagMainWindow1.pTimeText1[i]);
		tagMainWindow1.pTimeText1[i] = NULL;
	}

	for (i = 0; i < SIZEOF_NUM(tagMainWindow1.pBase64_Msg); i++)
	{
		free(tagMainWindow1.pBase64_Msg[i]);
		tagMainWindow1.pBase64_Msg[i] = NULL;
	}

	free(tagMainWindow1.pBecthResult);
	tagMainWindow1.pBecthResult = NULL;

	free(tagMainWindow1.pCmpErrorCount);
	tagMainWindow1.pCmpErrorCount = NULL;

	return TRUE;
}

int MainWindow_Malloc(size_t nSize)
{
	int i = 0;

	tagMainWindow1.pBuf = (TCHAR*)malloc(nSize * sizeof(TCHAR*));
	IF_UNLIKELY(tagMainWindow1.pBuf == NULL) {
		goto MALLOC_ERROR;
	}
	*tagMainWindow1.pBuf = '\0';

	tagMainWindow1.pStBuf = (TCHAR*)malloc(1024 * sizeof(TCHAR*));
	IF_UNLIKELY(tagMainWindow1.pStBuf == NULL) {
		goto MALLOC_ERROR;
	}
	*tagMainWindow1.pStBuf = '\0';

	tagMainWindow1.pTempBuf = (TCHAR*)malloc(MAX_PATH_SIZE * sizeof(TCHAR*));
	IF_UNLIKELY(tagMainWindow1.pTempBuf == NULL) {
		goto MALLOC_ERROR;
	}
	*tagMainWindow1.pTempBuf = '\0';

	tagMainWindow1.szIconRes_Load = (unsigned char*)_AlignedMalloc(32, 16);
	IF_UNLIKELY(tagMainWindow1.szIconRes_Load == NULL) {
		goto MALLOC_ERROR;
	}
	*tagMainWindow1.pTempBuf = '\0';

	tagMainWindow1.pTitle = (TCHAR*)malloc(MAX_LOADSTRING * sizeof(TCHAR*));
	IF_UNLIKELY(tagMainWindow1.pTitle == NULL) {
		goto MALLOC_ERROR;
	}
	*tagMainWindow1.pTitle = '\0';

	tagMainWindow1.pWindowClass = (TCHAR*)malloc(MAX_LOADSTRING * sizeof(TCHAR*));
	IF_UNLIKELY(tagMainWindow1.pWindowClass == NULL) {
		goto MALLOC_ERROR;
	}
	*tagMainWindow1.pWindowClass = '\0';

	tagMainWindow1.pVersion = (TCHAR*)malloc(MAX_LOADSTRING * sizeof(TCHAR*));
	IF_UNLIKELY(tagMainWindow1.pVersion == NULL) {
		goto MALLOC_ERROR;
	}
	*tagMainWindow1.pVersion = '\0';

	tagMainWindow1.pCopyRight = (TCHAR*)malloc(MAX_LOADSTRING * sizeof(TCHAR*));
	IF_UNLIKELY(tagMainWindow1.pCopyRight == NULL) {
		goto MALLOC_ERROR;
	}
	*tagMainWindow1.pCopyRight = '\0';

	tagMainWindow1.pStatusTitle = (TCHAR*)malloc(MAX_STRINGTABLE * sizeof(TCHAR*));
	IF_UNLIKELY(tagMainWindow1.pStatusTitle == NULL) {
		goto MALLOC_ERROR;
	}
	*tagMainWindow1.pStatusTitle = '\0';

	tagMainWindow1.pFileNameEdit = (TCHAR*)malloc(_MAX_FNAME * sizeof(TCHAR*));
	IF_UNLIKELY(tagMainWindow1.pFileNameEdit == NULL) {
		goto MALLOC_ERROR;
	}
	*tagMainWindow1.pFileNameEdit = '\0';

	tagMainWindow1.pClipboardString = (TCHAR*)malloc(HASH_LOADSTRING * 2 * sizeof(TCHAR*));
	IF_UNLIKELY(tagMainWindow1.pClipboardString == NULL) {
		goto MALLOC_ERROR;
	}
	*tagMainWindow1.pClipboardString = '\0';

	tagMainWindow1.pInputtext = (TCHAR*)malloc(HASH_LOADSTRING * sizeof(TCHAR*));
	IF_UNLIKELY(tagMainWindow1.pInputtext == NULL) {
		goto MALLOC_ERROR;
	}
	*tagMainWindow1.pInputtext = '\0';

	tagMainWindow1.pFile = (TCHAR*)malloc(MAX_PATH_SIZE * sizeof(TCHAR*));
	IF_UNLIKELY(tagMainWindow1.pFile == NULL) {
		goto MALLOC_ERROR;
	}
	*tagMainWindow1.pFile = '\0';

	tagMainWindow1.pHashFile = (TCHAR*)malloc(MAX_PATH_SIZE * sizeof(TCHAR*));
	IF_UNLIKELY(tagMainWindow1.pHashFile == NULL) {
		goto MALLOC_ERROR;
	}
	*tagMainWindow1.pHashFile = '\0';

	tagMainWindow1.pINIFile = (TCHAR*)malloc(MAX_PATH_SIZE * sizeof(TCHAR*));
	IF_UNLIKELY(tagMainWindow1.pINIFile == NULL) {
		goto MALLOC_ERROR;
	}
	*tagMainWindow1.pINIFile = '\0';

	tagMainWindow1.pFileExtension = (TCHAR*)malloc(_MAX_EXT * sizeof(TCHAR*));
	IF_UNLIKELY(tagMainWindow1.pFileExtension == NULL) {
		goto MALLOC_ERROR;
	}
	*tagMainWindow1.pFileExtension = '\0';

	tagMainWindow1.pCheckSumFile = (TCHAR*)malloc(1024 * sizeof(TCHAR*));
	IF_UNLIKELY(tagMainWindow1.pCheckSumFile == NULL) {
		goto MALLOC_ERROR;
	}
	*tagMainWindow1.pCheckSumFile = '\0';
	tagMainWindow1.ppCheckSumFile[0] = tagMainWindow1.pCheckSumFile;

	tagMainWindow1.pNotFoundFile = (TCHAR*)malloc(MAX_PATH_SIZE * 10 * sizeof(TCHAR*));
	IF_UNLIKELY(tagMainWindow1.pNotFoundFile == NULL) {
		goto MALLOC_ERROR;
	}
	*tagMainWindow1.pNotFoundFile = '\0';

	tagMainWindow1.pEmptyFolder = (TCHAR*)malloc(MAX_PATH_SIZE * 10 * sizeof(TCHAR*));
	IF_UNLIKELY(tagMainWindow1.pEmptyFolder == NULL) {
		goto MALLOC_ERROR;
	}
	*tagMainWindow1.pEmptyFolder = '\0';

	tagMainWindow1.pHashNotFoundFile = (TCHAR*)malloc(MAX_PATH_SIZE * 10 * sizeof(TCHAR*));
	IF_UNLIKELY(tagMainWindow1.pHashNotFoundFile == NULL) {
		goto MALLOC_ERROR;
	}
	*tagMainWindow1.pHashNotFoundFile = '\0';

	tagMainWindow1.pTimeText = (TCHAR*)malloc(MAX_LOADSTRING * sizeof(TCHAR*));
	IF_UNLIKELY(tagMainWindow1.pTimeText == NULL) {
		goto MALLOC_ERROR;
	}
	*tagMainWindow1.pTimeText = '\0';

	tagMainWindow1.pTimeView1_TransPerSec = (TCHAR*)malloc(NUMBER_LOADSTRING * sizeof(TCHAR*));
	IF_UNLIKELY(tagMainWindow1.pTimeView1_TransPerSec == NULL) {
		goto MALLOC_ERROR;
	}
	*tagMainWindow1.pTimeView1_TransPerSec = '\0';

	for (i = 0; i < SIZEOF_NUM(tagMainWindow1.pTimeView1_Size); i++)
	{
		tagMainWindow1.pTimeView1_Size[i] = (TCHAR*)malloc(NUMBER_LOADSTRING * sizeof(TCHAR*));
		IF_UNLIKELY(tagMainWindow1.pTimeView1_Size[i] == NULL) {
			goto MALLOC_ERROR;
		}
		*tagMainWindow1.pTimeView1_Size[i] = '\0';
	}

	tagMainWindow1.pTimeView1_Time = (TCHAR*)malloc(NUMBER_LOADSTRING * sizeof(TCHAR*));
	IF_UNLIKELY(tagMainWindow1.pTimeView1_Time == NULL) {
		goto MALLOC_ERROR;
	}
	*tagMainWindow1.pTimeView1_Time = '\0';

	tagMainWindow1.pOfn1Buf = (TCHAR*)malloc(MAX_PATH_SIZE * 256 * sizeof(TCHAR*));
	IF_UNLIKELY(tagMainWindow1.pOfn1Buf == NULL) {
		goto MALLOC_ERROR;
	}
	*tagMainWindow1.pOfn1Buf = '\0';

	for (i = 0; i < SIZEOF_NUM(tagMainWindow1.pWindowTitle); i++)
	{
		tagMainWindow1.pWindowTitle[i] = (TCHAR*)malloc(MAX_STRINGTABLE * sizeof(TCHAR*));
		IF_UNLIKELY(tagMainWindow1.pWindowTitle[i] == NULL) {
			goto MALLOC_ERROR;
		}
		*tagMainWindow1.pWindowTitle[i] = '\0';
	}

	for (i = 0; i < SIZEOF_NUM(tagMainWindow1.pStatusText); i++)
	{
		tagMainWindow1.pStatusText[i] = (TCHAR*)malloc(MAX_LOADSTRING * sizeof(TCHAR*));
		IF_UNLIKELY(tagMainWindow1.pStatusText[i] == NULL) {
			goto MALLOC_ERROR;
		}
		*tagMainWindow1.pStatusText[i] = '\0';
	}

	for (i = 0; i < SIZEOF_NUM(tagMainWindow1.pButtonText1); i++)
	{
		tagMainWindow1.pButtonText1[i] = (TCHAR*)malloc(NUMBER_LOADSTRING * sizeof(TCHAR*));
		IF_UNLIKELY(tagMainWindow1.pButtonText1[i] == NULL) {
			goto MALLOC_ERROR;
		}
		*tagMainWindow1.pButtonText1[i] = '\0';
	}

	for (i = 0; i < SIZEOF_NUM(tagMainWindow1.pButtonText2); i++)
	{
		tagMainWindow1.pButtonText2[i] = (TCHAR*)malloc(NUMBER_LOADSTRING * sizeof(TCHAR*));
		IF_UNLIKELY(tagMainWindow1.pButtonText2[i] == NULL) {
			goto MALLOC_ERROR;
		}
		*tagMainWindow1.pButtonText2[i] = '\0';
	}

	for (i = 0; i < SIZEOF_NUM(tagMainWindow1.pToolTipText); i++)
	{
		tagMainWindow1.pToolTipText[i] = (TCHAR*)malloc(MAX_LOADSTRING * sizeof(TCHAR*));
		IF_UNLIKELY(tagMainWindow1.pToolTipText[i] == NULL) {
			goto MALLOC_ERROR;
		}
		*tagMainWindow1.pToolTipText[i] = '\0';
	}

	for (i = 0; i < SIZEOF_NUM(tagMainWindow1.pTimeText1); i++)
	{
		tagMainWindow1.pTimeText1[i] = (TCHAR*)malloc(MAX_LOADSTRING * sizeof(TCHAR*));
		IF_UNLIKELY(tagMainWindow1.pTimeText1[i] == NULL) {
			goto MALLOC_ERROR;
		}
		*tagMainWindow1.pTimeText1[i] = '\0';
	}

	for (i = 0; i < SIZEOF_NUM(tagMainWindow1.pBase64_Msg); i++)
	{
		tagMainWindow1.pBase64_Msg[i] = (TCHAR*)malloc(HASH_LOADSTRING * sizeof(TCHAR*));
		IF_UNLIKELY(tagMainWindow1.pBase64_Msg[i] == NULL) {
			goto MALLOC_ERROR;
		}
		*tagMainWindow1.pBase64_Msg[i] = '\0';
	}

	tagMainWindow1.pBecthResult = (TCHAR*)malloc(1024 * sizeof(TCHAR*));
	IF_UNLIKELY(tagMainWindow1.pBecthResult == NULL) {
		goto MALLOC_ERROR;
	}
	*tagMainWindow1.pBecthResult = '\0';
	return TRUE;

MALLOC_ERROR:
	MainWindow_Free();
	return FALSE;
}


int APIENTRY _tWinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPTSTR lpCmdLine, int nCmdShow)
{
	UNREFERENCED_PARAMETER(hPrevInstance);
	UNREFERENCED_PARAMETER(lpCmdLine);

	// TODO: ここにコードを挿入してください。
	HACCEL hAccelTable = NULL;
	MSG msg = { 0 };
	const TCHAR* pMsg = NULL;
	TCHAR* ptr = NULL;
	DWORD dwErr = 0;
	DWORD dwLastErr = 0;
	DWORD dwArgHashType = (DWORD)-1;
	HRESULT hr = 0;
	int i = 0;
	int opt = 0;

#if _MSC_VER < 1500
	HMODULE hUser32 = NULL;
#endif /* __MSC_VER < 1500 */

	_tsetlocale(LC_ALL, _T("Japanese_Japan.932"));

	// オプション解析
	while ((opt = getopt_long(__argc, __targv, _T("afms:h:"), long_options, NULL)) != -1)
	{
		switch (opt)
		{
		case 'a':
			dwAppFrag |= APP_HASHFILE_ADDMODE;
			break;
		case 'f':
			dwAppFrag |= APP_ARG_FILELIST;
			break;
		case 'm':
			dwArgHashType = 0;
			dwAppFrag |= APP_ARG_HASHTYPE;
			break;
		case 's':
			switch (*optarg)
			{
			case '1':
				dwArgHashType = HASH_SHA1_TYPE;
				break;
			case '2':
				dwArgHashType = HASH_SHA_256_TYPE;
				break;
			case '3':
				dwArgHashType = HASH_SHA_384_TYPE;
				break;
			case '5':
				dwArgHashType = HASH_SHA_512_TYPE;
				break;
			}
			dwAppFrag |= APP_ARG_HASHTYPE;
			break;
		case 'h':
			dwArgHashType = _ttoi(optarg);
			dwAppFrag |= APP_ARG_HASHTYPE;
			if (dwArgHashType > MAX_HASH_TYPE) {
				dwArgHashType = (DWORD)-1;
			}
			break;
		}
	}

	IF_UNLIKELY(!MainWindow_Malloc(10240))
	{
		pMsg = _T("WinMain: MainWindow_Malloc()");
		dwErr = DEBUG_ALLOC_ERROR;
		goto APPEXIT;
	}


	for (i = 0; i < SIZEOF_NUM(tagMainWindow1.pWindowTitle); i++) {
		LoadString(hInstance, IDS_WINDOW_TITLE1 + i, tagMainWindow1.pWindowTitle[i], MAX_STRINGTABLE);
	}

	for (i = 0; i < SIZEOF_NUM(tagMainWindow1.pButtonText1); i++) {
		LoadString(hInstance, IDS_BUTTON1_TEXT1 + i, tagMainWindow1.pButtonText1[i], NUMBER_LOADSTRING);
	}

	for (i = 0; i < SIZEOF_NUM(tagMainWindow1.pButtonText2); i++) {
		LoadString(hInstance, IDS_BUTTON2_TEXT1 + i, tagMainWindow1.pButtonText2[i], NUMBER_LOADSTRING);
	}

	for (i = 0; i < SIZEOF_NUM(tagMainWindow1.pToolTipText); i++) {
		LoadString(hInstance, IDS_TOOLTIP_MSG1 + i, tagMainWindow1.pToolTipText[i], MAX_LOADSTRING);
	}

	for (i = 0; i < SIZEOF_NUM(tagMainWindow1.pStatusText); i++) {
		LoadString(hInstance, IDS_STATUS1 + i, tagMainWindow1.pStatusText[i], NUMBER_LOADSTRING);
	}

	for (i = 0; i < SIZEOF_NUM(tagMainWindow1.pTimeText1); i++) {
		LoadString(hInstance, IDS_TIME_TEXT1 + i, tagMainWindow1.pTimeText1[i], MAX_LOADSTRING);
	}

	LoadString(hInstance, IDS_BASE64_MSG1, tagMainWindow1.pBase64_Msg[0], MAX_LOADSTRING);

	SetSystemDllToTChar();

#if _MSC_VER < 1500
	if (IsWin2kOrGreater())
	{
		hUser32 = LOADSYSLIB(_T("USER32.DLL"));
		if (hUser32 != NULL)
		{
			const char* szFunc = "SetLayeredWindowAttributes";
			SetLayeredWindowAttributes_Func = (BOOL(WINAPI*)(HWND, COLORREF, BYTE, DWORD))(GetProcAddress(hUser32, szFunc));
		}
	}
#endif /* __MSC_VER < 1500 */

	// グローバル文字列を初期化しています。
	OPENSSL_cpuid_setup1();

#if !defined(UNICODE) && _MSC_VER < 1400
	// Windows 95はSSEを使わないようにする
	if (IsWin95()) {
		OPENSSL_ia32cap_P[1] &= ~(0x00000200);
	}
#endif

	memcpy(OPENSSL_ia32cap_P1, OPENSSL_ia32cap_P, sizeof(OPENSSL_ia32cap_P1));
	tagMainWindow1.hInst = hInstance;// グローバル変数にインスタンス処理を格納します。

	i = LoadString(hInstance, IDS_APP_TITLE, tagMainWindow1.pTitle, MAX_LOADSTRING - 1);

#if defined(_M_X64) && defined(_DEBUG)
	ptr = tagMainWindow1.pTitle + i;
	qtcscpy(ptr, _T(" [x64, Debug]"));
#elif defined(_DEBUG)
	ptr = tagMainWindow1.pTitle + i;
	qtcscpy(ptr, _T(" [Debug]"));
#endif

	LoadString(hInstance, IDC_HASH, tagMainWindow1.pWindowClass, MAX_LOADSTRING - 1);
	LoadString(hInstance, IDS_STATUS_TITLE1, tagMainWindow1.pStatusTitle, MAX_STRINGTABLE - 1);
	LoadString(hInstance, IDS_CHACKSUM_FILENAME, tagMainWindow1.pCheckSumFile, MAX_STRINGTABLE - 1);

	memset(&tagMainWindow1.tagHashThread1, 0, sizeof(tagMainWindow1.tagHashThread1));
	memset(&tagMainWindow1.ofnFileName[0], 0, sizeof(tagMainWindow1.ofnFileName[0]));
	memset(&tagMainWindow1.ofnFileName[1], 0, sizeof(tagMainWindow1.ofnFileName[1]));

	hr = CoInitialize(NULL);
	IF_UNLIKELY(FAILED(hr))
	{
		pMsg = _T("WinMain: CoInitialize()");
		dwErr = DEBUG_MISSING;
		dwLastErr = GetLastError();
		goto APPEXIT;
	}

	i = (int)GetModuleFileName(NULL, tagMainWindow1.pTempBuf, MAX_PATH_SIZE - 1);
	IF_UNLIKELY(!i)
	{
		pMsg = _T("WinMain: GetModuleFileName()");
		dwErr = DEBUG_MISSING;
		dwLastErr = GetLastError();
		goto APPEXIT;
	}

	// バージョンの更新
	ptr = GetFileVersion(tagMainWindow1.pBuf, tagMainWindow1.pTempBuf);
	IF_LIKELY(ptr != NULL)
	{
		LoadString(tagMainWindow1.hInst, IDS_VERSION, tagMainWindow1.pStBuf, MAX_STRINGTABLE - 1);
		MessageFormat(tagMainWindow1.pVersion, 2048, tagMainWindow1.pStBuf, tagMainWindow1.pBuf);
	}

	// Copyrightの更新
	ptr = GetFileVersion(tagMainWindow1.pCopyRight, tagMainWindow1.pTempBuf, _T("\\StringFileInfo\\041104b0\\LegalCopyright"));
	IF_LIKELY(ptr != NULL && (ptr - tagMainWindow1.pTempBuf) >= 4)
	{
		time_t time1 = 0;

#ifndef _NODLL
		struct tm tm1 = { 0 };
		errno_t error = 0;
#else
		struct tm* ltm1 = NULL;
#endif

		time(&time1);
		ptr -= 4;

#ifndef _NODLL
		error = localtime_s(&tm1, &time1);
		IF_LIKELY(!error) {
			_itot_s(tm1.tm_year + 1900, ptr, 5, 10);
		}
#else
		ltm1 = localtime(&time1);
		IF_LIKELY(ltm1 != NULL) {
			_itot(ltm1->tm_year + 1900, ptr, 10);
		}
#endif

	}

	tagMainWindow1.nTime = time(NULL) / 86400;
	ptr = GetTCharToExtension(tagMainWindow1.pTempBuf);
	qtcscpy(ptr, _T("IconRes.dll"));
	tagMainWindow1.hDllInst = LoadLibrary(tagMainWindow1.pTempBuf);
	IF_LIKELY(tagMainWindow1.hDllInst != NULL)
	{
		static const unsigned char szString[ICONRES_SIZE] =
		{
			0x4D, 0x44, 0x35, 0xE3, 0x83, 0x81, 0xE3, 0x82, 0xA7, 0xE3, 0x83, 0x83 ,0xE3, 0x82, 0xAF, 0xE3,
			0x83, 0x84, 0xE3, 0x83, 0xBC, 0xE3, 0x83, 0xAB, 0x49, 0x63, 0x6F, 0x6E, 0x52, 0x65, 0x73, 0x00
		};
		const char* cpFunc1 = "IconRes_Load";
		BOOL (WINAPI * IconRes_Load)(unsigned char*) = NULL;
		int nRet = 0;

		IconRes_Load = (BOOL(WINAPI*)(unsigned char*))(GetProcAddress(tagMainWindow1.hDllInst, cpFunc1));
		IF_UNLIKELY(IconRes_Load == NULL)
		{
			pMsg = _T("WinMain: GetProcAddress()");
			dwErr = DEBUG_MISSING;
			dwLastErr = GetLastError();
			goto APPEXIT;
		}

		nRet = IconRes_Load(tagMainWindow1.szIconRes_Load);
		IF_UNLIKELY(nRet == FALSE)
		{
			pMsg = _T("WinMain: IconRes.DLL IconRes_Load() の読み込み");
			dwErr = DEBUG_MISSING;
			goto APPEXIT;
		}


#if _MSC_VER > 1300
#if _MSC_VER < 1700
		if (CPUID_IS_SSE2)
#endif
		{
			__m128i* mmPtr = (__m128i*)tagMainWindow1.szIconRes_Load;
			const __m128i mmXor = { 'm', 'm', 'm', 'm', 'm', 'm', 'm', 'm', 'm', 'm', 'm', 'm', 'm', 'm', 'm', 'm' };

			for (i = 0; i < (sizeof(szString) / sizeof(__m128i)); i++)
			{
				__m128i const mmInput = _mm_loadu_si128(mmPtr + i);
				__m128i const mmRet = _mm_xor_si128(mmInput, mmXor);
				_mm_storeu_si128(mmPtr + i, mmRet);
			}
		}
#if _MSC_VER < 1700
		else
#endif
#endif

#if _MSC_VER < 1700
		{
			size_t* nPtr = (size_t*)tagMainWindow1.szIconRes_Load;
#if defined(_M_X64)
			const size_t nXor = MAKE_CHAR_TO_INT_8CHAR('m', 'm', 'm', 'm', 'm', 'm', 'm', 'm');
#else
			const size_t nXor = MAKE_CHAR_TO_INT_4CHAR('m', 'm', 'm', 'm');
#endif

			for (i = 0; i < (sizeof(szString) / sizeof(size_t) / 4); i++)
			{
				*nPtr++ ^= nXor;
				*nPtr++ ^= nXor;
				*nPtr++ ^= nXor;
				*nPtr++ ^= nXor;
			}
		}
#endif

		tagMainWindow1.szIconRes_Load[sizeof(szString) - 1] = '\0';

		nRet = memcmp(tagMainWindow1.szIconRes_Load, szString, sizeof(szString));
		IF_UNLIKELY(nRet != 0)
		{
			pMsg = _T("WinMain: IconRes_Load() 戻り値の検証");
			dwErr = DEBUG_MISSING;
			goto APPEXIT;
		}
	}
	else
	{
		pMsg = _T("WinMain: LoadLibrary()");
		dwErr = DEBUG_MISSING;
		dwLastErr = GetLastError();
		goto APPEXIT;
	}

	SetCheckSumFile();
	LoadString(tagMainWindow1.hInst, IDS_INIFILE_NAME, tagMainWindow1.pStBuf, MAX_STRINGTABLE - 1);
	GetIniFilePath(tagMainWindow1.pINIFile, tagMainWindow1.pStBuf, tagMainWindow1.pTitle);
	GetIniFileSetting(tagMainWindow1.pINIFile);

	if (dwArgHashType != (DWORD)-1) {
		tagMainWindow1.tagHashThread1.dwHashType = dwArgHashType;
	}

	IF_UNLIKELY(HashThread_Startup(&tagMainWindow1.tagHashThread1) == -1)
	{
		FileRecodeFail(NULL);
		goto APPEXIT;
	}
	HashThread_SetHashType(&tagMainWindow1.tagHashThread1, tagMainWindow1.tagHashThread1.dwHashType);

	// ウィンドウ クラスを登録します。
	IF_UNLIKELY(!MyRegisterClass(hInstance))
	{
		pMsg = _T("WinMain: ウィンドウクラスの登録");
		dwErr = DEBUG_MISSING;
		dwLastErr = GetLastError();
		goto APPEXIT;
	}

	// アプリケーションの初期化を実行します:
	IF_UNLIKELY(!InitInstance(nCmdShow))
	{
		if (tagMainWindow1.bIsWindowCancel == FALSE)
		{
			pMsg = _T("WinMain: アプリケーションの初期化");
			dwErr = DEBUG_MISSING;
			dwLastErr = GetLastError();
		}
		goto APPEXIT;
	}
	hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_HASH));

	// メイン メッセージ ループ:
	while (GetMessage(&msg, NULL, 0, 0))
	{
		if (!TranslateAccelerator(tagMainWindow1.hWnd, hAccelTable, &msg) && !IsDialogMessage(tagMainWindow1.hWnd, &msg))
		{
			TranslateMessage(&msg); 
			DispatchMessage(&msg); 
		}
	}

	dwAppFrag |= APP_CALCEL;
	tagMainWindow1.tagHashThread1.nSuspendMessage = APP_PRGRESS_EXIT;

	if (dwAppFrag & APP_HASHTHREAD)
	{
		if (dwAppFrag & APP_PAUSE) {
			ResumeThread(tagMainWindow1.tagHashThread1.hHashThreadHandle);
		}
		WaitForSingleObject(tagMainWindow1.tagHashThread1.hHashThreadHandle, INFINITE);
		CloseHandle(tagMainWindow1.tagHashThread1.hHashThreadHandle);
	}

APPEXIT:
	if (SUCCEEDED(hr)) {
		CoUninitialize();
	}

#if _MSC_VER < 1500
	if (hUser32 != NULL)
	{
		SetLayeredWindowAttributes_Func = SetLayeredWindowAttributes_Func1;
		FreeLibrary(hUser32);
		hUser32 = NULL;
	}
#endif /* __MSC_VER < 1500 */

	HashThread_Shutdown(&tagMainWindow1.tagHashThread1);
	Charlib_Shutdown();

	if (pMsg) {
		DebugText(dwErr, pMsg, dwLastErr);
	}

	MainWindow_Free();

#ifdef _DEBUG
	_CrtDumpMemoryLeaks();
#endif
	return dwErr ? dwLastErr : (int)msg.wParam;
}

//
//  関数: MyRegisterClass()
//
//  目的: ウィンドウ クラスを登録します。
//
//  コメント:
//
//    この関数および使い方は、'RegisterClassEx' 関数が追加された
//    Windows 95 より前の Win32 システムと互換させる場合にのみ必要です。
//    アプリケーションが、関連付けられた
//    正しい形式の小さいアイコンを取得できるようにするには、
//    この関数を呼び出してください。
//
ATOM MyRegisterClass(HINSTANCE hInstance)
{
	WNDCLASSEX wcex = { 0 };

	wcex.cbSize			= sizeof(WNDCLASSEX);
	wcex.style			= CS_HREDRAW | CS_VREDRAW;
	wcex.lpfnWndProc	= WndProc;
	wcex.cbWndExtra		= DLGWINDOWEXTRA;
	wcex.hInstance		= hInstance;
	wcex.hIcon			= LoadIcon(tagMainWindow1.hInst, MAKEINTRESOURCE(IDI_HASH));
	wcex.hCursor		= LoadCursor(NULL, IDC_ARROW);
	wcex.hbrBackground	= (HBRUSH)(COLOR_WINDOW+1);
	wcex.lpszMenuName	= MAKEINTRESOURCE(IDC_HASH);
	wcex.lpszClassName	= tagMainWindow1.pWindowClass;
	wcex.hIconSm		= LoadIcon(tagMainWindow1.hInst, MAKEINTRESOURCE(IDI_HASH));

	return RegisterClassEx(&wcex);
}

//
//   関数: InitInstance(HINSTANCE, int)
//
//   目的: インスタンス ハンドルを保存して、メイン ウィンドウを作成します。
//
//   コメント:
//
//        この関数で、グローバル変数でインスタンス ハンドルを保存し、
//        メイン プログラム ウィンドウを作成および表示します。
//
BOOL InitInstance(int nCmdShow)
{
	// アプリケーション マニフェストが visual スタイルを有効にするために、
	// ComCtl32.dll Version 6 以降の使用を指定する場合は、
	// Windows XP に InitCommonControlsEx() が必要です。さもなければ、ウィンドウ作成はすべて失敗します。

	InitCtrls.dwSize = sizeof(InitCtrls);
	// アプリケーションで使用するすべてのコモン コントロール クラスを含めるには、
	// これを設定します。
	InitCtrls.dwICC = ICC_WIN95_CLASSES;
	InitCommonControlsEx(&InitCtrls);

	tagMainWindow1.hWnd = CreateDialog(tagMainWindow1.hInst, MAKEINTRESOURCE(IDD_DIALOG1), NULL, (DLGPROC)WndProc);
	IF_UNLIKELY(!tagMainWindow1.hWnd) {
		return FALSE;
	}

	ShowWindow(tagMainWindow1.hWnd, nCmdShow);
	UpdateWindow(tagMainWindow1.hWnd);
	return TRUE;
}

//
//  関数: WndProc(HWND, UINT, WPARAM, LPARAM)
//
//  目的:  メイン ウィンドウのメッセージを処理します。
//
//  WM_COMMAND	- アプリケーション メニューの処理
//  WM_PAINT	- メイン ウィンドウの描画
//  WM_DESTROY	- 中止メッセージを表示して戻る
//
//
LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	if (message == tagMainWindow1.tagHashThread1.nMessageID)
	{
		LRESULT nRet = FALSE;

		switch (wParam)
		{
		case APP_MESSAGE_INIT:
			nRet = MainWindow_OnMessageInit(hWnd, wParam, lParam);
			break;
		case APP_MESSAGE_FILE_CHANGE:
			nRet = MainWindow_OnMessageFileChange(hWnd, wParam, lParam);
			break;
		case APP_MESSAGE_FILE_HASHCOMP:
			nRet = MainWindow_OnMessageHashComp(hWnd, wParam, lParam);
			break;
		case APP_MESSAGE_FOLDER_INIT:
			nRet = MainWindow_OnMessageFolderInit(hWnd, wParam, lParam);
			break;
		case APP_MESSAGE_SUBFOLDER_OPEN:
			nRet = MainWindow_OnMessageSubFolderInit(hWnd, wParam, lParam);
			break;
		case APP_MESSAGE_FOLDER_INIT_EXIT:
			nRet = MainWindow_OnMessageFolderExit(hWnd, wParam, lParam);
			break;
		case APP_MESSAGE_HASHFILE_INIT:
			nRet = MainWindow_OnMessageHashFileInit(hWnd, wParam, lParam);
			break;
		case APP_MESSAGE_HASHFILE_INIT_EXIT:
			nRet = MainWindow_OnMessageHashFileExit(hWnd, wParam, lParam);
			break;
		case APP_MESSAGE_START:
			//処理開始
			nRet = MainWindow_OnMessageStart(hWnd, wParam, lParam);
			break;
		case APP_MESSAGE_PROGRESS:
			nRet = MainWindow_OnMessageProgress(hWnd, wParam, lParam);
			//処理中の表示
			break;
		case APP_MESSAGE_END:
			//終了の表示、リセット
			nRet = MainWindow_OnMessageEnd(hWnd, wParam, lParam);
			break;
		case APP_MESSAGE_ERROR:
			//中断の表示、リセット
			nRet = MainWindow_OnMessageError(hWnd, wParam, lParam);
			break;
		case APP_MESSAGE_PAUSE:
			nRet = MainWindow_OnMessagePause(hWnd, wParam, lParam);
			break;
		case APP_MESSAGE_CHANGE_FILEVIEW:
			nRet = MainWindow_OnMessageFileView(hWnd, wParam, lParam);
			break;
		case APP_MESSAGE_LASTERROR:
			nRet = MainWindow_OnMessageLastError(hWnd, wParam, lParam);
			break;
		case APP_MESSAGE_HASHFILE_HASHCHANGE:
			nRet = MainWindow_OnMessageHashFileChange(hWnd, wParam, lParam);
			break;
		case APP_MESSAGE_WINDOWSET:
			nRet = MainWindow_OnMessageWindowSet(hWnd, wParam, lParam);
			break;
		}

		if (tagFileListWindow1.hWnd) {
			PostMessage(tagFileListWindow1.hWnd, tagFileListWindow1.nWindowMessage, wParam, lParam);
		}
		return nRet;
	}
	else if (message == tagMainWindow1.nTaskbarButtonMessage)
	{
#ifdef _DEBUG
		OutputDebugString(_T("MainWindow: WndProc(): TaskbarButtonCreated のメッセージを受信しました。\r\n"));
#endif /* _DEBUG */

		IF_LIKELY(wParam == 0)
		{
			if (tagMainWindow1.pTaskbarList3 == NULL)
			{
				//iTaskbarList3_Release(tagMainWindow1.pTaskbarList3);
				tagMainWindow1.hr3 = iTaskbarList3_CoCreateInstance(&tagMainWindow1.pTaskbarList3);
				if (SUCCEEDED(tagMainWindow1.hr3)) {
					return TRUE;
				}
				else
				{
#ifdef _DEBUG
					MainWindow_MessageBox(hWnd, _T("iTaskbarList3_CoCreateInstanceに失敗しました。"), tagMainWindow1.pTitle, MB_ICONQUESTION);
#endif /* _DEBUG */
					return FALSE;
				}
			}
		}
	}


	switch (message)
	{
	case WM_INITDIALOG:
		return MainWindow_OnInitDialog(hWnd, wParam, lParam);
	case WM_COMMAND:
		return MainWindow_OnCommand(hWnd, wParam, lParam);
	case WM_RBUTTONDOWN:
		return MainWindow_OnRButtonDown(hWnd, wParam, lParam);
	case WM_SYSCOLORCHANGE:
		return MainWindow_OnSysColorChange(hWnd, wParam, lParam);
	case WM_CTLCOLOREDIT:
		return MainWindow_OnCtlColorEdit(hWnd, wParam, lParam);
	case WM_DPICHANGED:
		return MainWindow_OnDpiChanged(hWnd, wParam, lParam);
	case WM_DROPFILES:
		return MainWindow_OnDropFiles(hWnd, wParam, lParam);
	case WM_GETMINMAXINFO:
		return MainWindow_OnGetMinMaxInfo(hWnd, wParam, lParam);
	case WM_MOVE:
		return MainWindow_OnMove(hWnd, wParam, lParam);
	case WM_SIZE:
		return MainWindow_OnSize(hWnd, wParam, lParam);
	case WM_TIMER:
		return MainWindow_OnTimer(hWnd, wParam, lParam);
	case WM_ACTIVATE:
		return MainWindow_OnActivate(hWnd, wParam, lParam);
	case WM_DRAWCLIPBOARD:
		return MainWindow_OnDrawClipboard(hWnd, wParam, lParam);
	case WM_CHANGECBCHAIN:
		return MainWindow_OnChangeCBChain(hWnd, wParam, lParam);
	case WM_THEMECHANGED:
		return MainWindow_OnThemeChanged(hWnd, wParam, lParam);
	case WM_CLOSE:
		return MainWindow_OnClose(hWnd, wParam, lParam);
	case WM_DESTROY:
		return MainWindow_OnDestroy(hWnd, wParam, lParam);
	}
	return FALSE;
}

LRESULT MainWindow_OnMessageInit(HWND hWnd, WPARAM wParam, LPARAM lParam)
{
	UNREFERENCED_PARAMETER(wParam);
	UNREFERENCED_PARAMETER(lParam);

#ifdef _DEBUG
	OutputDebugString(_T("MainWindow: MainWindow_OnMessageInit(): ハッシュスレッドの準備中...\r\n"));
#endif

	dwAppFrag &= ~(APP_HASH_INPUT_EDIT | APP_LASTERROR | APP_CALCEL | APP_HASHFILE_ADDMODE);

	SetTimer(hWnd, TIMER, 250, NULL);
	DragAcceptFiles(hWnd, FALSE);
	SendMessage(tagMainWindow1.hProgress[0], PBM_SETPOS, 0, 0);
	SendMessage(tagMainWindow1.hBmp[0], STM_SETIMAGE, IMAGE_ICON, (LPARAM)tagMainWindow1.hStatusIcon[STATUS_ICON_PLAY]);

	SetWindowText(hWnd, tagMainWindow1.pTitle);
	StatusOut(tagMainWindow1.pStatusText[0]);
	SetWindowText(tagMainWindow1.hStatic[1], _T(""));
	SetWindowText(tagMainWindow1.hStatic[7], tagMainWindow1.pBase64_Msg[0]);
	SetToolTip(tagMainWindow1.hTool[0], tagMainWindow1.hStatic[7], _T(""));
	SetWindowText(tagMainWindow1.hButton[0], tagMainWindow1.pButtonText1[1]);
	SetWindowText(tagMainWindow1.hButton[1], tagMainWindow1.pButtonText2[1]);
	SetToolTipText(tagMainWindow1.hTool[0], tagMainWindow1.hButton[0], tagMainWindow1.pToolTipText[6]);
	SetToolTipText(tagMainWindow1.hTool[0], tagMainWindow1.hButton[1], tagMainWindow1.pToolTipText[7]);
	SetWindowText(tagMainWindow1.hEdit[0], _T(""));
	SetWindowText(tagMainWindow1.hEdit[1], _T(""));
	EnableWindow(tagMainWindow1.hEdit[0], FALSE);
	EnableWindow(tagMainWindow1.hEdit[1], FALSE);
	EnableWindow(tagMainWindow1.hEdit[2], FALSE);
	SendMessage(tagMainWindow1.hEdit[2], EM_SETREADONLY, TRUE, 0);
	*tagMainWindow1.pHashCompString = '\0';
	EnableWindow(tagMainWindow1.hButton[1], TRUE);
	EnableWindow(tagMainWindow1.hButton[2], FALSE);
	EnableWindow(tagMainWindow1.hButton[3], FALSE);
	EnableMenuItem(tagMainWindow1.hPupMenu[0][0], ID_MAINSUBMENU1_HASH, MF_GRAYED);
	EnableMenuItem(tagMainWindow1.hPupMenu[0][0], ID_MAINSUBMENU1_CLEAR, MF_GRAYED);
	EnableMenuItem(tagMainWindow1.hPupMenu[0][0], ID_MAINSUBMENU1_SAVE, MF_GRAYED);
	EnableMenuItem(tagMainWindow1.hPupMenu[0][0], ID_MAINSUBMENU1_COPY, MF_GRAYED);
	EnableMenuItem(tagMainWindow1.hPupMenu[0][0], ID_MAINSUBMENU1_SIMPLECOPY, MF_GRAYED);
	EnableMenuItem(tagMainWindow1.hPupMenu[0][0], ID_MAINSUBMENU1_PASTE, MF_GRAYED);
	EnableMenuItem(tagMainWindow1.hPupMenu[0][0], 12, MF_BYPOSITION | MF_GRAYED);
	EnableMenuItem(tagMainWindow1.hPupMenu[0][0], ID_MAINSUBMENU1_BENCH, MF_GRAYED);
	EnableMenuItem(tagMainWindow1.hPupMenu[0][0], 17, MF_BYPOSITION | MF_GRAYED);
	*tagMainWindow1.pTimeText = '\0';
	Hash_SetFileInfo(0);
	tagMainWindow1.dwTimeViewFileCount = 0;
	tagMainWindow1.nCmpCount = 0;
	tagMainWindow1.nCmpErrorCount = 0;
	tagMainWindow1.dwStartTime = tagMainWindow1.tagHashThread1.dwStartTime;
	iTaskbarList3_SetProgressState(tagMainWindow1.pTaskbarList3, hWnd, TBPF_INDETERMINATE);
	iTaskbarList3_DeleteOverlayIcon(tagMainWindow1.pTaskbarList3, hWnd);
	SetFocus(tagMainWindow1.hButton[0]);
	return TRUE;
}


LRESULT MainWindow_OnMessageFileChange(HWND hWnd, WPARAM wParam, LPARAM lParam)
{
	UNREFERENCED_PARAMETER(hWnd);
	UNREFERENCED_PARAMETER(wParam);

	tagMainWindow1.dwOpenItem = (DWORD)lParam;
	if (~dwAppFrag & APP_DISABLE_MULTIFILE_UPDATE_DRAW) {
		Hash_SetFileInfo((DWORD)lParam);
	}
	return TRUE;
}

LRESULT MainWindow_OnMessageHashComp(HWND hWnd, WPARAM wParam, LPARAM lParam)
{
	UNREFERENCED_PARAMETER(hWnd);
	UNREFERENCED_PARAMETER(wParam);

	if (lParam != 0) {
		tagMainWindow1.nCmpErrorCount++;
	}
	else {
		tagMainWindow1.nCmpCount++;
	}
	return TRUE;
}


LRESULT MainWindow_OnMessageSubFolderInit(HWND hWnd, WPARAM wParam, LPARAM lParam)
{
	UNREFERENCED_PARAMETER(hWnd);
	UNREFERENCED_PARAMETER(wParam);
	UNREFERENCED_PARAMETER(lParam);

#ifdef _DEBUG
	TCHAR szDebugText[1024];

	STPRINTF_FUNC(szDebugText,
		_T("MainWindow: WndProc(): ハッシュスレッドのサブフォルダーを展開しています。 (%llu)\r\n"), (unsigned __int64)lParam);
	OutputDebugString(szDebugText);
#endif
	return TRUE;
}

LRESULT MainWindow_OnMessageHashFileInit(HWND hWnd, WPARAM wParam, LPARAM lParam)
{
	UNREFERENCED_PARAMETER(hWnd);
	UNREFERENCED_PARAMETER(wParam);
	UNREFERENCED_PARAMETER(lParam);

#ifdef _DEBUG
	OutputDebugString(_T("MainWindow: MainWindow_OnMessageHashFileInit(): ハッシュファイルを開いています...\r\n"));
#endif
	return TRUE;
}

LRESULT MainWindow_OnMessageHashFileExit(HWND hWnd, WPARAM wParam, LPARAM lParam)
{
	UNREFERENCED_PARAMETER(hWnd);
	UNREFERENCED_PARAMETER(wParam);
	UNREFERENCED_PARAMETER(lParam);

#ifdef _DEBUG
	OutputDebugString(_T("MainWindow: MainWindow_OnMessageHashFileExit(): ハッシュファイルが開き終わりました。\r\n"));
#endif
	return TRUE;
}

LRESULT MainWindow_OnMessageFileView(HWND hWnd, WPARAM wParam, LPARAM lParam)
{
	UNREFERENCED_PARAMETER(hWnd);
	UNREFERENCED_PARAMETER(wParam);

	if (~dwAppFrag & APP_HASHTHREAD)
	{
		tagMainWindow1.dwOpenItem = (DWORD)lParam;
		Hash_FileView((DWORD)lParam);
	}
	return TRUE;
}

LRESULT MainWindow_OnMessageLastError(HWND hWnd, WPARAM wParam, LPARAM lParam)
{
	UNREFERENCED_PARAMETER(hWnd);
	UNREFERENCED_PARAMETER(wParam);

	dwAppFrag |= APP_LASTERROR;
	tagMainWindow1.dwLasterror = (DWORD)lParam;
	return TRUE;
}

LRESULT MainWindow_OnMessageHashFileChange(HWND hWnd, WPARAM wParam, LPARAM lParam)
{
	UNREFERENCED_PARAMETER(hWnd);
	UNREFERENCED_PARAMETER(wParam);
	UNREFERENCED_PARAMETER(lParam);

	Hash_SetHashType(FALSE);
	return TRUE;
}

LRESULT MainWindow_OnMessageWindowSet(HWND hWnd, WPARAM wParam, LPARAM lParam)
{
	UNREFERENCED_PARAMETER(wParam);
	UNREFERENCED_PARAMETER(lParam);

	if (dwAppFrag & APP_ENABLE_TOPMOST) {
		SetWindowPos(hWnd, HWND_TOPMOST, 0, 0, 0, 0, (SWP_NOMOVE | SWP_NOSIZE | SWP_SHOWWINDOW));
	}
	Hash_GetDpi(0, 0);
	Hash_SetHashType(TRUE);
	return TRUE;
}

LRESULT MainWindow_OnMessageStart(HWND hWnd, WPARAM wParam, LPARAM lParam)
{
	UNREFERENCED_PARAMETER(wParam);
	UNREFERENCED_PARAMETER(lParam);

#ifdef _DEBUG
	OutputDebugString(_T("MainWindow: WndProc(): ハッシュスレッドが開始しました。\r\n"));
#endif

	dwAppFrag &= ~(APP_LASTERROR | APP_CALCEL);

	tagMainWindow1.dwStartTime = timeGetTime();
	iTaskbarList3_SetProgressState(tagMainWindow1.pTaskbarList3, hWnd, TBPF_NORMAL);
	return TRUE;
}

LRESULT MainWindow_OnMessageEnd(HWND hWnd, WPARAM wParam, LPARAM lParam)
{
	UNREFERENCED_PARAMETER(wParam);

#ifdef _DEBUG
	TCHAR szDebugText[1024];

	STPRINTF_FUNC(szDebugText,
		_T("MainWindow: MainWindow_OnMessageEnd(): ハッシュスレッドが正常に終了しました。(%llu)\r\n"), (unsigned __int64)lParam);
	OutputDebugString(szDebugText);
#endif

	dwAppState = 0;
	SendMessage(tagMainWindow1.hBmp[0], STM_SETIMAGE, IMAGE_ICON, (LPARAM)tagMainWindow1.hStatusIcon[STATUS_ICON_WARNING]);
	SetWindowText(hWnd, tagMainWindow1.pTitle);
	SendMessage(tagMainWindow1.hProgress[0], PBM_SETPOS, 100, 0);

	if (tagMainWindow1.dwReOpenItem == (DWORD)-1) {
		tagMainWindow1.dwOpenItem = (DWORD)lParam;
	}
	else {
		tagMainWindow1.dwOpenItem = tagMainWindow1.dwReOpenItem;
	}
	Hash_FileView(tagMainWindow1.dwOpenItem);
	EnableWindow(tagMainWindow1.hButton[3], ClipboardCheck(tagMainWindow1.dwOpenItem));
	EnableMenuItem(tagMainWindow1.hPupMenu[0][0], ID_MAINSUBMENU1_CLEAR, MF_ENABLED);
	EnableMenuItem(tagMainWindow1.hPupMenu[0][0], ID_MAINSUBMENU1_COPY, MF_ENABLED);
	EnableMenuItem(tagMainWindow1.hPupMenu[0][0], ID_MAINSUBMENU1_SIMPLECOPY, MF_ENABLED);
	EnableMenuItem(tagMainWindow1.hPupMenu[0][0], ID_MAINSUBMENU1_BENCH2, MF_ENABLED);
	EnableMenuItem(tagMainWindow1.hPupMenu[0][0], 12, MF_BYPOSITION | MF_ENABLED);
	EnableMenuItem(tagMainWindow1.hPupMenu[0][0], ID_MAINSUBMENU1_BENCH, MF_ENABLED);
	EnableMenuItem(tagMainWindow1.hPupMenu[0][0], 17, MF_BYPOSITION | MF_ENABLED);

	if (dwAppFrag & APP_HASHFILE_ADDMODE)
	{
		EnableMenuItem(tagMainWindow1.hPupMenu[0][0], ID_MAINSUBMENU1_HASH, MF_GRAYED);
		EnableWindow(tagMainWindow1.hButton[1], FALSE);

		SendMessage(tagMainWindow1.hEdit[1], WM_SETFONT, (WPARAM)tagMainWindow1.hFont[1], 0);
		SetWindowText(tagMainWindow1.hEdit[1], tagMainWindow1.pInputtext);
		MessageBeep(MB_ICONASTERISK);
		SetFocus(tagMainWindow1.hButton[5]);
	}
	else
	{
		const tagHashThread_FileRecode* cptagHashFile_Recode = HashThread_GetFileRecode_Core(&tagMainWindow1.tagHashThread1, tagMainWindow1.dwOpenItem);
		StatusOut(tagMainWindow1.pStatusText[1]);

		if (cptagHashFile_Recode->dwHashFileHashLine != HASHFILE_NOLINE && cptagHashFile_Recode->dwFileHashType == cptagHashFile_Recode->dwHashFileHashType) {
			Hash_HashComp(tagMainWindow1.dwOpenItem);
		}
		else
		{
			if (tagMainWindow1.dwReOpenMode)
			{
				const size_t nHashSize = (size_t)tagMainWindow1.tagHashThread1.dwHashLen * 2;

				dwAppFrag |= APP_HASH_INPUT_EDIT;

				SendMessage(tagMainWindow1.hEdit[1], WM_SETFONT, (WPARAM)tagMainWindow1.hFont[2], 0);
				SetWindowText(tagMainWindow1.hEdit[1], tagMainWindow1.pClipboardString);
				SendMessage(tagMainWindow1.hEdit[1], EM_SETSEL, nHashSize, nHashSize);
				*tagMainWindow1.pClipboardString = '\0';
				tagMainWindow1.dwReOpenMode = 0;
			}
			else
			{
				SendMessage(tagMainWindow1.hEdit[1], WM_SETFONT, (WPARAM)tagMainWindow1.hFont[1], 0);
				SetWindowText(tagMainWindow1.hEdit[1], tagMainWindow1.pInputtext);
				MessageBeep(MB_ICONASTERISK);
			}
		}

		KillTimer(hWnd, TIMER);
		Hash_TimeView(TRUE);

		SetWindowText(tagMainWindow1.hButton[0], tagMainWindow1.pButtonText1[0]);
		SetWindowText(tagMainWindow1.hButton[1], tagMainWindow1.pButtonText2[0]);
		SetToolTipText(tagMainWindow1.hTool[0], tagMainWindow1.hButton[0], tagMainWindow1.pToolTipText[0]);
		SetToolTipText(tagMainWindow1.hTool[0], tagMainWindow1.hButton[1], tagMainWindow1.pToolTipText[1]);
		iTaskbarList3_SetProgressState(tagMainWindow1.pTaskbarList3, hWnd, TBPF_NOPROGRESS);
		WaitForSingleObject(tagMainWindow1.tagHashThread1.hHashThreadHandle, INFINITE);
		CloseHandle(tagMainWindow1.tagHashThread1.hHashThreadHandle);
		tagMainWindow1.tagHashThread1.hHashThreadHandle = NULL;

		if (tagMainWindow1.dwAddFileModeFileCount == 0) {
			DragAcceptFiles(hWnd, TRUE);
		}

		SetFocus(tagMainWindow1.hButton[0]);
	}

	dwAppFrag &= ~(APP_HASHTHREAD);

	if (tagFileListWindow1.hWnd)
	{
		SetForegroundWindow(tagFileListWindow1.hWnd);
		return FALSE;
	}

	if (tagMainWindow1.nCmpErrorCount)
	{
		int nRet = 0;

		LoadString(tagMainWindow1.hInst, IDS_HASHFILE_COMP2, tagMainWindow1.pStBuf, MAX_STRINGTABLE - 1);
		MessageFormat(tagMainWindow1.pBuf, MAX_STRINGTABLE, tagMainWindow1.pStBuf, tagMainWindow1.nCmpErrorCount);
		iTaskbarList3_SetOverlayIcon(tagMainWindow1.pTaskbarList3, hWnd, tagMainWindow1.hIcon[TASKBAR_ICON_ERROR], L"Hash Comp Error");

		nRet = MainWindow_MessageBox(tagMainWindow1.hWnd, tagMainWindow1.pBuf, tagMainWindow1.pTitle, MB_YESNO | MB_ICONQUESTION);
		if (nRet == IDYES)
		{
			DWORD dwCount = 0;
			DWORD dwFileCount = 0;
			DWORD dwHashCmpErr = 0;
			TCHAR* pTemp = NULL;

			if (tagMainWindow1.nCmpErrorCount > tagMainWindow1.nCmpErrorCountSize)
			{
				const size_t nBufSize = (MAX_PATH_SIZE * tagMainWindow1.nCmpErrorCount) * sizeof(TCHAR);

				free(tagMainWindow1.pCmpErrorCount);
				tagMainWindow1.pCmpErrorCount = (TCHAR*)malloc(nBufSize);
				if (tagMainWindow1.pCmpErrorCount == NULL)
				{
					TCHAR szNum[33] = _T("");

					DecToTChar(nBufSize, szNum);
					DebugText(DEBUG_ALLOC_ERROR, szNum, 0);
					return FALSE;
				}
				tagMainWindow1.nCmpErrorCountSize = tagMainWindow1.nCmpErrorCount;
			}

			pTemp = tagMainWindow1.pCmpErrorCount;
			dwFileCount = tagMainWindow1.tagHashThread1.tagMultiFile->dwFileCount;

			for (dwCount = 0; dwCount < dwFileCount; dwCount++)
			{
				const tagHashThread_FileRecode* cptagHashFile_Recode = HashThread_GetFileRecode(&tagMainWindow1.tagHashThread1, dwCount);

				if (cptagHashFile_Recode->dwHashFileHashLine != HASHFILE_NOLINE && cptagHashFile_Recode->nHashFileCmp != 0)
				{
					pTemp = qtcscpy(pTemp, _T("□")); 
					pTemp = qtcscpy(pTemp, cptagHashFile_Recode->szFileName);
					pTemp = qtcscpy(pTemp, _T("\r\n"));
					dwHashCmpErr++;

					if (dwHashCmpErr > 10)
					{
						pTemp = qtcscpy(pTemp, _T("□"));
						LoadString(tagMainWindow1.hInst, IDS_HASHFILE_COMP3, tagMainWindow1.pStBuf, MAX_STRINGTABLE - 1);
						pTemp = qtcscpy(pTemp, tagMainWindow1.pStBuf);
						pTemp = qtcscpy(pTemp, _T("\r\n"));
						break;
					}
				}
			}
			MainWindow_MessageBox(hWnd, tagMainWindow1.pCmpErrorCount, tagMainWindow1.pTitle, MB_ICONINFORMATION);
		}
	}
	else if (tagMainWindow1.nCmpCount && tagMainWindow1.nCmpErrorCount == 0) {
		iTaskbarList3_SetOverlayIcon(tagMainWindow1.pTaskbarList3, hWnd, tagMainWindow1.hIcon[TASKBAR_ICON_CHECK], L"NoError");
	}
	return TRUE;
}

LRESULT MainWindow_OnMessageFolderInit(HWND hWnd, WPARAM wParam, LPARAM lParam)
{
	UNREFERENCED_PARAMETER(wParam);
	UNREFERENCED_PARAMETER(lParam);
	LONG_PTR lpStyle = 0;

#ifdef _DEBUG
	OutputDebugString(_T("MainWindow: MainWindow_OnMessageFolderInit(): ハッシュスレッドのフォルダーを開いています。\r\n"));
#endif
	dwAppFrag |= APP_FOLDERINIT;

	lpStyle = GetWindowLongPtr(tagMainWindow1.hProgress[0], GWL_STYLE);
	lpStyle |= PBS_MARQUEE;
	SetWindowLongPtr(tagMainWindow1.hProgress[0], GWL_STYLE, lpStyle);
	SendMessage(tagMainWindow1.hProgress[0], PBM_SETMARQUEE, TRUE, 0);
	EnableWindow(tagMainWindow1.hButton[0], FALSE);
	EnableWindow(tagMainWindow1.hButton[1], FALSE);
	iTaskbarList3_SetProgressState(tagMainWindow1.pTaskbarList3, hWnd, TBPF_INDETERMINATE);
	return TRUE;
}

LRESULT MainWindow_OnMessageFolderExit(HWND hWnd, WPARAM wParam, LPARAM lParam)
{
	UNREFERENCED_PARAMETER(wParam);
	LONG_PTR lpStyle = 0;

#ifdef _DEBUG
	OutputDebugString(_T("MainWindow: WndProc(): ハッシュスレッドのフォルダー展開が完了しました。\r\n"));
#endif

	IF_UNLIKELY((DWORD)lParam == (DWORD)-1)
	{
		FileRecodeFail(hWnd);
		return FALSE;
	}

	dwAppFrag &= ~APP_FOLDERINIT;

	lpStyle = GetWindowLongPtr(tagMainWindow1.hProgress[0], GWL_STYLE);
	lpStyle &= ~PBS_MARQUEE;
	SetWindowLongPtr(tagMainWindow1.hProgress[0], GWL_STYLE, lpStyle);
	EnableWindow(tagMainWindow1.hButton[0], TRUE);
	iTaskbarList3_SetProgressState(tagMainWindow1.pTaskbarList3, hWnd, TBPF_NOPROGRESS);
	return TRUE;
}

LRESULT MainWindow_OnMessageProgress(HWND hWnd, WPARAM wParam, LPARAM lParam)
{
	UNREFERENCED_PARAMETER(wParam);
	static int nVal = -1;

	if (lParam != nVal)
	{
		nVal = (int)lParam;
		SendMessage(tagMainWindow1.hProgress[0], PBM_SETPOS, lParam, 0);
		tagMainWindow1.dwPersent = (DWORD)lParam;

		MessageFormat(tagMainWindow1.pBuf, 2048, tagMainWindow1.pWindowTitle[1], (int)lParam, tagMainWindow1.pTitle);
		SetWindowText(hWnd, tagMainWindow1.pBuf);
		iTaskbarList3_SetProgressValue(tagMainWindow1.pTaskbarList3, hWnd, lParam);
	}
	return TRUE;
}

LRESULT MainWindow_OnMessageError(HWND hWnd, WPARAM wParam, LPARAM lParam)
{
	UNREFERENCED_PARAMETER(wParam);

#ifdef _DEBUG
	TCHAR szDebugText[1024];

	STPRINTF_FUNC(szDebugText,
		_T("MainWindow: MainWindow_OnMessageError(): ハッシュスレッドが異常終了しました。(%llu)\r\n"), (unsigned __int64)lParam);
	OutputDebugString(szDebugText);
#endif

	dwAppFrag &= ~(APP_HASHTHREAD);
	dwAppState = (DWORD)lParam;

	KillTimer(hWnd, TIMER);
	WaitForSingleObject(tagMainWindow1.tagHashThread1.hHashThreadHandle, INFINITE);
	CloseHandle(tagMainWindow1.tagHashThread1.hHashThreadHandle);
	tagMainWindow1.tagHashThread1.hHashThreadHandle = NULL;

	if (tagMainWindow1.dwAddFileModeFileCount == 0) {
		DragAcceptFiles(hWnd, TRUE);
	}

	tagMainWindow1.dwBackTime = 0;
	SetWindowText(tagMainWindow1.hButton[0], tagMainWindow1.pButtonText1[0]);
	SetWindowText(tagMainWindow1.hButton[1], tagMainWindow1.pButtonText2[0]);
	SetToolTipText(tagMainWindow1.hTool[0], tagMainWindow1.hButton[0], tagMainWindow1.pToolTipText[0]);
	SetToolTipText(tagMainWindow1.hTool[0], tagMainWindow1.hButton[1], tagMainWindow1.pToolTipText[1]);
	EnableMenuItem(tagMainWindow1.hPupMenu[0][0], ID_MAINSUBMENU1_HASH, MF_ENABLED);
	EnableMenuItem(tagMainWindow1.hPupMenu[0][0], ID_MAINSUBMENU1_CLEAR, MF_ENABLED);
	EnableMenuItem(tagMainWindow1.hPupMenu[0][0], ID_MAINSUBMENU1_BENCH, MF_ENABLED);
	EnableMenuItem(tagMainWindow1.hPupMenu[0][0], ID_MAINSUBMENU1_BENCH2, MF_ENABLED);

	if (dwAppFrag & APP_FILE_CLEAR) {
		Clear(TRUE);
	}
	else
	{
		SendMessage(tagMainWindow1.hBmp[0], STM_SETIMAGE, IMAGE_ICON, (LPARAM)tagMainWindow1.hStatusIcon[STATUS_ICON_WARNING]);
		tagMainWindow1.dwOpenItem = HashThread_GetCurrentItem(&tagMainWindow1.tagHashThread1);
		Hash_FileView(tagMainWindow1.dwOpenItem);
		iTaskbarList3_SetProgressState(tagMainWindow1.pTaskbarList3, hWnd, TBPF_ERROR);
		iTaskbarList3_SetOverlayIcon(tagMainWindow1.pTaskbarList3, hWnd, tagMainWindow1.hIcon[TASKBAR_ICON_WARNING], L"Hash Warning");
	}

	SetForegroundWindow(tagFileListWindow1.hWnd ? tagFileListWindow1.hWnd : hWnd);
	SetFocus(tagMainWindow1.hButton[0]);
	return TRUE;
}

LRESULT MainWindow_OnMessagePause(HWND hWnd, WPARAM wParam, LPARAM lParam)
{
	UNREFERENCED_PARAMETER(wParam);
	UNREFERENCED_PARAMETER(lParam);
	static DWORD dwPauseTime;

	IF_UNLIKELY(~dwAppFrag & APP_HASHTHREAD) {
		return FALSE;
	}

	if (dwAppFrag & APP_PAUSE)
	{
		dwAppFrag &= ~APP_PAUSE;

		tagMainWindow1.tagHashThread1.nSuspendMessage &= ~APP_PRGRESS_PAUSE;
		ResumeThread(tagMainWindow1.tagHashThread1.hHashThreadHandle);

		tagMainWindow1.dwStartTime += (timeGetTime() - dwPauseTime);
		tagMainWindow1.dwEndTime = 0;
		tagMainWindow1.dwBackTime = 0;

		StatusOut(tagMainWindow1.pStatusText[0]);
		SetWindowText(tagMainWindow1.hButton[0], tagMainWindow1.pButtonText1[1]);
		SetToolTipText(tagMainWindow1.hTool[0], tagMainWindow1.hButton[0], tagMainWindow1.pToolTipText[6]);
		SetToolTipText(tagMainWindow1.hTool[0], tagMainWindow1.hButton[1], tagMainWindow1.pToolTipText[7]);
		SendMessage(tagMainWindow1.hBmp[0], STM_SETIMAGE, IMAGE_ICON, (LPARAM)tagMainWindow1.hStatusIcon[STATUS_ICON_PLAY]);
		iTaskbarList3_SetProgressState(tagMainWindow1.pTaskbarList3, hWnd, TBPF_NORMAL);
	}
	else
	{
		dwAppFrag |= APP_PAUSE;

		tagMainWindow1.tagHashThread1.nSuspendMessage |= APP_PRGRESS_PAUSE;
		SuspendThread(tagMainWindow1.tagHashThread1.hHashThreadHandle);
		dwPauseTime = timeGetTime();

		StatusOut(tagMainWindow1.pStatusText[2]);
		SetWindowText(tagMainWindow1.hButton[0], tagMainWindow1.pButtonText1[2]);
		SetToolTipText(tagMainWindow1.hTool[0], tagMainWindow1.hButton[0], tagMainWindow1.pToolTipText[8]);
		SetToolTipText(tagMainWindow1.hTool[0], tagMainWindow1.hButton[1], tagMainWindow1.pToolTipText[7]);
		SendMessage(tagMainWindow1.hBmp[0], STM_SETIMAGE, IMAGE_ICON, (LPARAM)tagMainWindow1.hStatusIcon[STATUS_ICON_WARNING]);
		iTaskbarList3_SetProgressState(tagMainWindow1.pTaskbarList3, hWnd, TBPF_PAUSED);
	}
	return TRUE;
}

LRESULT MainWindow_OnInitDialog(HWND hWnd, WPARAM wParam, LPARAM lParam)
{
	UNREFERENCED_PARAMETER(wParam);
	UNREFERENCED_PARAMETER(lParam);
	int i = 0;
	int nRet = 0;
	DWORD dwRet = 0;

	/* ダイアログの初期化 ここから */
	tagMainWindow1.dwStructSize = sizeof(tagMainWindow1);
	tagMainWindow1.hWnd = hWnd;

	if (~dwAppFrag & APP_ENABLE_MINIMAM_HANDLE)
	{
		SetEnableThemeDialogTexture(hWnd);
		tagMainWindow1.hIcon[0] = LoadIcon(tagMainWindow1.hInst, MAKEINTRESOURCE((size_t)IDI_HASH));
		SendMessage(hWnd, WM_SETICON, ICON_SMALL, (LPARAM)tagMainWindow1.hIcon[0]);
		tagMainWindow1.hTool[0] = CreateWindowEx(NULL, TOOLTIPS_CLASS, NULL, WS_POPUP | TTS_ALWAYSTIP, CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, hWnd, NULL, tagMainWindow1.hInst, NULL);
	}

	SetWindowText(hWnd, tagMainWindow1.pTitle);
	tagMainWindow1.hMenu = LoadMenu(tagMainWindow1.hInst, MAKEINTRESOURCE(IDR_MAINSUBMENU));
	tagMainWindow1.hPupMenu[0][0] = GetSubMenu(tagMainWindow1.hMenu, 0);
	tagMainWindow1.hPupMenu[0][2] = GetSubMenu(tagMainWindow1.hPupMenu[0][0], 19);
	tagMainWindow1.hBmp[0] = GetDlgItem(hWnd, IDC_STATUS);
	tagMainWindow1.hProgress[0] = GetDlgItem(hWnd, IDC_PROGRESS1);
	SendMessage(tagMainWindow1.hProgress[0], PBM_SETRANGE32, 0, 100);

	tagMainWindow1.hGroup[0] = GetDlgItem(hWnd, IDC_GROUP1);
	for (i = 0; i < SIZEOF_NUM(tagMainWindow1.hEdit); i++) {
		tagMainWindow1.hEdit[i] = GetDlgItem(hWnd, IDC_EDIT1 + i);
	}

	for (i = 0; i < SIZEOF_NUM(tagMainWindow1.hButton); i++) {
		tagMainWindow1.hButton[i] = GetDlgItem(hWnd, IDC_BUTTON1 + i);
	}

	for (i = 0; i < SIZEOF_NUM(tagMainWindow1.hStatic); i++) {
		tagMainWindow1.hStatic[i] = GetDlgItem(hWnd, IDC_STATIC1 + i);
	}

	IF_LIKELY(tagMainWindow1.hPupMenu[0][2] != NULL)
	{
		int nCount = 0;

		nCount = GetMenuItemCount(tagMainWindow1.hPupMenu[0][2]);
		for (i = 0; i < nCount; i++) {
			RemoveMenu(tagMainWindow1.hPupMenu[0][2], 0, MF_BYPOSITION);
		}

		for (i = 0; i < MAX_HASH_TYPE; i++) {
			nRet = InsertMenu(tagMainWindow1.hPupMenu[0][2], i, MF_BYPOSITION, ID_MAINSUBMENU1_HASH1 + (size_t)i, HashThread_GetHashListName(i));
		}

		InsertMenu(tagMainWindow1.hPupMenu[0][2], 1, MF_BYPOSITION | MF_SEPARATOR, 0, NULL);
		InsertMenu(tagMainWindow1.hPupMenu[0][2], 3, MF_BYPOSITION | MF_SEPARATOR, 0, NULL);
		InsertMenu(tagMainWindow1.hPupMenu[0][2], 7, MF_BYPOSITION | MF_SEPARATOR, 0, NULL);
		InsertMenu(tagMainWindow1.hPupMenu[0][2], 12, MF_BYPOSITION | MF_SEPARATOR, 0, NULL);
		InsertMenu(tagMainWindow1.hPupMenu[0][2], 17, MF_BYPOSITION | MF_SEPARATOR, 0, NULL);
		InsertMenu(tagMainWindow1.hPupMenu[0][2], 20, MF_BYPOSITION | MF_SEPARATOR, 0, NULL);
		InsertMenu(tagMainWindow1.hPupMenu[0][2], 25, MF_BYPOSITION | MF_SEPARATOR, 0, NULL);
		InsertMenu(tagMainWindow1.hPupMenu[0][2], 28, MF_BYPOSITION | MF_SEPARATOR, 0, NULL);

		DrawMenuBar(hWnd);
	}
	tagMainWindow1.dwReOpenItem = (DWORD)-1;

	if (~dwAppFrag & APP_ENABLE_MINIMAM_HANDLE)
	{
		IF_LIKELY(IsWin7OrGreater())
		{
			for (i = 1; i < SIZEOF_NUM(tagMainWindow1.hIcon); i++) {
				tagMainWindow1.hIcon[i] = LoadIcon(tagMainWindow1.hDllInst, MAKEINTRESOURCE(IDI_TASKBAR_ICON_CHECK + (size_t)i - 1));
			}
			tagMainWindow1.nTaskbarButtonMessage = RegisterWindowMessage(_T("TaskbarButtonCreated"));
		}

		tagMainWindow1.clGrayTextColor = GetSysColor(COLOR_GRAYTEXT);
		tagMainWindow1.hBrush = CreateSolidBrush(GetSysColor(COLOR_WINDOW));

		for (i = 0; i < SIZEOF_NUM(tagMainWindow1.hButton); i++) {
			SetToolTip(tagMainWindow1.hTool[0], tagMainWindow1.hButton[i], tagMainWindow1.pToolTipText[i]);
		}

		if (dwAppFrag & APP_WINDOWALPHA) {
			SetLayeredWindow(hWnd);
		}

		// デバッグ中はクリップボードチェインを無効にする
		if (1)
		{
#if _MSC_VER < 1400
			HMODULE hDll;

			hDll = LOADSYSLIB(_T("KERNEL32.DLL"));
			if (hDll != NULL)
			{
				BOOL(WINAPI * IsDebuggerPresent)(VOID);
				const char* szFunc = "IsDebuggerPresent";

				IsDebuggerPresent = (BOOL(WINAPI*)())(GetProcAddress(hDll, szFunc));

				if (IsDebuggerPresent != NULL)
				{
#endif

					if (IsDebuggerPresent() == FALSE) {
						tagMainWindow1.hNextWnd = SetClipboardViewer(hWnd);
					}

#if _MSC_VER < 1400
				}
			}
#endif
		}
	}

	dwAppFrag &= ~APP_WINDOW_NOALPHA;

	SendMessage(tagMainWindow1.hProgress[0], PBM_SETPOS, 0, 0);
	SendMessage(tagMainWindow1.hEdit[1], EM_LIMITTEXT, (size_t)tagMainWindow1.tagHashThread1.dwHashLen * 2, 0);
	SendMessage(tagMainWindow1.hEdit[2], EM_SETREADONLY, TRUE, 0);
	SendMessage(tagMainWindow1.hEdit[2], EM_LIMITTEXT, _MAX_FNAME, 0);

	EnableWindow(tagMainWindow1.hButton[0], dwAppFrag & APP_HASHFILE_ADDMODE ? FALSE : TRUE);
	EnableWindow(tagMainWindow1.hButton[6], FALSE);
	EnableWindow(tagMainWindow1.hButton[7], FALSE);
	EnableWindow(tagMainWindow1.hButton[8], FALSE);
	EnableMenuItem(tagMainWindow1.hPupMenu[0][0], ID_MAINSUBMENU1_CLEAR, MF_GRAYED);
	EnableMenuItem(tagMainWindow1.hPupMenu[0][0], ID_MAINSUBMENU1_PREVIOUSFILE, MF_GRAYED);
	EnableMenuItem(tagMainWindow1.hPupMenu[0][0], ID_MAINSUBMENU1_NEXTFILE, MF_GRAYED);
	EnableMenuItem(tagMainWindow1.hPupMenu[0][0], ID_MAINSUBMENU1_SAVE, MF_GRAYED);
	EnableMenuItem(tagMainWindow1.hPupMenu[0][0], ID_MAINSUBMENU1_BENCHRESULT, MF_GRAYED);
	EnableMenuItem(tagMainWindow1.hPupMenu[0][0], 12, MF_BYPOSITION | MF_GRAYED);
	if (~dwAppFrag & APP_ENABLE_DEBUG) {
		EnableMenuItem(tagMainWindow1.hPupMenu[0][0], IDM_DEBUG, MF_GRAYED);
	}

	Hash_GetDpi(0, 0);

	// Windows98 対策 (VC2005コード)
	// Windows95, NT4.0 の処理は省略しています。> OPENFILENAME_SIZE_VERSION_400
	memset(&tagMainWindow1.ofnFileName[0], 0, sizeof(OPENFILENAME));
	memset(&tagMainWindow1.ofnFileName[1], 0, sizeof(OPENFILENAME));

#ifdef _UNICODE
	tagMainWindow1.ofnFileName[0].lStructSize = sizeof(OPENFILENAME);
#else
	tagMainWindow1.ofnFileName[0].lStructSize = IsWin2kOrGreater() ? sizeof(OPENFILENAME) : OPENFILENAME_SIZE_VERSION_400;
#endif /* _UNICODE */

	tagMainWindow1.ofnFileName[0].hwndOwner = hWnd;
	tagMainWindow1.ofnFileName[0].hInstance = tagMainWindow1.hInst;
	tagMainWindow1.ofnFileName[0].lpstrFilter = _T("すべてのファイル\0*.*\0\0");
	tagMainWindow1.ofnFileName[0].nFilterIndex = 0;
	tagMainWindow1.ofnFileName[0].lpstrFile = tagMainWindow1.pOfn1Buf;
	tagMainWindow1.ofnFileName[0].nMaxFile = MAX_PATH_SIZE * 256;
	tagMainWindow1.ofnFileName[0].lpstrDefExt = _T("");
	tagMainWindow1.ofnFileName[0].Flags = OFN_ALLOWMULTISELECT | OFN_EXPLORER | OFN_CREATEPROMPT | OFN_HIDEREADONLY | OFN_NODEREFERENCELINKS | OFN_OVERWRITEPROMPT;

	tagMainWindow1.ofnFileName[1].lStructSize = tagMainWindow1.ofnFileName[0].lStructSize;
	tagMainWindow1.ofnFileName[1].hwndOwner = hWnd;
	tagMainWindow1.ofnFileName[1].hInstance = tagMainWindow1.hInst;
	tagMainWindow1.ofnFileName[1].lpstrFile = tagMainWindow1.pHashFile;
	tagMainWindow1.ofnFileName[1].nMaxFile = MAX_PATH_SIZE;
	tagMainWindow1.ofnFileName[1].Flags = OFN_EXPLORER | OFN_CREATEPROMPT | OFN_HIDEREADONLY | OFN_NODEREFERENCELINKS | OFN_OVERWRITEPROMPT;

	tagMainWindow1.tagHashThread1.hWnd = hWnd;
	tagMainWindow1.tagHashThread1.nMessageID = RegisterWindowMessage(tagMainWindow1.pWindowClass);
	tagMainWindow1.tagHashThread1.pFilePath = tagMainWindow1.pFile;
	tagMainWindow1.dwOpenItem = 0;
	tagMainWindow1.dwTimeViewFileCount = 0;
	tagMainWindow1.dwAddFileModeFileCount = 0;

	IF_UNLIKELY(dwAppFrag & APP_NO_SETTING)
	{
		LoadString(tagMainWindow1.hInst, IDS_HASH_MSG1, tagMainWindow1.pStBuf, MAX_STRINGTABLE - 1);
		nRet = MainWindow_MessageBox(hWnd, tagMainWindow1.pStBuf, tagMainWindow1.pTitle, MB_ICONQUESTION | MB_YESNO);
		if (nRet == IDYES) {
			Hash_HashChange(HASH_SHA_256_TYPE);
		}
		SetIniFileSetting(tagMainWindow1.pINIFile);
		dwAppFrag &= ~APP_NO_SETTING;
	}

	Hash_HashChange(tagMainWindow1.tagHashThread1.dwHashType);
	WindowSize_LoadState(hWnd, _T("Option"), tagMainWindow1.pINIFile);
	Hash_FileView(tagMainWindow1.dwOpenItem);


	SetLayeredWindowAttributes_Func(hWnd, 0, ACTIVE_ALPHA, LWA_ALPHA);

	tagFileListWindow1.nWindowMessage = (UINT)-1;
	if (dwAppFrag & APP_ARG_FILELIST || dwAppFrag & APP_ENABLE_FILELIST) {
		FileListDlg();
	}

	dwAppFrag |= APP_WINDOW;
	dwRet = 0;

	// コマンドラインにファイル名が指定してあったら開きます。
	if (optind < __argc)
	{
		MainWindow_AddFile_Start();

		// 複数の引数ファイルを開く
		for (; optind < __argc; ++optind)
		{

#ifndef _NODLL
			_tcscpy_s(tagMainWindow1.pBuf, MAX_PATH_SIZE, __targv[optind]);
#else
			_tcsncpy(tagMainWindow1.pBuf, __targv[optind], MAX_PATH_SIZE);
			tagMainWindow1.pBuf[MAX_PATH_SIZE - 1] = '\0';
#endif // !_NODLL

			dwRet = HashThread_FileNameCheck(tagMainWindow1.pBuf);
			if (dwRet == 0) {
				continue;
			}

			if (_tfullpath(tagMainWindow1.pFile, tagMainWindow1.pBuf, MAX_PATH_SIZE) == NULL)
			{
				LoadString(tagMainWindow1.hInst, IDS_FILE_OPEN3, tagMainWindow1.pStBuf, MAX_STRINGTABLE - 1);
				MessageFormat(tagMainWindow1.pBuf, 2048, tagMainWindow1.pStBuf, tagMainWindow1.pBuf);
				MainWindow_MessageBox(hWnd, tagMainWindow1.pBuf, tagMainWindow1.pTitle, MB_ICONHAND);
				continue;
			}

			dwRet = MainWindow_AddFile((size_t)optind - 1);
			if (dwRet >= FILE_MAX_COUNTSIZE) {
				break;
			}
		}

		if (dwRet == (DWORD)-1)
		{
			FileRecodeFail(hWnd);
			return FALSE;
		}
		MainWindow_AddFile_End();
	}

	SetFocus(tagMainWindow1.hButton[0]);

	if (tagMainWindow1.tagHashThread1.tagMultiFile->dwFileCount == 0 && tagMainWindow1.dwAddFileModeFileCount == 0) {
		DragAcceptFiles(hWnd, TRUE);
	}
	PostMessage(hWnd, tagMainWindow1.tagHashThread1.nMessageID, APP_MESSAGE_WINDOWSET, 0);
	return TRUE;
}

LRESULT MainWindow_OnCommand(HWND hWnd, WPARAM wParam, LPARAM lParam)
{
	int wmId = LOWORD(wParam);
	//int wmEvent = HIWORD(wParam);

	// 選択されたメニューの解析:
	switch (wmId)
	{
	case IDC_BUTTON1:
		Open(TRUE);
		return TRUE;
	case IDM_OPEN:
	case ID_MAINSUBMENU1_OPEN:
		Open(FALSE);
		return TRUE;
	case IDM_SAVE:
	case IDC_BUTTON2:
	case ID_MAINSUBMENU1_SAVE:
		Save();
		return TRUE;
	case IDM_COPY:
	case IDM_NCOPY:
		if (tagFileListWindow1.hWnd)
		{
			SendMessage(tagFileListWindow1.hWnd, WM_COMMAND, wmId, lParam);
			break;
		}
		SWITCH_FALLTHROUGH
	case IDC_BUTTON3:
		if (tagMainWindow1.hSendWindow != NULL)
		{
			SendMessage(tagMainWindow1.hSendWindow, WM_COPY, NULL, NULL);
			SetFocus(tagMainWindow1.hButton[0]);
		}
		SWITCH_FALLTHROUGH
	case ID_MAINSUBMENU1_COPY:
	case ID_MAINSUBMENU1_SIMPLECOPY:
	case ID_MAINSUBMENU1_EXTCOPY1:
	case ID_MAINSUBMENU1_EXTCOPY2:
	case ID_MAINSUBMENU1_EXTCOPY3:
	case ID_MAINSUBMENU1_EXTCOPY4:
	case ID_MAINSUBMENU1_EXTCOPY5:
		return MainWindow_OnMessageClipboardCopy(hWnd, wParam, lParam);
	case IDM_PASTE:
	case IDC_BUTTON4:
	case ID_MAINSUBMENU1_PASTE:
		return MainWindow_OnMessageClipboardPaste(hWnd, wParam, lParam);
	case IDM_CLEAR:
	case ID_MAINSUBMENU1_CLEAR:
		return MainWindow_OnMessageClear(hWnd, wParam, lParam);
	case ID_MAINSUBMENU1_BENCH:
		return MainWindow_OnMessageBench(hWnd, wParam, lParam);
	case ID_MAINSUBMENU1_BENCH2:
		return MainWindow_OnMessageBench2(hWnd, wParam, lParam);
	case ID_MAINSUBMENU1_BENCHRESULT:
		if (tagMainWindow1.pBecthResult) {
			MainWindow_MessageBox(hWnd, tagMainWindow1.pBecthResult, tagMainWindow1.pTitle, MB_ICONINFORMATION);
		}
		return TRUE;
	case ID_MAINSUBMENU1_README:
		return ReadMeOpen();
	case ID_MAINSUBMENU1_UPDATE:
		return VersionCheck();
	case ID_MAINSUBMENU1_FILELIST:
		FileListDlg();
		return TRUE;
	case IDC_BUTTON5:
	case ID_MAINSUBMENU1_OPTION:
		OptionDlg(hWnd);
		return TRUE;
	case ID_MAINSUBMENU1_FILEASSOC:
		return MainWindow_OnMessageFileAssoc(hWnd, wParam, lParam);
	case IDM_ABOUT:
	case IDC_BUTTON6:
	case ID_MAINSUBMENU1_VERSION:
		AboutDlg(hWnd);
		return TRUE;
	case IDC_BUTTON7:
	case ID_MAINSUBMENU1_PREVIOUSFILE:
		return MainWindow_OnMessagePreviousFile(hWnd, wParam, lParam);
	case IDC_BUTTON8:
	case ID_MAINSUBMENU1_NEXTFILE:
		return MainWindow_OnMessageNextFile(hWnd, wParam, lParam);
	case IDC_EDIT1:
		return MainWindow_OnMessageEdit1(hWnd, wParam, lParam);
	case IDC_EDIT2:
		return MainWindow_OnMessageEdit2(hWnd, wParam, lParam);
	case IDC_EDIT3:
		return MainWindow_OnMessageEdit3(hWnd, wParam, lParam);
	case IDC_BUTTON9:
		return MainWindow_OnMessageRename(hWnd, wParam, lParam);
	case IDM_CANCEL:
		Cancel(FALSE);
		return TRUE;
	case IDM_VK_F5:
	case IDM_REOPEN:
	case ID_MAINSUBMENU1_HASH:
		return MainWindow_OnMessageReOpen(hWnd, wParam, lParam);
	case IDM_PAUSE:
		Pause();
		return TRUE;
	case ID_MAINSUBMENU1_HASH1:
	case ID_MAINSUBMENU1_HASH2:
	case ID_MAINSUBMENU1_HASH3:
	case ID_MAINSUBMENU1_HASH4:
	case ID_MAINSUBMENU1_HASH5:
	case ID_MAINSUBMENU1_HASH6:
	case ID_MAINSUBMENU1_HASH7:
	case ID_MAINSUBMENU1_HASH8:
	case ID_MAINSUBMENU1_HASH9:
	case ID_MAINSUBMENU1_HASH10:
	case ID_MAINSUBMENU1_HASH11:
	case ID_MAINSUBMENU1_HASH12:
	case ID_MAINSUBMENU1_HASH13:
	case ID_MAINSUBMENU1_HASH14:
	case ID_MAINSUBMENU1_HASH15:
	case ID_MAINSUBMENU1_HASH16:
	case ID_MAINSUBMENU1_HASH17:
	case ID_MAINSUBMENU1_HASH18:
	case ID_MAINSUBMENU1_HASH19:
	case ID_MAINSUBMENU1_HASH20:
	case ID_MAINSUBMENU1_HASH21:
	case ID_MAINSUBMENU1_HASH22:
	case ID_MAINSUBMENU1_HASH23:
	case ID_MAINSUBMENU1_HASH24:
	case ID_MAINSUBMENU1_HASH25:
	case ID_MAINSUBMENU1_HASH26:
	case ID_MAINSUBMENU1_HASH27:
	case ID_MAINSUBMENU1_HASH28:
	case ID_MAINSUBMENU1_HASH29:
	case ID_MAINSUBMENU1_HASH30:
		Hash_ChangeMsg(wmId - ID_MAINSUBMENU1_HASH1);
		return TRUE;
	case ID_MAINSUBMENU1_EXIT:
		SendMessage(hWnd, WM_CLOSE, 0, 0);
		return TRUE;
	case IDM_ALLSERECT:
		if (tagFileListWindow1.hWnd) {
			PostMessage(tagFileListWindow1.hWnd, WM_COMMAND, wParam, lParam);
		}
		return TRUE;
	case IDM_DEBUG:
		if (dwAppFrag & APP_ENABLE_DEBUG) {
			DebugDlg(hWnd);
		}
		return TRUE;
	}
	return FALSE;
}

LRESULT MainWindow_OnMessageClear(HWND hWnd, WPARAM wParam, LPARAM lParam)
{
	UNREFERENCED_PARAMETER(hWnd);
	UNREFERENCED_PARAMETER(wParam);
	UNREFERENCED_PARAMETER(lParam);

	if (dwAppFrag & APP_HASHTHREAD)
	{
		int nRet = 0;

		LoadString(tagMainWindow1.hInst, IDS_CLEAR2, tagMainWindow1.pStBuf, MAX_STRINGTABLE - 1);
		MessageFormat(tagMainWindow1.pBuf, 2048, tagMainWindow1.pStBuf, tagMainWindow1.cpHashName, tagMainWindow1.pFile);

		nRet = MainWindow_MessageBox(tagMainWindow1.hWnd, tagMainWindow1.pBuf, tagMainWindow1.pTitle, MB_YESNOCANCEL | MB_ICONQUESTION);
		if (nRet == IDYES) {
			Cancel(TRUE);
		}
	}
	else {
		Clear(TRUE);
	}
	return TRUE;
}

LRESULT MainWindow_OnMessageRename(HWND hWnd, WPARAM wParam, LPARAM lParam)
{
	UNREFERENCED_PARAMETER(wParam);
	UNREFERENCED_PARAMETER(lParam);
	int nRet = 0;

	if (dwAppFrag & APP_HASHTHREAD) {
		goto RENAME_END;
	}

	nRet = (int)SendMessage(tagMainWindow1.hEdit[2], EM_GETMODIFY, 0, 0);
	if (nRet == FALSE) {
		goto RENAME_END;
	}

	GetWindowText(tagMainWindow1.hEdit[2], tagMainWindow1.pFileNameEdit, MAX_PATH_SIZE);
	nRet = HashThread_ReNameFileRecode(&tagMainWindow1.tagHashThread1, tagMainWindow1.dwOpenItem, tagMainWindow1.pFileNameEdit);
	if (nRet != 0)
	{
		TCHAR* pFileName = NULL;
		TCHAR* pHashFileName = NULL;

		pFileName = GetTCharToFileName(tagMainWindow1.pFile);
		pHashFileName = GetTCharToFileName(tagMainWindow1.pHashFile);
		if (_tcsicmp(pFileName, pHashFileName) == 0)
		{
			pHashFileName = qtcscpy(pHashFileName, tagMainWindow1.pFileNameEdit);
			qtcscpy(pHashFileName, tagMainWindow1.pFileExtension);
		}
		qtcscpy(pFileName, tagMainWindow1.pFileNameEdit);
		Hash_FileView(tagMainWindow1.dwOpenItem);
		LoadString(tagMainWindow1.hInst, IDS_FILE_RENAME1, tagMainWindow1.pStBuf, MAX_STRINGTABLE - 1);
		MessageFormat(tagMainWindow1.pBuf, 2048, tagMainWindow1.pStBuf, tagMainWindow1.pFileNameEdit);
		tagMainWindow1.pFileNameEdit[0] = '\0';
		SendMessage(tagMainWindow1.hEdit[2], EM_SETMODIFY, TRUE, 0);
		MainWindow_MessageBox(hWnd, tagMainWindow1.pBuf, tagMainWindow1.pTitle, MB_ICONINFORMATION);
	}

RENAME_END:
	EnableWindow(tagMainWindow1.hButton[8], FALSE);
	SetFocus(tagMainWindow1.hButton[0]);
	return TRUE;
}

LRESULT MainWindow_OnMessageBench(HWND hWnd, WPARAM wParam, LPARAM lParam)
{
	UNREFERENCED_PARAMETER(hWnd);
	UNREFERENCED_PARAMETER(wParam);
	UNREFERENCED_PARAMETER(lParam);
	int nRet = 0;

	if (dwAppFrag & APP_HASHTHREAD) {
		return FALSE;
	}

	LoadString(tagMainWindow1.hInst, IDC_BENCH_TEXT1, tagMainWindow1.pStBuf, MAX_STRINGTABLE - 1);
	MessageFormat(tagMainWindow1.pBuf, 2048, tagMainWindow1.pStBuf, tagMainWindow1.pFile);
	nRet = MainWindow_MessageBox(tagMainWindow1.hWnd, tagMainWindow1.pBuf, tagMainWindow1.pTitle, MB_YESNOCANCEL | MB_ICONQUESTION);
	if (nRet == IDYES)
	{
		unsigned int threadID = 0;

		tagMainWindow1.hBenchHandle = (HANDLE)_beginthreadex(NULL, 0, Bench, &tagMainWindow1, 0, &threadID);
		IF_UNLIKELY(tagMainWindow1.hBenchHandle == NULL) {
			DebugText(DEBUG_MISSING, _T("ベンチスレッドの作成"), GetLastError());
		}
	}
	return TRUE;
}

LRESULT MainWindow_OnMessageBench2(HWND hWnd, WPARAM wParam, LPARAM lParam)
{
	UNREFERENCED_PARAMETER(hWnd);
	UNREFERENCED_PARAMETER(wParam);
	UNREFERENCED_PARAMETER(lParam);
	static int nSet = 0;
	DWORD* pFileBuffer = (DWORD*)tagMainWindow1.tagHashThread1.pFileBuffer;
	tagHashThread_FileRecode* ptagHashFile_Recode = tagMainWindow1.tagHashThread1.tagMultiFile->ptagFileRecode;
	extern unsigned __int64 nHashBenchSize[MAX_HASH_TYPE + 1];

	IF_UNLIKELY(dwAppFrag & APP_HASHTHREAD) {
		return FALSE;
	}

	dwAppFrag |= APP_HASHTHREAD;

	IF_LIKELY(!tagMainWindow1.nRandamDataState)
	{
		const size_t nBufSize = tagMainWindow1.tagHashThread1.dwFileBufferAllocateSize;
		size_t i = 0;

		StatusOut(_T("ランダムデータを作成しています..."));

		srand((unsigned int)tagMainWindow1.nTime);
		for (i = 0; i < nBufSize; i += 4) {
			*pFileBuffer++ = (rand() << 16) + rand();
		}
		tagMainWindow1.nRandamDataState = 1;
	}

	tagMainWindow1.tagHashThread1.tagMultiFile->dwFileCount = 1;
	tagMainWindow1.tagHashThread1.tagMultiFile->llFileAllCurrentSize = 0;

	ptagHashFile_Recode->szFileName[0] = '\0';
	ptagHashFile_Recode->nFileNameLength = 0;
	ptagHashFile_Recode->dwFileLastError = (DWORD)-1;
	ptagHashFile_Recode->dwFileHashType = tagMainWindow1.tagHashThread1.dwHashType;
	ptagHashFile_Recode->dwHashFileID = (DWORD)-1;
	ptagHashFile_Recode->dwHashFileHashLine = (DWORD)-1;
	ptagHashFile_Recode->nHashFileCmp = (DWORD)-1;
	ptagHashFile_Recode->llFileSize = nHashBenchSize[tagMainWindow1.tagHashThread1.dwHashType];
	ptagHashFile_Recode->dwFileAttributes = 0;
	memset(&ptagHashFile_Recode->ftFileLastWriteTime, 0, sizeof(FILETIME));
	tagMainWindow1.tagHashThread1.tagMultiFile->llFileAllSize = ptagHashFile_Recode->llFileSize;

	tagMainWindow1.dwEndTime = 0;
	tagMainWindow1.tagHashThread1.hHashThreadHandle = (HANDLE)_beginthreadex(NULL, 0, (*HashThread_MultiFile_Bench_Thread), &tagMainWindow1.tagHashThread1, 0, &tagMainWindow1.tagHashThread1.nThreadID);
	IF_UNLIKELY(tagMainWindow1.tagHashThread1.hHashThreadHandle == NULL)
	{
		dwAppFrag &= ~APP_HASHTHREAD;
		DebugText(DEBUG_MISSING, _T("HashBenchスレッドの作成"), GetLastError());
	}
	return TRUE;
}

LRESULT MainWindow_OnMessageClipboardCopy(HWND hWnd, WPARAM wParam, LPARAM lParam)
{
	UNREFERENCED_PARAMETER(hWnd);
	UNREFERENCED_PARAMETER(lParam);
	static char szTextBuf[2048];
	static char szRtfBuf[2048];
	TCHAR* pDst = tagMainWindow1.pBuf;
	DWORD dwHashType = tagMainWindow1.tagHashThread1.dwHashType;
	char szHashName[HASH_LOADSTRING] = { 0 };
	char szHashString[HASH_LOADSTRING] = { 0 };
	TCHAR szSize[HASH_LOADSTRING] = { 0 };
	TCHAR* pHashString = tagMainWindow1.pHashString;
	TCHAR* pFileName = NULL;
	char* pRtf = (char*)(tagMainWindow1.pBuf + 2048);
	char* pHtml = (char*)(tagMainWindow1.pBuf + 4096);
	int wmId = LOWORD(wParam);
	extern const size_t nHashColorType[MAX_HASH_TYPE + 1];
	extern const char* cpRtfColorTag[HASH_MAX_COLOR];
	extern const char* cpHtmlColorTag[HASH_MAX_COLOR];


	IF_UNLIKELY(dwAppFrag & APP_HASHTHREAD) {
		return FALSE;
	}

	// RTF Text
	pRtf = qstrcpy(pRtf, HASH_RTF_HEADER1);
	pRtf = qstrcpy(pRtf, cpRtfColorTag[nHashColorType[dwHashType]]);
	pRtf = qstrcpy(pRtf, HASH_RTF_HEADER2);

	// HTML Text
	pHtml = qstrcpy(pHtml, HASH_HTML_HEADER);

	switch (LOWORD(wParam))
	{
	case IDM_COPY:
	case IDC_BUTTON3:
	case ID_MAINSUBMENU1_COPY:
	case ID_MAINSUBMENU1_EXTCOPY4:
	case ID_MAINSUBMENU1_EXTCOPY5:
		TCharToCharConv2(tagMainWindow1.tagHashThread1.cpHashName, szHashName);

		// CF_TEXT
		pDst = qtcscpy(pDst, tagMainWindow1.tagHashThread1.cpHashName);
		pDst = qtcscpy(pDst, _T(HASH_NAME_SPACE));

		// RTF Text
		pRtf = qstrcpy(pRtf, HASH_RTF_COLOR_TAG1);
		pRtf = qstrcpy(pRtf, szHashName);
		pRtf = qstrcpy(pRtf, HASH_RTF_COLOR_TAG0);
		pRtf = qstrcpy(pRtf, HASH_NAME_SPACE);

		// HTML Text
		pHtml = qstrcpy(pHtml, HASH_HTML_COLOR_TAG0);
		pHtml = qstrcpy(pHtml, cpHtmlColorTag[nHashColorType[dwHashType]]);
		pHtml = qstrcpy(pHtml, HASH_HTML_COLOR_TAG1);
		pHtml = qstrcpy(pHtml, szHashName);
		pHtml = qstrcpy(pHtml, HASH_HTML_COLOR_TAG2);
		pHtml = qstrcpy(pHtml, HASH_NAME_SPACE);
		goto COPY_JUMP0;
	case ID_MAINSUBMENU1_SIMPLECOPY:
	case IDM_NCOPY:
COPY_JUMP0:
		if (wmId == ID_MAINSUBMENU1_EXTCOPY5) {
			pHashString = tagMainWindow1.pBase64String;
		}
		TCharToCharConv2(pHashString, szHashString);

		// CF_TEXT
		pDst = qtcscpy(pDst, pHashString);
		pDst = qtcscpy(pDst, _T(HASH_TEXT_RETURNCODE));

		// RTF Text
		pRtf = qstrcpy(pRtf, szHashString);
		pRtf = qstrcpy(pRtf, HASH_RTF_RETURNCODE);

		// HTML Text
		pHtml = qstrcpy(pHtml, szHashString);
		pHtml = qstrcpy(pHtml, HASH_HTML_RETURNCODE);

		if (wmId == ID_MAINSUBMENU1_EXTCOPY4)
		{
			const TCHAR* pName = _T("ファイル名");

			// CF_TEXT
			pDst = qtcscpy(pDst, pName);
			pDst = qtcscpy(pDst, _T(HASH_NAME_SPACE));

			// RTF Text
			TCharToCharConv2(pName, szTextBuf);
			CharToRtfEncodeChar(szTextBuf, szRtfBuf);
			pRtf = qstrcpy(pRtf, HASH_RTF_COLOR_TAG0);
			pRtf = qstrcpy(pRtf, szRtfBuf);
			pRtf = qstrcpy(pRtf, HASH_NAME_SPACE);

			// HTML Text
			TCharToUtf8CharConv2(pName, szTextBuf);
			pHtml = qstrcpy(pHtml, szTextBuf);
			pHtml = qstrcpy(pHtml, HASH_NAME_SPACE);
			goto EXTCOPY4_JUMP1;
		}
		break;
	case ID_MAINSUBMENU1_EXTCOPY1:
	case ID_MAINSUBMENU1_EXTCOPY2:
	case ID_MAINSUBMENU1_EXTCOPY3:
		if (1)
		{
			switch (wmId)
			{
			case ID_MAINSUBMENU1_EXTCOPY2:
				pFileName = tagMainWindow1.pFile;
				break;
			case ID_MAINSUBMENU1_EXTCOPY3:
EXTCOPY4_JUMP2:
				pFileName = szSize;
				FileSizeText2ToTChar2(HashThread_GetFileRecode_Core(&tagMainWindow1.tagHashThread1, tagMainWindow1.dwOpenItem)->llFileSize, pFileName);
				break;
			default:
EXTCOPY4_JUMP1:
				pFileName = GetTCharToFileName(tagMainWindow1.pFile);
			}


			// CF_TEXT
			pDst = qtcscpy(pDst, pFileName);
			pDst = qtcscpy(pDst, _T(HASH_TEXT_RETURNCODE));

			// RTF Text
			TCharToCharConv2(pFileName, szTextBuf);
			CharToRtfEncodeChar(szTextBuf, szRtfBuf);
			pRtf = qstrcpy(pRtf, HASH_RTF_COLOR_TAG0);
			pRtf = qstrcpy(pRtf, szRtfBuf);
			pRtf = qstrcpy(pRtf, HASH_RTF_RETURNCODE);

			// HTML Text
			TCharToUtf8CharConv2(pFileName, szTextBuf);
			pHtml = qstrcpy(pHtml, szTextBuf);
			pHtml = qstrcpy(pHtml, HASH_HTML_RETURNCODE);

			if (wmId == ID_MAINSUBMENU1_EXTCOPY4)
			{
				const TCHAR* pName = _T("サイズ");

				// CF_TEXT
				pDst = qtcscpy(pDst, pName);
				pDst = qtcscpy(pDst, _T(HASH_NAME_SPACE));

				// RTF Text
				TCharToCharConv2(pName, szTextBuf);
				CharToRtfEncodeChar(szTextBuf, szRtfBuf);
				pRtf = qstrcpy(pRtf, HASH_RTF_COLOR_TAG0);
				pRtf = qstrcpy(pRtf, szRtfBuf);
				pRtf = qstrcpy(pRtf, HASH_NAME_SPACE);

				// HTML Text
				TCharToUtf8CharConv2(pName, szTextBuf);
				pHtml = qstrcpy(pHtml, szTextBuf);
				pHtml = qstrcpy(pHtml, HASH_NAME_SPACE);
				wmId = 0;
				goto EXTCOPY4_JUMP2;
			}
		}
		break;
	}

	// CF_TEXT
	pDst -= 2;
	*pDst++ = '\0';

	// RTF Text
	pRtf -= sizeof(HASH_RTF_RETURNCODE) - 1;
	pRtf = qstrcpy(pRtf, HASH_RTF_FOOTER);

	// HTML Text
	pHtml = qstrcpy(pHtml, HASH_HTML_FOOTER);

	SetClipboardText2(tagMainWindow1.hWnd, tagMainWindow1.pBuf, (char*)(tagMainWindow1.pBuf + 2048), (char*)(tagMainWindow1.pBuf + 4096));
	return TRUE;
}

LRESULT MainWindow_OnMessageClipboardPaste(HWND hWnd, WPARAM wParam, LPARAM lParam)
{
	UNREFERENCED_PARAMETER(hWnd);
	UNREFERENCED_PARAMETER(wParam);
	UNREFERENCED_PARAMETER(lParam);
	DWORD dwItem = 0;

	if (dwAppFrag & APP_HASHTHREAD) {
		return FALSE;
	}

	dwItem = HashThread_GetCountItem(&tagMainWindow1.tagHashThread1);

	if (dwItem > 0 && HashThread_GetFileRecode_Core(&tagMainWindow1.tagHashThread1, tagMainWindow1.dwOpenItem)->dwFileHashType == tagMainWindow1.tagHashThread1.dwHashType) {
		ClipboardPaste();
	}
	return TRUE;
}

LRESULT MainWindow_OnSize(HWND hWnd, WPARAM wParam, LPARAM lParam)
{
	UNREFERENCED_PARAMETER(wParam);
	int nArg[4] = { 0 };

	nWindowX = LOWORD(lParam);
	nWindowY = HIWORD(lParam);

	//IDC_STATIC1,24,11,96,12
	nArg[0] = MDIVX(24);
	nArg[1] = MDIVY(11);
	nArg[2] = nWindowX / 2 - MDIVX(DIALOG1_X / 2 - 101);
	nArg[3] = nStringY[0] + nEditMarginY[0];
	MoveWindow(tagMainWindow1.hStatic[0], nArg[0], nArg[1], nArg[2], nArg[3], FALSE);

	//IDC_PROGRESS1,126,12,126,10
	nArg[0] = nWindowX - (nWindowX / 2) - MDIVX(DIALOG1_X / 2 - 126);
	nArg[1] = MDIVY(11) + 1;
	nArg[2] = nWindowX / 2 - MDIVX(DIALOG1_X / 2 - 126);
	nArg[3] = nStringY[0] + nEditMarginY[0] - 2;
	MoveWindow(tagMainWindow1.hProgress[0], nArg[0], nArg[1], nArg[2], nArg[3], FALSE);

	//IDC_STATIC2,24,24,216,12
	nArg[0] = MDIVX(24);
	nArg[1] = MDIVY(25);
	nArg[2] = nWindowX - MDIVX(DIALOG1_X - 216);
	nArg[3] = nStringY[0] + nEditMarginY[0];
	MoveWindow(tagMainWindow1.hStatic[1], nArg[0], nArg[1], nArg[2], nArg[3], FALSE);

	/*
	//IDC_EDIT1,6,36,180,15
	nArg[0] = MDIVX(6);
	nArg[1] = MDIVY(38);
	nArg[2] = nWindowX - MDIVX(DIALOG1_X - 180);
	nArg[3] = nHashY[dwHashType] + nEditMarginY[2];
	MoveWindow(tagMainWindow1.hEdit[0], nArg[0], nArg[1], nArg[2], nArg[3], FALSE);

	//IDC_EDIT2,6,54,180,15
	nArg[0] = MDIVX(6);
	nArg[1] = MDIVY(56) + nMLModeDiff[0];
	nArg[2] = nWindowX - MDIVX(DIALOG1_X - 180);
	nArg[3] = nHashY[dwHashType] + nEditMarginY[2];
	MoveWindow(tagMainWindow1.hEdit[1], nArg[0], nArg[1], nArg[2], nArg[3], FALSE);
	*/

	//IDC_BUTTON3,192,36,60,15
	nArg[0] = nWindowX - MDIVX(DIALOG1_X - 192);
	nArg[1] = MDIVY(38) - 1;
	nArg[2] = MDIVX(60);
	nArg[3] = nStringY[2] + nEditMarginY[0] + 2 + nMLModeDiff[0];
	MoveWindow(tagMainWindow1.hButton[2], nArg[0], nArg[1], nArg[2], nArg[3], FALSE);

	//IDC_BUTTON4,192,54,60,15
	nArg[0] = nWindowX - MDIVX(DIALOG1_X - 192);
	nArg[1] = MDIVY(56) - 1 + nMLModeDiff[0];
	nArg[2] = MDIVX(60);
	nArg[3] = nStringY[2] + nEditMarginY[0] + 2;
	MoveWindow(tagMainWindow1.hButton[3], nArg[0], nArg[1], nArg[2], nArg[3], FALSE);

	//IDC_BUTTON1,192,78,60,15
	nArg[0] = nWindowX - MDIVX(DIALOG1_X - 192);
	nArg[1] = MDIVY(80) - 1 + nMLModeDiff[0];
	nArg[2] = MDIVX(60);
	nArg[3] = nStringY[2] + nEditMarginY[0] + 2;
	MoveWindow(tagMainWindow1.hButton[0], nArg[0], nArg[1], nArg[2], nArg[3], FALSE);

	//IDC_GROUP1,6,78,180,84
	nArg[0] = MDIVX(6);
	nArg[1] = MDIVY(80) + nMLModeDiff[0];
	nArg[2] = nWindowX - MDIVX(DIALOG1_X - 180);
	nArg[3] = MDIVY(84);
	MoveWindow(tagMainWindow1.hGroup[0], nArg[0], nArg[1], nArg[2], nArg[3], FALSE);

	//IDC_BUTTON2,192,96,60,15
	nArg[0] = nWindowX - MDIVX(DIALOG1_X - 192);
	nArg[1] = MDIVY(98) - 1 + nMLModeDiff[0];
	nArg[2] = MDIVX(60);
	nArg[3] = nStringY[2] + nEditMarginY[0] + 2;
	MoveWindow(tagMainWindow1.hButton[1], nArg[0], nArg[1], nArg[2], nArg[3], FALSE);

	//IDC_EDIT3,12,96,168,13
	nArg[0] = MDIVX(12);
	nArg[1] = MDIVY(98) + nMLModeDiff[0];
	nArg[2] = nWindowX - MDIVX(DIALOG1_X - 138);
	nArg[3] = nStringY[0] + nEditMarginY[0];
	MoveWindow(tagMainWindow1.hEdit[2], nArg[0], nArg[1], nArg[2], nArg[3], FALSE);

	//IDC_BUTTON5,192,114,60,15
	nArg[0] = nWindowX - MDIVX(DIALOG1_X - 192);
	nArg[1] = MDIVY(116) - 1 + nMLModeDiff[0];
	nArg[2] = MDIVX(60);
	nArg[3] = nStringY[2] + nEditMarginY[0] + 2;
	MoveWindow(tagMainWindow1.hButton[4], nArg[0], nArg[1], nArg[2], nArg[3], FALSE);

	//IDC_STATIC3,12,114,35,12
	nArg[0] = MDIVX(12);
	nArg[1] = MDIVY(116) + nMLModeDiff[0];
	nArg[2] = nWindowX - MDIVX(DIALOG1_X - 35);
	nArg[3] = nStringY[0] + nEditMarginY[0];
	MoveWindow(tagMainWindow1.hStatic[2], nArg[0], nArg[1], nArg[2], nArg[3], FALSE);

	//IDC_STATIC4,36,114,144,12
	nArg[0] = MDIVX(40);
	nArg[1] = MDIVY(116) + nMLModeDiff[0];
	nArg[2] = nWindowX - MDIVX(DIALOG1_X - 140);
	nArg[3] = nStringY[0] + nEditMarginY[0];
	MoveWindow(tagMainWindow1.hStatic[3], nArg[0], nArg[1], nArg[2], nArg[3], FALSE);

	//IDC_STATIC5,12,126,35,13
	nArg[0] = MDIVX(12);
	nArg[1] = MDIVY(128) + nMLModeDiff[0];
	nArg[2] = nWindowX - MDIVX(DIALOG1_X - 35);
	nArg[3] = nStringY[0] + nEditMarginY[0];
	MoveWindow(tagMainWindow1.hStatic[4], nArg[0], nArg[1], nArg[2], nArg[3], FALSE);

	//IDC_STATIC6,36,126,144,12
	nArg[0] = MDIVX(40);
	nArg[1] = MDIVY(128) + nMLModeDiff[0];
	nArg[2] = nWindowX - MDIVX(DIALOG1_X - 140);
	nArg[3] = nStringY[0] + nEditMarginY[0];
	MoveWindow(tagMainWindow1.hStatic[5], nArg[0], nArg[1], nArg[2], nArg[3], FALSE);

	//IDC_BUTTON6,192,132,60,15
	nArg[0] = nWindowX - MDIVX(DIALOG1_X - 192);
	nArg[1] = MDIVY(134) - 1 + nMLModeDiff[0];
	nArg[2] = MDIVX(60);
	nArg[3] = nStringY[2] + nEditMarginY[0] + 2;
	MoveWindow(tagMainWindow1.hButton[5], nArg[0], nArg[1], nArg[2], nArg[3], FALSE);

	//IDC_BUTTON7,192,152,30,15
	nArg[0] = nWindowX - MDIVX(DIALOG1_X - 192);
	nArg[1] = MDIVY(152) - 1 + nMLModeDiff[0];
	nArg[2] = MDIVX(30);
	nArg[3] = nStringY[2] + nEditMarginY[0] + 2;
	MoveWindow(tagMainWindow1.hButton[6], nArg[0], nArg[1], nArg[2], nArg[3], FALSE);

	//IDC_BUTTON8,222,152,30,15
	nArg[0] = nWindowX - MDIVX(DIALOG1_X - 222);
	nArg[1] = MDIVY(152) - 1 + nMLModeDiff[0];
	nArg[2] = MDIVX(30);
	nArg[3] = nStringY[2] + nEditMarginY[0] + 2;
	MoveWindow(tagMainWindow1.hButton[7], nArg[0], nArg[1], nArg[2], nArg[3], FALSE);

	//IDC_BUTTON9,150,97,30,15
	nArg[0] = nWindowX - MDIVX(DIALOG1_X - 150);
	nArg[1] = MDIVY(98) - nEditMarginY[0] / 2 - 1 + nMLModeDiff[0];
	nArg[2] = MDIVX(30);
	nArg[3] = nStringY[2] + nEditMarginY[0] + 2;
	MoveWindow(tagMainWindow1.hButton[8], nArg[0], nArg[1], nArg[2], nArg[3], FALSE);

	//IDC_STATIC7,12,138,35,13
	nArg[0] = MDIVX(12);
	nArg[1] = MDIVY(140) + nMLModeDiff[0];
	nArg[2] = nWindowX - MDIVX(DIALOG1_X - 35);
	nArg[3] = nStringY[0] + nEditMarginY[0];
	MoveWindow(tagMainWindow1.hStatic[6], nArg[0], nArg[1], nArg[2], nArg[3], FALSE);

	//IDC_STATIC8,40,138,140,12
	nArg[0] = MDIVX(40);
	nArg[1] = MDIVY(140) + nMLModeDiff[0];
	nArg[2] = nWindowX - MDIVX(DIALOG1_X - 140);
	nArg[3] = nStringY[0] + nEditMarginY[0];
	MoveWindow(tagMainWindow1.hStatic[7], nArg[0], nArg[1], nArg[2], nArg[3], FALSE);

	if (!tagMainWindow1.nIsWindowSet) {
		InvalidateRect(hWnd, NULL, TRUE);
	}
	return TRUE;
}

LRESULT MainWindow_OnClose(HWND hWnd, WPARAM wParam, LPARAM lParam)
{
	UNREFERENCED_PARAMETER(wParam);
	UNREFERENCED_PARAMETER(lParam);

	if (dwAppFrag & APP_FOLDERINIT)
	{
		LoadString(tagMainWindow1.hInst, IDS_FILELIST_CLOSE, tagMainWindow1.pStBuf, MAX_STRINGTABLE - 1);
		MainWindow_MessageBox(hWnd, tagMainWindow1.pStBuf, tagMainWindow1.pTitle, MB_ICONHAND);
		return FALSE;
	}
	DestroyWindow(hWnd);
	return TRUE;
}

LRESULT MainWindow_OnMessageEdit1(HWND hWnd, WPARAM wParam, LPARAM lParam)
{
	switch (HIWORD(wParam))
	{
	case EN_SETFOCUS:
		return MainWindow_Edit1_OnSetFocus(hWnd, wParam, lParam);
	case EN_KILLFOCUS:
		return MainWindow_Edit1_OnKillFocus(hWnd, wParam, lParam);
	}
	return FALSE;
}

LRESULT MainWindow_Edit1_OnSetFocus(HWND hWnd, WPARAM wParam, LPARAM lParam)
{
	UNREFERENCED_PARAMETER(hWnd);
	UNREFERENCED_PARAMETER(wParam);
	UNREFERENCED_PARAMETER(lParam);

	tagMainWindow1.hSendWindow = tagMainWindow1.hEdit[0];
	return TRUE;
}

LRESULT MainWindow_Edit1_OnKillFocus(HWND hWnd, WPARAM wParam, LPARAM lParam)
{
	UNREFERENCED_PARAMETER(hWnd);
	UNREFERENCED_PARAMETER(wParam);
	UNREFERENCED_PARAMETER(lParam);

	tagMainWindow1.hSendWindow = NULL;
	return TRUE;
}

LRESULT MainWindow_OnMessageEdit2(HWND hWnd, WPARAM wParam, LPARAM lParam)
{
	switch (HIWORD(wParam))
	{
	case EN_CHANGE:
		return MainWindow_Edit2_OnChange(hWnd, wParam, lParam);
	case EN_SETFOCUS:
		return MainWindow_Edit2_OnSetFocus(hWnd, wParam, lParam);
	case EN_KILLFOCUS:
		return MainWindow_Edit2_OnKillFocus(hWnd, wParam, lParam);
	}
	return FALSE;
}

LRESULT MainWindow_Edit2_OnChange(HWND hWnd, WPARAM wParam, LPARAM lParam)
{
	UNREFERENCED_PARAMETER(hWnd);
	UNREFERENCED_PARAMETER(wParam);
	UNREFERENCED_PARAMETER(lParam);
	int nRet = 0;
	const tagHashThread_FileRecode* cptagHashFile_Recode = HashThread_GetFileRecode(&tagMainWindow1.tagHashThread1, tagMainWindow1.dwOpenItem);

	IF_UNLIKELY(cptagHashFile_Recode->dwFileAttributes == (DWORD)-1) {
		return FALSE;
	}

	if (~dwAppFrag & APP_HASH_INPUT_EDIT) {
		return FALSE;
	}

	nRet = GetWindowText(tagMainWindow1.hEdit[1], tagMainWindow1.pBuf, 255);
	if (nRet > 0)
	{
		const int nHashSize = (int)cdwHashLen[cptagHashFile_Recode->dwFileHashType] * 2;
		qtcscpy(tagMainWindow1.pHashCompString, tagMainWindow1.pBuf);
		if (nRet == nHashSize) {
			Hash_HashComp(tagMainWindow1.dwOpenItem);
		}
	}
	return TRUE;
}

LRESULT MainWindow_Edit2_OnSetFocus(HWND hWnd, WPARAM wParam, LPARAM lParam)
{
	UNREFERENCED_PARAMETER(hWnd);
	UNREFERENCED_PARAMETER(wParam);
	UNREFERENCED_PARAMETER(lParam);

	if (~dwAppFrag & APP_HASH_INPUT_EDIT)
	{
		dwAppFrag |= APP_HASH_INPUT_EDIT;

		SendMessage(tagMainWindow1.hEdit[1], WM_SETFONT, (WPARAM)tagMainWindow1.hFont[2], 0);
		SetWindowText(tagMainWindow1.hEdit[1], _T(""));
	}
	tagMainWindow1.hSendWindow = tagMainWindow1.hEdit[1];
	return TRUE;
}

LRESULT MainWindow_Edit2_OnKillFocus(HWND hWnd, WPARAM wParam, LPARAM lParam)
{
	UNREFERENCED_PARAMETER(hWnd);
	UNREFERENCED_PARAMETER(wParam);
	UNREFERENCED_PARAMETER(lParam);
	int nRet = 0;
	const tagHashThread_FileRecode* cptagHashFile_Recode = HashThread_GetFileRecode(&tagMainWindow1.tagHashThread1, tagMainWindow1.dwOpenItem);

	IF_UNLIKELY(cptagHashFile_Recode->dwFileAttributes == (DWORD)-1)
	{
		tagMainWindow1.hSendWindow = NULL;
		return TRUE;
	}

	nRet = GetWindowText(tagMainWindow1.hEdit[1], tagMainWindow1.pBuf, 255);
	if (nRet == 0)
	{
		dwAppFrag &= ~APP_HASH_INPUT_EDIT;

		SendMessage(tagMainWindow1.hEdit[1], WM_SETFONT, (WPARAM)tagMainWindow1.hFont[1], 0);
		SetWindowText(tagMainWindow1.hEdit[1], tagMainWindow1.pInputtext);
	}
	tagMainWindow1.hSendWindow = NULL;
	return TRUE;
}

LRESULT MainWindow_OnMessageEdit3(HWND hWnd, WPARAM wParam, LPARAM lParam)
{
	switch (HIWORD(wParam))
	{
	case EN_SETFOCUS:
		return MainWindow_Edit3_OnSetFocus(hWnd, wParam, lParam);
	case EN_KILLFOCUS:
		return MainWindow_Edit3_OnKillFocus(hWnd, wParam, lParam);
	}
	return FALSE;
}

LRESULT MainWindow_Edit3_OnSetFocus(HWND hWnd, WPARAM wParam, LPARAM lParam)
{
	UNREFERENCED_PARAMETER(wParam);
	UNREFERENCED_PARAMETER(lParam);

	if (~dwAppFrag & APP_HASHTHREAD && tagMainWindow1.tagHashThread1.tagMultiFile->dwFileCount > 0)
	{
		GetWindowText(tagMainWindow1.hEdit[2], tagMainWindow1.pFileNameEdit, _MAX_FNAME);
		EnableWindow(tagMainWindow1.hButton[8], TRUE);
		SendMessage(tagMainWindow1.hEdit[2], EM_SETREADONLY, FALSE, 0);
		SendMessage(hWnd, DM_SETDEFID, (WPARAM)IDC_BUTTON9, (LPARAM)0);
		tagMainWindow1.hSendWindow = tagMainWindow1.hEdit[2];
	}
	return TRUE;
}

LRESULT MainWindow_Edit3_OnKillFocus(HWND hWnd, WPARAM wParam, LPARAM lParam)
{
	UNREFERENCED_PARAMETER(wParam);
	UNREFERENCED_PARAMETER(lParam);

	if (~dwAppFrag & APP_HASHTHREAD && tagMainWindow1.tagHashThread1.tagMultiFile->dwFileCount > 0) {
		SendMessage(tagMainWindow1.hEdit[2], EM_SETREADONLY, TRUE, 0);
	}
	SendMessage(hWnd, DM_SETDEFID, (WPARAM)IDC_BUTTON1, (LPARAM)0);
	tagMainWindow1.hSendWindow = NULL;
	return TRUE;
}

LRESULT MainWindow_OnMessagePreviousFile(HWND hWnd, WPARAM wParam, LPARAM lParam)
{
	UNREFERENCED_PARAMETER(hWnd);
	UNREFERENCED_PARAMETER(wParam);
	UNREFERENCED_PARAMETER(lParam);

	if (tagMainWindow1.dwOpenItem > 0)
	{
		tagMainWindow1.dwOpenItem--;
		Hash_FileView(tagMainWindow1.dwOpenItem);
	}
	return TRUE;
}

LRESULT MainWindow_OnMessageNextFile(HWND hWnd, WPARAM wParam, LPARAM lParam)
{
	UNREFERENCED_PARAMETER(hWnd);
	UNREFERENCED_PARAMETER(wParam);
	UNREFERENCED_PARAMETER(lParam);

	if (tagMainWindow1.tagHashThread1.tagMultiFile->dwFileCount > 0 && tagMainWindow1.dwOpenItem < (tagMainWindow1.tagHashThread1.tagMultiFile->dwFileCount - 1))
	{
		tagMainWindow1.dwOpenItem++;
		Hash_FileView(tagMainWindow1.dwOpenItem);
	}
	return TRUE;
}

LRESULT MainWindow_OnMessageReOpen(HWND hWnd, WPARAM wParam, LPARAM lParam)
{
	UNREFERENCED_PARAMETER(hWnd);
	UNREFERENCED_PARAMETER(lParam);

	if (dwAppFrag & APP_HASHFILE_ADDMODE && LOWORD(wParam) == IDM_VK_F5) {
		return FALSE;
	}
	ReOpen();
	return TRUE;
}

LRESULT MainWindow_OnMessageFileAssoc(HWND hWnd, WPARAM wParam, LPARAM lParam)
{
	UNREFERENCED_PARAMETER(wParam);
	UNREFERENCED_PARAMETER(lParam);
	int nRet = 0;

	nRet = Hash_FileAssoc();
	if (nRet != 0)
	{
		UINT uType[] = { MB_ICONERROR, MB_ICONINFORMATION };

		LoadString(tagMainWindow1.hInst, IDC_FILEASSOC_MSG2 - nRet, tagMainWindow1.pStBuf, MAX_STRINGTABLE - 1);
		MainWindow_MessageBox(hWnd, tagMainWindow1.pStBuf, tagMainWindow1.pTitle, uType[nRet]);
	}
	return TRUE;
}

LRESULT MainWindow_OnRButtonDown(HWND hWnd, WPARAM wParam, LPARAM lParam)
{
	UNREFERENCED_PARAMETER(wParam);
	UNREFERENCED_PARAMETER(lParam);
	POINT pt = { 0 };

	GetCursorPos(&pt);
	DrawMenuBar(hWnd);
	TrackPopupMenu(tagMainWindow1.hPupMenu[0][0], TPM_LEFTALIGN | TPM_LEFTBUTTON, pt.x, pt.y, 0, hWnd, NULL);
	return TRUE;
}

LRESULT MainWindow_OnSysColorChange(HWND hWnd, WPARAM wParam, LPARAM lParam)
{
	UNREFERENCED_PARAMETER(hWnd);
	int i = 0;

	if (dwAppFrag & APP_ENABLE_MINIMAM_HANDLE) {
		return TRUE;
	}

	DeleteObject(tagMainWindow1.hBrush);
	tagMainWindow1.clGrayTextColor = GetSysColor(COLOR_GRAYTEXT);
	tagMainWindow1.hBrush = CreateSolidBrush(GetSysColor(COLOR_WINDOW));

	for (i = 0; i < SIZEOF_NUM(tagMainWindow1.hStatic); i++) {
		SendMessage(tagMainWindow1.hStatic[i], WM_SYSCOLORCHANGE, wParam, lParam);
	}

	SendMessage(tagMainWindow1.hBmp[0], WM_SYSCOLORCHANGE, wParam, lParam);
	SendMessage(tagMainWindow1.hProgress[0], WM_SYSCOLORCHANGE, wParam, lParam);

	for (i = 0; i < SIZEOF_NUM(tagMainWindow1.hEdit); i++) {
		SendMessage(tagMainWindow1.hEdit[i], WM_SYSCOLORCHANGE, wParam, lParam);
	}
	for (i = 0; i < SIZEOF_NUM(tagMainWindow1.hButton); i++) {
		SendMessage(tagMainWindow1.hButton[i], WM_SYSCOLORCHANGE, wParam, lParam);
	}
	return TRUE;
}

LRESULT MainWindow_OnCtlColorEdit(HWND hWnd, WPARAM wParam, LPARAM lParam)
{
	UNREFERENCED_PARAMETER(hWnd);

	if (dwAppFrag & APP_ENABLE_MINIMAM_HANDLE) {
		return FALSE;
	}

	if ((HWND)lParam == tagMainWindow1.hEdit[1] && ~dwAppFrag & APP_HASH_INPUT_EDIT)
	{
		HDC hDC = (HDC)wParam;

		SetBkMode(hDC, TRANSPARENT);
		SetTextColor(hDC, tagMainWindow1.clGrayTextColor);
		return (LRESULT)tagMainWindow1.hBrush;
	}
	return FALSE;
}

LRESULT MainWindow_OnDpiChanged(HWND hWnd, WPARAM wParam, LPARAM lParam)
{
	UNREFERENCED_PARAMETER(hWnd);
	UNREFERENCED_PARAMETER(lParam);

	Hash_GetDpi(HIWORD(wParam), LOWORD(wParam));
	return TRUE;
}

LRESULT MainWindow_OnDropFiles(HWND hWnd, WPARAM wParam, LPARAM lParam)
{
	UNREFERENCED_PARAMETER(lParam);
	DWORD dwRet = 0;
	DWORD dwCount = 0;
	DWORD i = 0;

	if ((dwAppFrag & APP_HASHFILE_ADDMODE) != 0 && tagFileListWindow1.hWnd == NULL) {
		FileListDlg();
	}

	if (tagFileListWindow1.hWnd) {
		return SendMessage(tagFileListWindow1.hWnd, WM_DROPFILES, wParam, lParam);
	}

	if (dwAppFrag & APP_WINDOWALPHA) {
		SetLayeredWindowAttributes_Func(hWnd, 0, ACTIVE_ALPHA, LWA_ALPHA);
	}

	MainWindow_AddFile_Start();
	dwCount = DragQueryFile((HDROP)wParam, (DWORD)-1, NULL, NULL);

	for (i = 0; i < dwCount; i++)
	{
		DragQueryFile((HDROP)wParam, i, tagMainWindow1.pFile, MAX_PATH_SIZE - 1);
		dwRet = MainWindow_AddFile(i);
		IF_UNLIKELY(dwRet == (DWORD)-1 || dwRet >= FILE_MAX_COUNTSIZE) {
			break;
		}
	}

	IF_UNLIKELY(dwRet == (DWORD)-1)
	{
		FileRecodeFail(hWnd);
		DragFinish((HDROP)wParam);
		DestroyWindow(hWnd);
		return FALSE;
	}
	MainWindow_AddFile_End();
	DragFinish((HDROP)wParam);
	return TRUE;
}

LRESULT MainWindow_OnGetMinMaxInfo(HWND hWnd, WPARAM wParam, LPARAM lParam)
{
	UNREFERENCED_PARAMETER(hWnd);
	UNREFERENCED_PARAMETER(wParam);
	const DWORD dwHashType = tagMainWindow1.tagHashThread1.dwHashType;
	const int nWindowWidth = (MDIVX(DIALOG1_X) + tagMainWindow1.dwWindowRectX - MDIVX(180)) + nHashX[dwHashType] + nEditMarginX[2];
	const int nWindowHeight = MDIVY(DIALOG1_Y) + tagMainWindow1.dwWindowRectY + nMLModeDiff[0];

	((MINMAXINFO*)lParam)->ptMinTrackSize.x = nWindowWidth;
	//((MINMAXINFO*)lParam)->ptMaxTrackSize.x = MDIVX(DIALOG1_Y) + tagMainWindow1.dwWindowRectX;
	((MINMAXINFO*)lParam)->ptMinTrackSize.y = nWindowHeight;
	((MINMAXINFO*)lParam)->ptMaxTrackSize.y = nWindowHeight;
	return TRUE;
}

LRESULT MainWindow_OnMove(HWND hWnd, WPARAM wParam, LPARAM lParam)
{
	UNREFERENCED_PARAMETER(hWnd);
	UNREFERENCED_PARAMETER(wParam);
	UNREFERENCED_PARAMETER(lParam);

	//tagMainWindow1.nLeftPos = LOWORD(lParam);
	//tagMainWindow1.nTopPos = HIWORD(lParam);
	if (tagMainWindow1.nIsWindowSet == FALSE)
	{
		RECT rc = { 0 };
		int nRet = 0;

		nRet = GetWindowRect(tagMainWindow1.hWnd, &rc);
		IF_UNLIKELY(nRet == FALSE) {
			return FALSE;
		}

		tagMainWindow1.nLeftPos = rc.left;
		tagMainWindow1.nTopPos = rc.top;
	}
	return TRUE;
}

LRESULT MainWindow_OnTimer(HWND hWnd, WPARAM wParam, LPARAM lParam)
{
	UNREFERENCED_PARAMETER(hWnd);
	UNREFERENCED_PARAMETER(lParam);

	if (wParam == TIMER && dwAppFrag & APP_HASHTHREAD) {
		Hash_TimeView(0);
	}
	return TRUE;
}

LRESULT MainWindow_OnActivate(HWND hWnd, WPARAM wParam, LPARAM lParam)
{
	UNREFERENCED_PARAMETER(lParam);

	if ((dwAppFrag & APP_WINDOWALPHA) != 0 && (~dwAppFrag & APP_WINDOW_NOALPHA) != 0) {
		SetLayeredWindowAttributes_Func(hWnd, 0, wParam ? ACTIVE_ALPHA : INACTIVE_ALPHA, LWA_ALPHA);
	}
	return FALSE;	// 「TRUE」を返すとエディットコントロールのフォーカスが失われます
}

LRESULT MainWindow_OnDrawClipboard(HWND hWnd, WPARAM wParam, LPARAM lParam)
{
	UNREFERENCED_PARAMETER(hWnd);

	if (~dwAppFrag & APP_HASHTHREAD) {
		EnableWindow(tagMainWindow1.hButton[3], ClipboardCheck(tagMainWindow1.dwOpenItem));
	}

	if (tagMainWindow1.hNextWnd) {
		SendMessage(tagMainWindow1.hNextWnd, WM_DRAWCLIPBOARD, wParam, lParam);
	}
	return TRUE;
}

LRESULT MainWindow_OnChangeCBChain(HWND hWnd, WPARAM wParam, LPARAM lParam)
{
	UNREFERENCED_PARAMETER(hWnd);

	if ((HWND)wParam == tagMainWindow1.hNextWnd)
	{
		tagMainWindow1.hNextWnd = (HWND)lParam;
		return FALSE;
	}
	else if (tagMainWindow1.hNextWnd) {
		return SendMessage(tagMainWindow1.hNextWnd, WM_CHANGECBCHAIN, wParam, lParam);
	}
	return TRUE;
}

LRESULT MainWindow_OnThemeChanged(HWND hWnd, WPARAM wParam, LPARAM lParam)
{
	UNREFERENCED_PARAMETER(hWnd);
	UNREFERENCED_PARAMETER(wParam);
	UNREFERENCED_PARAMETER(lParam);

	return TRUE;
}

LRESULT MainWindow_OnDestroy(HWND hWnd, WPARAM wParam, LPARAM lParam)
{
	UNREFERENCED_PARAMETER(lParam);
	int i = 0;

	dwAppFrag &= ~APP_WINDOW;

	DestroyWindow(tagFileListWindow1.hWnd);
	if (tagMainWindow1.hNextWnd) {
		ChangeClipboardChain(hWnd, tagMainWindow1.hNextWnd);
	}
	WindowSize_SaveState(hWnd, _T("Option"), tagMainWindow1.pINIFile);
	DestroyMenu(tagMainWindow1.hMenu);
	DeleteObject(tagMainWindow1.hBrush);
	EndDialog(hWnd, LOWORD(wParam));
	SetIniFileSetting(tagMainWindow1.pINIFile);
	iTaskbarList3_Release(tagMainWindow1.pTaskbarList3);
	for (i = 0; i < SIZEOF_NUM(tagMainWindow1.hIcon); i++)
	{
		DestroyIcon(tagMainWindow1.hIcon[i]);
		tagMainWindow1.hIcon[i] = NULL;
	}
	for (i = 0; i < SIZEOF_NUM(tagMainWindow1.hFont); i++)
	{
		DeleteObject(tagMainWindow1.hFont[i]);
		tagMainWindow1.hFont[i] = NULL;
	}
	for (i = 0; i < SIZEOF_NUM(tagMainWindow1.hStatusIcon); i++)
	{
		DeleteObject(tagMainWindow1.hStatusIcon[i]);
		tagMainWindow1.hStatusIcon[i] = NULL;
	}
	FreeLibrary(tagMainWindow1.hDllInst);
	tagMainWindow1.hDllInst = NULL;
	PostQuitMessage(0);
	return TRUE;
}

VOID Hash_GetDpi(DWORD dwDpiX, DWORD dwDpiY)
{
	HDC hDC = NULL;
	LOGFONT logfont = { 0 };
	RECT dlgRect = { 0 };
	double dlgBaseUnitX = 0.;
	double dlgBaseUnitY = 0.;
	double dScaleY = 0.;
	int nIconSizeX = 0;
	int nIconSizeY = 0;
	int nArg[4] = { 0 };
	int i = 0;
	int nRet = 0;
	size_t szRet = 0;
	const size_t nDpiOld[2] = { tagMainWindow1.nDpi[0], tagMainWindow1.nDpi[1] };

#ifdef _DEBUG
	TCHAR szDebugText[1024];
	OutputDebugString(_T("MainWindow: Hash_GetDpi()\r\n"));
#endif


	hDC = GetDC(tagFileListWindow1.hWnd);
	if (dwDpiX == 0)
	{
		if (hDC)
		{
			tagMainWindow1.nDpi[0] = GetDeviceCaps(hDC, LOGPIXELSX);
			tagMainWindow1.nDpi[1] = GetDeviceCaps(hDC, LOGPIXELSY);
		}
		else
		{
			tagMainWindow1.nDpi[0] = 96;
			tagMainWindow1.nDpi[1] = 96;
		}
	}
	else
	{
		tagMainWindow1.nDpi[0] = dwDpiX;
		tagMainWindow1.nDpi[1] = dwDpiY;
	}

	if (tagMainWindow1.nDpi[0] != nDpiOld[0] || tagMainWindow1.nDpi[1] != nDpiOld[1])
	{
		// dpi の取得
		//
		// 小 - 100% ...  96dpi
		// 中 - 125% ... 120dpi
		// 大 - 150% ... 144dpi

		tagMainWindow1.dDpiSize = tagMainWindow1.nDpi[0] / 96.;

		nIconSizeX = (int)(24. * tagMainWindow1.dDpiSize);
		nIconSizeY = (int)(24. * tagMainWindow1.dDpiSize);

#ifdef _DEBUG
		STPRINTF_FUNC(szDebugText, _T("dDpiSize = %.2f;\r\nnIconSizeX = %d, nIconSizeY = %d;\r\n"), tagMainWindow1.dDpiSize, nIconSizeX, nIconSizeY);
		OutputDebugString(szDebugText);
#endif

		if (~dwAppFrag & APP_ENABLE_MINIMAM_HANDLE)
		{
			for (i = 1; i < SIZEOF_NUM(tagMainWindow1.hStatusIcon); i++)
			{
				DeleteObject(tagMainWindow1.hStatusIcon[i]);
				tagMainWindow1.hStatusIcon[i] = (HICON)LoadImage(tagMainWindow1.hDllInst, MAKEINTRESOURCE(IDI_STATUS_ICON0 + (size_t)i), IMAGE_ICON, nIconSizeX, nIconSizeY, LR_SHARED);
			}
		}

		DeleteObject(tagMainWindow1.hFont[0]);
		SystemParametersInfo(SPI_GETICONTITLELOGFONT, sizeof(SPI_GETICONTITLELOGFONT), &logfont, 0);
		if (*logfont.lfFaceName == '\0') {
			memcpy(logfont.lfFaceName, _T("MS UI Gothic"), sizeof(_T("MS UI Gothic")));
		}
		tagMainWindow1.hFont[0] = CreateFont(-MulDiv(9, (int)tagMainWindow1.nDpi[0], 72), 0, 0, 0, FW_NORMAL, FALSE, FALSE, FALSE, SHIFTJIS_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS, DEFAULT_QUALITY, DEFAULT_PITCH | FF_DONTCARE, logfont.lfFaceName);

		for (i = 1; i < SIZEOF_NUM(tagMainWindow1.hFont) - 1; i++)
		{
			DeleteObject(tagMainWindow1.hFont[i]);
			LoadString(tagMainWindow1.hInst, (IDS_FONT1 - 1) + i, tagMainWindow1.pStBuf, MAX_STRINGTABLE - 1);
			tagMainWindow1.hFont[i] = CreateFont(-MulDiv(12, (int)tagMainWindow1.nDpi[0], 72), 0, 0, 0, FW_BOLD, FALSE, FALSE, FALSE, SHIFTJIS_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS, DEFAULT_QUALITY, DEFAULT_PITCH | FF_DONTCARE, tagMainWindow1.pStBuf);
		}
		tagMainWindow1.hFont[3] = CreateFont(-MulDiv(9, (int)tagMainWindow1.nDpi[0], 72), 0, 0, 0, FW_NORMAL, FALSE, FALSE, FALSE, SHIFTJIS_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS, DEFAULT_QUALITY, DEFAULT_PITCH | FF_DONTCARE, tagMainWindow1.pStBuf);

		SendMessage(tagMainWindow1.hGroup[0], WM_SETFONT, (WPARAM)tagMainWindow1.hFont[0], 0);
		SendMessage(tagMainWindow1.hEdit[0], WM_SETFONT, (WPARAM)tagMainWindow1.hFont[2], 0);
		nRet = dwAppFrag & APP_HASH_INPUT_EDIT ? 2 : 1;
		SendMessage(tagMainWindow1.hEdit[1], WM_SETFONT, (WPARAM)tagMainWindow1.hFont[nRet], 0);
		SendMessage(tagMainWindow1.hEdit[2], WM_SETFONT, (WPARAM)tagMainWindow1.hFont[0], 0);

		for (i = 0; i < SIZEOF_NUM(tagMainWindow1.hButton); i++) {
			SendMessage(tagMainWindow1.hButton[i], WM_SETFONT, (WPARAM)tagMainWindow1.hFont[0], 0);
		}

		SendMessage(tagMainWindow1.hStatic[0], WM_SETFONT, (WPARAM)tagMainWindow1.hFont[1], 0);
		for (i = 1; i < SIZEOF_NUM(tagMainWindow1.hStatic); i++) {
			SendMessage(tagMainWindow1.hStatic[i], WM_SETFONT, (WPARAM)tagMainWindow1.hFont[0], 0);
		}

		dlgRect.left = 0;
		dlgRect.top = 0;
		dlgRect.right = 1000;
		dlgRect.bottom = 1000;
		nRet = MapDialogRect(tagMainWindow1.hWnd, &dlgRect);
		IF_UNLIKELY(nRet == FALSE)
		{
			Hash_FileView(tagMainWindow1.dwOpenItem);
			ReleaseDC(tagFileListWindow1.hWnd, hDC);
			return;
		}

		dlgBaseUnitX = (double)dlgRect.right * 4.0 / 1000.0;
		dlgBaseUnitY = (double)dlgRect.bottom * 8.0 / 1000.0;
		dScaleY = dlgBaseUnitY / 12.0;

		tagMainWindow1.dwbaseX = (DWORD)(tagMainWindow1.nDpi[0] / (dScaleY * 96.) * dlgBaseUnitX + 0.5);
		tagMainWindow1.dwbaseY = (DWORD)(tagMainWindow1.nDpi[1] / (dScaleY * 96.) * dlgBaseUnitY + 0.5);
		if (tagMainWindow1.dwWindowRectX == 0)
		{
			nRet = GetWindowRect(tagMainWindow1.hWnd, &dlgRect);
			IF_LIKELY(nRet)
			{
				tagMainWindow1.dwWindowRectX = dlgRect.right - dlgRect.left - MDIVX(DIALOG1_X);
				tagMainWindow1.dwWindowRectY = dlgRect.bottom - dlgRect.top - MDIVY(DIALOG1_Y);
			}
		}

		GetStringWidth(hDC);
		nArg[0] = MDIVX(12);
		nArg[1] = MDIVY(98);
		nArg[2] = MDIVX(DIALOG1_X - 138);
		if (nWindowX) {
			nArg[2] = nWindowX - nArg[2];
		}
		nArg[3] = nStringY[0];
		MoveWindow(tagMainWindow1.hEdit[2], nArg[0], nArg[1], nArg[2], nArg[3], FALSE);
		SendMessage(tagMainWindow1.hEdit[2], EM_GETRECT, 0, (LPARAM)&dlgRect);

#ifdef _DEBUG
		STPRINTF_FUNC(szDebugText, _T("%s, dlgRect = { %d, %d, %d, %d };\r\n"), _T("hEdit[2]"), dlgRect.left, dlgRect.top, dlgRect.right, dlgRect.bottom);
		OutputDebugString(szDebugText);
#endif

		dlgRect.right -= dlgRect.left;
		dlgRect.bottom -= dlgRect.top;
		dlgRect.left = 0;
		dlgRect.top = 0;
		nEditMarginX[0] = nArg[2] - dlgRect.right;
		nEditMarginY[0] = nArg[3] - dlgRect.bottom;

#ifdef _DEBUG
		STPRINTF_FUNC(szDebugText, _T("%s, dlgRect = { %d, %d, %d, %d };\r\n"), _T("hEdit[2]"), dlgRect.left, dlgRect.top, dlgRect.right, dlgRect.bottom);
		OutputDebugString(szDebugText);
#endif


#ifdef _DEBUG
		szRet = SendMessage(tagMainWindow1.hEdit[0], EM_GETMARGINS, 0, 0);
		STPRINTF_FUNC(szDebugText, _T("%s, EM_GETMARGINS = 0x%08x;\r\n"), _T("hEdit[0]"), (int)szRet);
		OutputDebugString(szDebugText);

		szRet = SendMessage(tagMainWindow1.hEdit[1], EM_GETMARGINS, 0, 0);

		STPRINTF_FUNC(szDebugText, _T("%s, EM_GETMARGINS = 0x%08x;\r\n"), _T("hEdit[1]"), (int)szRet);
		OutputDebugString(szDebugText);

		//SendMessage(tagMainWindow1.hEdit[0], EM_SETMARGINS, EC_LEFTMARGIN | EC_RIGHTMARGIN, szRet);

		szRet = SendMessage(tagMainWindow1.hEdit[0], EM_GETMARGINS, 0, 0);
		STPRINTF_FUNC(szDebugText, _T("%s, EM_GETMARGINS = 0x%08x;\r\n"), _T("hEdit[0]"), (int)szRet);
		OutputDebugString(szDebugText);
#endif

		nArg[0] = MDIVX(6);
		nArg[1] = MDIVX(38);
		nArg[2] = nHashX[0];
		nArg[3] = nHashY[0];
		i = MoveWindow(tagMainWindow1.hEdit[0], nArg[0], nArg[1], nArg[2], nArg[3], TRUE);
		SendMessage(tagMainWindow1.hEdit[0], EM_GETRECT, 0, (LPARAM)&dlgRect);

#ifdef _DEBUG
		STPRINTF_FUNC(szDebugText, _T("%s, dlgRect = { %d, %d, %d, %d };\r\n"), _T("hEdit[0]"), dlgRect.left, dlgRect.top, dlgRect.right, dlgRect.bottom);
		OutputDebugString(szDebugText);
#endif

		dlgRect.right -= dlgRect.left;
		dlgRect.bottom -= dlgRect.top;
		dlgRect.left = 0;
		dlgRect.top = 0;
		nEditMarginX[2] = nArg[2] - dlgRect.right;
		nEditMarginY[2] = nArg[3] - dlgRect.bottom;

#ifdef _DEBUG
		STPRINTF_FUNC(szDebugText, _T("%s, dlgRect = { %d, %d, %d, %d };\r\n"), _T("hEdit[0]"), dlgRect.left, dlgRect.top, dlgRect.right, dlgRect.bottom);
		OutputDebugString(szDebugText);
#endif


		nArg[0] = MDIVX(6);
		nArg[1] = MDIVX(56);
		nArg[2] = nHashX[0];
		nArg[3] = nHashY[0];
		MoveWindow(tagMainWindow1.hEdit[1], nArg[0], nArg[1], nArg[2], nArg[3], TRUE);
		SendMessage(tagMainWindow1.hEdit[1], EM_GETRECT, 0, (LPARAM)&dlgRect);

#ifdef _DEBUG
		STPRINTF_FUNC(szDebugText, _T("%s, dlgRect = { %d, %d, %d, %d };\r\n"), _T("hEdit[1]"), dlgRect.left, dlgRect.top, dlgRect.right, dlgRect.bottom);
		OutputDebugString(szDebugText);
#endif

		dlgRect.right -= dlgRect.left;
		dlgRect.bottom -= dlgRect.top;
		dlgRect.left = 0;
		dlgRect.top = 0;
		nEditMarginX[3] = nArg[2] - dlgRect.right;
		nEditMarginY[3] = nArg[3] - dlgRect.bottom;

#ifdef _DEBUG
		STPRINTF_FUNC(szDebugText, _T("%s, dlgRect = { %d, %d, %d, %d };\r\n"), _T("hEdit[1]"), dlgRect.left, dlgRect.top, dlgRect.right, dlgRect.bottom);
		OutputDebugString(szDebugText);
#endif

		//nMLModeDiff[1] = GetSystemMetrics(SM_CXHSCROLL);
		//nMLModeDiff[2] = GetSystemMetrics(SM_CXVSCROLL);

#ifdef _DEBUG
		STPRINTF_FUNC(szDebugText, _T("nEditMarginX[] = { %d, %d, %d, %d };\r\n"), nEditMarginX[0], nEditMarginX[1], nEditMarginX[2], nEditMarginX[3]);
		OutputDebugString(szDebugText);
		STPRINTF_FUNC(szDebugText, _T("nEditMarginY[] = { %d, %d, %d, %d };\r\n"), nEditMarginY[0], nEditMarginY[1], nEditMarginY[2], nEditMarginY[3]);
		OutputDebugString(szDebugText);

		STPRINTF_FUNC(szDebugText, _T("nMLModeDiff[] = { %d, %d, %d, %d };\r\n"), nMLModeDiff[0], nMLModeDiff[1], nMLModeDiff[2], nMLModeDiff[3]);
		OutputDebugString(szDebugText);
#endif

		SendMessage(tagMainWindow1.hEdit[0], EM_SETMARGINS, EC_LEFTMARGIN | EC_RIGHTMARGIN, 0);
		SendMessage(tagMainWindow1.hEdit[1], EM_SETMARGINS, EC_LEFTMARGIN | EC_RIGHTMARGIN, 0);
		SendMessage(tagMainWindow1.hEdit[2], EM_SETMARGINS, EC_LEFTMARGIN | EC_RIGHTMARGIN, 0);

		Hash_SetWindowMove(FALSE);
	}

	SendMessage(tagFileListWindow1.hWnd, tagFileListWindow1.nWindowMessage, APP_MASSAGE_DPICHANGE, (WPARAM)dwDpiX);
	SendMessage(tagAboutWindow1.hWnd, tagAboutWindow1.nWindowMessage, APP_MASSAGE_DPICHANGE, (WPARAM)dwDpiX);
	SendMessage(tagOptionWindow1.hWnd, tagOptionWindow1.nWindowMessage, APP_MASSAGE_DPICHANGE, (WPARAM)dwDpiX);
	SendMessage(tagDebugWindow1.hWnd, tagDebugWindow1.nWindowMessage, APP_MASSAGE_DPICHANGE, (WPARAM)dwDpiX);

	Hash_FileView(tagMainWindow1.dwOpenItem);
	ReleaseDC(tagFileListWindow1.hWnd, hDC);
	return;
}

BOOL Hash_SetWindowMove(BOOL nIsLeftPos)
{
	RECT rc = { 0 };
	MONITORINFO mInfo = { 0 };
	HMONITOR hMonitor1 = NULL;
	int nArg[4] = { 0 };
	int nRet = 0;
	const DWORD dwHashType = tagMainWindow1.tagHashThread1.dwHashType;
	int nWindowWidth = (MDIVX(DIALOG1_X) + tagMainWindow1.dwWindowRectX - MDIVX(180)) + nEditMarginX[3];
	int nWindowHeight = MDIVY(DIALOG1_Y) + tagMainWindow1.dwWindowRectY;

#ifdef _DEBUG
	TCHAR szDebugText[1024];
	OutputDebugString(_T("MainWindow: Hash_SetWindowMove()\r\n"));
	STPRINTF_FUNC(szDebugText, _T("dwHashType = %d;\r\n"), dwHashType);
	OutputDebugString(szDebugText);
#endif

	nRet = GetWindowRect(tagMainWindow1.hWnd, &rc);
	IF_UNLIKELY(nRet == FALSE) {
		return FALSE;
	}
	else
	{
#if _MSC_VER < 1400
		HMODULE hDll = NULL;

		hDll = LOADSYSLIB(_T("USER32.DLL"));
		if (hDll != NULL)
		{
			BOOL (WINAPI * GetMonitorInfo)(HMONITOR hMonitor, LPMONITORINFO lpmi);
			HMONITOR (WINAPI * MonitorFromRect)(LPCRECT lprc, DWORD dwFlags);
#ifdef _UNICODE
			const char* szFunc1 = "GetMonitorInfoW";
#else
			const char* szFunc1 = "GetMonitorInfoA";
#endif
			const char* szFunc2 = "MonitorFromRect";

			GetMonitorInfo = (BOOL(WINAPI*)(HMONITOR, LPMONITORINFO))(GetProcAddress(hDll, szFunc1));
			MonitorFromRect = (HMONITOR(WINAPI*)(LPCRECT, DWORD))(GetProcAddress(hDll, szFunc2));

			if (GetMonitorInfo != NULL && MonitorFromRect != NULL)
			{
#endif

				hMonitor1 = MonitorFromRect(&rc, MONITOR_DEFAULTTONEAREST);
				IF_UNLIKELY(hMonitor1 == NULL) {
					return FALSE;
				}

				mInfo.cbSize = sizeof(MONITORINFO);
				nRet = GetMonitorInfo(hMonitor1, &mInfo);
				IF_UNLIKELY(nRet == FALSE) {
					return FALSE;
				}

#if _MSC_VER < 1400
			}
			else
			{
				mInfo.rcWork.right = GetSystemMetrics(SM_CXSCREEN);
				mInfo.rcWork.bottom = GetSystemMetrics(SM_CYSCREEN);
			}
		}
#endif

	}

	if (nIsLeftPos)
	{
		tagMainWindow1.nLeftPos = rc.left;
		tagMainWindow1.nTopPos = rc.top;
	}

	if (tagMainWindow1.nLeftPos)
	{
		rc.right = rc.right - rc.left - tagMainWindow1.nLeftPos;
		rc.left = tagMainWindow1.nLeftPos;
	}

	if (tagMainWindow1.nTopPos)
	{
		rc.bottom = rc.bottom - rc.top - tagMainWindow1.nTopPos;
		rc.top = tagMainWindow1.nTopPos;
	}

	tagMainWindow1.nIsWindowSet = TRUE;
	if (1)
	{
		unsigned int nDiffX = 0;
		int x = nEditMarginX[3];
		int y = nEditMarginY[3];

		nMLModeDiff[0] = 0;
		if (dwAppFrag & APP_ENABLE_MULTILINE)
		{
			nWindowWidth += nHashX[0];
			nWindowHeight += nHashY_YMode[dwHashType] - nHashY[0];
			nWindowHeight += nHashY_Rate[dwHashType];
			x += nHashX[0];
			y += nHashY_YMode[dwHashType];
			y += nHashY_Rate[dwHashType];
			nMLModeDiff[0] += nHashY_YMode[dwHashType] - nHashY[0];
			nMLModeDiff[0] += nHashY_Rate[dwHashType];
		}
		else
		{
			nWindowWidth += nHashX[dwHashType];
			x += nHashX[dwHashType];
			y += nHashY[dwHashType];
		}

#ifdef _DEBUG
		STPRINTF_FUNC(szDebugText, _T("nMLModeDiff[0] = %d, x = %d, y = %d;\r\n"), nMLModeDiff[0], x, y);
		OutputDebugString(szDebugText);
#endif

		if ((rc.left + nWindowWidth) > mInfo.rcWork.right)
		{
			if (nWindowWidth >= mInfo.rcWork.right)
			{
				nArg[0] = 0;
				nArg[2] = mInfo.rcWork.right;
				nDiffX = nWindowWidth - mInfo.rcWork.right;
			}
			else
			{
				nArg[0] = rc.left;
				nArg[2] = nWindowWidth;
				nDiffX = (rc.left + nWindowWidth) - mInfo.rcWork.right;
			}
		}
		else
		{
			nArg[0] = rc.left;
			nArg[2] = nWindowWidth;
		}
		nArg[1] = rc.top;
		nArg[3] = nWindowHeight;

		if ((nArg[1] + nArg[3]) > mInfo.rcWork.bottom) {
			nArg[1] = mInfo.rcWork.bottom - nArg[3];
		}

#ifdef _DEBUG
		STPRINTF_FUNC(szDebugText, _T("%s = { %d, %d, %d, %d };\r\n"), _T("nArg[]"), nArg[0], nArg[1], nArg[2], nArg[3]);
		OutputDebugString(szDebugText);
#endif

		MoveWindow(tagMainWindow1.hWnd, nArg[0], nArg[1], nArg[2] + 1, nArg[3] + 1, TRUE);
		MoveWindow(tagMainWindow1.hWnd, nArg[0], nArg[1], nArg[2], nArg[3], TRUE);

		//IDC_EDIT1,6,38,180,15
		nArg[0] = MDIVX(6);
		nArg[1] = MDIVY(38);
		nArg[2] = x - nDiffX;
		nArg[3] = y;
		MoveWindow(tagMainWindow1.hEdit[0], nArg[0], nArg[1], nArg[2], nArg[3], FALSE);

		//IDC_EDIT2,6,56,180,15
		nArg[1] = MDIVY(56) + nMLModeDiff[0];
		nArg[3] = nHashY[dwHashType] + nEditMarginY[3];
		MoveWindow(tagMainWindow1.hEdit[1], nArg[0], nArg[1], nArg[2], nArg[3], FALSE);

		nArg[0] = MDIVX(12);
		nArg[1] = MDIVY(98) + nMLModeDiff[0];
		nArg[2] = nWindowX - MDIVX(DIALOG1_X - 138);
		nArg[3] = nStringY[0] + nEditMarginY[0];
		MoveWindow(tagMainWindow1.hEdit[2], nArg[0], nArg[1], nArg[2], nArg[3], FALSE);
	}
	InvalidateRect(tagMainWindow1.hWnd, NULL, TRUE);
	tagMainWindow1.nIsWindowSet = FALSE;
	return TRUE;
}

BOOL Hash_ChangeMsg(DWORD dwHashType)
{
	const tagHashThread_FileRecode* cptagHashFile_Recode = HashThread_GetFileRecode_Core(&tagMainWindow1.tagHashThread1, tagMainWindow1.dwOpenItem);

#ifdef _DEBUG
	TCHAR szDebugText[1024];

	STPRINTF_FUNC(szDebugText, _T("MainWindow: Hash_ChangeMsg(): %d\r\n"), dwHashType);
	OutputDebugString(szDebugText);
#endif

	if (HashThread_GetFileRecode_Core(&tagMainWindow1.tagHashThread1, 0)->dwHashFileHashLine != (DWORD)-1 && tagMainWindow1.tagHashThread1.dwHashType != dwHashType && cptagHashFile_Recode->dwHashFileHashType != dwHashType)
	{
		LoadString(tagMainWindow1.hInst, IDS_HASHCHANGE2, tagMainWindow1.pStBuf, MAX_STRINGTABLE - 1);
		MessageFormat(tagMainWindow1.pBuf, 2048, tagMainWindow1.pStBuf, tagMainWindow1.cpHashName);
		const int nRet = MainWindow_MessageBox(tagMainWindow1.hWnd, tagMainWindow1.pBuf, tagMainWindow1.pTitle, MB_YESNOCANCEL | MB_ICONQUESTION);
		if (nRet != IDYES) {
			return FALSE;
		}
	}
	Hash_HashChange(dwHashType);
	return TRUE;
}

VOID Hash_HashChange(DWORD dwHashType)
{
	tagMainWindow1.dwHashTypeBack = tagMainWindow1.tagHashThread1.dwHashType;
	HashThread_SetHashType(&tagMainWindow1.tagHashThread1, dwHashType);
	return;
}

BOOL Hash_SetHashType(BOOL nIsLeftPos)
{
	const DWORD dwHashType = tagMainWindow1.tagHashThread1.dwHashType;
	DWORD dwRet = dwHashType;
	const tagHashThread_FileRecode* cptagHashFile_Recode = HashThread_GetFileRecode_Core(&tagMainWindow1.tagHashThread1, tagMainWindow1.dwOpenItem);
	const TCHAR* cpHashName = tagMainWindow1.tagHashThread1.cpHashName;
	const TCHAR* cpHashExt;
	const TCHAR* cpFileFilter[MAX_HASH_TYPE + 1] = {
		_T("MD5ファイル (md5sum 形式)\0*.md5;*.txt\0MD5ファイル (MD5チェックツール形式)\0*.md5;*.txt\0\0"),
		_T("SHA-1ファイル (sha1sum 形式)\0*.sha1;*.txt\0SHA-1ファイル (MD5チェックツール形式)\0*.sha1;*.txt\0\0"),
		_T("SHA-256/192ファイル (sha256-192sum 形式)\0*.sha256_192;*.txt\0SHA-256/192ファイル (MD5チェックツール形式)\0*.sha256_192;*.txt\0\0"),
		_T("SHA-224ファイル (sha224sum 形式)\0*.sha224;*.txt\0SHA-256ファイル (MD5チェックツール形式)\0*.sha224;*.txt\0\0"),
		_T("SHA-256ファイル (sha256sum 形式)\0*.sha256;*.txt\0SHA-256ファイル (MD5チェックツール形式)\0*.sha256;*.txt\0\0"),
		_T("SHA-384ファイル (sha384sum 形式)\0*.sha384;*.txt\0SHA-384ファイル (MD5チェックツール形式)\0*.sha384;*.txt\0\0"),
		_T("SHA-512ファイル (sha512sum 形式)\0*.sha512;*.txt\0SHA-512ファイル (MD5チェックツール形式)\0*.sha512;*.txt\0\0"),
		_T("SHA-512/224ファイル (sha512-224sum 形式)\0*.sha512_224;*.txt\0SHA-512/224ファイル (MD5チェックツール形式)\0*.sha512_224;*.txt\0\0"),
		_T("SHA-512/256ファイル (sha512-256sum 形式)\0*.sha512_256;*.txt\0SHA-512/256ファイル (MD5チェックツール形式)\0*.sha512_256;*.txt\0\0"),
		_T("SHA3-224ファイル (sha3-224sum 形式)\0*.sha3_224;*.txt\0SHA3-224ファイル (MD5チェックツール形式)\0*.sha3_224;*.txt\0\0"),
		_T("SHA3-256ファイル (sha3-256sum 形式)\0*.sha3_256;*.txt\0SHA3-256ファイル (MD5チェックツール形式)\0*.sha3_256;*.txt\0\0"),
		_T("SHA3-384ファイル (sha3-384sum 形式)\0*.sha3_384;*.txt\0SHA3-384ファイル (MD5チェックツール形式)\0*.sha3_384;*.txt\0\0"),
		_T("SHA3-512ファイル (sha3-512sum 形式)\0*.sha3_512;*.txt\0SHA3-512ファイル (MD5チェックツール形式)\0*.sha3_512;*.txt\0\0"),
		_T("crc32ファイル (crc32sum 形式)\0*.crc32;*.txt\0crc32ファイル (MD5チェックツール形式)\0*.crc32;*.txt\0\0"),
		_T("crc32cファイル (crc32csum 形式)\0*.crc32c;*.txt\0crc32cファイル (MD5チェックツール形式)\0*.crc32c;*.txt\0\0"),
		_T("XXH32ファイル (xxh32sum 形式)\0*.xxh32;*.txt\0XXH32ファイル (MD5チェックツール形式)\0*.xxh32;*.txt\0\0"),
		_T("XXH64ファイル (xxh64sum 形式)\0*.xxh64;*.txt\0XXH64ファイル (MD5チェックツール形式)\0*.xxh64;*.txt\0\0"),
		_T("XXH3ファイル (xxh3sum 形式)\0*.xxh3;*.txt\0XXH3ファイル (MD5チェックツール形式)\0*.xxh3;*.txt\0\0"),
		_T("XXH128ファイル (xxh128sum 形式)\0*.xxh128;*.txt\0XXH128ファイル (MD5チェックツール形式)\0*.xxh128;*.txt\0\0"),
		_T("MD2ファイル (md2sum 形式)\0*.md2;*.txt\0MD2ファイル (MD5チェックツール形式)\0*.md2;*.txt\0\0"),
		_T("MD4ファイル (md4sum 形式)\0*.md4;*.txt\0MD4ファイル (MD5チェックツール形式)\0*.md4;*.txt\0\0")
		_T("Noneファイル (nonesum 形式)\0*.none;*.txt\0Noneファイル (MD5チェックツール形式)\0*.none;*.txt\0\0")
	};

	tagMainWindow1.ofnFileName[1].lpstrFilter = cpFileFilter[dwHashType];
	cpHashExt = HashThread_GetHashExt(dwHashType);
	tagMainWindow1.ofnFileName[1].lpstrDefExt = cpHashExt;

	if (*tagMainWindow1.pHashFile != '\0')
	{
		TCHAR* pExt = GetTCharToExtension2(tagMainWindow1.pHashFile, TRUE);

		if (pExt) {
			qtcscpy(pExt, cpHashExt);
		}
	}

	tagMainWindow1.cpHashName = cpHashName;
	SendMessage(tagMainWindow1.hEdit[1], EM_LIMITTEXT, (size_t)tagMainWindow1.tagHashThread1.dwHashLen * 2, 0);
	LoadString(tagMainWindow1.hInst, IDS_INPUT_TEXT, tagMainWindow1.pStBuf, NUMBER_LOADSTRING);
	MessageFormat(tagMainWindow1.pInputtext, NUMBER_LOADSTRING, tagMainWindow1.pStBuf, cpHashName);
	LoadString(tagMainWindow1.hInst, IDS_TOOLTIP_MSG2, tagMainWindow1.pStBuf, NUMBER_LOADSTRING);
	MessageFormat(tagMainWindow1.pToolTipText[1], NUMBER_LOADSTRING, tagMainWindow1.pStBuf, cpHashName);
	LoadString(tagMainWindow1.hInst, IDS_TOOLTIP_MSG9, tagMainWindow1.pStBuf, NUMBER_LOADSTRING);
	MessageFormat(tagMainWindow1.pToolTipText[6], NUMBER_LOADSTRING, tagMainWindow1.pStBuf, cpHashName);
	LoadString(tagMainWindow1.hInst, IDS_TOOLTIP_MSG11, tagMainWindow1.pStBuf, NUMBER_LOADSTRING);
	MessageFormat(tagMainWindow1.pToolTipText[7], NUMBER_LOADSTRING, tagMainWindow1.pStBuf, cpHashName);
	LoadString(tagMainWindow1.hInst, IDS_TOOLTIP_MSG12, tagMainWindow1.pStBuf, NUMBER_LOADSTRING);
	MessageFormat(tagMainWindow1.pToolTipText[8], NUMBER_LOADSTRING, tagMainWindow1.pStBuf, cpHashName);
	qtcscpy(tagMainWindow1.pFileExtension, tagMainWindow1.tagHashThread1.cpHashExt);
	CheckMenuRadioItem(tagMainWindow1.hPupMenu[0][2], ID_MAINSUBMENU1_HASH1, ID_MAINSUBMENU1_HASH1 + MAX_HASH_TYPE, (UINT)(ID_MAINSUBMENU1_HASH1 + dwRet), MF_BYCOMMAND);

	if (cptagHashFile_Recode->dwFileHashType == dwHashType)
	{
		if (dwAppFrag & APP_HASH_INPUT_EDIT)
		{
			SetWindowText(tagMainWindow1.hEdit[1], tagMainWindow1.pHashCompString);
			dwRet = 2;
		}
		else
		{
			SetWindowText(tagMainWindow1.hEdit[1], tagMainWindow1.pInputtext);
			dwRet = 1;
		}
		SendMessage(tagMainWindow1.hEdit[1], WM_SETFONT, (WPARAM)tagMainWindow1.hFont[dwRet], 0);

		dwRet = FALSE;
		switch (cptagHashFile_Recode->dwFileLastError)
		{
		case 0:
			dwRet = TRUE;
			break;
		case (DWORD)-1:
			if (dwAppFrag & APP_HASHTHREAD) {
				dwRet = TRUE;
			}
			break;
		}
	}
	else
	{
		TCHAR* ptr = NULL;

		if (tagMainWindow1.tagHashThread1.tagMultiFile->dwFileCount > 0) {
			ptr = tagMainWindow1.pBase64_Msg[0];
		}
		SetWindowText(tagMainWindow1.hEdit[1], _T(""));
		SetWindowText(tagMainWindow1.hStatic[7], ptr);
		dwRet = FALSE;
	}

	Hash_SetWindowMove(nIsLeftPos);
	Hash_FileView(tagMainWindow1.dwOpenItem);
	EnableWindow(tagMainWindow1.hButton[1], dwRet != 0);
	SetFocus(tagMainWindow1.hButton[0]);
	return TRUE;
}

BOOL Hash_FileAssoc()
{
	int nRet = FALSE;
	BOOL (WINAPI * IconRes_FileAssoc)(const TCHAR*) = NULL;

#ifdef _UNICODE
	const char* cpFunc1 = "IconRes_FileAssocW";
#else
	const char* cpFunc1 = "IconRes_FileAssocA";
#endif

/*
	if (tagMainWindow1.hDllInst == NULL) {
		return FALSE;
	}
*/

	IconRes_FileAssoc = (BOOL(WINAPI*)(const TCHAR*))(GetProcAddress(tagMainWindow1.hDllInst, cpFunc1));
	IF_LIKELY(IconRes_FileAssoc != NULL)
	{
		GetModuleFileName(NULL, tagMainWindow1.pTempBuf, MAX_PATH_SIZE - 1);
		nRet = IconRes_FileAssoc(tagMainWindow1.pTempBuf);
	}
	return nRet;
}

int MainWindow_MessageBox(HWND hWnd, LPCTSTR lpText, LPCTSTR lpCaption, UINT uType)
{
	int nRet = 0;

	// ウィンドウロックします。
	WINDOW_LOCK;

	nRet = MessageBox(hWnd, lpText, lpCaption, uType);

	// ウィンドウロックの解除します。
	WINDOW_UNLOCK;

	return nRet;
}

VOID Clear(BOOL nIsSendMessage)
{
	dwAppFrag |= APP_FILE_CLEAR;

	if (tagMainWindow1.dwAddFileModeFileCount != 0)
	{
		LoadString(tagMainWindow1.hInst, IDS_FILELIST_CLEAR1, tagMainWindow1.pStBuf, MAX_STRINGTABLE);
		const int nRet = MainWindow_MessageBox(tagMainWindow1.hWnd, tagMainWindow1.pStBuf, tagMainWindow1.pTitle, MB_YESNO | MB_ICONQUESTION);
		if (nRet != IDYES)
		{
			dwAppFrag &= ~APP_FILE_CLEAR;
			return;
		}

		dwAppFrag &= ~APP_HASHFILE_ADDMODE;
		tagMainWindow1.dwAddFileModeFileCount = 0;
		SendMessage(tagMainWindow1.hProgress[0], PBM_SETPOS, 0, 0);
		EnableWindow(tagMainWindow1.hButton[0], TRUE);
		DragAcceptFiles(tagMainWindow1.hWnd, TRUE);
	}

	*tagMainWindow1.pFile = '\0';
	*tagMainWindow1.pOfn1Buf = '\0';
	*tagMainWindow1.pHashFile = '\0';
	*tagMainWindow1.pHashString = '\0';
	*tagMainWindow1.pHashCompString = '\0';
	tagMainWindow1.dwOpenItem = 0;
	//tagMainWindow1.nIsFileNameEdit = FALSE;
	tagMainWindow1.nCmpCount = 0;
	tagMainWindow1.nCmpErrorCount = 0;
	tagMainWindow1.tagHashThread1.tagMultiFile->dwFileCount = 0;
	tagMainWindow1.tagHashThread1.tagMultiFile->dwFileCurrentCount = 0;
	*tagMainWindow1.tagHashThread1.tagMultiFile->szCurrentFolder = '\0';
	tagMainWindow1.bIsNotFoundFile = 0;
	*tagMainWindow1.pNotFoundFile = '\0';
	tagMainWindow1.pNextNotFoundFile = tagMainWindow1.pNotFoundFile;
	tagMainWindow1.bIsEmptyFolder = 0;
	*tagMainWindow1.pEmptyFolder = '\0';
	tagMainWindow1.pNextEmptyFolder = tagMainWindow1.pEmptyFolder;
	tagMainWindow1.bIsHashNotFoundFile = 0;
	*tagMainWindow1.pHashNotFoundFile = '\0';
	tagMainWindow1.pNextHashNotFoundFile = tagMainWindow1.pHashNotFoundFile;
	HashThread_ClearFileRecode(&tagMainWindow1.tagHashThread1);
	SendMessage(tagMainWindow1.hProgress[0], PBM_SETPOS, 0, 0);
	SetWindowText(tagMainWindow1.hEdit[0], _T(""));
	SendMessage(tagMainWindow1.hEdit[2], EM_SETREADONLY, TRUE, 0);
	EnableWindow(tagMainWindow1.hButton[6], FALSE);
	EnableWindow(tagMainWindow1.hButton[7], FALSE);
	EnableWindow(tagMainWindow1.hButton[8], FALSE);
	Hash_FileView(tagMainWindow1.dwOpenItem);
	EnableMenuItem(tagMainWindow1.hPupMenu[0][0], ID_MAINSUBMENU1_CLEAR, MF_GRAYED);
	EnableMenuItem(tagMainWindow1.hPupMenu[0][0], 17, MF_BYPOSITION | MF_ENABLED);
	if (nIsSendMessage && tagFileListWindow1.hWnd) {
		SendMessage(tagFileListWindow1.hWnd, tagFileListWindow1.nWindowMessage, APP_MESSAGE_FILECLEAR, TRUE);
	}
	iTaskbarList3_SetProgressState(tagMainWindow1.pTaskbarList3, tagMainWindow1.hWnd, TBPF_NOPROGRESS);
	iTaskbarList3_DeleteOverlayIcon(tagMainWindow1.pTaskbarList3, tagMainWindow1.hWnd);
	dwAppFrag &= ~APP_FILE_CLEAR;
	return;
}

unsigned __stdcall Bench(void* lpThread)
{
	MainWindow* pMainWindowThread = (MainWindow*)lpThread;
	WCHAR szHashBuf[HASH_LOADSTRING] = L"";
	WCHAR szHashComp[HASH_LOADSTRING] = L"";
	DWORD dwHashByte[HASH_LEN * 2 / sizeof(DWORD)] = { 0 };
	__int64 dwTimeStart = 0;
	DWORD dwRet = 0;
	DWORD dwCount = 0;
	DWORD dwFileSize = 0;
	double dScoreSum = 0.;
	double dScoreK[] = {
		7145.89665653496,
		2618.181818181818,
		1890.57750759878,
		2836.879432624107,
		7164.13373860182,
		7142.85714285714,
		2227.96352583587,
		3242.147922998979,
		920.9726443769
	};
	double dScore[SIZEOF_NUM(dScoreK)] = { 0. };
	unsigned __int64 dwTime[SIZEOF_NUM(dScoreK)] = { 0 };
	const TCHAR* cpFuncName[] = {
		_T("MD5ToString_Old"),
		_T("MD5ToString_no_sprintf"),
		_T("MD5ToString_Default"),
		_T("BinaryToChar"),
		_T("MD5ToHash_Old"),
		_T("MD5ToHash_no_scanf"),
		_T("MD5ToHash_Default"),
		_T("MD5ToHash_no_strtoul"),
		_T("HashThread_MultiFile_Thread"),
		NULL};

	const int nResultTab[] = {
		2,
		1,
		1,
		2,
		2,
		1,
		1,
		1,
		1,
		0};

	const TCHAR* cpResultTab[] = {
		_T(""),
		_T("\t"),
		_T("\t\t"),
		_T("\t\t\t"),
		_T("\t\t\t\t"),
		NULL};

	static __int64 llClock = (__int64)-1;
	static TCHAR szClock[32];
	static TCHAR szBenchStep[SIZEOF_NUM(dScoreK)][100];
	HANDLE hProcess = NULL;
	HANDLE hFile = NULL;
	DWORD dwWrite = 0;
	TCHAR szSwapTimeText[MAX_LOADSTRING];
	TCHAR* pTempFile = NULL;
	TCHAR* ptr = NULL;
	TCHAR szNum[33] = _T("");
	BYTE* pbyFileBuf = NULL;
	tagHashThread_MultiFile *ptagHashThread_MultiFile_Swap1 = NULL;
	tagHashThread_FileRecode *ptagHashThread_FileRecode_Swap1 = NULL;
	int i = 0;

#ifdef _DEBUG
	TCHAR szDebugText[1024];
#endif

	pTempFile = (TCHAR*)malloc((MAX_PATH_SIZE * sizeof(TCHAR))); 
	ptagHashThread_MultiFile_Swap1 = (tagHashThread_MultiFile*)malloc(sizeof(tagHashThread_MultiFile));
	ptagHashThread_FileRecode_Swap1 = (tagHashThread_FileRecode*)malloc(sizeof(tagHashThread_FileRecode));
	pbyFileBuf = (BYTE*)malloc(BENCH_FILESIZE);

	IF_UNLIKELY(pTempFile == NULL || ptagHashThread_MultiFile_Swap1 == NULL || ptagHashThread_FileRecode_Swap1 == NULL || pbyFileBuf == NULL)
	{
		free(pTempFile);
		free(ptagHashThread_MultiFile_Swap1);
		free(ptagHashThread_FileRecode_Swap1);
		free(pbyFileBuf);

		if (pMainWindowThread->hBenchHandle)
		{
			pMainWindowThread->hBenchHandle = NULL;
			_endthreadex(FALSE);
		}
		return FALSE;
	}

	*pTempFile = '\0';
	hProcess = GetCurrentProcess();
	SetPriorityClass(hProcess, HIGH_PRIORITY_CLASS);
	GetTempPath(MAX_PATH_SIZE - 1, pMainWindowThread->pBuf);
	GetTempFileName(pMainWindowThread->pBuf, _T("Hash"), NULL, pTempFile);
	memcpy(ptagHashThread_MultiFile_Swap1, pMainWindowThread->tagHashThread1.tagMultiFile, sizeof(tagHashThread_MultiFile));
	memcpy(ptagHashThread_FileRecode_Swap1, pMainWindowThread->tagHashThread1.tagMultiFile->ptagFileRecode, sizeof(tagHashThread_FileRecode));
	ZeroMemory(pbyFileBuf, BENCH_FILESIZE);
	GetWindowText(tagMainWindow1.hStatic[1], szSwapTimeText, MAX_LOADSTRING - 1);

	WINDOW_LOCK;
	EnableWindow(pMainWindowThread->hWnd, FALSE);

	if (llClock == -1)
	{
		for (i = 0; i < SIZEOF_NUM(dScoreK); i++)
		{
			ptr = szBenchStep[i];
			*ptr++ = 'S';
			*ptr++ = 't';
			*ptr++ = 'e';
			*ptr++ = 'p';
			_itot_s(i + 1, szNum, 32, 10);
			ptr = qtcscpy(ptr, szNum);
			*ptr++ = ':';
			*ptr++ = ' ';
			ptr = qtcscpy(ptr, cpFuncName[i]);
			*ptr++ = '(';
			*ptr++ = ')';
			*ptr++ = ' ';
			ptr = qtcscpy(ptr, _T("を実行しています..."));
		}

		LoadString(pMainWindowThread->hInst, IDC_BENCH_CLOCK1, pMainWindowThread->pStBuf, MAX_STRINGTABLE - 1);
		SetWindowText(pMainWindowThread->hStatic[1], pMainWindowThread->pStBuf);

#if _MSC_VER > 1600
		dwTime[0] = GetTickCount64() + 1000;
		while (dwTime[0] > GetTickCount64()) {
			;
		}
		llClock = __rdtsc();

		dwTime[0] = GetTickCount64() + 1000;
		while (dwTime[0] > GetTickCount64()) {
			;
		}
		llClock = __rdtsc() - llClock;
#else
		dwTime[0] = GetTickCount() + 1000;
		while (dwTime[0] > GetTickCount()) {
			;
		}
		llClock = __rdtsc();

		dwTime[0] = GetTickCount() + 1000;
		while (dwTime[0] > GetTickCount()) {
			;
		}
		llClock = __rdtsc() - llClock;
#endif

		STPRINTF_FUNC(szClock, _T("%.2f"), (double)llClock / 1000000000.);

		LoadString(pMainWindowThread->hInst, IDC_BENCH_CLOCK2, pMainWindowThread->pStBuf, MAX_STRINGTABLE - 1);
		MessageFormat(pMainWindowThread->pBuf, MAX_STRINGTABLE, pMainWindowThread->pStBuf, szClock);
		SetWindowText(pMainWindowThread->hStatic[1], pMainWindowThread->pBuf);

#ifdef _DEBUG
		STPRINTF_FUNC(szDebugText, _T("MainWindow: Bench(): クロック数が取得できました。%.2fGHz\r\n"), (double)llClock / 1000000000);
		OutputDebugString(szDebugText);
#endif
	}

	hFile = CreateFile(pTempFile, GENERIC_WRITE, 0, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
	if (hFile == INVALID_HANDLE_VALUE)
	{
		LoadString(pMainWindowThread->hInst, IDC_BENCH_TEXT2, pMainWindowThread->pStBuf, MAX_STRINGTABLE - 1);
		MessageBox(pMainWindowThread->hWnd, pMainWindowThread->pStBuf, pMainWindowThread->pTitle, MB_ICONINFORMATION);
		goto BENCH_EXIT;
	}

	if (DeviceIoControl(hFile, FSCTL_SET_SPARSE, NULL, 0, NULL, 0, &dwRet, NULL))
	{
		LARGE_INTEGER liSize = { 0 };

#if _MSC_VER < 1500

		HMODULE hDll;

		hDll = LOADSYSLIB(_T("KERNEL32.DLL"));
		if (hDll != NULL)
		{
			const char* szFunc = "SetFilePointerEx";
			BOOL(WINAPI* SetFilePointerEx)(HANDLE, LARGE_INTEGER, PLARGE_INTEGER, DWORD);

			SetFilePointerEx = (BOOL(WINAPI*)(HANDLE, LARGE_INTEGER, PLARGE_INTEGER, DWORD))(GetProcAddress(hDll, szFunc));
			if (SetFilePointerEx != NULL)
			{

#endif /* _MSC_VER < 1500 */

#ifdef _DEBUG
				OutputDebugString(_T("MainWindow: Bench(): スパースファイルの作成します。\r\n"));
#endif

				liSize.QuadPart = BENCH_FILESIZE;
				SetFilePointerEx(hFile, liSize, NULL, FILE_BEGIN);
				SetEndOfFile(hFile);

#if _MSC_VER < 1500

			}
			else
			{
				FreeLibrary(hDll);
				goto BENCH_DLLFAIL;
			}
			FreeLibrary(hDll);
		}
#endif /* _MSC_VER < 1500 */

	}
	else
	{

#if _MSC_VER < 1500
BENCH_DLLFAIL:
#endif /* _MSC_VER < 1500 */

#ifdef _DEBUG
		OutputDebugString(_T("MainWindow: Bench(): スパースファイルの作成に失敗しました。\r\n"));
#endif

		WriteFile(hFile, pbyFileBuf, BENCH_FILESIZE, &dwWrite, NULL);
	}
	dwFileSize = GetFileSize(hFile, NULL);
	CloseHandle(hFile);

	if (dwFileSize < BENCH_FILESIZE)
	{
		LoadString(pMainWindowThread->hInst, IDC_BENCH_TEXT2, pMainWindowThread->pStBuf, MAX_STRINGTABLE - 1);
		MessageBox(pMainWindowThread->hWnd, pMainWindowThread->pStBuf, pMainWindowThread->pTitle, MB_ICONINFORMATION);
		goto BENCH_EXIT;
	}
	HashThread_MultiFile_Init(&pMainWindowThread->tagHashThread1, pTempFile, FALSE);


	// ランダムなハッシュデータを作ります。
	srand((unsigned)time(NULL));
	for (dwCount = 0; dwCount < HASH_LEN * 2 / sizeof(DWORD); dwCount++) {
		dwHashByte[dwCount] = (rand() << 16) | rand();
	}
	HashThread_HashToString_Old(szHashComp, (const BYTE*)&dwHashByte, FALSE, HASH_LEN);
	memset(szHashBuf, 0, HASH_LEN * sizeof(WCHAR));

	SetWindowText(pMainWindowThread->hStatic[1], szBenchStep[0]);
	//Sleep(10);
	dwTimeStart = timeGetTime();
	for (dwCount = 0; dwCount < BENCH_LOOP; dwCount++) {
		HashThread_HashToString_Old(szHashBuf, (const BYTE*)&dwHashByte, FALSE, HASH_MD5_LEN);
	}
	dwTime[0] = timeGetTime() - dwTimeStart;
	if (dwTime[0] == 0) {
		dwTime[0] = 1;
	}

	SetWindowText(pMainWindowThread->hStatic[1], szBenchStep[1]);
	//Sleep(10);
	dwTimeStart = timeGetTime();
	for (dwCount = 0; dwCount < BENCH_LOOP * 100; dwCount++) {
		HashThread_HashToString_no_sprintf(szHashBuf, (const BYTE*)&dwHashByte, FALSE, HASH_MD5_LEN);
	}
	dwTime[1] = timeGetTime() - dwTimeStart;
	if (dwTime[1] == 0) {
		dwTime[1] = 1;
	}

	SetWindowText(pMainWindowThread->hStatic[1], szBenchStep[2]);
	//Sleep(10);
	dwTimeStart = timeGetTime();
	for (dwCount = 0; dwCount < BENCH_LOOP; dwCount++) {
		HashThread_HashToString_Default(szHashBuf, (const BYTE*)&dwHashByte, FALSE, HASH_MD5_LEN);
	}
	dwTime[2] = timeGetTime() - dwTimeStart;
	if (dwTime[2] == 0) {
		dwTime[2] = 1;
	}

	SetWindowText(pMainWindowThread->hStatic[1], szBenchStep[3]);
	//Sleep(10);
	dwTimeStart = timeGetTime();
	for (dwCount = 0; dwCount < BENCH_LOOP * 100; dwCount++) {
		AlignedBinaryToWCharToLower(szHashBuf, (const BYTE*)&dwHashByte, HASH_MD5_LEN);
	}
	dwTime[3] = timeGetTime() - dwTimeStart;
	if (dwTime[3] == 0) {
		dwTime[3] = 1;
	}

	SetWindowText(pMainWindowThread->hStatic[1], szBenchStep[4]);
	//Sleep(10);
	dwTimeStart = timeGetTime();
	for (dwCount = 0; dwCount < BENCH_LOOP; dwCount++) {
		HashThread_HashToHash_Old((BYTE*)&dwHashByte, szHashBuf, HASH_MD5_LEN);
	}
	dwTime[4] = timeGetTime() - dwTimeStart;
	if (dwTime[4] == 0) {
		dwTime[4] = 1;
	}

	SetWindowText(pMainWindowThread->hStatic[1], szBenchStep[5]);
	//Sleep(10);
	dwTimeStart = timeGetTime();
	for (dwCount = 0; dwCount < BENCH_LOOP * 10; dwCount++) {
		HashThread_HashToHash_no_scanf((BYTE*)&dwHashByte, szHashBuf, HASH_MD5_LEN);
	}
	dwTime[5] = timeGetTime() - dwTimeStart;
	if (dwTime[5] == 0) {
		dwTime[5] = 1;
	}

	SetWindowText(pMainWindowThread->hStatic[1], szBenchStep[6]);
	//Sleep(10);
	dwTimeStart = timeGetTime();
	for (dwCount = 0; dwCount < BENCH_LOOP; dwCount++) {
		HashThread_HashToHash_Default((BYTE*)&dwHashByte, szHashBuf, HASH_MD5_LEN);
	}
	dwTime[6] = timeGetTime() - dwTimeStart;
	if (dwTime[6] == 0) {
		dwTime[6] = 1;
	}

	SetWindowText(pMainWindowThread->hStatic[1], szBenchStep[7]);
	//Sleep(10);
	dwTimeStart = timeGetTime();
	for (dwCount = 0; dwCount < BENCH_LOOP * 100; dwCount++) {
		HashThread_HashToHash_no_strtoul((BYTE*)&dwHashByte, szHashBuf, HASH_MD5_LEN);
	}
	dwTime[7] = timeGetTime() - dwTimeStart;
	if (dwTime[7] == 0) {
		dwTime[7] = 1;
	}

	SetWindowText(pMainWindowThread->hStatic[1], szBenchStep[8]);
	//Sleep(10);
	pMainWindowThread->tagHashThread1.hWnd = NULL;
	dwTimeStart = timeGetTime();
	HashThread_MultiFile_Thread(&pMainWindowThread->tagHashThread1);
	dwTime[8] = timeGetTime() - dwTimeStart;
	pMainWindowThread->tagHashThread1.hWnd = pMainWindowThread->hWnd;

	for (i = 0; i < (SIZEOF_NUM(dScoreK)); i++)
	{
		dScore[i] = dScoreK[i] / ((__int64)dwTime[i] / ((double)llClock / 1000000.));
		dScoreSum += dScore[i];
	}


	LoadString(pMainWindowThread->hInst, IDC_BENCH_STEP1, pMainWindowThread->pStBuf, MAX_STRINGTABLE - 1);
	MessageFormat(pMainWindowThread->pBuf, MAX_STRINGTABLE, pMainWindowThread->pStBuf, (DWORD)dScoreSum, szClock);
	SetWindowText(pMainWindowThread->hStatic[1], pMainWindowThread->pBuf);
	LoadString(pMainWindowThread->hInst, IDC_BENCH_TEXT3, pMainWindowThread->pStBuf, MAX_STRINGTABLE * 4 - 1);


	ptr = pMainWindowThread->pBecthResult;
	ptr = qtcscpy(ptr, _T("ベンチマークが完了しました。"));
	*ptr++ = '[';
	ptr = qtcscpy(ptr, _T("クロック数: "));
	ptr = qtcscpy(ptr, szClock);
	*ptr++ = 'G';
	*ptr++ = 'H';
	*ptr++ = 'z';
	*ptr++ = ']';
	*ptr++ = '\r';
	*ptr++ = '\n';
	*ptr++ = '\r';
	*ptr++ = '\n';
	ptr = qtcscpy(ptr, _T("-------------------------------------------------"));
	*ptr++ = '\r';
	*ptr++ = '\n';

	for (i = 0; i < SIZEOF_NUM(dScoreK); i++)
	{
		ptr = qtcscpy(ptr, cpFuncName[i]);
		*ptr++ = '(';
		*ptr++ = ')';
		*ptr++ = ':';
		ptr = qtcscpy(ptr, cpResultTab[nResultTab[i]]);
		_itot_s((DWORD)dScore[i], szNum, 32, 10);
		ptr = qtcscpy(ptr, szNum);
		ptr = qtcscpy(ptr, _T("点"));
		*ptr++ = ' ';
		*ptr++ = '(';
		I64TOT_FUNC(dwTime[i], szNum, 32, 10);
		ptr = qtcscpy(ptr, szNum);
		*ptr++ = 'm';
		*ptr++ = 's';
		*ptr++ = ')';
		*ptr++ = '\r';
		*ptr++ = '\n';
	}

	ptr = qtcscpy(ptr, _T("-------------------------------------------------"));
	*ptr++ = '\r';
	*ptr++ = '\n';
	ptr = qtcscpy(ptr, _T("スコア:"));
	*ptr++ = '\t';
	*ptr++ = '\t';
	*ptr++ = '\t';
	I64TOT_FUNC((__int64)dScoreSum, szNum, 32, 10);
	ptr = qtcscpy(ptr, szNum);
	ptr = qtcscpy(ptr, _T("点"));

	memcpy(pMainWindowThread->tagHashThread1.tagMultiFile, ptagHashThread_MultiFile_Swap1, sizeof(tagHashThread_MultiFile));
	memcpy(pMainWindowThread->tagHashThread1.tagMultiFile->ptagFileRecode, ptagHashThread_FileRecode_Swap1, sizeof(tagHashThread_FileRecode));
	MessageBox(pMainWindowThread->hWnd, pMainWindowThread->pBecthResult, pMainWindowThread->pTitle, MB_ICONINFORMATION);

BENCH_EXIT:
	DeleteFile(pTempFile);
	free(pTempFile);
	free(ptagHashThread_MultiFile_Swap1);
	free(ptagHashThread_FileRecode_Swap1);
	free(pbyFileBuf);
	//SetWindowText(pMainWindowThread->hStatic[1], szSwapTimeText);
	EnableMenuItem(tagMainWindow1.hPupMenu[0][0], ID_MAINSUBMENU1_BENCHRESULT, MF_ENABLED);
	EnableWindow(pMainWindowThread->hWnd, TRUE);
	WINDOW_UNLOCK;
	IF_LIKELY(pMainWindowThread->hBenchHandle)
	{
		pMainWindowThread->hBenchHandle = NULL;
		_endthreadex(TRUE);
	}
	return TRUE;
}

DWORD GetSubFolderCount(const TCHAR* cpInString)
{
	HANDLE hFind = NULL;
	WIN32_FIND_DATA fdFindData = { 0 };
	TCHAR szFile[MAX_PATH_SIZE] = _T("");
	DWORD dwRet = 0;
	size_t iBufferSize = 0;

	dwRet = ExpandEnvironmentStrings(cpInString, szFile, MAX_PATH_SIZE - 1);
	if (dwRet != 0) {
		iBufferSize = dwRet;
	}
	else
	{
		const TCHAR* pTemp = qtcscpy(szFile, cpInString);
		iBufferSize = pTemp - szFile;
	}

	if (*(szFile + iBufferSize - 1) == '\\')
	{
		iBufferSize--;
		*(szFile + iBufferSize) = '\0';
	}

	IF_UNLIKELY(iBufferSize > MAX_PATH_SIZE) {
		return FALSE;
	}

	qtcscpy(szFile + iBufferSize, _T("\\*"));

	hFind = FindFirstFile(szFile, &fdFindData);
	IF_UNLIKELY(hFind == INVALID_HANDLE_VALUE) {
		return dwRet;
	}

	do
	{
		if (~fdFindData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY || fdFindData.dwFileAttributes & ~tagMainWindow1.tagHashThread1.dwFileAttributeMask || _tcscmp(fdFindData.cFileName, _T(".")) == 0 || _tcscmp(fdFindData.cFileName, _T("..")) == 0) {
			continue;
		}
		dwRet++;
	}
	while (FindNextFile(hFind, &fdFindData));
	FindClose(hFind);
	return dwRet;
}

VOID MainWindow_AddFile_Start(VOID)
{
	Clear(TRUE);
}

DWORD MainWindow_AddFile(size_t nCount)
{
	BOOL nIsHashFileRet = 0;
	DWORD dwFileCount = tagMainWindow1.tagHashThread1.tagMultiFile->dwFileCount;
	DWORD dwRet = 0;
	size_t nFileLen = 0;

	IF_UNLIKELY(dwFileCount >= FILE_MAX_COUNTSIZE)
	{
		LoadString(tagMainWindow1.hInst, IDS_OVERFILE1, tagMainWindow1.pStBuf, MAX_STRINGTABLE - 1);
		MessageFormat(tagMainWindow1.pBuf, 2048, tagMainWindow1.pStBuf, tagMainWindow1.pFile);
		MainWindow_MessageBox(tagMainWindow1.hWnd, tagMainWindow1.pBuf, tagMainWindow1.pTitle, MB_ICONHAND);
		return FILE_MAX_COUNTSIZE;
	}

	//tagMainWindow1.tagHashThread1.nIsFileNoCheck = dwFileCount;
	nIsHashFileRet = GetHashFilePath();
	nFileLen = qtcslen(tagMainWindow1.pFile);

#ifdef _UNICODE
	IF_UNLIKELY(nFileLen > MAX_PATH_SIZE)
#else
	IF_UNLIKELY(nFileLen > _MAX_PATH)
#endif /* !UNICODE */
	{
		LoadString(tagMainWindow1.hInst, IDS_FILE_OPEN3, tagMainWindow1.pStBuf, MAX_STRINGTABLE - 1);
		MessageFormat(tagMainWindow1.pBuf, 2048, tagMainWindow1.pStBuf, tagMainWindow1.pFile);
		MainWindow_MessageBox(tagMainWindow1.hWnd, tagMainWindow1.pBuf, tagMainWindow1.pTitle, MB_ICONHAND);
		return 0;
	}

	if (dwAppFrag & APP_FOLDEROPEN)
	{
		dwRet = HashThread_Folder_Init(&tagMainWindow1.tagHashThread1, tagMainWindow1.pFile, nCount != 0 ? TRUE : FALSE);
		if (dwFileCount == dwRet)
		{
			if (tagMainWindow1.bIsEmptyFolder < 9)
			{
				tagMainWindow1.pNextEmptyFolder = qtcscpy(tagMainWindow1.pNextEmptyFolder, tagMainWindow1.pFile);
				tagMainWindow1.pNextEmptyFolder = qtcscpy(tagMainWindow1.pNextEmptyFolder, _T("\r\n"));
			}
			else if (tagMainWindow1.bIsEmptyFolder < 10)
			{
				tagMainWindow1.pNextEmptyFolder = qtcscpy(tagMainWindow1.pNextEmptyFolder, _T("..."));
				tagMainWindow1.pNextEmptyFolder = qtcscpy(tagMainWindow1.pNextEmptyFolder, _T("\r\n"));
			}
			tagMainWindow1.bIsEmptyFolder++;
		}
	}
	else if (nIsHashFileRet != FALSE)
	{
		dwRet = HashFile_FileOpen(tagMainWindow1.tagHashThread1.tagMultiFile->dwFileCount != 0 ? TRUE : FALSE);

		if (dwFileCount == dwRet && tagMainWindow1.tagHashThread1.dwHashThreadLastError != -2)
		{
			if (tagMainWindow1.bIsHashNotFoundFile < 9)
			{
				tagMainWindow1.pNextHashNotFoundFile = qtcscpy(tagMainWindow1.pNextHashNotFoundFile, tagMainWindow1.pFile);
				tagMainWindow1.pNextHashNotFoundFile = qtcscpy(tagMainWindow1.pNextHashNotFoundFile, _T("\r\n"));
			}
			else if (tagMainWindow1.bIsHashNotFoundFile < 10)
			{
				tagMainWindow1.pNextHashNotFoundFile = qtcscpy(tagMainWindow1.pNextHashNotFoundFile, _T("..."));
				tagMainWindow1.pNextHashNotFoundFile = qtcscpy(tagMainWindow1.pNextHashNotFoundFile, _T("\r\n"));
			}
			tagMainWindow1.bIsHashNotFoundFile++;
		}
	}
	else
	{
		dwRet = HashThread_MultiFile_Init(&tagMainWindow1.tagHashThread1, tagMainWindow1.pFile, nCount != 0 ? TRUE : FALSE);

		if (dwFileCount == dwRet && tagMainWindow1.tagHashThread1.dwHashThreadLastError != -2)
		{
			if (tagMainWindow1.bIsNotFoundFile < 9)
			{
				tagMainWindow1.pNextNotFoundFile = qtcscpy(tagMainWindow1.pNextNotFoundFile, tagMainWindow1.pFile);
				tagMainWindow1.pNextNotFoundFile = qtcscpy(tagMainWindow1.pNextNotFoundFile, _T("\r\n"));
			}
			else if (tagMainWindow1.bIsNotFoundFile < 10)
			{
				tagMainWindow1.pNextNotFoundFile = qtcscpy(tagMainWindow1.pNextNotFoundFile, _T("..."));
				tagMainWindow1.pNextNotFoundFile = qtcscpy(tagMainWindow1.pNextNotFoundFile, _T("\r\n"));
			}
			tagMainWindow1.bIsNotFoundFile++;
		}
		else if (tagFileListWindow1.hWnd) {
			FileListDlg_ListView(dwRet - 1, FALSE);
		}
	}
	return dwRet;
}

VOID MainWindow_AddFile_End(BOOL nIsHashThread)
{
	if (tagMainWindow1.bIsNotFoundFile > 0 || tagMainWindow1.bIsHashNotFoundFile > 0 || tagMainWindow1.bIsEmptyFolder > 0)
	{
		TCHAR* szLargeBuf = NULL;

		szLargeBuf = (TCHAR*)malloc((size_t)2048 * 40 * sizeof(TCHAR));
		if (szLargeBuf != NULL)
		{
			TCHAR* p1 = szLargeBuf;

			if (tagMainWindow1.bIsNotFoundFile > 0)
			{
				LoadString(tagMainWindow1.hInst, IDS_FILE_OPEN2, tagMainWindow1.pStBuf, MAX_STRINGTABLE - 1);
				MessageFormat(szLargeBuf, 2048 * 20, tagMainWindow1.pStBuf, tagMainWindow1.pNotFoundFile);
			}

			if (tagMainWindow1.bIsHashNotFoundFile > 0)
			{
				if (p1 != szLargeBuf)
				{
					p1 = _tcslen(szLargeBuf) + szLargeBuf;
					p1 = qtcscpy(p1, _T("\r\n\r\n"));
				}
				LoadString(tagMainWindow1.hInst, IDS_HASHFILE_OPEN1, tagMainWindow1.pStBuf, MAX_STRINGTABLE - 1);
				MessageFormat(p1, 2048 * 20, tagMainWindow1.pStBuf, tagMainWindow1.tagHashThread1.cpHashName, tagMainWindow1.pHashNotFoundFile);
			}

			if (tagMainWindow1.bIsEmptyFolder > 0)
			{
				if (p1 != szLargeBuf)
				{
					p1 = _tcslen(szLargeBuf) + szLargeBuf;
					p1 = qtcscpy(p1, _T("\r\n\r\n"));
				}
				LoadString(tagMainWindow1.hInst, IDS_FOLDER_OPEN3, tagMainWindow1.pStBuf, MAX_STRINGTABLE - 1);
				MessageFormat(p1, 2048 * 20, tagMainWindow1.pStBuf, tagMainWindow1.pEmptyFolder);
			}
			MainWindow_MessageBox(tagMainWindow1.hWnd, szLargeBuf, tagMainWindow1.pTitle, MB_ICONHAND);
			free(szLargeBuf);
		}
	}

	if (tagMainWindow1.tagHashThread1.tagMultiFile->dwFileCount > 0)
	{
		tagHashThread_FileRecode* ptagHashFile_Recode = HashThread_GetFileRecode_Core(&tagMainWindow1.tagHashThread1, HashThread_GetCountItem(&tagMainWindow1.tagHashThread1) - 1);
		size_t nCount = 0;

		if (tagMainWindow1.tagHashThread1.tagMultiFile->dwFileCount > 1) {
			dwAppFrag |= APP_FILELISTBOX_EDIT;
		}
		else {
			dwAppFrag &= ~APP_FILELISTBOX_EDIT;
		}

		nCount = qtcslen(ptagHashFile_Recode->szFileName);

		if (nCount < _MAX_PATH)
		{
			TCHAR* pFileName = ptagHashFile_Recode->szFileName;

			if (TCHAR_COMP_TO_INT_4CHAR(ptagHashFile_Recode->szFileName, '\\', '\\', '?', '\\')) {
				pFileName += 4;
			}
			qtcscpy(tagMainWindow1.pOfn1Buf, pFileName);
		}
		else {
			*tagMainWindow1.pOfn1Buf = '\0';
		}

		if (tagMainWindow1.tagHashThread1.tagMultiFile->dwFileCount > 1)
		{
			TCHAR* pHashFileName = NULL;
			TCHAR* pExt = NULL;

			pHashFileName = GetTCharToFileName(tagMainWindow1.pHashFile);
			if (*(pHashFileName - 1) == _T('\\'))
			{
				pHashFileName--;
				*pHashFileName = '\0';
			}
			LoadString(tagMainWindow1.hInst, IDS_FILENAME, tagMainWindow1.pStBuf, MAX_STRINGTABLE - 1);
			pExt = GetTCharToExtension2(tagMainWindow1.pStBuf, TRUE);
			if (pExt != NULL) {
				qtcscpy(pExt, tagMainWindow1.tagHashThread1.cpHashExt);
			}
			qtcscpy(pHashFileName, tagMainWindow1.pStBuf);
		}

		if (dwAppFrag & APP_HASHFILE_ADDMODE)
		{
			static int init = 0;

			if (init == 0)
			{
				tagMainWindow1.dwAddFileModeFileCount = HashThread_GetCountItem(&tagMainWindow1.tagHashThread1);
				tagMainWindow1.dwOpenItem = tagMainWindow1.dwAddFileModeFileCount - 1;
				init++;
			}
			SendMessage(tagMainWindow1.hWnd, tagMainWindow1.tagHashThread1.nMessageID, APP_MESSAGE_END, HashThread_GetCurrentItem(&tagMainWindow1.tagHashThread1));
			return;
		}

		SetCurrentDirectory(tagMainWindow1.tagHashThread1.tagMultiFile->szCurrentFolder);

		if (nIsHashThread) {
			GetHash();
		}
	}
	else
	{
		if (dwAppFrag & APP_HASHFILE_ADDMODE)
		{
			dwAppFrag &= ~APP_HASHFILE_ADDMODE;

			tagMainWindow1.dwAddFileModeFileCount = 0;
			tagMainWindow1.dwOpenItem = 0;
			tagMainWindow1.tagHashThread1.tagMultiFile->ptagFileRecode->dwHashFileHashLine = HASHFILE_NOLINE;
			EnableWindow(tagMainWindow1.hButton[0], TRUE);
		}
	}
	return;
}

DWORD ReadMeOpen(VOID)
{
	HINSTANCE hInst = NULL;
	TCHAR* ptr = NULL;
	DWORD nRet = 0;
	const TCHAR* cpFile = _T("ReadMe.txt");

	IF_UNLIKELY(GetModuleFileName(NULL, tagMainWindow1.pTempBuf, MAX_PATH_SIZE - 1) == 0) {
		return 0;
	}

	ptr = _tcsrchr(tagMainWindow1.pTempBuf, '\\');
	if (ptr == NULL) {
		return 0;
	}

	ptr++;
	qtcscpy(ptr, cpFile);

	hInst = ShellExecute(tagMainWindow1.hWnd, _T("open"), tagMainWindow1.pTempBuf, 0, 0, SW_SHOWNORMAL);
	if ((size_t)hInst < 32)
	{
		switch ((size_t)hInst)
		{
		case ERROR_FILE_NOT_FOUND:
		case ERROR_PATH_NOT_FOUND:
			LoadString(tagMainWindow1.hInst, IDS_READMEOPEN1, tagMainWindow1.pStBuf, MAX_STRINGTABLE);
			MainWindow_MessageBox(tagMainWindow1.hWnd, tagMainWindow1.pStBuf, tagMainWindow1.pTitle, MB_OK | MB_ICONERROR);
			break;
		default:
			nRet = TRUE;
		}
	}
	return nRet;
}

DWORD VersionCheck(VOID)
{
	UINT nIds = IDS_APP_URL1;

	if (!IsWin8OrGreater()) {
		nIds++;
	}

	LoadString(tagMainWindow1.hInst, nIds, tagMainWindow1.pStBuf, MAX_STRINGTABLE - 1);
	if (*tagMainWindow1.pStBuf != '\0') {
		ShellExecute(tagMainWindow1.hWnd, _T("open"), tagMainWindow1.pStBuf, 0, 0, SW_SHOWNORMAL);
	}
	return TRUE;
}

VOID Hash_SetFileInfo(DWORD dwItem)
{
	const tagHashThread_FileRecode* cptagHashFile_Recode = NULL;
	DWORD dwRet = 0;
	int nEnableMenuItemRet[] = { MF_GRAYED, MF_ENABLED };
	int nRet = 0;

	if (tagMainWindow1.tagHashThread1.tagMultiFile->dwFileCount == 0 || dwItem > tagMainWindow1.tagHashThread1.tagMultiFile->dwFileCount)
	{
		SetWindowText(tagMainWindow1.hEdit[2], _T(""));
		SendMessage(tagMainWindow1.hEdit[2], EM_SETREADONLY, TRUE, 0);
		SetWindowText(tagMainWindow1.hStatic[3], _T(""));
		SetToolTip(tagMainWindow1.hTool[0], tagMainWindow1.hStatic[3], _T(""));
		SetWindowText(tagMainWindow1.hStatic[5], _T(""));
		SetWindowText(tagMainWindow1.hStatic[7], _T(""));
		SetToolTip(tagMainWindow1.hTool[0], tagMainWindow1.hStatic[7], _T(""));
		return;
	}

	cptagHashFile_Recode = HashThread_GetFileRecode_Core(&tagMainWindow1.tagHashThread1, dwItem);
	if (*cptagHashFile_Recode->szFileName != '\0')
	{
		SetWindowText(tagMainWindow1.hEdit[2], GetTCharToFileName(cptagHashFile_Recode->szFileName));
		qtcscpy(tagMainWindow1.pBuf, cptagHashFile_Recode->szFileName);
		GetTCharToFolderName(tagMainWindow1.pBuf);//PathRemoveFileSpec
		SetWindowText(tagMainWindow1.hStatic[3], tagMainWindow1.pBuf);

		dwRet = ExpandEnvironmentStrings(tagMainWindow1.pBuf, tagMainWindow1.pTempBuf, MAX_PATH_SIZE);
		if (dwRet == 0) {
			qtcscpy(tagMainWindow1.pTempBuf, tagMainWindow1.pBuf);
		}
		SetToolTip(tagMainWindow1.hTool[0], tagMainWindow1.hStatic[3], tagMainWindow1.pTempBuf);
	}
	else
	{
		TCHAR szMemName[sizeof(size_t) * 2 + 5] = { 0 };

		SetWindowText(tagMainWindow1.hEdit[2], _T("tagHashThread1.pFileBuffer"));
		VariableView(szMemName, (size_t)tagMainWindow1.tagHashThread1.pFileBuffer, sizeof(tagMainWindow1.tagHashThread1.pFileBuffer), TRUE, FALSE);
		szMemName[sizeof(size_t) * 2 + 2] = '\0';
		SetWindowText(tagMainWindow1.hStatic[3], szMemName);
	}

	switch (cptagHashFile_Recode->dwFileLastError)
	{
	case 0:
	case (DWORD)-1:
		FileSizeText2ToTChar2(cptagHashFile_Recode->llFileSize, tagMainWindow1.pBuf);
		break;
	case ERROR_FILE_NOT_FOUND:
	case ERROR_PATH_NOT_FOUND:
	case ERROR_TOO_MANY_OPEN_FILES:
		LoadString(tagMainWindow1.hInst, IDS_FILESIZE_TEXT2, tagMainWindow1.pBuf, MAX_STRINGTABLE - 1);
		break;
	default:
		GetLastErrorMsg(cptagHashFile_Recode->dwFileLastError);
		FileSizeText2ToTChar2(cptagHashFile_Recode->llFileSize, tagMainWindow1.pBuf);
	}
	SetWindowText(tagMainWindow1.hStatic[5], tagMainWindow1.pBuf);

	nRet = dwItem > 0 ? TRUE : FALSE;
	EnableWindow(tagMainWindow1.hButton[6], nRet);
	EnableMenuItem(tagMainWindow1.hPupMenu[0][0], ID_MAINSUBMENU1_PREVIOUSFILE, nEnableMenuItemRet[nRet]);

	nRet = tagMainWindow1.tagHashThread1.tagMultiFile->dwFileCount > 0 && dwItem < (tagMainWindow1.tagHashThread1.tagMultiFile->dwFileCount - 1) ? TRUE : FALSE;
	EnableWindow(tagMainWindow1.hButton[7], nRet);
	EnableMenuItem(tagMainWindow1.hPupMenu[0][0], ID_MAINSUBMENU1_NEXTFILE, nEnableMenuItemRet[nRet]);
	return;
}

DWORD HashFile_FileOpen(BOOL nIsAdd)
{
	TCHAR szOption[10] = _T("");
	TCHAR* pOptionPtr = szOption;
	DWORD dwRet = 0;

	if (nIsAdd == FALSE) {
		tagMainWindow1.tagHashThread1.tagMultiFile->dwFileCount = 0;
	}

	if (dwAppFrag & APP_HASHFILE_ADDMODE)
	{
		*pOptionPtr++ = 'a';
		*pOptionPtr = '\0';

		FileListDlg();
		dwRet = (DWORD)SendMessage(tagFileListWindow1.hWnd, tagFileListWindow1.nWindowMessage, APP_MESSAGE_HASHFILE_OPEN, 0);
		return dwRet;
	}

	dwRet = HashThread_HashFile_Init(&tagMainWindow1.tagHashThread1, tagMainWindow1.pFile, tagMainWindow1.tagHashThread1.tagMultiFile->dwFileCount, szOption);//tagFileListWindow1.dwNewFile
	return dwRet;
}

BOOL ClipboardCheck(DWORD dwItem)
{
	int nRet = FALSE;

	if (HashThread_GetFileRecode_Core(&tagMainWindow1.tagHashThread1, dwItem)->dwFileLastError == 0 && OpenClipboard(tagMainWindow1.hWnd))
	{
		nRet = IsClipboardFormatAvailable(CF_TEXT);
		CloseClipboard();
	}
	return nRet;
}

VOID Open(BOOL nIsPause)
{
	IF_UNLIKELY(dwAppFrag & APP_FOLDERINIT) {
		return;
	}
	else if (dwAppFrag & APP_HASHTHREAD && nIsPause) {
		Pause();
	}
	else {
		OpenFile();
	}
	return;
}

BOOL OpenFile(VOID)
{
	DWORD dwRet = 0;

	// ウィンドウロックします。
	WINDOW_LOCK;

	// 最初のファイル名以降はゼロクリアする。
	*tagMainWindow1.pClipboardString = '\0';
	qtcscpy(tagMainWindow1.pTempBuf, tagMainWindow1.pOfn1Buf);
	memset(tagMainWindow1.pOfn1Buf, 0, ((size_t)MAX_PATH_SIZE * 256) * sizeof(TCHAR));
	qtcscpy(tagMainWindow1.pOfn1Buf, tagMainWindow1.pTempBuf);
	*(tagMainWindow1.pOfn1Buf + _MAX_PATH) = '\0';

	if (GetOpenFileName(&tagMainWindow1.ofnFileName[0]))
	{
		TCHAR* pFileTextPos = NULL; 
		DWORD dwCount = 0;
		DWORD dwErrCount = 0;//ファイルのパスが長いと _tfullpath() に失敗してこの値が増えていきます。

		dwAppFrag &= ~(APP_FOLDEROPEN | APP_FILELISTBOX_EDIT);

		// OpenFileNameの解析
		MainWindow_AddFile_Start();
		pFileTextPos = tagMainWindow1.pOfn1Buf + tagMainWindow1.ofnFileName[0].nFileOffset;

		while (1)
		{
			tagMainWindow1.tagHashThread1.nIsFileNoCheck = 0;
			if (_tfullpath(tagMainWindow1.pFile, pFileTextPos, MAX_PATH_SIZE) != NULL)
			{
				dwRet = MainWindow_AddFile(dwCount);
				if (dwRet == (DWORD)-1) {
					break;
				}
			}
			else {
				dwErrCount++;
			}

			while (*pFileTextPos++ != NULL) {
				;
			}

			if (*pFileTextPos == NULL) {
				break;
			}
			dwCount++;
		}

		IF_UNLIKELY(dwRet == (DWORD)-1)
		{
			FileRecodeFail(tagMainWindow1.hWnd);
			DestroyWindow(tagMainWindow1.hWnd);
			return FALSE;
		}

		if (dwErrCount == 0)
		{
			tagHashThread_FileRecode* ptagHashFile_Recode = HashThread_GetFileRecode_Core(&tagMainWindow1.tagHashThread1, HashThread_GetCountItem_Core(&tagMainWindow1.tagHashThread1) - 1);

#ifndef _NODLL
			// 複数選択時に次回のフォルダー表示がおかしくならないように最後のファイルにしておきます。
			_tcscpy_s(tagMainWindow1.pOfn1Buf, MAX_PATH_SIZE - 1, ptagHashFile_Recode->szFileName);
			_tcscpy_s(tagMainWindow1.pFile, MAX_PATH_SIZE - 1, tagMainWindow1.pOfn1Buf);
#else
			// szFileName が初期化されていない場合に tagMainWindow1 の構造体を破壊してしまうため終端文字を追加します。
			ptagHashFile_Recode->szFileName[MAX_PATH_SIZE - 1] = '\0';
			// 複数選択時に次回のフォルダー表示がおかしくならないように最後のファイルにしておきます。
			_tcscpy(tagMainWindow1.pOfn1Buf, ptagHashFile_Recode->szFileName);
			_tcscpy(tagMainWindow1.pFile, tagMainWindow1.pOfn1Buf);
#endif
			MainWindow_AddFile_End();

		}
		else
		{
			LoadString(tagMainWindow1.hInst, IDS_FILE_OPEN3, tagMainWindow1.pStBuf, MAX_STRINGTABLE - 1);
			MessageFormat(tagMainWindow1.pBuf, 2048, tagMainWindow1.pStBuf, tagMainWindow1.pOfn1Buf);
			MainWindow_MessageBox(tagMainWindow1.hWnd, tagMainWindow1.pBuf, tagMainWindow1.pTitle, MB_ICONHAND);
		}

		if (HashThread_GetCountItem_Core(&tagMainWindow1.tagHashThread1) > 0) {
			dwRet = TRUE;
		}
	}
	else
	{
		const DWORD dwError = CommDlgExtendedError();
		if (dwError) {
			GetCommDlgExtendedErrorMsg(dwError);
		}
	}

	// ウィンドウロックの解除します。
	WINDOW_UNLOCK;

	return dwRet;
}

BOOL GetHashFilePath(VOID)
{
	TCHAR* pExt = NULL;
	TCHAR* pHashFilePos = NULL;
	const TCHAR* cpFileName;
	size_t nRet = 0;
	size_t nCount = 0;

	cpFileName = GetTCharToFileName(tagMainWindow1.pFile);
	pExt = GetTCharToExtension2(tagMainWindow1.pFile, TRUE);
	if (pExt == NULL || _tcscmp(pExt, tagMainWindow1.tagHashThread1.cpHashExt) != 0)
	{
		if (tagMainWindow1.tagHashThread1.tagMultiFile->dwFileCount > 0) {
			return FALSE;
		}
	}

	pExt = tagMainWindow1.pHashFile;
	if (TCHAR_COMP_TO_INT_4CHAR(tagMainWindow1.pFile, '\\', '\\', '?', '\\')) {
		nCount = 4;
	}

	nRet = ExpandEnvironmentStrings(tagMainWindow1.pFile + nCount, tagMainWindow1.pBuf, (DWORD)(MAX_PATH_SIZE - nCount));
	if (nRet == 0) {
		qtcscpy(tagMainWindow1.pBuf, tagMainWindow1.pFile + nCount);
	}

	if (PathIsDirectory(tagMainWindow1.pBuf))
	{
		dwAppFrag |= APP_FOLDEROPEN;

		if (*tagMainWindow1.pHashFile == '\0')
		{
			pHashFilePos = qtcscpy(tagMainWindow1.pHashFile, tagMainWindow1.pFile);
			if ((pHashFilePos - tagMainWindow1.pFile) > 0 && *(pHashFilePos - 1) == '\\')
			{
				pHashFilePos--;
				*pHashFilePos = '\0';
			}
			goto CREATE_FILENAME;
		}
	}
	else
	{
		DWORD dwHashType = 0;
		DWORD dwRet = 0;

		dwAppFrag &= ~APP_FOLDEROPEN;

		dwRet = HashThread_GetHashFileType(&tagMainWindow1.tagHashThread1, tagMainWindow1.pBuf);
		if (dwRet != (DWORD)-1)
		{
			dwHashType = dwRet;

			if (HashThread_GetFileCurrentFileRecode_Core(&tagMainWindow1.tagHashThread1) == 0) {
				tagMainWindow1.tagHashThread1.dwHashType = dwHashType;
			}

			if (~dwAppFrag & APP_HASHFILE_ENABLE_HASH)
			{
				if (*tagMainWindow1.pHashFile == '\0') {
					pHashFilePos = qtcscpy(tagMainWindow1.pHashFile, tagMainWindow1.pBuf);
				}
				return TRUE;
			}
			pHashFilePos = qtcscpy(tagMainWindow1.pHashFile, tagMainWindow1.pBuf);
		}
		else
		{
			const TCHAR** ppCheckSumFile = (const TCHAR**)tagMainWindow1.ppCheckSumFile;

			while (*ppCheckSumFile != NULL)
			{
				if (_tcsicmp(cpFileName, *ppCheckSumFile) == 0)
				{
					TCHAR* pChackSumFileName = NULL;

					pHashFilePos = qtcscpy(tagMainWindow1.pHashFile, tagMainWindow1.pBuf);
					pChackSumFileName = GetTCharToFileName(tagMainWindow1.pHashFile);
					qtcscpy(pChackSumFileName, *ppCheckSumFile);
					return TRUE;
				}
				ppCheckSumFile++;
			}
		}

		if (*tagMainWindow1.pHashFile == '\0')
		{
			qtcscpy(tagMainWindow1.pHashFile, tagMainWindow1.pBuf);
			PathRemoveFileSpec(tagMainWindow1.pHashFile);
			pHashFilePos = tagMainWindow1.pHashFile + _tcslen(tagMainWindow1.pHashFile);
			goto CREATE_FILENAME;
		}
	}
	return FALSE;
CREATE_FILENAME:

	// ルートだと "C:\\FileList.md5" の '\\'が2つになり、無効なファイル名になるから '\\'を1つ削除する。
	if (*(pHashFilePos - 1) == '\\')
	{
		pHashFilePos--;
		*pHashFilePos = '\0';
	}

	if (dwAppFrag & APP_FOLDEROPEN)
	{
		nRet = LoadString(tagMainWindow1.hInst, IDS_FILENAME, tagMainWindow1.pStBuf, MAX_STRINGTABLE);
#ifndef _UNICODE
		nRet = qmbslen(tagMainWindow1.pStBuf);
#endif
		pExt = GetTCharToExtension2(tagMainWindow1.pStBuf, TRUE);
		if (pExt != NULL) {
			nRet += tagMainWindow1.tagHashThread1.nHashExtLen - _tcslen(pExt);
		}
	}
	else
	{
		*(tagMainWindow1.pStBuf) = '\\';
		pExt = qtcscpy(tagMainWindow1.pStBuf + 1, cpFileName);
		nRet = pExt - tagMainWindow1.pStBuf;
		nRet += tagMainWindow1.tagHashThread1.nHashExtLen;
	}

	if (pExt) {
		qtcscpy(pExt, tagMainWindow1.tagHashThread1.cpHashExt);
	}

	nCount = pHashFilePos - tagMainWindow1.pHashFile;
	if ((nCount + nRet) > _MAX_PATH)
	{
		*tagMainWindow1.pHashFile = '\0';
		return FALSE;
	}

#ifdef _UNICODE
	qwcscpy(tagMainWindow1.pHashFile + nCount, tagMainWindow1.pStBuf);
#else
	nCount = strlen(tagMainWindow1.pHashFile);
	qstrcpy(tagMainWindow1.pHashFile + nCount, tagMainWindow1.pStBuf);
#endif /* _UNICODE	 */

	return FALSE;
}

VOID Save(VOID)
{
	IF_UNLIKELY(dwAppFrag & APP_FOLDERINIT) {
		return;
	}
	else if (dwAppFrag & APP_HASHTHREAD) {
		Cancel(FALSE);
	}
	else {
		SaveFile();
	}
	SetFocus(tagMainWindow1.hButton[0]);
	return;
}

BOOL SaveFile(VOID)
{
	int nIsHashComp = 0;
	int nRet = 0;

	// ウィンドウロックします。
	WINDOW_LOCK;

	if (tagMainWindow1.dwAddFileModeFileCount != 0) {
		nRet = TRUE;
	}

	IF_UNLIKELY(tagMainWindow1.tagHashThread1.tagMultiFile->dwFileCount == 0) {
		goto SAVEFILE_EXIT;
	}

	if (!nRet)
	{
		if (GetSaveFileName(&tagMainWindow1.ofnFileName[1])) {
			nRet = TRUE;
		}
		else
		{
			const DWORD dwError = CommDlgExtendedError();
			if (dwError) {
				GetCommDlgExtendedErrorMsg(dwError);
			}
			goto SAVEFILE_EXIT;
		}
	}

	nIsHashComp = (tagMainWindow1.tagHashThread1.tagMultiFile->ptagFileRecode + tagMainWindow1.dwOpenItem)->dwHashFileHashLine != HASHFILE_NOLINE ? TRUE : FALSE;
	if (tagMainWindow1.ofnFileName[1].nFilterIndex == 2) {
		dwAppFrag |= APP_OLDHASHFILE;
	}
	else {
		dwAppFrag &= ~APP_OLDHASHFILE;
	}

	nRet = HashFile_Save();
	if (nRet == -1) {
		goto SAVEFILE_EXIT;
	}

	if (nIsHashComp != FALSE) {
		Hash_FileView(tagMainWindow1.dwOpenItem);
	}
	nRet++;

SAVEFILE_EXIT:
	// ウィンドウロックの解除します。
	WINDOW_UNLOCK;

	SetFocus(tagMainWindow1.hButton[0]);
	return nRet;
}

BOOL HashFile_Save(VOID)
{
	TCHAR szOption[8] = _T("");
	TCHAR* p = szOption;
	BOOL nRet = 0;

	IF_UNLIKELY(dwAppFrag & APP_FOLDERINIT) {
		return (DWORD)-1;
	}

	*p++ = 'c';
	*p++ = (TCHAR)(tagMainWindow1.dwSaveHashFileCharCode & 0xFF) + '0';
	*p++ = dwAppFrag & APP_HASHOUT_LOWER ? 'l' : 'u';
	if (dwAppFrag & APP_OLDHASHFILE) {
		*p++ = 'o';
	}
	if (dwAppFrag & APP_ENABLE_HASHFILE_WRITETIME) {
		*p++ = 'F';
	}
	*p = '\0';

	nRet = HashThread_CreateHashFile(&tagMainWindow1.tagHashThread1, tagMainWindow1.pHashFile, szOption);
	if (!nRet)
	{
		if (tagFileListWindow1.hWnd) {
			FileListDlg_ListView(0, FALSE);
		}
	}
	else
	{
		DWORD dwErr = GetLastError();
		LoadString(tagMainWindow1.hInst, IDS_HASHFILE_SAVE1, tagMainWindow1.pStBuf, MAX_STRINGTABLE - 1);
		GetLastError_String(tagMainWindow1.pTempBuf, dwErr);
		MessageFormat(tagMainWindow1.pBuf, 2048, tagMainWindow1.pStBuf, tagMainWindow1.cpHashName, tagMainWindow1.pHashFile, dwErr, tagMainWindow1.pTempBuf);
		MainWindow_MessageBox(tagMainWindow1.hWnd, tagMainWindow1.pBuf, tagMainWindow1.pTitle, MB_OK | MB_ICONERROR);
	}
	return nRet;
}

VOID Cancel(BOOL nIsClear)
{
	IF_UNLIKELY(~dwAppFrag & APP_HASHTHREAD) {
		return;
	}
	else IF_UNLIKELY(dwAppFrag & APP_FOLDERINIT) {
		return;
	}

	if (nIsClear != FALSE) {
		dwAppFrag |= APP_FILE_CLEAR;
	}

	dwAppFrag |= APP_CALCEL;
	tagMainWindow1.tagHashThread1.nSuspendMessage = APP_PRGRESS_CANCEL;

	IF_UNLIKELY(dwAppFrag & APP_PAUSE)
	{
		dwAppFrag &= ~APP_PAUSE;
		ResumeThread(tagMainWindow1.tagHashThread1.hHashThreadHandle);
	}
	EnableMenuItem(tagMainWindow1.hPupMenu[0][0], 17, MF_BYPOSITION | MF_ENABLED);
	return;
}

VOID Pause(VOID)
{
	IF_UNLIKELY(dwAppFrag & APP_FOLDERINIT) {
		return;
	}
	SendMessage(tagMainWindow1.hWnd, tagMainWindow1.tagHashThread1.nMessageID, APP_MESSAGE_PAUSE, dwAppFrag & APP_PAUSE ? FALSE : TRUE);
	return;
}

VOID ReOpen(VOID)
{
	IF_UNLIKELY(dwAppFrag & APP_FOLDERINIT) {
		return;
	}
	else IF_UNLIKELY(dwAppFrag & APP_HASHTHREAD) {
		Cancel(FALSE);
	}
	else
	{
		if (HashThread_GetCountItem_Core(&tagMainWindow1.tagHashThread1)) {
			GetHash();
		}
		else {
			OpenFile();
		}
	}
	return;
}

VOID ClipboardCopy(const TCHAR* cpInString)
{
	dwAppFrag |= APP_WINDOW_NOALPHA;
	SetClipboardText(tagMainWindow1.hWnd, cpInString);
	dwAppFrag &= ~APP_WINDOW_NOALPHA;
}

VOID ClipboardPaste(VOID)
{
	TCHAR* pClipboardText = NULL;

	IF_UNLIKELY(dwAppFrag & APP_HASHTHREAD) {
		return;
	}

	dwAppFrag |= APP_WINDOW_NOALPHA;
	SetFocus(tagMainWindow1.hEdit[1]);

	pClipboardText = GetClipboardMallocText(tagMainWindow1.hWnd);
	if (pClipboardText != NULL)
	{
		const DWORD dwRetHashType = FixString(pClipboardText);
		const size_t nHashSize = (size_t)tagMainWindow1.tagHashThread1.dwHashLen * 2;
		size_t nClipboardTextSize = 0;
		int nRet = IDNO;

		nClipboardTextSize = _tcslen(pClipboardText);
		if (nClipboardTextSize > nHashSize)
		{
			*(pClipboardText + nHashSize + 1) = '\0';
			nClipboardTextSize = nHashSize;
		}

		if (dwRetHashType != (DWORD)-1 && dwRetHashType != (DWORD)-2 && tagMainWindow1.tagHashThread1.dwHashType != dwRetHashType)
		{
			LoadString(tagMainWindow1.hInst, IDS_HASHCHANGE1, tagMainWindow1.pStBuf, MAX_STRINGTABLE - 1);
			MessageFormat(tagMainWindow1.pBuf, 512, tagMainWindow1.pStBuf, HashThread_GetHashName(dwRetHashType));
			nRet = MainWindow_MessageBox(tagMainWindow1.hWnd, tagMainWindow1.pBuf, tagMainWindow1.pTitle, MB_YESNO | MB_ICONQUESTION);
		}

		nClipboardTextSize++;
		if (nRet == IDYES)
		{
			tagMainWindow1.dwReOpenMode = 1;
			tagMainWindow1.dwReOpenItem = tagMainWindow1.dwOpenItem;
			Hash_HashChange(dwRetHashType);
			memcpy(tagMainWindow1.pClipboardString, pClipboardText, nClipboardTextSize * sizeof(TCHAR));
			ReOpen();
		}
		else
		{
			memcpy(tagMainWindow1.pClipboardString, pClipboardText, nClipboardTextSize * sizeof(TCHAR));
			SetWindowText(tagMainWindow1.hEdit[1], tagMainWindow1.pClipboardString);
			SendMessage(tagMainWindow1.hEdit[1], EM_SETSEL, nHashSize, nHashSize);
		}
		free(pClipboardText);
		pClipboardText = NULL;
	}
	dwAppFrag &= ~APP_WINDOW_NOALPHA;
	return;
}

VOID Hash_TimeView(BOOL nIsView)
{
	static __int64 llAvg[4] = { 0 };
	static __int64 llReadSize[2] = { 0 };
	static unsigned int nAgvC = 0;
	static unsigned int nAgvN = 2;
	static int stnIsView = 0;
	static int nProgress = 0;
	DWORD dwTime = 0;
	const __int64 llFileAllCurrentSize = (__int64)tagMainWindow1.tagHashThread1.tagMultiFile->llFileAllCurrentSize;
	const __int64 llFileAllSize = (__int64)tagMainWindow1.tagHashThread1.tagMultiFile->llFileAllSize;
	const __int64 llFileReadSize = (__int64)tagMainWindow1.tagHashThread1.llFileReadSize;

#ifdef _DEBUG
	TCHAR szDebugText[1024];
	TCHAR szNum[33];
	TCHAR* ptr = NULL;
#endif

	IF_UNLIKELY(dwAppFrag & (APP_PAUSE | APP_LASTERROR)) {
		return;
	}

	const int nPersent = ((int)((double)(llFileAllCurrentSize + llFileReadSize) / llFileAllSize * 100.) & 0x7F);
	if (nPersent != nProgress)
	{
		PostMessage(tagMainWindow1.hWnd, tagMainWindow1.tagHashThread1.nMessageID, APP_MESSAGE_PROGRESS, (LPARAM)nPersent);
		nProgress = nPersent;
	}

	if (dwAppFrag & APP_DISABLE_MULTIFILE_UPDATE_DRAW && tagMainWindow1.dwOpenItem > tagMainWindow1.dwTimeViewFileCount)
	{
		tagMainWindow1.dwTimeViewFileCount = tagMainWindow1.dwOpenItem;
		Hash_SetFileInfo(tagMainWindow1.dwTimeViewFileCount);
	}

	dwTime = timeGetTime();
	tagMainWindow1.dwCurrentTime = dwTime - tagMainWindow1.dwStartTime;

#ifndef _DEBUG
	IF_UNLIKELY(tagMainWindow1.dwCurrentTime < 500)
	{
		stnIsView = 0;
		return;
	}
#endif
	stnIsView = stnIsView ? FALSE : TRUE;


	if (nIsView != FALSE || stnIsView != FALSE)
	{
		__int64 llSum = 0;
		unsigned int i = 0;

		llReadSize[0] = llFileAllCurrentSize + llFileReadSize;
		tagMainWindow1.dwEndTime = tagMainWindow1.dwCurrentTime;

		if (llReadSize[0] == 0)
		{
			llReadSize[1] = 0;
			return;
		}

		if (nIsView != FALSE)
		{
			double dTime;

			dwTime = tagMainWindow1.tagHashThread1.dwEndTime;
			dwTime -= tagMainWindow1.dwStartTime;
			dTime = ((double)dwTime / 1000.);
			if (!dTime) {
				dTime = 0.0001;
			}
			tagMainWindow1.llTimeView1_TransPerSec = (__int64)((double)(llFileAllSize) / dTime);
			STRFORMATBYTESIZE_MACRO(llFileAllSize, tagMainWindow1.pTimeView1_Size[0], NUMBER_LOADSTRING);
			StrFromTimeInterval(tagMainWindow1.pTimeView1_Time, NUMBER_LOADSTRING, tagMainWindow1.dwCurrentTime, 3);

			if (tagMainWindow1.llTimeView1_TransPerSec)
			{
				STRFORMATBYTESIZE_MACRO(tagMainWindow1.llTimeView1_TransPerSec, tagMainWindow1.pTimeView1_TransPerSec, NUMBER_LOADSTRING);
				MessageFormat(tagMainWindow1.pTimeText, MAX_LOADSTRING, tagMainWindow1.pTimeText1[1], tagMainWindow1.pTimeView1_Time, tagMainWindow1.pTimeView1_Size[0], tagMainWindow1.pTimeView1_TransPerSec);
				SetWindowText(tagMainWindow1.hStatic[1], tagMainWindow1.pTimeText);
			}

			tagMainWindow1.dwBackTime = 0;
			llReadSize[1] = 0;
			nAgvC = 0;
			nAgvN = 2;
			nProgress = 0;
		}
		else
		{
			__int64 llVal;

			if (tagMainWindow1.dwBackTime == 0)
			{
				tagMainWindow1.dwBackTime = tagMainWindow1.dwStartTime;
				llReadSize[1] = 0;
				nAgvC = 0;
				nAgvN = 1;
			}

			llVal = (__int64)((double)(llReadSize[0] - llReadSize[1]) / ((dwTime - tagMainWindow1.dwBackTime) / 1000.));
			if (!nAgvN) {
				llAvg[nAgvC] = llVal;
			}
			else
			{

#ifdef _DEBUG
				ptr = qtcscpy(szDebugText, _T("MainWindow: Hash_TimeView(): "));
				ptr = qtcscpy(ptr, _T("nAgvN = "));
				ITOT_FUNC(nAgvN, szNum, SIZEOF_NUM(szNum), 10);
				ptr = qtcscpy(ptr, szNum);
				ptr = qtcscpy(ptr, _T(";\r\n"));
				OutputDebugString(szDebugText);
#endif

				for (i = 0; i < SIZEOF_NUM(llAvg); i++) {
					llAvg[i] = llVal;
				}
				nAgvN = 0;
			}

			llSum = 0;
			for (i = 0; i < SIZEOF_NUM(llAvg); i++)	{
				llSum += llAvg[i];
			}
			tagMainWindow1.llTimeView1_TransPerSec = llSum / SIZEOF_NUM(llAvg);

			STRFORMATBYTESIZE_MACRO(tagMainWindow1.llTimeView1_TransPerSec, tagMainWindow1.pTimeView1_TransPerSec, NUMBER_LOADSTRING);
			STRFORMATBYTESIZE_MACRO(tagMainWindow1.tagHashThread1.tagMultiFile->llFileAllSize, tagMainWindow1.pTimeView1_Size[0], NUMBER_LOADSTRING);
			STRFORMATBYTESIZE_MACRO(tagMainWindow1.tagHashThread1.tagMultiFile->llFileAllCurrentSize + tagMainWindow1.tagHashThread1.llFileReadSize, tagMainWindow1.pTimeView1_Size[1], NUMBER_LOADSTRING);

			llVal = llFileAllSize - (llFileAllCurrentSize + llFileReadSize);
			if (tagMainWindow1.llTimeView1_TransPerSec != 0) {
				llVal = (DWORD)ceil((double)llVal / (double)tagMainWindow1.llTimeView1_TransPerSec);
			}
			tagMainWindow1.dwTimeView1_Time = (DWORD)llVal;

			StrFromTimeInterval(tagMainWindow1.pTimeView1_Time, NUMBER_LOADSTRING, tagMainWindow1.dwTimeView1_Time * 1000, 3);
			MessageFormat(tagMainWindow1.pTimeText, MAX_LOADSTRING, tagMainWindow1.pTimeText1[0], tagMainWindow1.pTimeView1_Time, tagMainWindow1.pTimeView1_Size[0], tagMainWindow1.pTimeView1_Size[1], tagMainWindow1.pTimeView1_TransPerSec);
			SetWindowText(tagMainWindow1.hStatic[1], tagMainWindow1.pTimeText);
			tagMainWindow1.dwBackTime = dwTime;
			llReadSize[1] = llReadSize[0];
		}

#ifdef _DEBUG
		ptr = qtcscpy(szDebugText, _T("MainWindow: Hash_TimeView(): "));
		ptr = qtcscpy(ptr, _T("llAvg = "));
		I64TOT_FUNC(tagMainWindow1.llTimeView1_TransPerSec, szNum, SIZEOF_NUM(szNum), 10);
		ptr = qtcscpy(ptr, szNum);
		ptr = qtcscpy(ptr, _T(" ("));
		I64TOT_FUNC(llSum, szNum, SIZEOF_NUM(szNum), 10);
		ptr = qtcscpy(ptr, szNum);
		ptr = qtcscpy(ptr, _T("); "));

		for (i = 0; i < SIZEOF_NUM(llAvg); i++)
		{
			ptr = qtcscpy(ptr, _T("["));

			if (i == nAgvC) {
				*ptr++ = '*';
			}

			ITOT_FUNC(i, szNum, SIZEOF_NUM(szNum), 10);
			ptr = qtcscpy(ptr, szNum);
			ptr = qtcscpy(ptr, _T("]"));
			I64TOT_FUNC(llAvg[i], szNum, SIZEOF_NUM(szNum), 10);
			ptr = qtcscpy(ptr, szNum);
			ptr = qtcscpy(ptr, _T(", "));
		}

		if (*(ptr - 2) == ',') {
			ptr -= 2;
		}

		ptr = qtcscpy(ptr, _T(";\r\n"));
		OutputDebugString(szDebugText);
#endif

		nAgvC++;
		nAgvC = nAgvC & (SIZEOF_NUM(llAvg) - 1);
	}
	return;
}

VOID Hash_HashComp(DWORD dwItem)
{
	static DWORD dwOpenItem = (DWORD)-1;
	UINT uiType = 0;
	int nBitmap = 0;
	int nStatusText = 0;

	if (*tagMainWindow1.pHashString == '\0' || *tagMainWindow1.pHashCompString == '\0') {
		return;
	}

	dwAppFrag |= APP_WINDOW_NOALPHA;
	if (dwItem == dwOpenItem) {
		FixString(tagMainWindow1.pHashCompString);
	}

	if (_tcsicmp(tagMainWindow1.pHashCompString, tagMainWindow1.pHashString))
	{
		nBitmap = STATUS_ICON_ERROR;
		nStatusText = 6;
		uiType = MB_ICONERROR;
	}
	else
	{
		nBitmap = STATUS_ICON_CHECK;
		nStatusText = 5;
		uiType = MB_ICONASTERISK;
	}
	SendMessage(tagMainWindow1.hBmp[0], STM_SETIMAGE, IMAGE_ICON, (LPARAM)tagMainWindow1.hStatusIcon[nBitmap]);
	StatusOut(tagMainWindow1.pStatusText[nStatusText]);
	MessageFormat(tagMainWindow1.pBuf, 2048, tagMainWindow1.pWindowTitle[0], tagMainWindow1.pStatusText[nStatusText], tagMainWindow1.pTitle);
	SetWindowText(tagMainWindow1.hWnd, tagMainWindow1.pBuf);
	MessageBeep(uiType);
	dwOpenItem = dwItem;
	dwAppFrag &= ~APP_WINDOW_NOALPHA;
	return;
}

VOID Hash_FileView(DWORD dwItem)
{
	const tagHashThread_FileRecode* cptagHashFile_Recode = HashThread_GetFileRecode_Core(&tagMainWindow1.tagHashThread1, dwItem);
	const DWORD dwHashSize = cdwHashLen[cptagHashFile_Recode->dwFileHashType];
	TCHAR* pBase64 = NULL;
	TCHAR* ptr = NULL;
	int nRet = 0;

	IF_UNLIKELY(dwAppFrag & APP_PAUSE || dwItem > HashThread_GetCountItem_Core(&tagMainWindow1.tagHashThread1))
	{
		//MessageBeep(MB_ICONERROR);
		return;
	}

	if (tagMainWindow1.tagHashThread1.tagMultiFile->dwFileCount == 0)
	{
		SetWindowText(tagMainWindow1.hWnd, tagMainWindow1.pTitle);
		tagMainWindow1.dwOpenItem = 0;
		tagMainWindow1.tagHashThread1.tagMultiFile->ptagFileRecode->dwFileLastError = (DWORD)-1;
		SendMessage(tagMainWindow1.hBmp[0], STM_SETIMAGE, IMAGE_ICON, (LPARAM)tagMainWindow1.hStatusIcon[STATUS_ICON_DEFAULT]);
		StatusOut(tagMainWindow1.pStatusText[7]);
		LoadString(tagMainWindow1.hInst, IDS_STATIC_TEXT2, tagMainWindow1.pStBuf, MAX_STRINGTABLE - 1);
		SetWindowText(tagMainWindow1.hStatic[1], tagMainWindow1.pStBuf);
		SetWindowText(tagMainWindow1.hEdit[1], _T(""));
		EnableMenuItem(tagMainWindow1.hPupMenu[0][0], ID_MAINSUBMENU1_HASH, MF_GRAYED);
		goto NO_FILE_STEP;
	}
	else if (tagMainWindow1.tagHashThread1.dwHashType != cptagHashFile_Recode->dwFileHashType)
	{
		SendMessage(tagMainWindow1.hBmp[0], STM_SETIMAGE, IMAGE_ICON, (LPARAM)tagMainWindow1.hStatusIcon[STATUS_ICON_WARNING]);
		StatusOut(tagMainWindow1.pStatusText[9]);
		LoadString(tagMainWindow1.hInst, IDS_STATIC_TEXT5, tagMainWindow1.pStBuf, MAX_STRINGTABLE - 1);
		SetWindowText(tagMainWindow1.hStatic[1], tagMainWindow1.pStBuf);
		SetWindowText(tagMainWindow1.hEdit[0], _T(""));
		goto HASHCHANGE_FILE_STEP;
	}

	if (cptagHashFile_Recode->dwFileLastError != 0)
	{
		// Visual C++ 6.0 対策: ラベルから変数初期化ができない
		if (1)
		{
			const DWORD dwLastError = cptagHashFile_Recode->dwFileLastError;
			int nArg = 9;

			switch (dwLastError)
			{
			case (DWORD)-1:
				if (dwAppFrag & APP_CALCEL) {
					nArg = 3;
				}
				else if (dwAppFrag & APP_HASHFILE_ADDMODE)
				{
					nArg = 8;
					LoadString(tagMainWindow1.hInst, IDS_STATIC_TEXT4, tagMainWindow1.pStBuf, MAX_STRINGTABLE - 1);
					SetWindowText(tagMainWindow1.hStatic[1], tagMainWindow1.pStBuf);
				}
				else if (dwAppFrag & APP_HASHTHREAD && tagMainWindow1.tagHashThread1.tagMultiFile->dwFileCurrentCount == dwItem) {
					nArg = 0;
				}
				StatusOut(tagMainWindow1.pStatusText[nArg]);
				break;
			default:
				SendMessage(tagMainWindow1.hBmp[0], STM_SETIMAGE, IMAGE_ICON, (LPARAM)tagMainWindow1.hStatusIcon[STATUS_ICON_WARNING]);
				MessageFormat(tagMainWindow1.pBuf, 2048, tagMainWindow1.pStatusText[4], (DWORD)dwLastError);
				StatusOut(tagMainWindow1.pBuf);
				MessageFormat(tagMainWindow1.pBuf, 2048, tagMainWindow1.pWindowTitle[2], (DWORD)dwLastError, tagMainWindow1.pTitle);
				SetWindowText(tagMainWindow1.hWnd, tagMainWindow1.pBuf);
				GetLastErrorMsg(dwLastError);
			}
		}
NO_FILE_STEP:
		;
HASHCHANGE_FILE_STEP:
		EnableWindow(tagMainWindow1.hEdit[0], FALSE);
		EnableWindow(tagMainWindow1.hEdit[1], FALSE);
		EnableWindow(tagMainWindow1.hEdit[2], FALSE);
		EnableWindow(tagMainWindow1.hButton[1], dwAppFrag & APP_HASHTHREAD ? TRUE : FALSE);
		EnableWindow(tagMainWindow1.hButton[2], FALSE);
		EnableWindow(tagMainWindow1.hButton[3], FALSE);
		EnableMenuItem(tagMainWindow1.hPupMenu[0][0], ID_MAINSUBMENU1_OPEN, MF_ENABLED);
		EnableMenuItem(tagMainWindow1.hPupMenu[0][0], ID_MAINSUBMENU1_COPY, MF_GRAYED);
		EnableMenuItem(tagMainWindow1.hPupMenu[0][0], ID_MAINSUBMENU1_SIMPLECOPY, MF_GRAYED);
		EnableMenuItem(tagMainWindow1.hPupMenu[0][0], ID_MAINSUBMENU1_PASTE, MF_GRAYED);
		EnableMenuItem(tagMainWindow1.hPupMenu[0][0], ID_MAINSUBMENU1_EXTCOPY4, MF_GRAYED);
		EnableMenuItem(tagMainWindow1.hPupMenu[0][0], ID_MAINSUBMENU1_EXTCOPY5, MF_GRAYED);
	}
	else
	{
		SendMessage(tagMainWindow1.hBmp[0], STM_SETIMAGE, IMAGE_ICON, (LPARAM)tagMainWindow1.hStatusIcon[STATUS_ICON_DEFAULT]);
		SetWindowText(tagMainWindow1.hWnd, tagMainWindow1.pTitle);
		if (dwAppFrag & APP_HASHFILE_ADDMODE)
		{
			StatusOut(tagMainWindow1.pStatusText[8]);
			LoadString(tagMainWindow1.hInst, IDS_STATIC_TEXT4, tagMainWindow1.pStBuf, MAX_STRINGTABLE - 1);
			SetWindowText(tagMainWindow1.hStatic[1], tagMainWindow1.pStBuf);
		}
		else
		{
			EnableWindow(tagMainWindow1.hButton[1], TRUE);
			EnableMenuItem(tagMainWindow1.hPupMenu[0][0], ID_MAINSUBMENU1_SAVE, MF_ENABLED);
			StatusOut(tagMainWindow1.pStatusText[1]);
			if (*tagMainWindow1.pTimeText != '\0') {
				SetWindowText(tagMainWindow1.hStatic[1], tagMainWindow1.pTimeText);
			}
		}
		nRet = cptagHashFile_Recode->szFileName[0] != '\0';
		EnableWindow(tagMainWindow1.hEdit[0], TRUE);
		EnableWindow(tagMainWindow1.hEdit[1], TRUE);
		EnableWindow(tagMainWindow1.hEdit[2], nRet);
		EnableWindow(tagMainWindow1.hButton[1], nRet);
		EnableWindow(tagMainWindow1.hButton[2], TRUE);
		EnableMenuItem(tagMainWindow1.hPupMenu[0][0], ID_MAINSUBMENU1_HASH, MF_ENABLED);
		EnableMenuItem(tagMainWindow1.hPupMenu[0][0], ID_MAINSUBMENU1_COPY, MF_ENABLED);
		EnableMenuItem(tagMainWindow1.hPupMenu[0][0], ID_MAINSUBMENU1_SIMPLECOPY, MF_ENABLED);
		EnableMenuItem(tagMainWindow1.hPupMenu[0][0], ID_MAINSUBMENU1_PASTE, MF_ENABLED);
		EnableMenuItem(tagMainWindow1.hPupMenu[0][0], ID_MAINSUBMENU1_EXTCOPY4, MF_ENABLED);
		EnableMenuItem(tagMainWindow1.hPupMenu[0][0], ID_MAINSUBMENU1_EXTCOPY5, MF_ENABLED);
	}
	Hash_SetFileInfo(dwItem);

	if (tagMainWindow1.tagHashThread1.dwHashType != cptagHashFile_Recode->dwFileHashType) {
		return;
	}

	if (cptagHashFile_Recode->dwFileLastError == 0)
	{
		pBase64 = tagMainWindow1.pBase64String;
		HashThread_GetString(tagMainWindow1.pHashString, cptagHashFile_Recode->pFileHashByte, cptagHashFile_Recode->dwFileHashType, ~dwAppFrag & APP_HASHOUT_LOWER, pBase64);

		if (dwAppFrag & APP_ENABLE_MULTILINE)
		{
			TCHAR* src = tagMainWindow1.pHashString;

			ptr = tagMainWindow1.pBuf;
			switch (dwHashSize)
			{
			case HASH_CRC32_LEN:
				memcpy(ptr, src, 8 * sizeof(TCHAR));
				ptr += 8;
				break;
			case HASH_XXH64_LEN:
				memcpy(ptr, src, 16 * sizeof(TCHAR));
				ptr += 16;
				break;
			case HASH_MD5_LEN:
				memcpy(ptr, src, 32 * sizeof(TCHAR));
				ptr += 32;
				break;
			case HASH_SHA1_LEN:
				memcpy(ptr, src, 32 * sizeof(TCHAR));
				ptr += 32;
				*ptr++ = '\r';
				*ptr++ = '\n';
				src += 32;
				memcpy(ptr, src, 8 * sizeof(TCHAR));
				ptr += 8;
				break;
			case HASH_SHA_256_192_LEN:
				memcpy(ptr, src, 32 * sizeof(TCHAR));
				ptr += 32;
				*ptr++ = '\r';
				*ptr++ = '\n';
				src += 32;
				memcpy(ptr, src, 16 * sizeof(TCHAR));
				ptr += 16;
				break;
			case HASH_SHA_224_LEN:
				memcpy(ptr, src, 32 * sizeof(TCHAR));
				ptr += 32;
				*ptr++ = '\r';
				*ptr++ = '\n';
				src += 32;
				memcpy(ptr, src, 24 * sizeof(TCHAR));
				ptr += 24;
				break;
			case HASH_SHA_256_LEN:
				memcpy(ptr, src, 32 * sizeof(TCHAR));
				ptr += 32;
				*ptr++ = '\r';
				*ptr++ = '\n';
				src += 32;
				memcpy(ptr, src, 32 * sizeof(TCHAR));
				ptr += 32;
				break;
			case HASH_SHA_384_LEN:
				memcpy(ptr, src, 32 * sizeof(TCHAR));
				ptr += 32;
				*ptr++ = '\r';
				*ptr++ = '\n';
				src += 32;
				memcpy(ptr, src, 32 * sizeof(TCHAR));
				ptr += 32;
				*ptr++ = '\r';
				*ptr++ = '\n';
				src += 32;
				memcpy(ptr, src, 32 * sizeof(TCHAR));
				ptr += 32;
				break;
			case HASH_SHA_512_LEN:
				memcpy(ptr, src, 32 * sizeof(TCHAR));
				ptr += 32;
				*ptr++ = '\r';
				*ptr++ = '\n';
				src += 32;
				memcpy(ptr, src, 32 * sizeof(TCHAR));
				ptr += 32;
				*ptr++ = '\r';
				*ptr++ = '\n';
				src += 32;
				memcpy(ptr, src, 32 * sizeof(TCHAR));
				ptr += 32;
				*ptr++ = '\r';
				*ptr++ = '\n';
				src += 32;
				memcpy(ptr, src, 32 * sizeof(TCHAR));
				ptr += 32;
				break;
			default: {
				const int n = (int)dwHashSize / 16 * 16;
				int i = 0;

				for (i = 0; i < n; i += 16)
				{
					memcpy(ptr, src, 32 * sizeof(TCHAR));
					ptr += 32;
					*ptr++ = '\r';
					*ptr++ = '\n';
					src += 32;
				}

				if ((size_t)dwHashSize - i)
				{
					size_t nSize = ((size_t)dwHashSize - i) * 2;

					memcpy(ptr, src, nSize * sizeof(TCHAR));
					ptr += nSize;
				}
			}}
			*ptr++ = '\0';
			ptr = tagMainWindow1.pBuf;
		}
		else {
			ptr = tagMainWindow1.pHashString;
		}
		SetWindowText(tagMainWindow1.hEdit[0], ptr);
	}
	else
	{
		*tagMainWindow1.pHashString = '\0';
		if (tagMainWindow1.tagHashThread1.tagMultiFile->dwFileCount != 0) {
			pBase64 = tagMainWindow1.pBase64_Msg[0];
		}
		else {
			pBase64 = (TCHAR*)_T("");
		}
	}

	SetWindowText(tagMainWindow1.hStatic[7], pBase64);
	SetToolTip(tagMainWindow1.hTool[0], tagMainWindow1.hStatic[7], pBase64);

#ifdef _DEBUG
	OutputDebugString(_T("MainWindow: Hash_FileView():\r\n"));
	OutputDebugString(tagMainWindow1.pHashString);
	OutputDebugString(_T("\r\n"));
#endif

	if (cptagHashFile_Recode->dwHashFileHashLine != HASHFILE_NOLINE && cptagHashFile_Recode->dwFileLastError == 0 && ~dwAppFrag & APP_HASHFILE_ADDMODE && cptagHashFile_Recode->dwHashFileHashType == cptagHashFile_Recode->dwFileHashType)
	{
		dwAppFrag |= APP_HASH_INPUT_EDIT;

		/*
		HashThread_HashToString(tagMainWindow1.pHashCompString, cptagHashFile_Recode->bHashFileHashByte, ~dwAppFrag & APP_HASHOUT_LOWER, cdwHashLen(cptagHashFile_Recode->dwFileHashType));
		*/

		HashThread_GetString(tagMainWindow1.pHashCompString, cptagHashFile_Recode->nHashFileHashByte, cptagHashFile_Recode->dwFileHashType, ~dwAppFrag & APP_HASHOUT_LOWER);
		SendMessage(tagMainWindow1.hEdit[1], WM_SETFONT, (WPARAM)tagMainWindow1.hFont[2], 0);
		SetWindowText(tagMainWindow1.hEdit[1], tagMainWindow1.pHashCompString);
	}
	else if(HashThread_GetCountItem_Core(&tagMainWindow1.tagHashThread1) > 0)
	{
		dwAppFrag &= ~APP_HASH_INPUT_EDIT;
		SendMessage(tagMainWindow1.hEdit[1], WM_SETFONT, (WPARAM)tagMainWindow1.hFont[1], 0);
		SetWindowText(tagMainWindow1.hEdit[1], tagMainWindow1.pInputtext);
	}
	return;
}

VOID StatusOut(const TCHAR* cpInString)
{
	MessageFormat(tagMainWindow1.pStBuf, 2048, tagMainWindow1.pStatusTitle, tagMainWindow1.tagHashThread1.cpHashName, cpInString);
	SetWindowText(tagMainWindow1.hStatic[0], tagMainWindow1.pStBuf);
	return;
}

BOOL OpenFileCancel(VOID)
{
	SendMessage(tagMainWindow1.hBmp[0], STM_SETIMAGE, IMAGE_ICON, (LPARAM)tagMainWindow1.hStatusIcon[STATUS_ICON_WARNING]);
	EnableWindow(tagMainWindow1.hEdit[1], FALSE);
	EnableWindow(tagMainWindow1.hEdit[2], FALSE);
	EnableWindow(tagMainWindow1.hButton[1], FALSE);
	EnableWindow(tagMainWindow1.hButton[2], FALSE);
	EnableWindow(tagMainWindow1.hButton[3], FALSE);
	DragAcceptFiles(tagMainWindow1.hWnd, TRUE);
	SetFocus(tagMainWindow1.hButton[0]);
	return TRUE;
}

BOOL GetHash(VOID)
{
	IF_UNLIKELY(tagMainWindow1.tagHashThread1.tagMultiFile->dwFileCount == 0)
	{
		LoadString(tagMainWindow1.hInst, IDS_FILE_OPEN1, tagMainWindow1.pStBuf, MAX_STRINGTABLE - 1);
		MessageFormat(tagMainWindow1.pBuf, 2048, tagMainWindow1.pStBuf, tagMainWindow1.pHashFile);
		MainWindow_MessageBox(tagMainWindow1.hWnd, tagMainWindow1.pBuf, tagMainWindow1.pTitle, MB_OK | MB_ICONERROR);
		return FALSE;
	}

	if ((tagMainWindow1.tagHashThread1.tagMultiFile->dwFileCurrentCount + 1) >= tagMainWindow1.tagHashThread1.tagMultiFile->dwFileCount) {
		tagMainWindow1.tagHashThread1.tagMultiFile->dwFileCurrentCount = tagMainWindow1.dwAddFileModeFileCount != 0 ? tagMainWindow1.dwAddFileModeFileCount : 0;
	}

	dwAppFrag |= APP_HASHTHREAD;

	tagMainWindow1.nRandamDataState = 0;
	tagMainWindow1.dwEndTime = 0;

	tagMainWindow1.tagHashThread1.hHashThreadHandle = (HANDLE)_beginthreadex(NULL, 0, (*HashThread_MultiFile_Thread_Func), &tagMainWindow1.tagHashThread1, 0, &tagMainWindow1.tagHashThread1.nThreadID);
	IF_UNLIKELY(tagMainWindow1.tagHashThread1.hHashThreadHandle == NULL)
	{
		dwAppFrag &= ~APP_HASHTHREAD;
		DebugText(DEBUG_MISSING, _T("Hashスレッドの作成"), GetLastError());
	}
	return TRUE;
}

// デバッグ用にテキストの出力する。
// 引数2の文字列は 100文字以内 にする。
VOID DebugText(DWORD dwType, const TCHAR* cpInString, DWORD dwLastError)
{
	static TCHAR szOutputBuf[1024] = _T("");
	static TCHAR szLastErrorBuf[256] = _T("");
	DWORD dwRet = 0;

	switch (dwType)
	{
	case DEBUG_ALLOC_ERROR:
#if _MSC_VER > 1300
		dwRet = (DWORD)_stprintf_s(szOutputBuf, _T("%sに失敗しました。\r\n%s"), _T("メモリの動的確保"), cpInString);
#else
		dwRet = (DWORD)_stprintf(szOutputBuf, _T("%sに失敗しました。\r\n%s"), _T("メモリの動的確保"), cpInString);
#endif
		break;
	case DEBUG_MISSING:
		IF_LIKELY(dwLastError != 0)
		{
			GetLastError_String(szLastErrorBuf, dwLastError);
			IF_UNLIKELY(szLastErrorBuf[0] == '\0') {
				qtcscpy(szLastErrorBuf, _T("内容が取得できませんでした。"));
			}

#if _MSC_VER > 1300
			dwRet = (DWORD)_stprintf_s(szOutputBuf, _T("%sに失敗しました。\r\nエラーの詳細(#%d): %s"), cpInString, dwLastError, szLastErrorBuf);
#else
			dwRet = (DWORD)_stprintf(szOutputBuf, _T("%sに失敗しました。\r\nエラーの詳細(#%d): %s"), cpInString, dwLastError, szLastErrorBuf);
#endif
		}
		else
		{
#if _MSC_VER > 1300
			dwRet = (DWORD)_stprintf_s(szOutputBuf, _T("%sに失敗しました。"), cpInString);
#else
			dwRet = (DWORD)_stprintf(szOutputBuf, _T("%sに失敗しました。"), cpInString);
#endif
		}

		IF_UNLIKELY(!dwRet) {
			dwRet = (DWORD)(qtcscpy(szOutputBuf, _T("デバッグ文字が表示できません。")) - szOutputBuf);
		}
		break;
	case DEBUG_FALSE_RETURN:
#if _MSC_VER > 1300
		dwRet = (DWORD)_stprintf_s(szOutputBuf, _T("%sが「FALSE」を返しました。"), cpInString);
#else
		dwRet = (DWORD)_stprintf(szOutputBuf, _T("%sが「FALSE」を返しました。"), cpInString);
#endif
		break;
	default:
		dwRet = (DWORD)(qtcscpy(szOutputBuf, cpInString) - szOutputBuf);
		break;
	}

	IF_UNLIKELY(szOutputBuf[0] == '\0') {
		dwRet = (DWORD)(qtcscpy(szOutputBuf, _T("メッセージはありません。")) - szOutputBuf);
	}

#ifdef _DEBUG
	qtcscpy(szOutputBuf + dwRet, _T("\r\n"));
	OutputDebugString(szOutputBuf);
#endif
	dwAppFrag |= APP_WINDOW_NOALPHA;

	MainWindow_MessageBox(tagMainWindow1.hWnd, szOutputBuf, tagMainWindow1.pTitle, MB_OK | MB_ICONERROR);

	dwAppFrag &= ~APP_WINDOW_NOALPHA;
}

DWORD GetCommDlgExtendedErrorMsg(DWORD dwMsg)
{
	DWORD dwRet = 0;

	dwRet = GetCommDlgExtendedError_String(tagMainWindow1.pBuf, dwMsg);
	IF_UNLIKELY(dwRet == 0) {
		return 0;
	}
	SetWindowText(tagMainWindow1.hStatic[1], tagMainWindow1.pBuf);

#ifdef _DEBUG
	OutputDebugString(_T("MainWindow: GetLastErrorMsg()\r\n"));
	OutputDebugString(tagMainWindow1.pBuf);
	OutputDebugString(_T("\r\n"));
#endif

	return dwMsg;
}

DWORD GetLastErrorMsg(DWORD dwMsg)
{
	const DWORD dwRet = GetLastError_String(tagMainWindow1.pBuf, dwMsg);
	IF_UNLIKELY(dwRet == FALSE) {
		return FALSE;
	}
	SetWindowText(tagMainWindow1.hStatic[1], tagMainWindow1.pBuf);

#ifdef _DEBUG
	OutputDebugString(_T("MainWindow: GetLastErrorMsg()\r\n"));
	OutputDebugString(tagMainWindow1.pBuf);
	OutputDebugString(_T("\r\n"));
#endif

	return dwMsg;
}

DWORD GetCommDlgExtendedError_String(TCHAR* cpInString, DWORD dwMsg)
{
	UNREFERENCED_PARAMETER(cpInString);

	if (dwMsg == 0) {
		dwMsg = CommDlgExtendedError();
	}

#if _MSC_VER > 1300
	_stprintf_s(cpInString, 64, _T("CommDlgExtendedError(): 0x%8X"), dwMsg);
#else
	_stprintf(cpInString, _T("CommDlgExtendedError(): 0x%8X"), dwMsg);
#endif

	/*
	switch (dwMsg)
	{
	case 0xFFFF://CDERR_DIALOGFAILURE
		qtcscpy(cpInString, _T("ダイアログボックスが作成できませんでした。"));
		break;
	case 0x0006://CDERR_FINDRESFAILURE
		qtcscpy(cpInString, _T("リソースが見つかりません。"));
		break;
	case 0x0002://CDERR_INITIALIZATION
		qtcscpy(cpInString, _T("メモリ不足などの問題で、初期化に失敗しました。"));
		break;
	case 0x0007://CDERR_LOADRESFAILURE
		qtcscpy(cpInString, _T("リソースの読み込みに失敗しました。"));
		break;
	case 0x0005://CDERR_LOADSTRFAILURE
		qtcscpy(cpInString, _T("文字列の読み込みに失敗しました。"));
		break;
	case 0x0008://CDERR_LOCKRESFAILURE
		qtcscpy(cpInString, _T("リソースのロックに失敗しました。"));
		break;
	case 0x0009://CDERR_MEMALLOCFAILURE
		qtcscpy(cpInString, _T("内部の構造体のメモリ確保に失敗しました。"));
		break;
	case 0x000a://CDERR_MEMLOCKFAILURE
		qtcscpy(cpInString, _T("ハンドルに関連付けられているメモリのロックに失敗しました。"));
		break;
	case 0x0004://CDERR_NOHINSTANCE
		qtcscpy(cpInString, _T("初期化構造体の Flags メンバに ENABLETEMPLATE フラグが設定されていますが、対応するインスタンスハンドルが正しく指定されませんでした。"));
		break;
	case 0x000b://CDERR_NOHOOK
		qtcscpy(cpInString, _T("初期化構造体の Flags メンバに ENABLEHOOK フラグが設定されていますが、対応するフックプロシージャへのポインタが正しく指定されませんでした。"));
		break;
	case 0x0003://CDERR_NOTEMPLATE
		qtcscpy(cpInString, _T("初期化構造体の Flags メンバに ENABLETEMPLATE フラグが設定されていますが、対応するテンプレートが正しく指定されませんでした。"));
		break;
	case 0x000c://CDERR_REGISTERMSGFAIL
		qtcscpy(cpInString, _T("コモンダイアログボックス関数に呼び出された RegisterWindowMessage 関数が、エラー コードを返しました。"));
		break;
	case 0x0001://CDERR_STRUCTSIZE
		qtcscpy(cpInString, _T("初期化構造体の lStructSize メンバが無効です。"));
		break;
	// PrintDlg function.
	case 0x100A://PDERR_CREATEICFAILURE
		qtcscpy(cpInString, _T("情報コンテキストの作成に失敗しました。"));
		break;
	case 0x100C://PDERR_DEFAULTDIFFERENT
		qtcscpy(cpInString, _T("DEVNAMES 構造体の wDefault メンバに DN_DEFAULTPRN フラグが設定されていますが、他の構造体のメンバが指定するプリンタと現在のデフォルトのプリンタが一致しませんでした。"));
		break;
	case 0x1009://PDERR_DNDMMISMATCH
		qtcscpy(cpInString, _T("DEVMODE 構造体と DEVNAMES 構造体が、異なったプリンタを指定しています。"));
		break;
	case 0x1005://PDERR_GETDEVMODEFAIL
		qtcscpy(cpInString, _T("プリンタドライバが、DEVMODE 構造体を初期化できませんでした（Ver3.0 以降の Windows 用プリンタ ドライバのみ）。"));
		break;
	case 0x1006://PDERR_INITFAILURE
		qtcscpy(cpInString, _T("初期化中に失敗しましたが、失敗を解説する特定の拡張エラーコードがありません。"));
		break;
	case 0x1004://PDERR_LOADDRVFAILURE
		qtcscpy(cpInString, _T("指定されたプリンタのデバイスドライバのロードに失敗しました。"));
		break;
	case 0x1008://PDERR_NODEFAULTPRN
		qtcscpy(cpInString, _T("デフォルトのプリンタが存在しません。"));
		break;
	case 0x1007://PDERR_NODEVICES
		qtcscpy(cpInString, _T("プリンタドライバが見つかりません。"));
		break;
	case 0x1002://PDERR_PARSEFAILURE
		qtcscpy(cpInString, _T("WIN.INI ファイルの [devices] セクション内の文字列の解析に失敗しました。"));
		break;
	case 0x100B://PDERR_PRINTERNOTFOUND
		qtcscpy(cpInString, _T("WIN.INI ファイルの [devices] セクションが、要求されたプリンタのエントリを持っていませんでした。"));
		break;
	case 0x1003://PDERR_RETDEFFAILURE
		qtcscpy(cpInString, _T("PRINTDLG 構造体の Flags メンバに PD_RETURNDEFAULT フラグが指定されていますが、hDevMode メンバまたは hDevNames メンバが NULL ではありませんでした。"));
		break;
	case 0x1001://PDERR_SETUPFAILURE
		qtcscpy(cpInString, _T("リソースのロードに失敗しました。"));
		break;
	// ChooseFont function
	case 0x2001://CFERR_NOFONTS
		qtcscpy(cpInString, _T("フォントが存在しません。"));
		break;
	// GetOpenFileName and GetSaveFileName functions.
	case 0x3003://FNERR_BUFFERTOOSMALL
		qtcscpy(cpInString, _T("OPENFILENAME 構造体の lpstrFile メンバが指すバッファのサイズが小さすぎて、ユーザーが指定したファイル名がを格納することができませんでした。lpstrFile バッファの最初の 2 バイトに、ファイル名全体を格納するために必要なバイト数を格納します。"));
		break;
	case 0x3002://FNERR_INVALIDFILENAME
		qtcscpy(cpInString, _T("ファイル名が無効です。"));
		break;
	case 0x3001://FNERR_SUBCLASSFAILURE
		qtcscpy(cpInString, _T("メモリ不足のため、リストボックスをサブクラス化できませんでした。"));
		break;
	// FindText and ReplaceText functions.
	case 0x4001://FRERR_BUFFERLENGTHZERO
		qtcscpy(cpInString, _T("FINDREPLACE 構造体のメンバが、無効なバッファを指しています。"));
		break;
	default:
		*cpInString = '\0';
		break;
	}*/

	return dwMsg;
}

DWORD GetLastError_String(TCHAR* pInString, DWORD dwMsg)
{
	TCHAR szBuf[MAX_STRINGTABLE] = { 0 };

	IF_UNLIKELY(dwMsg == (DWORD)-1) {
		return FALSE;
	}

	IF_LIKELY(dwMsg == 0) {
		dwMsg = GetLastError();
	}

	const DWORD dwRet = FormatMessage(FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS, NULL, dwMsg, MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT), szBuf, MAX_STRINGTABLE, NULL);
	IF_LIKELY(dwRet > 0)
	{
		TCHAR* dst = pInString;
		int i = 0;

		for (i = 0; i < SIZEOF_NUM(szBuf); i++)
		{
			if (szBuf[i] == '\r') {
				i += 2;
			}
			*dst++ = szBuf[i];
		}
	}
	return dwMsg;
}

BOOL SetLayeredWindow(HWND hWnd)
{
	int nRet = FALSE;

#if _MSC_VER < 1500
	if (IsWin2kOrGreater())
	{
#endif /* _MSC_VER < 1500 */
		LONG lStyle = 0;

		lStyle = GetWindowLong(hWnd, GWL_EXSTYLE);
		lStyle |= WS_EX_LAYERED;
		nRet = (BOOL)SetWindowLong(hWnd, GWL_EXSTYLE, lStyle);
#if _MSC_VER < 1500
	}
#endif /* _MSC_VER < 1500 */
	return nRet;
}

DWORD MessageFormat(TCHAR* pBuffer, const DWORD cdwSize, const TCHAR* cpFormat, ...)
{
	va_list	vaList = NULL;

	va_start(vaList, cpFormat);
	const DWORD dwRet = FormatMessage(FORMAT_MESSAGE_FROM_STRING, cpFormat, 0, MAKELANGID(LANG_NEUTRAL, SUBLANG_SYS_DEFAULT), pBuffer, cdwSize, &vaList);
	va_end(vaList);
	return dwRet;
}

DWORD FixString(TCHAR* pString)
{
	TCHAR* pBuf = NULL;// コピーバッファ
	TCHAR* pTempBuf = NULL;// 作業バッファ
	TCHAR* pSrc = NULL;
	TCHAR* pDst = NULL;
	TCHAR* pRet = NULL;
	DWORD dwHashType = (DWORD)-1;
	size_t nBufSize = 0;

	IF_UNLIKELY(!pString) {
		return (DWORD)-2;
	}
	nBufSize = _tcslen(pString);

#ifdef _UNICODE

	pSrc = pString;
	pBuf = (WCHAR*)malloc(((nBufSize + 1) * 2) * sizeof(WCHAR));
	IF_UNLIKELY(pBuf == NULL) {
		return (DWORD)-2;
	}
	pDst = pBuf;
	pTempBuf = pBuf + nBufSize;
	*pBuf = '\0';
	*pTempBuf = '\0';

	// 全角英数字があれば半角に変換しながら作業バッファにコピーする
	while (*pSrc != NULL)
	{
		// 全角英数字の半角変換
		if (*pSrc >= 0xFF10 && _istalnum(*pSrc - 0xFEE0)) {
			*pDst++ = (WCHAR)towlower(*pSrc - 0xFEE0);
		}
		else if (_istalnum(*pSrc)) {
			*pDst++ = (WCHAR)towlower(*pSrc);
		}
		else {
			*pDst++ = ' ';
		}
		pSrc++;
	}
	*pDst = '\0';

#else

	pSrc = pString;
	pBuf = (char*)malloc((nBufSize + 1) * 2 * sizeof(char));
	if (pBuf == NULL) {
		return(DWORD) - 2;
	}
	pDst = pBuf;
	pTempBuf = pBuf + nBufSize;
	*pBuf = '\0';
	*pTempBuf = '\0';

	for (unsigned char* pUSrc = (unsigned char*)pString; *pUSrc != NULL; pUSrc++)
	{
		// シフトJISか判定
		if (_ismbblead(*pUSrc))
		{
			pUSrc++;
			// 全角英文字の半角変換
			if (*pUSrc >= 0x4f && _istalpha(*pUSrc - 0x1F)) {
				*pDst++ = (unsigned char)tolower(*pUSrc - 0x1F);
			}
			// 全角数字の半角変換
			else if (*pUSrc >= 0x81 && _istdigit(*pUSrc - 0x20)) {
				*pDst++ = (unsigned char)tolower(*pUSrc - 0x20);
			}
			else {
				*pDst++ = ' ';
			}
		}
		else
		{
			if (_istalnum(*pSrc)) {
				*pDst++ = (unsigned char)tolower(*pUSrc);
			}
			else {
				*pDst++ = ' ';
			}
		}
	}
	*pDst = '\0';

#endif

	// Hashの文字列があるか探す
	if ((pRet = _tcsstr(pBuf, _T("md"))) != NULL)
	{
		pSrc = pRet;
		pSrc += 2;

		switch (*pSrc)
		{
		case '2':
			dwHashType = HASH_MD2_TYPE;
			break;
		case '4':
			dwHashType = HASH_MD4_TYPE;
			break;
		case '5':
			dwHashType = HASH_MD5_TYPE;
			break;
		}
	}
	else if ((pRet= _tcsstr(pBuf, _T("sha"))) != NULL)
	{
		pSrc = pRet;
		pSrc += 3;

		if (*pSrc == ' ' || *pSrc == '-' || *pSrc == '_') {
			pSrc++;
		}

		switch (*pSrc)
		{
		case '1':
			pSrc++;
			dwHashType = HASH_SHA1_TYPE;
			break;
		case '2':
			if ((pRet = _tcsstr(pBuf, _T("224"))) != NULL)
			{
				pSrc = pRet;
				pSrc += 3;
				dwHashType = HASH_SHA_224_TYPE;
			}
			else if ((pRet = _tcsstr(pBuf, _T("256"))) != NULL)
			{
				pSrc = pRet;
				pSrc += 3;

				if (*pSrc == '-' || *pSrc == '_')
				{
					pSrc++;
					dwHashType = HASH_SHA_256_192_TYPE;
				}
				else {
					dwHashType = HASH_SHA_256_TYPE;
				}
			}
			break;
		case '3':
			pSrc++;
			if (*pSrc == '-' || *pSrc == '_')
			{
				pSrc++;

				if ((pRet = _tcsstr(pBuf, _T("224"))) != NULL)
				{
					pSrc = pRet;
					pSrc += 3;
					dwHashType = HASH_SHA3_224_TYPE;
				}
				else if ((pRet = _tcsstr(pBuf, _T("256"))) != NULL)
				{
					pSrc = pRet;
					pSrc += 3;
					dwHashType = HASH_SHA3_256_TYPE;
				}
				else if ((pRet = _tcsstr(pBuf, _T("384"))) != NULL)
				{
					pSrc = pRet;
					pSrc += 3;
					dwHashType = HASH_SHA3_384_TYPE;
				}
				else if ((pRet = _tcsstr(pBuf, _T("512"))) != NULL)
				{
					pSrc = pRet;
					pSrc += 3;
					dwHashType = HASH_SHA3_512_TYPE;
				}
			}
			else
			{
				pSrc--;
				if ((pRet = _tcsstr(pBuf, _T("384"))) != NULL)
				{
					pSrc = pRet;
					pSrc += 2;
					dwHashType = HASH_SHA_384_TYPE;
				}
			}
			break;
		case '5':
			if ((pRet = _tcsstr(pBuf, _T("512"))) != NULL)
			{
				pSrc = pRet;
				pSrc += 3;

				if (*pSrc == '-' || *pSrc == '_')
				{
					pSrc++;

					if ((pRet = _tcsstr(pBuf, _T("224"))) != NULL)
					{
						pSrc = pRet;
						pSrc += 3;
						dwHashType = HASH_SHA_512_224_TYPE;
					}
					else if ((pRet = _tcsstr(pBuf, _T("256"))) != NULL)
					{
						pSrc = pRet;
						pSrc += 3;
						dwHashType = HASH_SHA_512_256_TYPE;
					}
				}
				else {
					dwHashType = HASH_SHA_512_TYPE;
				}
			}
			break;
		}
	}
	else if ((pRet = _tcsstr(pBuf, _T("crc"))) != NULL)
	{
		pSrc = pRet;
		pSrc += 4;

		if ((pRet = _tcsstr(pBuf, _T("32c"))) != NULL)
		{
			pSrc = pRet;
			pSrc += 4;
			dwHashType = HASH_CRC32C_TYPE;
		}
		else if ((pRet = _tcsstr(pBuf, _T("32"))) != NULL)
		{
			pSrc = pRet;
			pSrc += 3;
			dwHashType = HASH_CRC32_TYPE;
		}
	}
	else if ((pRet = _tcsstr(pBuf, _T("xxh"))) != NULL)
	{
		pSrc = pRet;
		pSrc += 4;

		if ((pRet = _tcsstr(pBuf, _T("128"))) != NULL)
		{
			pSrc = pRet;
			pSrc += 4;
			dwHashType = HASH_XXH128_TYPE;
		}
		else if ((pRet = _tcsstr(pBuf, _T("32"))) != NULL)
		{
			pSrc = pRet;
			pSrc += 3;
			dwHashType = HASH_XXH32_TYPE;
		}
		else if ((pRet = _tcsstr(pBuf, _T("3"))) != NULL)
		{
			pSrc = pRet;
			pSrc += 2;
			dwHashType = HASH_XXH3_TYPE;
		}
		else if ((pRet = _tcsstr(pBuf, _T("64"))) != NULL)
		{
			pSrc = pRet;
			pSrc += 3;
			dwHashType = HASH_XXH64_TYPE;
		}
	}

	for (; *pSrc != NULL; pSrc++)
	{
		if (_istxdigit(*pSrc)) {
			break;
		}
	}

	if (dwHashType == (DWORD) - 1)
	{
		const size_t nBuflen2 = _tcslen(pBuf);

		pSrc = pBuf;
		switch (nBuflen2)
		{
		case HASH_CRC32_LEN * 2:
			if (!(tagMainWindow1.tagHashThread1.dwHashType == HASH_CRC32_TYPE || tagMainWindow1.tagHashThread1.dwHashType == HASH_CRC32C_TYPE || tagMainWindow1.tagHashThread1.dwHashType == HASH_XXH32_TYPE)) {
				dwHashType = HASH_CRC32_TYPE;
			}
			break;
		case HASH_XXH64_LEN * 2:
			dwHashType = HASH_XXH64_TYPE;
			break;
		case HASH_MD5_LEN * 2:
			if (!(tagMainWindow1.tagHashThread1.dwHashType == HASH_MD5_TYPE || tagMainWindow1.tagHashThread1.dwHashType == HASH_XXH64_TYPE || tagMainWindow1.tagHashThread1.dwHashType == HASH_XXH3_TYPE)) {
				dwHashType = HASH_MD5_TYPE;
			}
			break;
		case HASH_SHA1_LEN * 2:
			dwHashType = HASH_SHA1_TYPE;
			break;
		case HASH_SHA_256_192_LEN * 2:
			if (!(tagMainWindow1.tagHashThread1.dwHashType == HASH_SHA_256_192_TYPE)) {
				dwHashType = HASH_SHA_256_192_TYPE;
			}
			break;
		case HASH_SHA_224_LEN * 2:
			if (!(tagMainWindow1.tagHashThread1.dwHashType == HASH_SHA_224_TYPE || tagMainWindow1.tagHashThread1.dwHashType == HASH_SHA_512_224_TYPE || tagMainWindow1.tagHashThread1.dwHashType == HASH_SHA3_224_TYPE)) {
				dwHashType = HASH_SHA_224_TYPE;
			}
			break;
		case HASH_SHA_256_LEN * 2:
			if (!(tagMainWindow1.tagHashThread1.dwHashType == HASH_SHA_256_TYPE || tagMainWindow1.tagHashThread1.dwHashType == HASH_SHA_512_256_TYPE || tagMainWindow1.tagHashThread1.dwHashType == HASH_SHA3_256_TYPE)) {
				dwHashType = HASH_SHA_256_TYPE;
			}
			break;
		case HASH_SHA_384_LEN * 2:
			if (!(tagMainWindow1.tagHashThread1.dwHashType == HASH_SHA_384_TYPE || tagMainWindow1.tagHashThread1.dwHashType == HASH_SHA3_384_TYPE)) {
				dwHashType = HASH_SHA_384_TYPE;
			}
			break;
		case HASH_SHA_512_LEN * 2:
			if (!(tagMainWindow1.tagHashThread1.dwHashType == HASH_SHA_512_TYPE || tagMainWindow1.tagHashThread1.dwHashType == HASH_SHA3_512_TYPE)) {
				dwHashType = HASH_SHA_512_TYPE;
			}
			break;
		}
	}

	pDst = pString;
	if (dwAppFrag & APP_HASHOUT_LOWER)
	{
		size_t n = nBufSize;

		while (n > 0 && *pSrc != '\0')
		{
			--n;
			if (_istxdigit(*pSrc)) {
				*pDst++ = *pSrc;
			}
			pSrc++;
		}
	}
	else
	{
		size_t n = nBufSize;

		while (n > 0 && *pSrc != '\0')
		{
			--n;
			if (_istxdigit(*pSrc)) {
				*pDst++ = (TCHAR)toupper(*pSrc);
			}
			pSrc++;
		}
	}
	*pDst = '\0';
	free(pBuf);
	return dwHashType;
}

__int64 GetFileSizeEx(const TCHAR* cpInFile)
{
	TCHAR szBuf[MAX_PATH_SIZE] = _T("");
	WIN32_FIND_DATA fdFindData = { 0 };
	HANDLE hFind = NULL;
	__int64 llFileSize = 0;

	ExpandEnvironmentStrings(cpInFile, szBuf, MAX_PATH_SIZE);
	if (*szBuf == '\0') {
		qtcscpy(szBuf, cpInFile);
	}

	hFind = FindFirstFile(szBuf, &fdFindData);
	IF_LIKELY(hFind != INVALID_HANDLE_VALUE)
	{
		llFileSize = fdFindData.nFileSizeLow;
		if (fdFindData.nFileSizeHigh > 0) {
			llFileSize += (__int64)fdFindData.nFileSizeHigh << 32;
		}
		FindClose(hFind);
	}
	return llFileSize;
}

TCHAR* GetFileVersion(TCHAR* pOutVersionString, const TCHAR* cpInFileName, const TCHAR* cpInQueryValue)
{
	DWORD dwZero = 0;
	static DWORD dwVerInfoSize = GetFileVersionInfoSize(cpInFileName, &dwZero);
	TCHAR* pRet = NULL;

#define __VERINFOSIZE	0x069c

	IF_UNLIKELY(pOutVersionString == NULL || cpInFileName == NULL) {
		return NULL;
	}

	if (1)
	{
		static unsigned char ppvVffInfo[__VERINFOSIZE] = { 0 };
		VOID* pvVffInfo = ppvVffInfo;
		VOID* pvVersion = NULL;
		UINT uiVersionLen = 0;

		IF_UNLIKELY(dwVerInfoSize > __VERINFOSIZE)
		{

#ifdef _DEBUG
			TCHAR szDebugText[MAX_LOADSTRING];

			STPRINTF_FUNC(szDebugText, _T("%s: %s のメモリー確保できました。(0x%08p, %dbyte)\r\n"), _T("GetFileVersion()"), _T("pvVffInfo"), pvVffInfo, dwVerInfoSize);
			OutputDebugString(szDebugText);
#endif

			pvVffInfo = malloc(dwVerInfoSize);
			IF_UNLIKELY(pvVffInfo == NULL) {
				return NULL;
			}
		}

		GetFileVersionInfo(cpInFileName, NULL, dwVerInfoSize, pvVffInfo);
		if (cpInQueryValue == NULL)
		{
			VS_FIXEDFILEINFO* pFileInfo = NULL;

			IF_LIKELY(VerQueryValue(pvVffInfo, _T("\\"), (void**)&pFileInfo, &uiVersionLen))
			{
				TCHAR szNumber[33] = _T("");
				TCHAR* ptr = pOutVersionString;
				int nNumber = 0;

				nNumber = HIWORD(pFileInfo->dwFileVersionMS);
				_itot(nNumber, szNumber, 10);
				ptr = qtcscpy(ptr, szNumber);
				*ptr++ = '.';

				nNumber = LOWORD(pFileInfo->dwFileVersionMS);
				if (nNumber < 10) {
					*ptr++ = '0';
				}
				_itot(nNumber, szNumber, 10);
				ptr = qtcscpy(ptr, szNumber);
				*ptr++ = '.';

				nNumber = HIWORD(pFileInfo->dwFileVersionLS);
				if (nNumber < 10) {
					*ptr++ = '0';
				}
				_itot(nNumber, szNumber, 10);
				ptr = qtcscpy(ptr, szNumber);

				nNumber = LOWORD(pFileInfo->dwFileVersionLS);
				if (nNumber > 0)
				{
					*ptr++ = '.';
					_itot(nNumber, szNumber, 10);
					ptr = qtcscpy(ptr, szNumber);
				}
				pRet = ptr;
			}
			else {
				*pOutVersionString = '\0';
			}
		}
		else
		{
			IF_LIKELY(VerQueryValue(pvVffInfo, (TCHAR*)cpInQueryValue, &pvVersion, &uiVersionLen)) {
				pRet = qtcscpy(pOutVersionString, (TCHAR*)pvVersion);
			}
			else {
				*pOutVersionString = '\0';
			}
		}

		IF_UNLIKELY(dwVerInfoSize > __VERINFOSIZE) {
			free(pvVffInfo);
		}
	}
	return pRet;
}
