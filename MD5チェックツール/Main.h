#if _MSC_VER > 1000
#pragma once
#endif

#ifndef __MAIN_H_67D4B6EE_611F_4b63_88B8_6CD2E8B733EB__
#define __MAIN_H_67D4B6EE_611F_4b63_88B8_6CD2E8B733EB__

#if _MSC_VER > 1300
#include <intrin.h>
#endif

#include "..\shared\targetver.h"
#include <windows.h>
#include <math.h>
#include <time.h>
#include "hashthread.h"
#include "resource.h"

#ifndef MAX_PATH_SIZE
#ifdef _UNICODE
#define MAX_PATH_SIZE					(_MAX_PATH * 4)
#define MAX_PATH_SIZE_ALIGNMENT			(272 * 4)
#else
#define MAX_PATH_SIZE					(_MAX_PATH * 2)
#define MAX_PATH_SIZE_ALIGNMENT			(272 * 2)
#endif
#endif

#ifndef MAX_STRINGTABLE
#define MAX_STRINGTABLE					255
#define MAX_STRINGTABLE_ALIGNMENT		256
#endif

#ifndef MAX_LOADSTRING
#define MAX_LOADSTRING					100
#define MAX_LOADSTRING_ALIGNMENT		112
#endif

#ifndef NUMBER_LOADSTRING
#define NUMBER_LOADSTRING				33
#define NUMBER_LOADSTRING_ALIGNMENT		48
#endif

#define ACTIVE_ALPHA					255
#define INACTIVE_ALPHA					192

#define BENCH_FILESIZE					104857600

#define	MDIVX(X)	MulDiv(X, tagMainWindow1.dwbaseX, 4)				// ダイアログ単位からピクセル数への変換用
#define	MDIVY(Y)	MulDiv(Y, tagMainWindow1.dwbaseY, 8)

// アプリケーションフラグの設定します。
#define APP_WINDOW							0x00000001
#define APP_WINDOW_NOALPHA					0x00000002
#define APP_HASHTHREAD						0x00000004
#define APP_HASH_INPUT_EDIT					0x00000008
#define APP_PAUSE							0x00000010
#define APP_CALCEL							0x00000020
#define APP_FOLDERINIT						0x00000040
#define APP_FOLDEROPEN						0x00000080
#define APP_OLDHASHFILE						0x00000100
#define APP_ENABLE_DEBUG					0x00000200
#define APP_ENABLE_MINIMAM_HANDLE			0x00000400
#define APP_HASHFILE_ADDMODE				0x00000800
#define APP_FILE_CLEAR						0x00001000
#define APP_ARG_HASHTYPE					0x00002000
#define APP_ARG_FILELIST					0x00004000
#define APP_HASHOUT_LOWER					0x00008000
#define APP_WINDOWALPHA						0x00010000
#define APP_HASHFILE_ENABLE_HASH			0x00020000
#define APP_ENABLE_FILELIST					0x00040000
#define APP_FILE_NOCACHE					0x00080000
#define APP_DISABLE_MULTIFILE_UPDATE_DRAW	0x00100000
#define APP_ENABLE_HIDDENFILE				0x00200000
#define APP_ENABLE_HASHFILE_WRITETIME		0x00400000
#define APP_ENABLE_LARGE_BUFFERSIZE			0x00800000
#define APP_EDIT_CHECKSUMFILE				0x01000000
#define APP_DISABLE_HASH_NOASM				0x02000000
#define APP_ENABLE_TOPMOST					0x04000000
#define APP_FILELISTBOX_EDIT				0x08000000
#define APP_ENABLE_MULTILINE				0x10000000
#define APP_NO_SETTING						0x20000000
#define APP_ECOQOS							0x40000000
#define APP_LASTERROR						0x80000000

// ウインドウメッセージ
#define APP_MESSAGE_CHANGE_FILEVIEW			0x00100000
#define APP_MESSAGE_HASHFILE_OPEN			0x00200000
#define APP_MESSAGE_WINDOWSET				0x00400000
#define APP_MASSAGE_HASHCHANGE				0x00800000
#define APP_MASSAGE_DPICHANGE				0x01000000
#define APP_MESSAGE_FILECLEAR				0x00010000

#define DEBUG_PLAIN_TEXT    				0				// そのまま文字列を出力する。
#define DEBUG_ALLOC_ERROR 					1				// 動的メモリの確保に失敗しましたの文字列を出力する。
#define DEBUG_MISSING     					2				// ○○に失敗しましたの文字列を出力する。
#define DEBUG_FALSE_RETURN					3				// 関数が FALSE 返しました。

enum
{
	STATUS_ICON0 = 0,
	STATUS_ICON1,
	STATUS_ICON2,
	STATUS_ICON3,
	STATUS_ICON4,
	STATUS_ICON5,
	STATUS_ICON6,
};

enum
{
	STATUS_ICON_APP = 0,
	STATUS_ICON_DEFAULT,
	STATUS_ICON_WARNING,
	STATUS_ICON_PLAY,
	STATUS_ICON_CHECK,
	STATUS_ICON_ERROR,
	STATUS_ICON_UPDATE,
};

enum
{
	TASKBAR_ICON_CHECK = 1,
	TASKBAR_ICON_ERROR,
	TASKBAR_ICON_WARNING,
	TASKBAR_ICON_UPDATE
};


typedef struct tagMainWindow {
	DWORD			dwStructSize;			// バージョンアップ用に予約
	HWND			hWnd;
	HINSTANCE		hInst;					// 現在のインターフェイス
	HINSTANCE		hDllInst;
	unsigned char*	szIconRes_Load;
	HashThread		tagHashThread1;
	UINT			nTaskbarButtonMessage;
	HRESULT			hr3;
	HBRUSH			hBrush;
	HWND			hNextWnd;
	HANDLE			hBenchHandle;
	HWND			hSendWindow;
	HWND			hStatic[8];
	HWND			hBmp[1];
	HWND			hProgress[1];
	HWND			hEdit[3];
	HWND			hButton[9];
	HWND			hGroup[1];
	HWND			hTool[1];
	HMENU			hMenu;
	HMENU			hPupMenu[2][3];
	HICON			hIcon[5];
	HICON			hStatusIcon[6];
	HFONT			hFont[4];
	OPENFILENAME	ofnFileName[2];
	ITASKBARLIST3*	pTaskbarList3;
	TCHAR*			pBuf;
	TCHAR*			pStBuf;
	TCHAR*			pTempBuf;
	TCHAR*			pTitle;				// タイトル バーのテキスト
	TCHAR*			pWindowClass;			// メイン ウィンドウ クラス名
	TCHAR*			pVersion;
	TCHAR*			pCopyRight;
	TCHAR*			pStatusTitle;
	const TCHAR*	cpHashName;
	TCHAR			pHashString[HASH_LOADSTRING];
	TCHAR			pBase64String[HASH_LOADSTRING];
	TCHAR			pHashCompString[HASH_LOADSTRING];
	TCHAR*			pFileNameEdit;
	TCHAR*			pClipboardString;
	TCHAR*			pInputtext;
	TCHAR*			pFile;
	TCHAR*			pHashFile;
	TCHAR*			pINIFile;
	TCHAR*			pFileExtension;
	TCHAR*			ppCheckSumFile[256];
	TCHAR*			pCheckSumFile;
	TCHAR*			pNotFoundFile;
	TCHAR*			pNextNotFoundFile;//追記用のポインター
	TCHAR*			pEmptyFolder;
	TCHAR*			pNextEmptyFolder;//追記用のポインター
	TCHAR*			pHashNotFoundFile;
	TCHAR*			pNextHashNotFoundFile;//追記用のポインター
	TCHAR*			pTimeText;
	TCHAR*			pTimeView1_TransPerSec;
	TCHAR*			pTimeView1_Size[2];
	TCHAR*			pTimeView1_Time;
	TCHAR*			pOfn1Buf;
	TCHAR*			pWindowTitle[3];
	TCHAR*			pStatusText[10];
	TCHAR*			pButtonText1[3];
	TCHAR*			pButtonText2[2];
	TCHAR*			pToolTipText[9];
	TCHAR*			pTimeText1[2];
	TCHAR*			pBase64_Msg[2];
	TCHAR*			pBecthResult;
	TCHAR*			pCmpErrorCount;
	__int64			llTimeView1_TransPerSec;
	DWORD			dwTimeView1_Time;
	DWORD			dwOpenItem;
	DWORD			dwReOpenItem;
	DWORD			dwReOpenMode;
	DWORD			dwStartTime;
	DWORD			dwCurrentTime;
	DWORD			dwEndTime;
	DWORD			dwBackTime;
	DWORD			dwPersent;
	DWORD			dwTimeViewFileCount;
	DWORD			dwLasterror;
	DWORD			dwSaveHashFileCharCode;	// 0.Shift_JIS　1.Unicode　2.UTF-8
	DWORD			dwHashTypeBack;
	size_t			nCmpCount;
	size_t			nCmpErrorCount;
	size_t			nCmpErrorCountSize;
	BOOL			nIsSubFolder;
	BOOL			bIsNotFoundFile;
	BOOL			bIsEmptyFolder;
	BOOL			bIsHashNotFoundFile;
	BOOL			bIsWindowCancel;
	//BOOL			nIsFileNameEdit;
	DWORD			dwAddFileModeFileCount;
	COLORREF		clGrayTextColor;
	size_t			nDpi[2];
	DWORD			dwbaseX;
	DWORD			dwbaseY;
	DWORD			dwWindowRectX;
	DWORD			dwWindowRectY;
	//DWORD			dwDialogRect;
	double			dDpiSize;
	BOOL			nIsWindowSet;
	LONG			nLeftPos;
	LONG			nTopPos;
	time_t			nTime;
	int				nRandamDataState;
} MainWindow, *lpMainWindow;

#define WINDOW_LOCK if(tagFileListWindow1.hWnd) \
EnableWindow(tagFileListWindow1.hWnd, FALSE); \
if (tagAboutWindow1.hWnd) \
EnableWindow(tagAboutWindow1.hWnd, FALSE)

#define WINDOW_UNLOCK if(tagFileListWindow1.hWnd) \
EnableWindow(tagFileListWindow1.hWnd, TRUE); \
if (tagAboutWindow1.hWnd) \
EnableWindow(tagAboutWindow1.hWnd, TRUE)

#ifdef _DEBUG
#define BENCH_LOOP 25000
#else
#define BENCH_LOOP 500000
#endif


// このコード モジュールに含まれる関数の宣言を転送します:
ATOM				MyRegisterClass(HINSTANCE hInstance);
BOOL				InitInstance(int);
LRESULT CALLBACK	WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);

LRESULT MainWindow_OnInitDialog(HWND hWnd, WPARAM wParam, LPARAM lParam);
LRESULT MainWindow_OnSize(HWND hWnd, WPARAM wParam, LPARAM lParam);
LRESULT MainWindow_OnCommand(HWND hWnd, WPARAM wParam, LPARAM lParam);
LRESULT MainWindow_OnRButtonDown(HWND hWnd, WPARAM wParam, LPARAM lParam);
LRESULT MainWindow_OnSysColorChange(HWND hWnd, WPARAM wParam, LPARAM lParam);
LRESULT MainWindow_OnCtlColorEdit(HWND hWnd, WPARAM wParam, LPARAM lParam);
LRESULT MainWindow_OnDpiChanged(HWND hWnd, WPARAM wParam, LPARAM lParam);
LRESULT MainWindow_OnDropFiles(HWND hWnd, WPARAM wParam, LPARAM lParam);
LRESULT MainWindow_OnGetMinMaxInfo(HWND hWnd, WPARAM wParam, LPARAM lParam);
LRESULT MainWindow_OnMove(HWND hWnd, WPARAM wParam, LPARAM lParam);
LRESULT MainWindow_OnTimer(HWND hWnd, WPARAM wParam, LPARAM lParam);
LRESULT MainWindow_OnActivate(HWND hWnd, WPARAM wParam, LPARAM lParam);
LRESULT MainWindow_OnDrawClipboard(HWND hWnd, WPARAM wParam, LPARAM lParam);
LRESULT MainWindow_OnChangeCBChain(HWND hWnd, WPARAM wParam, LPARAM lParam);
LRESULT MainWindow_OnThemeChanged(HWND hWnd, WPARAM wParam, LPARAM lParam);
LRESULT MainWindow_OnClose(HWND hWnd, WPARAM wParam, LPARAM lParam);
LRESULT MainWindow_OnDestroy(HWND hWnd, WPARAM wParam, LPARAM lParam);

LRESULT MainWindow_OnMessageFileChange(HWND hWnd, WPARAM wParam, LPARAM lParam);
LRESULT MainWindow_OnMessageHashComp(HWND hWnd, WPARAM wParam, LPARAM lParam);
LRESULT MainWindow_OnMessageSubFolderInit(HWND hWnd, WPARAM wParam, LPARAM lParam);
LRESULT MainWindow_OnMessageHashFileInit(HWND hWnd, WPARAM wParam, LPARAM lParam);
LRESULT MainWindow_OnMessageHashFileExit(HWND hWnd, WPARAM wParam, LPARAM lParam);
LRESULT MainWindow_OnMessageFileView(HWND hWnd, WPARAM wParam, LPARAM lParam);
LRESULT MainWindow_OnMessageLastError(HWND hWnd, WPARAM wParam, LPARAM lParam);
LRESULT MainWindow_OnMessageHashFileChange(HWND hWnd, WPARAM wParam, LPARAM lParam);
LRESULT MainWindow_OnMessageWindowSet(HWND hWnd, WPARAM wParam, LPARAM lParam);
LRESULT MainWindow_OnMessageInit(HWND hWnd, WPARAM wParam, LPARAM lParam);
LRESULT MainWindow_OnMessageStart(HWND hWnd, WPARAM wParam, LPARAM lParam);
LRESULT MainWindow_OnMessageEnd(HWND hWnd, WPARAM wParam, LPARAM lParam);
LRESULT MainWindow_OnMessageError(HWND hWnd, WPARAM wParam, LPARAM lParam);
LRESULT MainWindow_OnMessageFolderInit(HWND hWnd, WPARAM wParam, LPARAM lParam);
LRESULT MainWindow_OnMessageFolderExit(HWND hWnd, WPARAM wParam, LPARAM lParam);
LRESULT MainWindow_OnMessagePause(HWND hWnd, WPARAM wParam, LPARAM lParam);
LRESULT MainWindow_OnMessageProgress(HWND hWnd, WPARAM wParam, LPARAM lParam);
LRESULT MainWindow_OnMessageClipboardCopy(HWND hWnd, WPARAM wParam, LPARAM lParam);
LRESULT MainWindow_OnMessageClipboardPaste(HWND hWnd, WPARAM wParam, LPARAM lParam);
LRESULT MainWindow_OnMessageClear(HWND hWnd, WPARAM wParam, LPARAM lParam);
LRESULT MainWindow_OnMessageRename(HWND hWnd, WPARAM wParam, LPARAM lParam);
LRESULT MainWindow_OnMessageBench(HWND hWnd, WPARAM wParam, LPARAM lParam);
LRESULT MainWindow_OnMessageBench2(HWND hWnd, WPARAM wParam, LPARAM lParam);
LRESULT MainWindow_OnMessageEdit1(HWND hWnd, WPARAM wParam, LPARAM lParam);
LRESULT MainWindow_Edit1_OnSetFocus(HWND hWnd, WPARAM wParam, LPARAM lParam);
LRESULT MainWindow_Edit1_OnKillFocus(HWND hWnd, WPARAM wParam, LPARAM lParam);
LRESULT MainWindow_OnMessageEdit2(HWND hWnd, WPARAM wParam, LPARAM lParam);
LRESULT MainWindow_Edit2_OnChange(HWND hWnd, WPARAM wParam, LPARAM lPara);
LRESULT MainWindow_Edit2_OnSetFocus(HWND hWnd, WPARAM wParam, LPARAM lParam);
LRESULT MainWindow_Edit2_OnKillFocus(HWND hWnd, WPARAM wParam, LPARAM lParam);
LRESULT MainWindow_OnMessageEdit3(HWND hWnd, WPARAM wParam, LPARAM lParam);
LRESULT MainWindow_Edit3_OnSetFocus(HWND hWnd, WPARAM wParam, LPARAM lParam);
LRESULT MainWindow_Edit3_OnKillFocus(HWND hWnd, WPARAM wParam, LPARAM lParam);
LRESULT MainWindow_OnMessagePreviousFile(HWND hWnd, WPARAM wParam, LPARAM lParam);
LRESULT MainWindow_OnMessageNextFile(HWND hWnd, WPARAM wParam, LPARAM lParam);
LRESULT MainWindow_OnMessageReOpen(HWND hWnd, WPARAM wParam, LPARAM lParam);
LRESULT MainWindow_OnMessageFileAssoc(HWND hWnd, WPARAM wParam, LPARAM lParam);


int		MainWindow_MessageBox(HWND hWnd, LPCTSTR lpText, LPCTSTR lpCaption, UINT uType);
unsigned __stdcall Bench(void* lpThread);
DWORD	ReadMeOpen(VOID);
DWORD	VersionCheck(VOID);
BOOL	GetHash(VOID);
BOOL	OpenFile(VOID);
BOOL	OpenFileCancel(VOID);
BOOL	SaveFile(VOID);
BOOL	SetLayeredWindow(HWND hWnd);
BOOL	GetHashFilePath(VOID);
VOID	Hash_GetDpi(DWORD dwDpiX, DWORD dwDpiY);
VOID	Hash_HashChange(DWORD dwHashType);
BOOL	Hash_SetHashType(BOOL nIsLeftPos);
BOOL	Hash_SetWindowMove(BOOL nIsLeftPos);
VOID	Hash_FileView(DWORD dwItem);
VOID	Hash_HashComp(DWORD dwItem);
VOID	Hash_TimeView(BOOL nIsView);
BOOL	Hash_ChangeMsg(DWORD dwHashType);
BOOL	Hash_FileAssoc();
VOID	DebugText(DWORD dwType, const TCHAR* inString, DWORD dwLastError);
VOID	StatusOut(const TCHAR* inString);
DWORD	FixString(TCHAR* lpString);
VOID	ClipboardCopy(const TCHAR* inString), Paste(VOID);
VOID	ClipboardPaste(VOID);
VOID	Open(BOOL nIsPause);
VOID	Save(VOID);
BOOL	HashFile_Save(VOID);
VOID	ReOpen(VOID);
VOID	Cancel(BOOL nIsClear);
VOID	Clear(BOOL nIsSendMessage);
VOID	Pause(VOID);
DWORD	HashFile_FileOpen(BOOL nIsAdd);
BOOL	ClipboardCheck(DWORD dwItem);
VOID	Hash_SetFileInfo(DWORD dwItem);
DWORD	GetSubFolderCount(const TCHAR* inFile);
VOID	MainWindow_AddFile_Start(VOID);
DWORD	MainWindow_AddFile(size_t nCount);
VOID	MainWindow_AddFile_End(BOOL IsMD5Thread = TRUE);
__int64	GetFileSizeEx(const TCHAR* inFile);
DWORD	GetCommDlgExtendedErrorMsg(DWORD dwMsg);
DWORD	GetCommDlgExtendedError_String(TCHAR* inString, DWORD dwMsg = 0);
DWORD	GetLastErrorMsg(DWORD dwMsg);
DWORD	GetLastError_String(TCHAR* inString, DWORD dwMsg = 0);
DWORD	MessageFormat(TCHAR* lpBuffer, const DWORD nSize, const TCHAR* lpszFormat, ...);
TCHAR* GetFileVersion(TCHAR* pOutVersionString, const TCHAR* cpInFileName, const TCHAR* cpInQueryValue = NULL);

#endif
