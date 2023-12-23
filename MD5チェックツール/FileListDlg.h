#if _MSC_VER > 1000
#pragma once
#endif

#ifndef __FILELISTDLG_H_B496E73F_D09C_4c6b_8D82_BC24C3691B1E__
#define __FILELISTDLG_H_B496E73F_D09C_4c6b_8D82_BC24C3691B1E__

#define APP_FILEDLG_FOLDER_OPEN		0x00000001
#define APP_FILEDLG_FOLDER_EXIT		0x00000002

typedef struct tagListItem {
	DWORD dwListSelectCount;
	DWORD dwListBufSize;
	DWORD *dwListSelectItem;
} FileListItem, *lpFileListItem;

typedef struct tagFileListWindow {
	DWORD dwStructSize; // バージョンアップ用に予約
	HWND hWnd;
	UINT nWindowMessage;
	HWND hList;
	HWND hStatus;
	HWND hGroup[1];
	HWND hButton[4];
	HWND hRadio[3];
	HMENU hMenu;
	HMENU hPupMenu[2];
	TCHAR *pBuf;
	TCHAR *pStBuf;
	TCHAR *pFileChangeHash;
	TCHAR *pButtonText1[3];
	TCHAR *pButtonText2[2];
	DWORD dwNewFile;
	DWORD dwFileCount;
	DWORD dwOpenOption;
	DWORD dwDragListNo;
	DWORD dwMoveListNo;
	DWORD dwHashCheckCount[3];
	BOOL nIsHashFileCompMassege;
	COLORREF clListTextColor[3];
	DWORD dwStatus1_Height;
	int nScroll_Width;
	BOOL nIsSubFolder;
	BOOL nIsListLock;
	tagListItem tagFileListItem1;
	DWORD dwDialogRect;
	HIMAGELIST hListViewImg;
	HICON hListViewIcon[4];
} FileListWindow, *lpFileListWindow;

#define FILELIST_WINDOW_LOCK EnableWindow(tagMainWindow1.hWnd, FALSE); \
if (tagAboutWindow1.hWnd) \
EnableWindow(tagAboutWindow1.hWnd, FALSE)

#define FILELIST_WINDOW_UNLOCK EnableWindow(tagMainWindow1.hWnd, TRUE); \
if (tagAboutWindow1.hWnd) \
EnableWindow(tagAboutWindow1.hWnd, TRUE)

// このコード モジュールに含まれる関数の宣言を転送します:
LRESULT CALLBACK FileListDlg_WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);
LRESULT CALLBACK FileListDlg_SortProc(LPARAM, LPARAM, LPARAM);

LRESULT FileListDlg_OnInitDialog(HWND hWnd, WPARAM wParam, LPARAM lParam);
LRESULT FileListDlg_OnCommand(HWND hWnd, WPARAM wParam, LPARAM lParam);
LRESULT FileListDlg_OnSize(HWND hWnd, WPARAM wParam, LPARAM lParam);
LRESULT FileListDlg_OnItemChanged(HWND hWnd, WPARAM wParam, LPARAM lParam);
LRESULT FileListDlg_OnClick(HWND hWnd, WPARAM wParam, LPARAM lParam);
LRESULT FileListDlg_OnKeyDown(HWND hWnd, WPARAM wParam, LPARAM lParam);
LRESULT FileListDlg_OnColumNClick(HWND hWnd, WPARAM wParam, LPARAM lParam);
LRESULT FileListDlg_OnBeginDrag(HWND hWnd, WPARAM wParam, LPARAM lParam);
LRESULT FileListDlg_OnCustomDraw(HWND hWnd, WPARAM wParam, LPARAM lParam);
LRESULT FileListDlg_OnDropFiles(HWND hWnd, WPARAM wParam, LPARAM lParam);
LRESULT FileListDlg_OnGetMinMaxInfo(HWND hWnd, WPARAM wParam, LPARAM lParam);
LRESULT FileListDlg_OnNotify(HWND hWnd, WPARAM wParam, LPARAM lParam);
LRESULT FileListDlg_OnMouseMove(HWND hWnd, WPARAM wParam, LPARAM lParam);
LRESULT FileListDlg_OnLButtonUp(HWND hWnd, WPARAM wParam, LPARAM lParam);
LRESULT FileListDlg_OnSysColorChange(HWND hWnd, WPARAM wParam, LPARAM lParam);
LRESULT FileListDlg_OnThemeChanged(HWND hWnd, WPARAM wParam, LPARAM lParam);
LRESULT FileListDlg_OnClose(HWND hWnd, WPARAM wParam, LPARAM lParam);
LRESULT FileListDlg_OnDestroy(HWND hWnd, WPARAM wParam, LPARAM lParam);

LRESULT FileListDlg_OnMessageHashFileOpen(HWND hWnd, WPARAM wParam, LPARAM lParam);
LRESULT FileListDlg_OnMessageFolderInit(HWND hWnd, WPARAM wParam, LPARAM lParam);
LRESULT FileListDlg_OnMessageSubFolderInit(HWND hWnd, WPARAM wParam, LPARAM lParam);
LRESULT FileListDlg_OnMessageFolderExit(HWND hWnd, WPARAM wParam, LPARAM lParam);
LRESULT FileListDlg_OnMessageHashFileInit(HWND hWnd, WPARAM wParam, LPARAM lParam);
LRESULT FileListDlg_OnMessageHashFileExit(HWND hWnd, WPARAM wParam, LPARAM lParam);
LRESULT FileListDlg_OnMessageInit(HWND hWnd, WPARAM wParam, LPARAM lParam);
LRESULT FileListDlg_OnMessageFileChange(HWND hWnd, WPARAM wParam, LPARAM lParam);
LRESULT FileListDlg_OnMessageFileExit(HWND hWnd, WPARAM wParam, LPARAM lParam);
LRESULT FileListDlg_OnMessageClear(HWND hWnd, WPARAM wParam, LPARAM lParam);
LRESULT FileListDlg_OnMessageEnd(HWND hWnd, WPARAM wParam, LPARAM lParam);
LRESULT FileListDlg_OnMessageError(HWND hWnd, WPARAM wParam, LPARAM lParam);
LRESULT FileListDlg_OnMessagePause(HWND hWnd, WPARAM wParam, LPARAM lParam);
LRESULT FileListDlg_OnMessageWindowSet(HWND hWnd, WPARAM wParam, LPARAM lParam);
LRESULT FileListDlg_OnMessageHashChange(HWND hWnd, WPARAM wParam, LPARAM lParam);
LRESULT FileListDlg_OnMessageHashFileChange(HWND hWnd, WPARAM wParam, LPARAM lParam);
LRESULT FileListDlg_OnMessageDpiChange(HWND hWnd, WPARAM wParam, LPARAM lParam);
LRESULT FileListDlg_OnMessageClipboardCopy(HWND hWnd, WPARAM wParam, LPARAM lParam);
LRESULT FileListDlg_DpiChangeMain(HWND hWnd, WPARAM wParam, LPARAM lParam);

int FileListDlg_MessageBox(HWND hWnd, LPCTSTR lpText, LPCTSTR lpCaption, UINT uType);
VOID FileListDlg(VOID);
VOID FileListDlg_Init(VOID);
VOID FileListDlg_ListView(DWORD dwStart, BOOL nIsDelete);
VOID FileListDlg_SetListView(DWORD dwItem, BOOL nIsAdd);
VOID FileListDlg_Clear(BOOL nIsFilePathClear, BOOL nIsClearFunc);
VOID FileListDlg_FileOpen(VOID);
VOID FileListDlg_FolderOpen(VOID);
VOID FileListDlg_HashFileOpen(VOID);
VOID FileListDlg_MenuShow(HWND hWnd, HMENU hMenu, POINT *pt);
VOID FileListDlg_FileMenu(HWND hWnd, DWORD dwItem, POINT *pt);
VOID FileListDlg_NoFileMenu(HWND hWnd, POINT *pt);
VOID FileListDlg_FileDelete(VOID);
VOID FileListDlg_GetDpi(VOID);
DWORD FileListDlg_AddFile(VOID);
DWORD FileListDlg_AddFolder(VOID);
DWORD FileListDlg_AddHashFile(VOID);
DWORD FileListDlg_DropFile(HDROP hdropFile);
VOID FileListDlg_SetHashType(VOID);
VOID FileListDlg_SetWindowMove(BOOL nIsDefaultMove = FALSE);
VOID FileRecodeFail(HWND hWnd);

#endif
