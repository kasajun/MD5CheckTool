// FileListDlg.cpp : アプリケーションのエントリ ポイントを定義します。
//

#include "stdafx.h"
#include "..\shared\WindowSize.h"


// グローバル変数の定義します:
FileListWindow tagFileListWindow1;

// 外部ファイルの変数定義します。
extern MainWindow tagMainWindow1;
extern AboutWindow tagAboutWindow1;
extern DWORD dwAppFrag;
extern DWORD dwAppState;
extern DWORD dwHashFileOutToCopyFolderMode;
extern int nEditMarginX[4];
extern int nEditMarginY[4];
extern int nStringX[4];
extern int nStringY[4];

#define FILELISTDLG_X 378
#define FILELISTDLG_Y 193

static HIMAGELIST hDragImage;
static SHELLEXECUTEINFO	ShellInfo;

static TCHAR* p;
static TCHAR szBuf[4096 +						// szFldBuf[4096]
	MAX_STRINGTABLE_ALIGNMENT +					// szFldStBuf[256]
	MAX_PATH_SIZE_ALIGNMENT +					// szFileChangeHash[274 * 4]
	(NUMBER_LOADSTRING_ALIGNMENT * (SIZEOF_NUM(tagFileListWindow1.pButtonText1))) +	// szButtonText1[][48]
	(NUMBER_LOADSTRING_ALIGNMENT * (SIZEOF_NUM(tagFileListWindow1.pButtonText2)))	// szButtonText2[][48]];
	] = _T("");
static TCHAR szBuf2[1240];
static TCHAR szBuf3[1240];
static TCHAR szListViewColumn[3][32];
static int nListFileSizeWidth;
static int nFileSortSubNo[4];
static int nHashStringWidth[MAX_HASH_TYPE + 1] = {
	235, 295, 403, 451, 379,
	641, 833, 425, 473, 407,
	449, 647, 839,  94, 150,
	252, 233, 233,   0 };

#ifdef _DEBUG
	DWORD dwTime1;
	DWORD dwTime2;
	TCHAR szDebugText[1024];
#endif


static int ListView_HashStringWidth(HWND hList, DWORD dwHashType)
{
	static TCHAR szHash[] = _T("bbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbb");
	const DWORD dwHashLen = HashThread_GetHashLen(dwHashType);
	TCHAR* ptr = NULL;
	size_t* aligned_dst = (size_t*)szHash;

	ptr = qtcscpy(szHash, HashThread_GetHashName(dwHashType));
	*ptr++ = ':';
	*ptr++ = ' ';
	ptr += (size_t)dwHashLen * 2;
	*ptr = '\0';
	const int nRet = ListView_GetStringWidth(hList, szHash);
	*ptr = 'b';

#ifdef UNICODE
#if SIZE_MAX == 0xffffffff
	*aligned_dst++ = MAKE_WCHAR_TO_INT_2CHAR('b', 'b');
	*aligned_dst++ = MAKE_WCHAR_TO_INT_2CHAR('b', 'b');
	*aligned_dst++ = MAKE_WCHAR_TO_INT_2CHAR('b', 'b');
	*aligned_dst++ = MAKE_WCHAR_TO_INT_2CHAR('b', 'b');
	*aligned_dst++ = MAKE_WCHAR_TO_INT_2CHAR('b', 'b');
	*aligned_dst++ = MAKE_WCHAR_TO_INT_2CHAR('b', 'b');
	*aligned_dst++ = MAKE_WCHAR_TO_INT_2CHAR('b', 'b');
	*aligned_dst++ = MAKE_WCHAR_TO_INT_2CHAR('b', 'b');
#else
	*aligned_dst++ = MAKE_WCHAR_TO_INT_4CHAR('b', 'b', 'b', 'b');
	*aligned_dst++ = MAKE_WCHAR_TO_INT_4CHAR('b', 'b', 'b', 'b');
	*aligned_dst++ = MAKE_WCHAR_TO_INT_4CHAR('b', 'b', 'b', 'b');
	*aligned_dst++ = MAKE_WCHAR_TO_INT_4CHAR('b', 'b', 'b', 'b');
#endif
#else
	memset(aligned_dst, 'b', 16);
#endif

	return nRet + 14;
}

static int ListView_AllHashStringWidth(HWND hList)
{
	int i = 0;

#ifdef _DEBUG
	OutputDebugString(_T("FileListDlg: ListView_AllHashStringWidth()\r\n"));
#endif

	for (i = 0; i < MAX_HASH_TYPE; i++)
	{
		nHashStringWidth[i] = ListView_HashStringWidth(hList, i);

#ifdef _DEBUG
		STPRINTF_FUNC(szDebugText,
			_T("nHashStringWidth[%d] = %d;// [%s]\r\n"),
			i,
			nHashStringWidth[i],
			HashThread_GetHashName(i));
		OutputDebugString(szDebugText);
#endif

	}
	return i;
}

// ファイルリストのメッセージ ハンドラです。
LRESULT CALLBACK FileListDlg_WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	if (message == tagFileListWindow1.nWindowMessage)
	{
		switch (wParam)
		{
		case APP_MESSAGE_HASHFILE_OPEN:
			return FileListDlg_OnMessageHashFileOpen(hWnd, wParam, lParam);
		case APP_MESSAGE_FOLDER_INIT:
			return FileListDlg_OnMessageFolderInit(hWnd, wParam, lParam);
		case APP_MESSAGE_SUBFOLDER_OPEN:
			return FileListDlg_OnMessageSubFolderInit(hWnd, wParam, lParam);
		case APP_MESSAGE_FOLDER_INIT_EXIT:
			return FileListDlg_OnMessageFolderExit(hWnd, wParam, lParam);
		case APP_MESSAGE_HASHFILE_INIT:
			return FileListDlg_OnMessageHashFileInit(hWnd, wParam, lParam);
		case APP_MESSAGE_HASHFILE_INIT_EXIT:
			return FileListDlg_OnMessageHashFileExit(hWnd, wParam, lParam);
		case APP_MESSAGE_INIT:
			return FileListDlg_OnMessageInit(hWnd, wParam, lParam);
		case APP_MESSAGE_FILE_CHANGE:
			return FileListDlg_OnMessageFileChange(hWnd, wParam, lParam);
		case APP_MESSAGE_FILE_EXIT:
			return FileListDlg_OnMessageFileExit(hWnd, wParam, lParam);
		case APP_MESSAGE_FILECLEAR:
			return FileListDlg_OnMessageClear(hWnd, wParam, lParam);
		case APP_MESSAGE_END:
			return FileListDlg_OnMessageEnd(hWnd, wParam, lParam);
		case APP_MESSAGE_ERROR:
			return FileListDlg_OnMessageError(hWnd, wParam, lParam);
		case APP_MESSAGE_PAUSE:
			return FileListDlg_OnMessagePause(hWnd, wParam, lParam);
		case APP_MESSAGE_WINDOWSET:
			return FileListDlg_OnMessageWindowSet(hWnd, wParam, lParam);
		case APP_MESSAGE_HASHFILE_HASHCHANGE:
			return FileListDlg_OnMessageHashFileChange(hWnd, wParam, lParam);
		case APP_MASSAGE_HASHCHANGE:
			return FileListDlg_OnMessageHashChange(hWnd, wParam, lParam);
		case APP_MASSAGE_DPICHANGE:
			return FileListDlg_OnMessageDpiChange(hWnd, wParam, lParam);
		}
		return FALSE;
	}

	switch (message)
	{
	case WM_INITDIALOG:
		return FileListDlg_OnInitDialog(hWnd, wParam, lParam);
	case WM_COMMAND:
		return FileListDlg_OnCommand(hWnd, wParam, lParam);
	case WM_DROPFILES:
		return FileListDlg_OnDropFiles(hWnd, wParam, lParam);
	case WM_GETMINMAXINFO:
		return FileListDlg_OnGetMinMaxInfo(hWnd, wParam, lParam);
	case WM_SIZE:
		return FileListDlg_OnSize(hWnd, wParam, lParam);
	case WM_NOTIFY:
		return FileListDlg_OnNotify(hWnd, wParam, lParam);
	case WM_MOUSEMOVE:
		return FileListDlg_OnMouseMove(hWnd, wParam, lParam);
	case WM_LBUTTONUP:
		return FileListDlg_OnLButtonUp(hWnd, wParam, lParam);
	case WM_SYSCOLORCHANGE:
		return FileListDlg_OnSysColorChange(hWnd, wParam, lParam);
	case WM_THEMECHANGED:
		return FileListDlg_OnThemeChanged(hWnd, wParam, lParam);
	case WM_CLOSE:
		return FileListDlg_OnClose(hWnd, wParam, lParam);
	case WM_DESTROY:
		return FileListDlg_OnDestroy(hWnd, wParam, lParam);
	}
	return FALSE;
}

LRESULT FileListDlg_OnInitDialog(HWND hWnd, WPARAM wParam, LPARAM lParam)
{
	UNREFERENCED_PARAMETER(wParam);
	UNREFERENCED_PARAMETER(lParam);
	LV_COLUMN lvColumn = { 0 };
	RECT rc = { 0 };
	LONG_PTR lpStyle = 0;
	int nRet = 0;
	int i = 0;

	dwAppFrag |= APP_WINDOW_NOALPHA;

	tagFileListWindow1.dwStructSize = sizeof(tagFileListWindow1);
	tagFileListWindow1.hWnd = hWnd;
	tagFileListWindow1.pBuf = szBuf;
	*tagFileListWindow1.pBuf = '\0';
	tagFileListWindow1.pStBuf = 2048 + tagFileListWindow1.pBuf;
	*tagFileListWindow1.pStBuf = '\0';
	tagFileListWindow1.pFileChangeHash = MAX_STRINGTABLE_ALIGNMENT + tagFileListWindow1.pStBuf;
	*tagFileListWindow1.pFileChangeHash = '\0';

	if (~dwAppFrag & APP_ENABLE_MINIMAM_HANDLE)
	{
		SetEnableThemeDialogTexture(hWnd);
		SendMessage(hWnd, WM_SETICON, ICON_SMALL, (LPARAM)tagMainWindow1.hIcon[0]);
	}

	tagFileListWindow1.pButtonText1[0] = MAX_PATH_SIZE_ALIGNMENT + tagFileListWindow1.pFileChangeHash;
	LoadString(tagMainWindow1.hInst, IDS_FILELIST_BUTTON1_TEXT1, tagFileListWindow1.pButtonText1[0], NUMBER_LOADSTRING);
	for (i = 1; i < SIZEOF_NUM(tagFileListWindow1.pButtonText1); i++)
	{
		tagFileListWindow1.pButtonText1[i] = NUMBER_LOADSTRING_ALIGNMENT + tagFileListWindow1.pButtonText1[i - 1];
		LoadString(tagMainWindow1.hInst, IDS_FILELIST_BUTTON1_TEXT1 + i, tagFileListWindow1.pButtonText1[i], NUMBER_LOADSTRING);
	}

	tagFileListWindow1.pButtonText2[0] = NUMBER_LOADSTRING_ALIGNMENT + tagFileListWindow1.pButtonText1[SIZEOF_NUM(tagFileListWindow1.pButtonText1) - 1];
	LoadString(tagMainWindow1.hInst, IDS_FILELIST_BUTTON2_TEXT1, tagFileListWindow1.pButtonText2[0], NUMBER_LOADSTRING);
	for (i = 1; i < SIZEOF_NUM(tagFileListWindow1.pButtonText2); i++)
	{
		tagFileListWindow1.pButtonText2[i] = NUMBER_LOADSTRING_ALIGNMENT + tagFileListWindow1.pButtonText2[i - 1];
		LoadString(tagMainWindow1.hInst, IDS_FILELIST_BUTTON2_TEXT1 + i, tagFileListWindow1.pButtonText2[i], NUMBER_LOADSTRING);
	}

	tagFileListWindow1.nWindowMessage = RegisterWindowMessage(_T("FileListDlg"));
	tagFileListWindow1.hList = GetDlgItem(hWnd, IDC_FILELIST_LIST1);
	lpStyle = ListView_GetExtendedListViewStyle(tagFileListWindow1.hList);
	lpStyle |= LVS_EX_GRIDLINES | LVS_EX_FULLROWSELECT;
	ListView_SetExtendedListViewStyle(tagFileListWindow1.hList, lpStyle);
	tagFileListWindow1.hGroup[0] = GetDlgItem(hWnd, IDC_FILELIST_GROUP1);
	for (i = 0; i < SIZEOF_NUM(tagFileListWindow1.hButton); i++) {
		tagFileListWindow1.hButton[i] = GetDlgItem(hWnd, IDC_FILELIST_BUTTON1 + i);
	}
	for (i = 0; i < SIZEOF_NUM(tagFileListWindow1.hRadio); i++) {
		tagFileListWindow1.hRadio[i] = GetDlgItem(hWnd, IDC_FILELIST_RADIO1 + i);
	}

	tagFileListWindow1.hMenu = LoadMenu(tagMainWindow1.hInst, MAKEINTRESOURCE(IDR_FILELIST_LISTMENU));
	tagFileListWindow1.hPupMenu[0] = GetSubMenu(tagFileListWindow1.hMenu, 0);
	tagFileListWindow1.hPupMenu[1] = GetSubMenu(tagFileListWindow1.hMenu, 1);

	FileListDlg_GetDpi();

	if (szListViewColumn[0][0] == '\0')
	{
		for (i = 0; i < 3; i++) {
			LoadString(tagMainWindow1.hInst, IDS_FILELIST_LISTVIEW_FILENAME + i, szListViewColumn[i], 32 - 1);
		}
	}

	GetWindowRect(hWnd, &rc);
	i = (int)(80. * tagMainWindow1.dDpiSize);
	nListFileSizeWidth = i;
	nRet = rc.right - rc.left;
	nRet = nRet - MDIVX(FILELISTDLG_X - 288) - i - nHashStringWidth[0] - (int)(tagFileListWindow1.nScroll_Width / tagMainWindow1.dDpiSize) - 4;

	lvColumn.mask = LVCF_FMT | LVCF_WIDTH | LVCF_TEXT | LVCF_SUBITEM;
	lvColumn.fmt = LVCFMT_LEFT;
	lvColumn.cx = nRet;
	lvColumn.pszText = szListViewColumn[0];
	lvColumn.iSubItem = 0;
	ListView_InsertColumn(tagFileListWindow1.hList, 0, &lvColumn);

	lvColumn.cx = i;
	lvColumn.fmt = LVCFMT_RIGHT;
	lvColumn.pszText = szListViewColumn[1];
	lvColumn.iSubItem = 1;
	ListView_InsertColumn(tagFileListWindow1.hList, 1, &lvColumn);

	lvColumn.cx = nHashStringWidth[tagMainWindow1.tagHashThread1.dwHashType];
	lvColumn.fmt = LVCFMT_LEFT;
	lvColumn.pszText = szListViewColumn[2];
	lvColumn.iSubItem = 2;
	ListView_InsertColumn(tagFileListWindow1.hList, 2, &lvColumn);

	tagFileListWindow1.hStatus = CreateWindowEx(0, STATUSCLASSNAME, NULL, WS_CHILD | SBARS_SIZEGRIP | CCS_BOTTOM | WS_VISIBLE, 0, 0, 0, 0, hWnd, (HMENU)IDC_STATUS1, tagMainWindow1.hInst, NULL);
	SendMessage(tagFileListWindow1.hStatus, SB_SIMPLE, TRUE, 0);

	GetWindowRect(tagFileListWindow1.hStatus, &rc);
	tagFileListWindow1.dwStatus1_Height = rc.bottom - rc.top;

	memset(&ShellInfo, 0, sizeof(ShellInfo));
	ShellInfo.cbSize = sizeof(ShellInfo);
	ShellInfo.fMask = SEE_MASK_NOCLOSEPROCESS | SEE_MASK_INVOKEIDLIST | SEE_MASK_FLAG_NO_UI;
	ShellInfo.hwnd = hWnd;
	ShellInfo.lpVerb = _T("Properties");//プロパティダイアログを開くオプション。

	GetPrivateProfileString(_T("FileList"), _T("OpenOption"), _T("0"), tagFileListWindow1.pBuf, 2, tagMainWindow1.pINIFile);
	tagFileListWindow1.dwOpenOption = _ttoi(tagFileListWindow1.pBuf);
	// 開くオプションのチェック
	if (tagFileListWindow1.dwOpenOption >= SIZEOF_NUM(tagFileListWindow1.hRadio)) {
		tagFileListWindow1.dwOpenOption = 0;
	}
	SendMessage(tagFileListWindow1.hRadio[tagFileListWindow1.dwOpenOption], BM_SETCHECK, BST_CHECKED, 0);
	FileListDlg_SetHashType();
	nRet = WindowSize_LoadState(hWnd, _T("FileList"), tagMainWindow1.pINIFile);
	if (nRet == FALSE) {
		FileListDlg_SetWindowMove(TRUE);
	}
	tagFileListWindow1.nIsHashFileCompMassege = 1;
	tagFileListWindow1.tagFileListItem1.dwListBufSize = 0;
	FileListDlg_ListView(0, FALSE);
	if (tagFileListWindow1.tagFileListItem1.dwListSelectItem != 0) {
		*tagFileListWindow1.tagFileListItem1.dwListSelectItem = (DWORD)-1;
	}
	else {
		DestroyWindow(hWnd);
	}

	// 計算中は自分のウインドウに APP_MESSAGE_INIT を送る。
	if (dwAppFrag & APP_HASHTHREAD) {
		SendMessage(hWnd, tagFileListWindow1.nWindowMessage, APP_MESSAGE_INIT, 0);
	}
	else {
		SendMessage(hWnd, tagFileListWindow1.nWindowMessage, dwAppState != 0 ? APP_MESSAGE_ERROR : APP_MESSAGE_END, 0);
	}

	if (dwAppFrag & APP_PAUSE) {
		SendMessage(hWnd, tagFileListWindow1.nWindowMessage, APP_MESSAGE_PAUSE, dwAppFrag & APP_PAUSE ? TRUE : FALSE);
	}
	PostMessage(hWnd, tagFileListWindow1.nWindowMessage, APP_MESSAGE_WINDOWSET, 0);
	return TRUE;
}

LRESULT FileListDlg_OnCommand(HWND hWnd, WPARAM wParam, LPARAM lParam)
{
	int wmId = LOWORD(wParam);
	//int wmEvent = HIWORD(wParam);

	// 選択されたメニューの解析:
	switch (wmId)
	{
	case IDC_FILELIST_RADIO1:
		tagFileListWindow1.dwOpenOption = 0;
		break;
	case IDC_FILELIST_RADIO2:
		tagFileListWindow1.dwOpenOption = 1;
		break;
	case IDC_FILELIST_RADIO3:
		tagFileListWindow1.dwOpenOption = 2;
		break;
	case IDC_FILELIST_BUTTON1:
		SendMessage(tagMainWindow1.tagHashThread1.hWnd, WM_COMMAND, dwAppFrag & APP_HASHTHREAD ? IDM_PAUSE : IDM_REOPEN, 0);
		break;
	case IDC_FILELIST_BUTTON2:
		if (dwAppFrag & APP_HASHTHREAD)
		{
			SendMessage(tagMainWindow1.tagHashThread1.hWnd, WM_COMMAND, IDM_CANCEL, 0);
			return 0;
		}
		FileListDlg_Clear(TRUE, TRUE);
		SendMessage(tagMainWindow1.tagHashThread1.hWnd, WM_COMMAND, IDM_CLEAR, 0);
		break;
	case IDC_FILELIST_BUTTON3:
		switch (tagFileListWindow1.dwOpenOption)
		{
		case 1:
			FileListDlg_FolderOpen();
			break;
		case 2:
			FileListDlg_HashFileOpen();
			break;
		default:
			FileListDlg_FileOpen();
		}
		break;
	case IDC_FILELIST_BUTTON4:
		if (dwAppFrag & APP_HASHTHREAD) {
			return TRUE;
		}
		SaveFile();
		break;
	case ID_FILELIST_LISTMENU1_1:
		if (*tagFileListWindow1.tagFileListItem1.dwListSelectItem != (DWORD)-1) {
			SendMessage(tagMainWindow1.tagHashThread1.hWnd, tagMainWindow1.tagHashThread1.nMessageID, APP_MESSAGE_CHANGE_FILEVIEW, *tagFileListWindow1.tagFileListItem1.dwListSelectItem);
		}
		break;
	case ID_FILELIST_LISTMENU1_2:
		if (*tagFileListWindow1.tagFileListItem1.dwListSelectItem != (DWORD)-1)
		{
			TCHAR* ptr = NULL;
			int nItem = -1;

			while ((nItem = ListView_GetNextItem(tagFileListWindow1.hList, nItem, LVNI_SELECTED)) != -1)
			{
				const tagHashThread_FileRecode* cptagHashFile_Recode = HashThread_GetFileRecode_Core(&tagMainWindow1.tagHashThread1, nItem);
				ptr = qtcscpy(tagFileListWindow1.pBuf, _T("/select,"));
				qtcscpy(ptr, cptagHashFile_Recode->szFileName);
				ShellExecute(hWnd, _T("open"), _T("EXPLORER.EXE"), tagFileListWindow1.pBuf, NULL, SW_SHOWNORMAL);
			}
		}
		break;
	case IDM_NCOPY:
	case ID_FILELIST_LISTMENU1_3:
	case ID_FILELIST_LISTMENU1_4:
	case ID_FILELIST_LISTMENU1_7:
	case ID_FILELIST_LISTMENU1_EXTCOPY1:
	case ID_FILELIST_LISTMENU1_EXTCOPY2:
	case ID_FILELIST_LISTMENU1_EXTCOPY3:
	case ID_FILELIST_LISTMENU1_EXTCOPY4:
	case ID_FILELIST_LISTMENU1_EXTCOPY5:
		return FileListDlg_OnMessageClipboardCopy(hWnd, wParam, lParam);
	case ID_FILELIST_LISTMENU1_5:
		FileListDlg_FileDelete();
		break;
	case ID_FILELIST_LISTMENU1_6:
		if (*tagFileListWindow1.tagFileListItem1.dwListSelectItem != (DWORD)-1)
		{
			int nRet = -1;

			while ((nRet = ListView_GetNextItem(tagFileListWindow1.hList, nRet, LVNI_SELECTED)) != -1)
			{
				const tagHashThread_FileRecode* cptagHashFile_Recode = HashThread_GetFileRecode_Core(&tagMainWindow1.tagHashThread1, nRet);
				ShellInfo.lpFile = cptagHashFile_Recode->szFileName;
				ShellExecuteEx(&ShellInfo);
			}
		}
		break;
	case ID_FILELIST_LISTMENU2_1:
		FileListDlg_FileOpen();
		break;
	case ID_FILELIST_LISTMENU2_2:
		FileListDlg_FolderOpen();
		break;
	case ID_FILELIST_LISTMENU2_3:
		FileListDlg_HashFileOpen();
		break;
	case ID_FILELIST_LISTMENU2_4:
		if (OpenClipboard(hWnd))
		{
			HANDLE hMem = NULL;
			HDROP hdropFile = NULL;
			DWORD dwRet = 0;

			hMem = GetClipboardData(CF_HDROP);
			if (hMem == NULL) {
				return FALSE;
			}

			hdropFile = (HDROP)GlobalLock(hMem);
			dwRet = FileListDlg_DropFile(hdropFile);

			GlobalUnlock(hMem);
			CloseClipboard();

			if (dwRet == (DWORD)-1)
			{
				FileRecodeFail(hWnd);
				DestroyWindow(tagMainWindow1.tagHashThread1.hWnd);
				return FALSE;
			}
		}
		return TRUE;
	case ID_FILELIST_LISTMENU2_5:
		FileListDlg_Clear(TRUE, TRUE);
		return TRUE;
	case IDM_ALLSERECT:
		ListView_SetItemState(tagFileListWindow1.hList, -1, LVIS_SELECTED, LVIS_SELECTED);
		return TRUE;
	}
	return FALSE;
}

LRESULT FileListDlg_OnDropFiles(HWND hWnd, WPARAM wParam, LPARAM lParam)
{
	UNREFERENCED_PARAMETER(lParam);
	DWORD dwRet = 0;

	dwRet = FileListDlg_DropFile((HDROP)wParam);
	DragFinish((HDROP)wParam);
	IF_UNLIKELY(dwRet == (DWORD)-1)
	{
		FileRecodeFail(hWnd);
		DestroyWindow(tagMainWindow1.tagHashThread1.hWnd);
		return FALSE;
	}
	return TRUE;
}

LRESULT FileListDlg_OnGetMinMaxInfo(HWND hWnd, WPARAM wParam, LPARAM lParam)
{
	UNREFERENCED_PARAMETER(hWnd);
	UNREFERENCED_PARAMETER(wParam);

	((MINMAXINFO*)lParam)->ptMinTrackSize.x = MDIVX(FILELISTDLG_X) + tagFileListWindow1.dwDialogRect + tagMainWindow1.dwWindowRectX;
	((MINMAXINFO*)lParam)->ptMinTrackSize.y = MDIVY(FILELISTDLG_Y) + tagMainWindow1.dwWindowRectY;
	return TRUE;
}

LRESULT FileListDlg_OnNotify(HWND hWnd, WPARAM wParam, LPARAM lParam)
{
	switch (((LPNMHDR)lParam)->code)
	{
	case LVN_ITEMCHANGED:
		switch (((LPNMHDR)lParam)->idFrom)
		{
		case IDC_FILELIST_LIST1:
			return FileListDlg_OnItemChanged(hWnd, wParam, lParam);
		}
		break;
	case NM_CLICK:
	case NM_DBLCLK:
	case NM_RCLICK:
		switch (((LPNMHDR)lParam)->idFrom)
		{
		case IDC_FILELIST_LIST1:
			return FileListDlg_OnClick(hWnd, wParam, lParam);
		}
		break;
	case LVN_KEYDOWN:
		switch (((LPNMHDR)lParam)->idFrom)
		{
		case IDC_FILELIST_LIST1:
			return FileListDlg_OnKeyDown(hWnd, wParam, lParam);
		}
		break;
	case LVN_COLUMNCLICK:
		switch (((LPNMHDR)lParam)->idFrom)
		{
		case IDC_FILELIST_LIST1:
			return FileListDlg_OnColumNClick(hWnd, wParam, lParam);
		}
		return TRUE;
	case LVN_BEGINDRAG:
		switch (((LPNMHDR)lParam)->idFrom)
		{
		case IDC_FILELIST_LIST1:
			return FileListDlg_OnBeginDrag(hWnd, wParam, lParam);
		}
		break;
	case NM_CUSTOMDRAW:
		switch (((LPNMHDR)lParam)->idFrom)
		{
		case IDC_FILELIST_LIST1:
			return FileListDlg_OnCustomDraw(hWnd, wParam, lParam);
		}
		break;
	}
	return FALSE;
}

LRESULT FileListDlg_OnMouseMove(HWND hWnd, WPARAM wParam, LPARAM lParam)
{
	UNREFERENCED_PARAMETER(wParam);
	LVHITTESTINFO lvHitInfo = { 0 };
	RECT dlgRect = { 0 };
	POINT pt = { 0 };

	if (dwAppFrag & APP_HASHTHREAD || GetCapture() != hWnd) {
		return FALSE;
	}

	pt.x = LOWORD(lParam);
	pt.y = HIWORD(lParam);

	ClientToScreen(hWnd, &pt);
	GetWindowRect(hWnd, &dlgRect);
	ImageList_DragMove(pt.x - dlgRect.left, pt.y - dlgRect.top);

	lvHitInfo.pt.x = pt.x;
	lvHitInfo.pt.y = pt.y;
	ScreenToClient(tagFileListWindow1.hList, &lvHitInfo.pt);
	ListView_SubItemHitTest(tagFileListWindow1.hList, &lvHitInfo);

	if (lvHitInfo.iItem != -1 && tagFileListWindow1.dwDragListNo != (DWORD)lvHitInfo.iItem)
	{
		if (tagFileListWindow1.dwMoveListNo != -1) {
			ListView_SetItemState(tagFileListWindow1.hList, tagFileListWindow1.dwMoveListNo, 0, LVIS_SELECTED | LVIS_FOCUSED | LVIS_DROPHILITED);
		}
		tagFileListWindow1.dwMoveListNo = lvHitInfo.iItem;
		ListView_SetItemState(tagFileListWindow1.hList, tagFileListWindow1.dwMoveListNo, LVIS_DROPHILITED, LVIS_DROPHILITED);
	}
	return TRUE;
}

LRESULT FileListDlg_OnLButtonUp(HWND hWnd, WPARAM wParam, LPARAM lParam)
{
	UNREFERENCED_PARAMETER(wParam);
	UNREFERENCED_PARAMETER(lParam);
	LVHITTESTINFO lvHitInfo = { 0 };

	if (dwAppFrag & APP_HASHTHREAD && GetCapture() != hWnd) {
		return FALSE;
	}

	ImageList_DragLeave(hWnd);
	ImageList_EndDrag();
	ImageList_Destroy(hDragImage);
	ReleaseCapture();

	GetCursorPos(&lvHitInfo.pt);
	ScreenToClient(tagFileListWindow1.hList, &lvHitInfo.pt);
	ListView_SubItemHitTest(tagFileListWindow1.hList, &lvHitInfo);
	ListView_SetItemState(tagFileListWindow1.hList, -1, 0, LVIS_SELECTED | LVIS_DROPHILITED);

#ifdef _DEBUG
	STPRINTF_FUNC(szDebugText, _T("FileList: FileListDlg_OnLButtonUp(): ドラッグ処理しています...[%d]\r\n"), lvHitInfo.iItem);
	OutputDebugString(szDebugText);
#endif

	if (lvHitInfo.iItem != -1 && tagFileListWindow1.dwDragListNo != (DWORD)lvHitInfo.iItem)
	{
		tagFileListWindow1.dwMoveListNo = lvHitInfo.iItem;
		HashThread_SwapItemBuffer(&tagMainWindow1.tagHashThread1, tagFileListWindow1.dwDragListNo, tagFileListWindow1.dwMoveListNo);

		FileListDlg_SetListView(tagFileListWindow1.dwDragListNo, FALSE);
		FileListDlg_SetListView(tagFileListWindow1.dwMoveListNo, FALSE);
		ListView_SetItemState(tagFileListWindow1.hList, tagFileListWindow1.dwMoveListNo, LVIS_FOCUSED | LVIS_SELECTED , LVIS_FOCUSED | LVNI_SELECTED);

#ifdef _DEBUG
		OutputDebugString(_T("FileList: FileListDlg_OnLButtonUp(): ドラッグ処理終了\r\n"));
#endif

	}
	return FALSE;
}

LRESULT FileListDlg_OnSysColorChange(HWND hWnd, WPARAM wParam, LPARAM lParam)
{
	UNREFERENCED_PARAMETER(hWnd);
	UNREFERENCED_PARAMETER(wParam);
	UNREFERENCED_PARAMETER(lParam);

	SendMessage(tagFileListWindow1.hList, WM_SYSCOLORCHANGE, 0, 0);
	return TRUE;
}

LRESULT FileListDlg_OnThemeChanged(HWND hWnd, WPARAM wParam, LPARAM lParam)
{
	UNREFERENCED_PARAMETER(hWnd);
	UNREFERENCED_PARAMETER(wParam);
	UNREFERENCED_PARAMETER(lParam);

	return TRUE;
}

LRESULT FileListDlg_OnClose(HWND hWnd, WPARAM wParam, LPARAM lParam)
{
	UNREFERENCED_PARAMETER(wParam);
	UNREFERENCED_PARAMETER(lParam);

	if (dwAppFrag & APP_FOLDERINIT)
	{
		LoadString(tagMainWindow1.hInst, IDS_FILELIST_CLOSE, tagFileListWindow1.pStBuf, MAX_STRINGTABLE);
		FileListDlg_MessageBox(hWnd, tagFileListWindow1.pStBuf, tagMainWindow1.pTitle, MB_ICONHAND);
		return FALSE;
	}
	DestroyWindow(hWnd);
	return TRUE;
}

LRESULT FileListDlg_OnDestroy(HWND hWnd, WPARAM wParam, LPARAM lParam)
{
	UNREFERENCED_PARAMETER(wParam);
	UNREFERENCED_PARAMETER(lParam);
	int i = 0;

	ImageList_Destroy(tagFileListWindow1.hListViewImg);
	tagFileListWindow1.hListViewImg = NULL;
	for (i = 0; i < SIZEOF_NUM(tagFileListWindow1.hListViewIcon); i++)
	{
		DeleteObject(tagFileListWindow1.hListViewIcon[i]);
		tagFileListWindow1.hListViewIcon[i] = NULL;
	}

#ifndef _NODLL
	_itot_s(tagFileListWindow1.dwOpenOption, tagFileListWindow1.pBuf, 33, 10);
#else
	_itot(tagFileListWindow1.dwOpenOption, tagFileListWindow1.pBuf, 10);
#endif

	WritePrivateProfileString(_T("FileList"), _T("OpenOption"), tagFileListWindow1.pBuf, tagMainWindow1.pINIFile);
	WindowSize_SaveState(hWnd, _T("FileList"), tagMainWindow1.pINIFile);
	free(tagFileListWindow1.tagFileListItem1.dwListSelectItem);
	tagFileListWindow1.tagFileListItem1.dwListSelectItem = NULL;
	tagFileListWindow1.tagFileListItem1.dwListBufSize = 0;
	tagFileListWindow1.pBuf = NULL;
	DestroyMenu(tagFileListWindow1.hMenu);
	tagFileListWindow1.hMenu = NULL;
	tagFileListWindow1.hWnd = NULL;
	tagFileListWindow1.nWindowMessage = (UINT)-1;
	//ShowWindow(tagMainWindow1.hWnd, SW_SHOW);
	return TRUE;
}

LRESULT FileListDlg_OnSize(HWND hWnd, WPARAM wParam, LPARAM lParam)
{
	UNREFERENCED_PARAMETER(wParam);
	int nArg[4] = { 0 };
	const int cx = LOWORD(lParam);
	const int cy = HIWORD(lParam);
	const int i = cx - MDIVX(FILELISTDLG_X - 288) - nListFileSizeWidth - nHashStringWidth[tagMainWindow1.tagHashThread1.dwHashType] - (int)(tagFileListWindow1.nScroll_Width / tagMainWindow1.dDpiSize) - 4;

	nArg[0] = 0;
	nArg[1] = 0;
	nArg[2] = cx - MDIVX(FILELISTDLG_X - 288);
	nArg[3] = cy - tagFileListWindow1.dwStatus1_Height;
	MoveWindow(tagFileListWindow1.hList, nArg[0], nArg[1], nArg[2], nArg[3], FALSE);

	ListView_SetColumnWidth(tagFileListWindow1.hList, 0, i);

	nArg[0] = cx - MDIVX(FILELISTDLG_X - 294);
	nArg[1] = MDIVY(12) - 1;
	nArg[2] = MDIVX(78);
	nArg[3] = nStringY[2] + nEditMarginY[0] + 2;
	MoveWindow(tagFileListWindow1.hButton[0], nArg[0], nArg[1], nArg[2], nArg[3], FALSE);

	nArg[0] = cx - MDIVX(FILELISTDLG_X - 294);
	nArg[1] = MDIVY(32) - 1;
	MoveWindow(tagFileListWindow1.hButton[1], nArg[0], nArg[1], nArg[2], nArg[3], FALSE);

	nArg[0] = cx - MDIVX(FILELISTDLG_X - 294);
	nArg[1] = MDIVY(52) - 1;
	MoveWindow(tagFileListWindow1.hButton[2], nArg[0], nArg[1], nArg[2], nArg[3], FALSE);

	nArg[0] = cx - MDIVX(FILELISTDLG_X - 294);
	nArg[1] = MDIVY(77) - 1;
	nArg[3] = MDIVY(54);
	MoveWindow(tagFileListWindow1.hGroup[0], nArg[0], nArg[1], nArg[2], nArg[3], FALSE);

	nArg[0] = cx - MDIVX(FILELISTDLG_X - 300);
	nArg[1] = MDIVY(89);
	nArg[2] = MDIVX(68);
	nArg[3] = nStringY[0] + nEditMarginY[0];
	MoveWindow(tagFileListWindow1.hRadio[0], nArg[0], nArg[1], nArg[2], nArg[3], FALSE);

	nArg[0] = cx - MDIVX(FILELISTDLG_X - 300);
	nArg[1] = MDIVY(101);
	nArg[2] = MDIVX(68);
	nArg[3] = nStringY[0] + nEditMarginY[0];
	MoveWindow(tagFileListWindow1.hRadio[1], nArg[0], nArg[1], nArg[2], nArg[3], FALSE);

	nArg[0] = cx - MDIVX(FILELISTDLG_X - 300);
	nArg[1] = MDIVY(113);
	nArg[2] = MDIVX(68);
	nArg[3] = nStringY[0] + nEditMarginY[0];
	MoveWindow(tagFileListWindow1.hRadio[2], nArg[0], nArg[1], nArg[2], nArg[3], FALSE);

	nArg[0] = cx - MDIVX(FILELISTDLG_X - 294);
	nArg[1] = MDIVY(136) - 1;
	nArg[2] = MDIVX(78);
	nArg[3] = nStringY[2] + nEditMarginY[0] + 2;
	MoveWindow(tagFileListWindow1.hButton[3], nArg[0], nArg[1], nArg[2], nArg[3], FALSE);

	SendMessage(tagFileListWindow1.hStatus, WM_SIZE, 0, lParam);
	InvalidateRect(hWnd, NULL, TRUE);
	return TRUE;
}

LRESULT FileListDlg_OnItemChanged(HWND hWnd, WPARAM wParam, LPARAM lParam)
{
	UNREFERENCED_PARAMETER(hWnd);
	UNREFERENCED_PARAMETER(wParam);
	UNREFERENCED_PARAMETER(lParam);

	if (tagFileListWindow1.nIsListLock == FALSE)
	{
		DWORD dwListSelect = (DWORD)-1;

#ifdef _DEBUG
		OutputDebugString(_T("FileList: FileListDlg_WndProc(): LVN_ITEMCHANGED\r\n"));
#endif

		tagFileListWindow1.tagFileListItem1.dwListSelectCount = 0;
		while (1)
		{
			dwListSelect = ListView_GetNextItem(tagFileListWindow1.hList, dwListSelect, LVNI_ALL | LVNI_SELECTED);
			*(tagFileListWindow1.tagFileListItem1.dwListSelectItem + tagFileListWindow1.tagFileListItem1.dwListSelectCount) = dwListSelect;
			if (dwListSelect == -1) {
				break;
			}
			tagFileListWindow1.tagFileListItem1.dwListSelectCount++;
		}
		*(tagFileListWindow1.tagFileListItem1.dwListSelectItem + tagFileListWindow1.tagFileListItem1.dwListSelectCount) = (DWORD)-1;

		if (*tagFileListWindow1.tagFileListItem1.dwListSelectItem != (DWORD)-1)
		{
			const tagHashThread_FileRecode* cptagHashFile_Recode = HashThread_GetFileRecode_Core(&tagMainWindow1.tagHashThread1, *tagFileListWindow1.tagFileListItem1.dwListSelectItem);

#ifdef _DEBUG
			TCHAR szNum[NUMBER_LOADSTRING];

			p = qtcscpy(tagFileListWindow1.pStBuf, _T("選択された項目: "));
			for (DWORD dwI = 0; dwI < tagFileListWindow1.tagFileListItem1.dwListSelectCount; dwI++)
			{
				ITOT_FUNC(*(tagFileListWindow1.tagFileListItem1.dwListSelectItem + dwI), szNum, SIZEOF_NUM(szNum), 10);
				p = qtcscpy(qtcscpy(p, szNum), _T(", "));
			}
			p -= 2;
			*p = '\0';

			p = qtcscpy(p, _T("\r\n"));
			OutputDebugString(tagFileListWindow1.pStBuf);
#endif

			LoadString(tagMainWindow1.hInst, IDS_FILELIST_STATUS_TEXT2, tagFileListWindow1.pStBuf, MAX_STRINGTABLE);
			MessageFormat(tagFileListWindow1.pBuf, 1024, tagFileListWindow1.pStBuf, tagFileListWindow1.tagFileListItem1.dwListSelectCount, cptagHashFile_Recode->szFileName);
			SendMessage(tagFileListWindow1.hStatus, SB_SETTEXT, SB_SIMPLEID, (WPARAM)tagFileListWindow1.pBuf);
		}
	}
	return TRUE;
}

LRESULT FileListDlg_OnClick(HWND hWnd, WPARAM wParam, LPARAM lParam)
{
	UNREFERENCED_PARAMETER(wParam);
	LVHITTESTINFO lvHitInfo = { 0 };
	POINT pt = { 0 };
	DWORD dwFileCount = HashThread_GetCountItem_Core(&tagMainWindow1.tagHashThread1);
	DWORD dwCount = 0;

	GetCursorPos(&lvHitInfo.pt);
	ScreenToClient(tagFileListWindow1.hList, &lvHitInfo.pt);
	ListView_SubItemHitTest(tagFileListWindow1.hList, &lvHitInfo);

	if (lvHitInfo.iItem != -1 && dwFileCount > 0)
	{
		DWORD dwListSelect = (DWORD)-1;

		dwCount = 0;
		*tagFileListWindow1.tagFileListItem1.dwListSelectItem = lvHitInfo.iItem;

		while (1)
		{
			dwListSelect = ListView_GetNextItem(tagFileListWindow1.hList, dwListSelect, LVNI_ALL | LVNI_SELECTED);
			if (dwListSelect == (DWORD)-1) {
				break;
			}
			dwCount++;
		}

		switch (((LV_DISPINFO*)lParam)->hdr.code)
		{
		case NM_DBLCLK:
			if (dwAppFrag & APP_HASHTHREAD) {
				MessageBeep(MB_ICONERROR);
			}
			else {
				SendMessage(tagMainWindow1.tagHashThread1.hWnd, tagMainWindow1.tagHashThread1.nMessageID, APP_MESSAGE_CHANGE_FILEVIEW, *tagFileListWindow1.tagFileListItem1.dwListSelectItem);
			}
			break;
		case NM_CLICK:
			break;
		case NM_RCLICK:
			GetCursorPos(&pt);
			FileListDlg_FileMenu(hWnd, *tagFileListWindow1.tagFileListItem1.dwListSelectItem, &pt);
			break;
		}
	}
	else
	{
		LoadString(tagMainWindow1.hInst, IDS_FILELIST_STATUS_TEXT1, tagFileListWindow1.pStBuf, MAX_STRINGTABLE);
		MessageFormat(tagFileListWindow1.pBuf, 1024, tagFileListWindow1.pStBuf, dwFileCount);
		SendMessage(tagFileListWindow1.hStatus, SB_SETTEXT, SB_SIMPLEID, (WPARAM)tagFileListWindow1.pBuf);

		switch (((LV_DISPINFO*)lParam)->hdr.code)
		{
		case NM_RCLICK:
			GetCursorPos(&pt);
			FileListDlg_NoFileMenu(hWnd, &pt);
			break;
		}
	}
	return TRUE;
}

LRESULT FileListDlg_OnKeyDown(HWND hWnd, WPARAM wParam, LPARAM lParam)
{
	UNREFERENCED_PARAMETER(wParam);
	switch (((LPNMLVKEYDOWN)lParam)->wVKey)
	{
	case VK_RETURN:
		if (*tagFileListWindow1.tagFileListItem1.dwListSelectItem != (DWORD)-1) {
			SendMessage(tagMainWindow1.tagHashThread1.hWnd, tagMainWindow1.tagHashThread1.nMessageID, APP_MESSAGE_CHANGE_FILEVIEW, *tagFileListWindow1.tagFileListItem1.dwListSelectItem);
		}
		else {
			MessageBeep(MB_ICONERROR);
		}
		break;
	case VK_APPS:
		if (*tagFileListWindow1.tagFileListItem1.dwListSelectItem != (DWORD)-1)
		{
			static DWORD dwPos = (DWORD)-1;
			RECT subRect = { 0 };
			POINT pt = { 0 };

			if (dwPos == (DWORD)-1)
			{
				ListView_GetItemRect(tagFileListWindow1.hList, *tagFileListWindow1.tagFileListItem1.dwListSelectItem, &subRect, LVIR_ICON);
				dwPos = (subRect.bottom - subRect.top) / 2;
			}
			ListView_GetItemPosition(tagFileListWindow1.hList, *tagFileListWindow1.tagFileListItem1.dwListSelectItem, &pt);
			pt.x += dwPos;
			pt.y += dwPos;
			ClientToScreen(tagFileListWindow1.hList, &pt);
			FileListDlg_FileMenu(hWnd, *tagFileListWindow1.tagFileListItem1.dwListSelectItem, &pt);
		}
		else
		{
			POINT pt = { 0 };

			ClientToScreen(tagFileListWindow1.hList, &pt);
			FileListDlg_NoFileMenu(hWnd, &pt);
		}
		break;
	case VK_DELETE:
		if (tagMainWindow1.dwAddFileModeFileCount != 0 && dwAppFrag & APP_HASHTHREAD) {
			MessageBeep(MB_ICONERROR);
		}
		else {
			FileListDlg_FileDelete();
		}
		break;
	case VK_TAB:
		return FALSE;
	}
	return TRUE;
}

LRESULT FileListDlg_OnColumNClick(HWND hWnd, WPARAM wParam, LPARAM lParam)
{
	UNREFERENCED_PARAMETER(wParam);

	if (tagMainWindow1.dwAddFileModeFileCount == 0 && ~dwAppFrag & APP_HASHTHREAD)
	{
		NMLISTVIEW* pNMLV = (NM_LISTVIEW*)lParam;
		DWORD dwFileCount = HashThread_GetCountItem_Core(&tagMainWindow1.tagHashThread1);
		DWORD dwI = 0;
		DWORD dwRet = 0;

		// コラム別にソートの状態を保存します。
		nFileSortSubNo[pNMLV->iSubItem] = nFileSortSubNo[pNMLV->iSubItem] ? 0 : 1;

#ifdef _DEBUG
		OutputDebugString(_T("FileList: FileListDlg_WndProc(): ソートの処理開始\r\n"));
#endif

		dwRet = HashThread_CreateBuffer(&tagMainWindow1.tagHashThread1, dwFileCount * 2);
		IF_UNLIKELY(dwRet == (DWORD)-1)
		{
			FileRecodeFail(hWnd);
			return FALSE;
		}

#ifdef _DEBUG
		OutputDebugString(_T("FileList: FileListDlg_WndProc(): ソート前のファイルリスト\r\n"));

		for (; dwI < dwFileCount; dwI++)
		{
			STPRINTF_FUNC(szDebugText, _T("FileList: FileListDlg_WndProc(): No.%d,%s\r\n"), dwI, GetTCharToFileName(HashThread_GetFileRecode_Core(&tagMainWindow1.tagHashThread1, dwI)->szFileName));
			OutputDebugString(szDebugText);
		}
		OutputDebugString(_T("FileList: FileListDlg_WndProc(): ソート開始\r\n"));
#endif

		dwRet = ListView_SortItems(tagFileListWindow1.hList, FileListDlg_SortProc, pNMLV);
		if (dwRet)
		{
			DWORD dwNextFile = dwFileCount;
			LV_ITEM lvItem1 = { 0 };
#ifdef _DEBUG
			OutputDebugString(_T("FileList: FileListDlg_WndProc(): ソート完了\r\n"));
#endif

			memset(&lvItem1, 0, sizeof(LV_ITEM));
			lvItem1.mask = LVIF_PARAM;

#ifdef _DEBUG
			OutputDebugString(_T("FileList: FileListDlg_WndProc(): ソートの結果を HashThreadバッファに反映します。\r\n"));
			dwTime1 = timeGetTime();
#endif

			for (dwI = 0; dwI < dwFileCount; dwI++, dwNextFile++)
			{
				lvItem1.iItem = (int)dwI;
				dwRet = ListView_GetItem(tagFileListWindow1.hList, &lvItem1);
				if (dwRet > 0)
				{

#ifdef _DEBUG
					STPRINTF_FUNC(szDebugText, _T("FileList: FileListDlg_WndProc(): No.%d(%d),%s\r\n"), dwI, (int)lvItem1.lParam, GetTCharToFileName(HashThread_GetFileRecode_Core(&tagMainWindow1.tagHashThread1, (DWORD)lvItem1.lParam)->szFileName));
					OutputDebugString(szDebugText);
#endif

					memcpy(tagMainWindow1.tagHashThread1.tagMultiFile->ptagFileRecode + dwNextFile, tagMainWindow1.tagHashThread1.tagMultiFile->ptagFileRecode + lvItem1.lParam, sizeof(tagHashThread_FileRecode));

					lvItem1.lParam = dwI;
					ListView_SetItem(tagFileListWindow1.hList, &lvItem1);
				}
			}
			memmove(HashThread_GetFileRecode_Core(&tagMainWindow1.tagHashThread1, 0), HashThread_GetFileRecode_Core(&tagMainWindow1.tagHashThread1, dwFileCount), sizeof(tagHashThread_FileRecode) * dwFileCount);

#ifdef _DEBUG
			dwTime2 = timeGetTime();
			STPRINTF_FUNC(szDebugText, _T("FileList: FileListDlg_WndProc(): ソート処理完了  %dミリ秒\r\n"), 	dwTime2 - dwTime1);
			OutputDebugString(szDebugText);
#endif

		}

#ifdef _DEBUG
		else {
			OutputDebugString(_T("FileList: FileListDlg_WndProc(): ソート失敗\r\n"));
		}
#endif

		return TRUE;
	}
	else
	{
		UINT uID = IDS_FILELIST_SORT2;

		if (dwAppFrag & APP_PAUSE) {
			uID = IDS_FILELIST_SORT3;
		}
		LoadString(tagMainWindow1.hInst, uID, tagFileListWindow1.pStBuf, MAX_STRINGTABLE);
		MessageFormat(tagFileListWindow1.pBuf, 1024, tagFileListWindow1.pStBuf, tagMainWindow1.tagHashThread1.cpHashName);
		FileListDlg_MessageBox(hWnd, tagFileListWindow1.pBuf, tagMainWindow1.pTitle, MB_ICONHAND);
	}
	return TRUE;
}

LRESULT FileListDlg_OnBeginDrag(HWND hWnd, WPARAM wParam, LPARAM lParam)
{
	UNREFERENCED_PARAMETER(wParam);

	if (tagMainWindow1.dwAddFileModeFileCount == 0 && ~dwAppFrag & APP_HASHTHREAD)
	{
		NMLISTVIEW* pNMLV = (NM_LISTVIEW*)lParam;
		LVHITTESTINFO lvHitInfo = { 0 };
		POINT pt = { 0 };
		DWORD dwListSelect = (DWORD)-1;
		DWORD dwCount = 0;

		GetCursorPos(&lvHitInfo.pt);
		ScreenToClient(tagFileListWindow1.hList, &lvHitInfo.pt);
		ListView_SubItemHitTest(tagFileListWindow1.hList, &lvHitInfo);

		if (lvHitInfo.iItem == -1) {
			return FALSE;
		}

		tagFileListWindow1.dwDragListNo = lvHitInfo.iItem;
		tagFileListWindow1.dwMoveListNo = (UINT)-1;
		dwCount = 0;

		while (1)
		{
			dwListSelect = ListView_GetNextItem(tagFileListWindow1.hList, dwListSelect, LVNI_ALL | LVNI_SELECTED);
			if (dwListSelect == (DWORD)-1) {
				break;
			}
			dwCount++;

			if (tagFileListWindow1.dwDragListNo != dwListSelect) {
				ListView_SetItemState(tagFileListWindow1.hList, dwListSelect, 0, LVIS_SELECTED | LVIS_FOCUSED);
			}
		}
		ListView_SetItemState(tagFileListWindow1.hList, tagFileListWindow1.dwDragListNo, LVIS_FOCUSED | LVIS_SELECTED, LVIS_FOCUSED | LVNI_SELECTED);
		hDragImage = ListView_CreateDragImage(tagFileListWindow1.hList, pNMLV->iItem, &pt);
		ScreenToClient(hWnd, &pt);
		ImageList_BeginDrag(hDragImage, 0, 0, 0);
		ImageList_DragEnter(hWnd, pt.x, pt.y);
		SetCapture(hWnd);
	}
	return TRUE;
}

LRESULT FileListDlg_OnCustomDraw(HWND hWnd, WPARAM wParam, LPARAM lParam)
{
	UNREFERENCED_PARAMETER(wParam);
	LPNMLVCUSTOMDRAW lpCustomDraw = (LPNMLVCUSTOMDRAW)lParam;
	DWORD dwCurrentItem = 0;

	switch (lpCustomDraw->nmcd.dwDrawStage)
	{
	case CDDS_PREPAINT:
		SetWindowLong(hWnd, DWLP_MSGRESULT, CDRF_NOTIFYITEMDRAW);
		break;
	case CDDS_ITEMPREPAINT:
		dwCurrentItem = HashThread_GetCurrentItem_Core(&tagMainWindow1.tagHashThread1);

		if (dwCurrentItem >= 0 && lpCustomDraw->nmcd.dwItemSpec <= dwCurrentItem)
		{
			DWORD dwFileCount = (DWORD)lpCustomDraw->nmcd.dwItemSpec;
			const tagHashThread_FileRecode* cptagHashFile_Recode = HashThread_GetFileRecode_Core(&tagMainWindow1.tagHashThread1, dwFileCount);
			LV_ITEM lvItem1 = { 0 };

			lvItem1.mask = LVIF_IMAGE;
			lvItem1.iItem = (int)dwFileCount;
			lvItem1.iImage = 1;

			if (cptagHashFile_Recode->dwHashFileHashLine != HASHFILE_NOLINE)
			{
				if (cptagHashFile_Recode->dwFileLastError == 0)
				{
					if (cptagHashFile_Recode->dwFileHashType == cptagHashFile_Recode->dwHashFileHashType)
					{
						if (cptagHashFile_Recode->nHashFileCmp == 0)
						{
							lpCustomDraw->clrText = (tagMainWindow1.dwAddFileModeFileCount != 0 && dwFileCount < tagMainWindow1.dwAddFileModeFileCount) ?
								tagMainWindow1.clGrayTextColor : tagFileListWindow1.clListTextColor[0];
						}
						else
						{
							lpCustomDraw->clrText = tagFileListWindow1.clListTextColor[1];
							lvItem1.iImage = 2;
						}
					}
				}
				else if (cptagHashFile_Recode->dwFileLastError != (DWORD)-1)
				{
					lpCustomDraw->clrText = tagFileListWindow1.clListTextColor[2];
					lvItem1.iImage = 3;
				}
				ListView_SetItem(tagFileListWindow1.hList, &lvItem1);
				SetWindowLong(hWnd, DWLP_MSGRESULT, CDRF_NEWFONT);
			}
			else
			{
				if (cptagHashFile_Recode->dwFileLastError == (DWORD)-1) {
					lvItem1.iImage = 0;
				}
				else if (cptagHashFile_Recode->dwFileLastError != 0)
				{
					lpCustomDraw->clrText = tagFileListWindow1.clListTextColor[2];//[tagMainWindow1.dwAddFileModeFileCount < dwFileCount ? 2 : 5];
					lvItem1.iImage = 3;
				}
				else if (cptagHashFile_Recode->dwFileHashType != tagMainWindow1.tagHashThread1.dwHashType)
				{
					lpCustomDraw->clrText = tagFileListWindow1.clListTextColor[2];//[tagMainWindow1.dwAddFileModeFileCount < dwFileCount ? 2 : 5];
					lvItem1.iImage = 3;
				}
				ListView_SetItem(tagFileListWindow1.hList, &lvItem1);
			}
			return TRUE;
		}
		break;
	}
	return TRUE;
}

LRESULT FileListDlg_OnMessageHashFileOpen(HWND hWnd, WPARAM wParam, LPARAM lParam)
{
	UNREFERENCED_PARAMETER(wParam);
	UNREFERENCED_PARAMETER(lParam);
	DWORD dwRet = 0;

	dwRet = FileListDlg_AddHashFile();
	SetWindowLong(hWnd, DWLP_MSGRESULT, dwRet);
	return TRUE;
}

LRESULT FileListDlg_OnMessageFolderInit(HWND hWnd, WPARAM wParam, LPARAM lParam)
{
	UNREFERENCED_PARAMETER(wParam);

	tagFileListWindow1.dwFileCount = (DWORD)lParam;
	DragAcceptFiles(hWnd, FALSE);
	return TRUE;
}

LRESULT FileListDlg_OnMessageSubFolderInit(HWND hWnd, WPARAM wParam, LPARAM lParam)
{
	UNREFERENCED_PARAMETER(hWnd);
	UNREFERENCED_PARAMETER(wParam);
	UNREFERENCED_PARAMETER(lParam);

	return TRUE;
}

LRESULT FileListDlg_OnMessageFolderExit(HWND hWnd, WPARAM wParam, LPARAM lParam)
{
	UNREFERENCED_PARAMETER(wParam);

	if (lParam == (DWORD)-1)
	{
		DestroyWindow(hWnd);
		return FALSE;
	}
	dwAppFrag |= APP_FILELISTBOX_EDIT;
	dwAppFrag &= ~APP_FOLDERINIT;
	tagFileListWindow1.dwNewFile++;
	tagMainWindow1.tagHashThread1.nIsFileNoCheck = 1;
	FileListDlg_ListView(tagFileListWindow1.dwFileCount, FALSE);
	EnableWindow(tagFileListWindow1.hButton[0], TRUE);
	EnableWindow(tagFileListWindow1.hButton[1], TRUE);
	EnableWindow(tagFileListWindow1.hButton[2], TRUE);
	EnableWindow(tagFileListWindow1.hRadio[0], TRUE);
	EnableWindow(tagFileListWindow1.hRadio[1], TRUE);
	EnableWindow(tagFileListWindow1.hRadio[2], TRUE);
	EnableWindow(tagFileListWindow1.hButton[3], FALSE);
	tagFileListWindow1.dwNewFile++;
	DragAcceptFiles(hWnd, TRUE);
	return TRUE;
}

LRESULT FileListDlg_OnMessageHashFileInit(HWND hWnd, WPARAM wParam, LPARAM lParam)
{
	UNREFERENCED_PARAMETER(wParam);

	if (lParam > 0) {
		FileListDlg_ListView(tagFileListWindow1.dwFileCount, FALSE);
	}
	tagFileListWindow1.dwFileCount = (DWORD)lParam;
	DragAcceptFiles(hWnd, FALSE);
	return TRUE;
}

LRESULT FileListDlg_OnMessageHashFileExit(HWND hWnd, WPARAM wParam, LPARAM lParam)
{
	UNREFERENCED_PARAMETER(wParam);

	if (lParam > 0) {
		FileListDlg_ListView(tagFileListWindow1.dwFileCount, FALSE);
	}
	tagFileListWindow1.dwFileCount = (DWORD)lParam;
	DragAcceptFiles(hWnd, TRUE);
	return TRUE;
}

LRESULT FileListDlg_OnMessageInit(HWND hWnd, WPARAM wParam, LPARAM lParam)
{
	UNREFERENCED_PARAMETER(wParam);
	UNREFERENCED_PARAMETER(lParam);

	DragAcceptFiles(hWnd, FALSE);
	tagFileListWindow1.dwFileCount = 0;
	tagFileListWindow1.nIsHashFileCompMassege = 0;
	SetWindowText(tagFileListWindow1.hButton[0], tagFileListWindow1.pButtonText1[1]);
	EnableWindow(tagFileListWindow1.hButton[0], TRUE);
	SetWindowText(tagFileListWindow1.hButton[1], tagFileListWindow1.pButtonText2[1]);
	EnableWindow(tagFileListWindow1.hButton[1], TRUE);
	EnableWindow(tagFileListWindow1.hButton[2], FALSE);
	EnableWindow(tagFileListWindow1.hRadio[0], FALSE);
	EnableWindow(tagFileListWindow1.hRadio[1], FALSE);
	EnableWindow(tagFileListWindow1.hRadio[2], FALSE);
	EnableWindow(tagFileListWindow1.hButton[3], FALSE);
	return TRUE;
}

LRESULT FileListDlg_OnMessageFileExit(HWND hWnd, WPARAM wParam, LPARAM lParam)
{
	UNREFERENCED_PARAMETER(hWnd);
	UNREFERENCED_PARAMETER(wParam);

	if (lParam >= 0) {
		FileListDlg_SetListView((DWORD)lParam, FALSE);
	}
	return TRUE;
}

LRESULT FileListDlg_OnMessageFileChange(HWND hWnd, WPARAM wParam, LPARAM lParam)
{
	UNREFERENCED_PARAMETER(hWnd);
	UNREFERENCED_PARAMETER(wParam);

	if (lParam >= 0) {
		ListView_SetItemText(tagFileListWindow1.hList, lParam, 2, (TCHAR*)_T("計算中..."));
	}
	return TRUE;
}

LRESULT FileListDlg_OnMessageClear(HWND hWnd, WPARAM wParam, LPARAM lParam)
{
	UNREFERENCED_PARAMETER(hWnd);
	UNREFERENCED_PARAMETER(wParam);
	UNREFERENCED_PARAMETER(lParam);

	if (dwAppFrag & APP_FILE_CLEAR)
	{
		FileListDlg_Clear(FALSE, FALSE);
		return TRUE;
	}
	return TRUE;
}

LRESULT FileListDlg_OnMessageEnd(HWND hWnd, WPARAM wParam, LPARAM lParam)
{
	UNREFERENCED_PARAMETER(lParam);
	DWORD dwFileCount = 0;
	DWORD dwCount = 0;
	DWORD dwRet = 0;

	dwFileCount = HashThread_GetCountItem_Core(&tagMainWindow1.tagHashThread1);

	if (dwAppFrag & APP_HASHFILE_ADDMODE) {
		return FALSE;
	}

	if (!tagFileListWindow1.dwNewFile) {
		tagFileListWindow1.dwNewFile++;
	}

	memset(&tagFileListWindow1.dwHashCheckCount, 0, sizeof(tagFileListWindow1.dwHashCheckCount));
	for (dwCount = 0; dwCount < dwFileCount; dwCount++)
	{
		const tagHashThread_FileRecode* cptagHashFile_Recode = HASHTHREAD_FILERECODE(&tagMainWindow1.tagHashThread1, dwCount);

		IF_UNLIKELY(cptagHashFile_Recode == NULL) {
			continue;
		}

		if (cptagHashFile_Recode->dwHashFileHashLine != HASHFILE_NOLINE && cptagHashFile_Recode->dwFileLastError == 0 && cptagHashFile_Recode->dwFileHashType == tagMainWindow1.tagHashThread1.dwHashType && cptagHashFile_Recode->dwFileHashType == cptagHashFile_Recode->dwHashFileHashType)
		{
			tagFileListWindow1.dwHashCheckCount[0]++;
			if (cptagHashFile_Recode->nHashFileCmp == 0) {
				tagFileListWindow1.dwHashCheckCount[1]++;
			}
			else {
				tagFileListWindow1.dwHashCheckCount[2]++;
			}
		}
	}

	tagFileListWindow1.dwFileCount = dwFileCount;
	dwRet = tagFileListWindow1.dwFileCount > 0;
	InvalidateRect(tagFileListWindow1.hList, NULL, TRUE);
	SetWindowText(tagFileListWindow1.hButton[0], tagFileListWindow1.pButtonText1[0]);
	SetWindowText(tagFileListWindow1.hButton[1], tagFileListWindow1.pButtonText2[0]);
	if (tagMainWindow1.dwAddFileModeFileCount != 0)
	{
		EnableWindow(tagFileListWindow1.hButton[0], TRUE);
		EnableWindow(tagFileListWindow1.hButton[1], TRUE);
	}
	else
	{
		EnableWindow(tagFileListWindow1.hButton[0], tagFileListWindow1.dwFileCount ? TRUE : FALSE);
		EnableWindow(tagFileListWindow1.hButton[1], dwRet ? TRUE : FALSE);
	}
	EnableWindow(tagFileListWindow1.hButton[2], TRUE);
	EnableWindow(tagFileListWindow1.hButton[3], dwRet && wParam == APP_MESSAGE_END ? TRUE : FALSE);
	EnableWindow(tagFileListWindow1.hRadio[0], TRUE);
	EnableWindow(tagFileListWindow1.hRadio[1], TRUE);
	EnableWindow(tagFileListWindow1.hRadio[2], TRUE);

	if (tagFileListWindow1.nIsHashFileCompMassege == 0 && tagFileListWindow1.dwHashCheckCount[2] > 0)
	{
		LoadString(tagMainWindow1.hInst, IDS_HASHFILE_COMP1, tagFileListWindow1.pStBuf, MAX_STRINGTABLE);
		MessageFormat(tagFileListWindow1.pBuf, MAX_STRINGTABLE, tagFileListWindow1.pStBuf, tagFileListWindow1.dwHashCheckCount[2]);
		FileListDlg_MessageBox(hWnd, tagFileListWindow1.pBuf, tagMainWindow1.pTitle, MB_ICONHAND);
		tagFileListWindow1.nIsHashFileCompMassege++;
	}
	DragAcceptFiles(hWnd, TRUE);
	SetFocus(tagFileListWindow1.hButton[3]);
	return TRUE;
}

LRESULT FileListDlg_OnMessageError(HWND hWnd, WPARAM wParam, LPARAM lParam)
{
	return FileListDlg_OnMessageEnd(hWnd, wParam, lParam);
}

LRESULT FileListDlg_OnMessagePause(HWND hWnd, WPARAM wParam, LPARAM lParam)
{
	UNREFERENCED_PARAMETER(hWnd);
	UNREFERENCED_PARAMETER(wParam);

	SetWindowText(tagFileListWindow1.hButton[0], lParam ? tagFileListWindow1.pButtonText1[2] : tagFileListWindow1.pButtonText1[1]);
	return TRUE;
}

LRESULT FileListDlg_OnMessageWindowSet(HWND hWnd, WPARAM wParam, LPARAM lParam)
{
	int nRet = 0;

	ListView_AllHashStringWidth(tagFileListWindow1.hList);
	nRet = GetPrivateProfileInt(_T("FileList"), _T("WindowState"), -1, tagMainWindow1.pINIFile);
	FileListDlg_SetWindowMove(nRet == -1 ? TRUE : FALSE);
	FileListDlg_DpiChangeMain(hWnd, wParam, lParam);
	return TRUE;
}

LRESULT FileListDlg_OnMessageHashFileChange(HWND hWnd, WPARAM wParam, LPARAM lParam)
{
	FileListDlg_SetHashType();
	FileListDlg_DpiChangeMain(hWnd, wParam, lParam);
	return TRUE;
}

LRESULT FileListDlg_OnMessageHashChange(HWND hWnd, WPARAM wParam, LPARAM lParam)
{
	return FileListDlg_OnMessageHashFileChange(hWnd, wParam, lParam);
}

LRESULT FileListDlg_OnMessageDpiChange(HWND hWnd, WPARAM wParam, LPARAM lParam)
{
	FileListDlg_GetDpi();
	ListView_AllHashStringWidth(tagFileListWindow1.hList);
	FileListDlg_DpiChangeMain(hWnd, wParam, lParam);
	return TRUE;
}

LRESULT FileListDlg_DpiChangeMain(HWND hWnd, WPARAM wParam, LPARAM lParam)
{
	UNREFERENCED_PARAMETER(wParam);
	UNREFERENCED_PARAMETER(lParam);
	LV_COLUMN lvColumn = { 0 };
	RECT rc = { 0 };
	DWORD dwFileCount = 0;
	DWORD dwCount = 0;
	int i = 0;
	int y = 0;
	int nRet = 0;

	dwFileCount = HashThread_GetCountItem_Core(&tagMainWindow1.tagHashThread1);

	i = nHashStringWidth[tagMainWindow1.tagHashThread1.dwHashType];
	tagFileListWindow1.nScroll_Width = (int)(GetSystemMetrics(SM_CXVSCROLL) * tagMainWindow1.dDpiSize);
	y = ListView_GetColumnWidth(tagFileListWindow1.hList, 1);

	GetWindowRect(tagFileListWindow1.hList, &rc);
	nRet = rc.right - rc.left - y - i - (int)(tagFileListWindow1.nScroll_Width / tagMainWindow1.dDpiSize) - 4;
	ListView_SetColumnWidth(tagFileListWindow1.hList, 0, nRet);
	nListFileSizeWidth = y;

	memset(&lvColumn, 0, sizeof(lvColumn));
	lvColumn.cx = i;
	lvColumn.fmt = LVCFMT_LEFT;
	lvColumn.pszText = szListViewColumn[2];
	lvColumn.iSubItem = 2;
	ListView_SetColumn(tagFileListWindow1.hList, 2, &lvColumn);
	ListView_SetColumnWidth(tagFileListWindow1.hList, 2, i);

	for (dwCount = 0; dwCount < dwFileCount; dwCount++) {
		FileListDlg_SetListView(dwCount, FALSE);
	}
	EnableWindow(tagFileListWindow1.hButton[0], TRUE);
	nRet = HASHTHREAD_FILECURRENT(&tagMainWindow1.tagHashThread1)->dwFileHashType == tagMainWindow1.tagHashThread1.dwHashType;
	EnableWindow(tagFileListWindow1.hButton[3], nRet);
	InvalidateRect(hWnd, NULL, TRUE);
	return TRUE;
}

LRESULT FileListDlg_OnMessageClipboardCopy(HWND hWnd, WPARAM wParam, LPARAM lParam)
{
	UNREFERENCED_PARAMETER(lParam);
	static size_t nCF_TEXT[MAX_HASH_TYPE + 1] = { 0 };
	static size_t nCF_Rtf[MAX_HASH_TYPE + 1] = { 0 };
	static size_t nCF_Rtf_N[MAX_HASH_TYPE + 1] = { 0 };
	static size_t nCF_Html = 0;
	static size_t nCF_Html_N[MAX_HASH_TYPE + 1] = { 0 };
	TCHAR* pBuf = NULL;
	TCHAR* pDst = NULL;
	TCHAR szHashBuf[HASH_LOADSTRING] = _T("");
	DWORD dwHashType = tagMainWindow1.tagHashThread1.dwHashType;
	char szHashName[HASH_LOADSTRING] = "";
	char szHashString[HASH_LOADSTRING] = "";
	char* pRtfBuf = NULL;
	char* pRtf = NULL;
	char* pHtmlBuf = NULL;
	char* pHtml = NULL;
	size_t nCount = 0;
	size_t nBufSize[] = { 0, 0, 0 };
	size_t nFileNameLength[] = { 0, 0, 0 };
	TCHAR* pFilePath = szBuf2;
	int wmId = LOWORD(wParam);
	int i = 0;
	int nItem = -1;
	extern const size_t nHashColorType[MAX_HASH_TYPE + 1];
	extern const char* cpRtfColorTag[HASH_MAX_COLOR];
	extern const char* cpHtmlColorTag[HASH_MAX_COLOR];


	if (*tagFileListWindow1.tagFileListItem1.dwListSelectItem == (DWORD)-1) {
		return FALSE;
	}

	if (nCF_TEXT[0] == 0)
	{
		for (i = 0; i < MAX_HASH_TYPE; i++)
		{
			const size_t nHashLen = (size_t)HashThread_GetHashLen(i) * 2 + HashThread_GetHashNameLen(i);

			nCF_TEXT[i] = 4;
			nCF_TEXT[i] += nHashLen;

			nCF_Rtf[i] = sizeof(HASH_RTF_HEADER1) - 1;
			nCF_Rtf[i] += sizeof(HASH_RTF_HEADER2) - 1;
			nCF_Rtf[i] += sizeof(HASH_RTF_FOOTER) - 1;
			nCF_Rtf[i] += strlen(cpRtfColorTag[nHashColorType[i]]);

			nCF_Rtf_N[i] = 2;
			nCF_Rtf_N[i] += sizeof(HASH_RTF_COLOR_TAG0) - 1;
			nCF_Rtf_N[i] += sizeof(HASH_RTF_COLOR_TAG1) - 1;
			nCF_Rtf_N[i] += sizeof(HASH_RTF_RETURNCODE) - 1;
			nCF_Rtf_N[i] += nHashLen;

			nCF_Html_N[i] = 2;
			nCF_Html_N[i] += sizeof(HASH_HTML_COLOR_TAG0) - 1;
			nCF_Html_N[i] += strlen(cpHtmlColorTag[nHashColorType[i]]);
			nCF_Html_N[i] += sizeof(HASH_HTML_COLOR_TAG1) - 1;
			nCF_Html_N[i] += sizeof(HASH_HTML_COLOR_TAG2) - 1;
			nCF_Html_N[i] += sizeof(HASH_HTML_RETURNCODE) - 1;
			nCF_Html_N[i] += nHashLen;
		}

		nCF_Html = sizeof(HASH_HTML_HEADER) - 1;
		nCF_Html += sizeof(HASH_HTML_FOOTER) - 1;
	}


	if (wmId == ID_FILELIST_LISTMENU1_EXTCOPY4)
	{
		TCHAR* ptr = NULL;

		switch (dwHashFileOutToCopyFolderMode)
		{
		case 1:// 同じフォルダー
		case 2:// 1つ上のフォルダー
		case 3:// 2つ上のフォルダー
		case 4:// 3つ上のフォルダー

#if _MSC_VER > 1300
			_tcscpy_s(pFilePath, 1240, tagMainWindow1.tagHashThread1.tagMultiFile->szCurrentFolder);
#else
			_tcscpy(pFilePath, tagMainWindow1.tagHashThread1.tagMultiFile->szCurrentFolder);
#endif
			for (i = dwHashFileOutToCopyFolderMode - 1; i > 0; i--)
			{
				ptr = GetTCharToFileName(pFilePath);
				if (pFilePath == ptr) {
					break;
				}
				ptr--;
				*ptr = '\0';
			}
			break;
		default:
			*szBuf2 = '\0';
			break;
		}
	}

	while ((nItem = ListView_GetNextItem(tagFileListWindow1.hList, nItem, LVNI_SELECTED)) != -1)
	{
		const tagHashThread_FileRecode* cptagHashFile_Recode = HashThread_GetFileRecode_Core(&tagMainWindow1.tagHashThread1, nItem);
		const TCHAR* pFileName = cptagHashFile_Recode->szFileName;
		const char* pBufChar = TCharToCharConv2(pFileName, (char*)szBuf);

		nFileNameLength[0] += cptagHashFile_Recode->nFileNameLength;
		nFileNameLength[1] += CharToRtfEncodeCharLength(pBufChar);
		nFileNameLength[2] += TCharToUtf8CharLength(pFileName);
		nCount++;
	}

	if (!nCount) {
		return FALSE;
	}

	nBufSize[0] = (nCF_TEXT[dwHashType] * nCount) + 1;
	nBufSize[0] += nFileNameLength[0];

	nBufSize[1] = nCF_Rtf[dwHashType] + 1;
	nBufSize[1] += nCF_Rtf_N[dwHashType] * nCount;
	nBufSize[1] += nFileNameLength[1];

	nBufSize[2] = nCF_Html + 1;
	nBufSize[2] += nCF_Html_N[dwHashType] * nCount;
	nBufSize[2] += nFileNameLength[2];


	pBuf = (TCHAR*)malloc(nBufSize[0] * sizeof(TCHAR));
	pRtfBuf = (char*)malloc(nBufSize[1]);
	pHtmlBuf = (char*)malloc(nBufSize[2]);
	IF_UNLIKELY(pBuf == NULL || pRtfBuf == NULL || pHtmlBuf == NULL)
	{
		free(pBuf);
		free(pRtfBuf);
		free(pHtmlBuf);
		return FALSE;
	}
	*pBuf = '\0';
	pDst = pBuf;

	pRtf = qstrcpy(pRtfBuf, HASH_RTF_HEADER1);
	pRtf = qstrcpy(pRtf, cpRtfColorTag[nHashColorType[dwHashType]]);
	pRtf = qstrcpy(pRtf, HASH_RTF_HEADER2);

	pHtml = qstrcpy(pHtmlBuf, HASH_HTML_HEADER);
	nItem = -1;

	switch (wmId)
	{
	case ID_FILELIST_LISTMENU1_3:
		TCharToCharConv2(tagMainWindow1.tagHashThread1.cpHashName, szHashName);

		while ((nItem = ListView_GetNextItem(tagFileListWindow1.hList, nItem, LVNI_SELECTED)) != -1)
		{
			const tagHashThread_FileRecode* cptagHashFile_Recode = HashThread_GetFileRecode_Core(&tagMainWindow1.tagHashThread1, nItem);
			if (cptagHashFile_Recode->dwFileLastError == 0)
			{
				HashThread_GetString(szHashBuf, cptagHashFile_Recode->pFileHashByte, cptagHashFile_Recode->dwFileHashType, ~dwAppFrag & APP_HASHOUT_LOWER);
				TCharToCharConv2(szHashBuf, szHashString);

				// CF_TEXT
				pDst = qtcscpy(pDst, tagMainWindow1.tagHashThread1.cpHashName);
				pDst = qtcscpy(pDst, _T(HASH_NAME_SPACE));
				pDst = qtcscpy(pDst, szHashBuf);
				pDst = qtcscpy(pDst, _T(HASH_TEXT_RETURNCODE));

				// RTF Text
				pRtf = qstrcpy(pRtf, HASH_RTF_COLOR_TAG1);
				pRtf = qstrcpy(pRtf, szHashName);
				pRtf = qstrcpy(pRtf, HASH_RTF_COLOR_TAG0);
				pRtf = qstrcpy(pRtf, HASH_NAME_SPACE);
				pRtf = qstrcpy(pRtf, szHashString);
				pRtf = qstrcpy(pRtf, HASH_RTF_RETURNCODE);

				// HTML Text
				pHtml = qstrcpy(pHtml, HASH_HTML_COLOR_TAG0);
				pHtml = qstrcpy(pHtml, cpHtmlColorTag[nHashColorType[dwHashType]]);
				pHtml = qstrcpy(pHtml, HASH_HTML_COLOR_TAG1);
				pHtml = qstrcpy(pHtml, szHashName);
				pHtml = qstrcpy(pHtml, HASH_HTML_COLOR_TAG2);
				pHtml = qstrcpy(pHtml, HASH_NAME_SPACE);
				pHtml = qstrcpy(pHtml, szHashString);
				pHtml = qstrcpy(pHtml, HASH_HTML_RETURNCODE);
			}
		}
		break;
	case IDM_NCOPY:
	case ID_FILELIST_LISTMENU1_7:
		TCharToCharConv2(tagMainWindow1.tagHashThread1.cpHashName, szHashName);

		while ((nItem = ListView_GetNextItem(tagFileListWindow1.hList, nItem, LVNI_SELECTED)) != -1)
		{
			const tagHashThread_FileRecode* cptagHashFile_Recode = HashThread_GetFileRecode_Core(&tagMainWindow1.tagHashThread1, nItem);
			if (cptagHashFile_Recode->dwFileLastError == 0)
			{
				HashThread_GetString(szHashBuf, cptagHashFile_Recode->pFileHashByte, cptagHashFile_Recode->dwFileHashType, ~dwAppFrag & APP_HASHOUT_LOWER);

				// CF_TEXT
				pDst = qtcscpy(pDst, szHashBuf);
				pDst = qtcscpy(pDst, _T(HASH_TEXT_RETURNCODE));

				TCharToCharConv2(szHashBuf, szHashString);

				// RTF Text
				pRtf = qstrcpy(pRtf, HASH_RTF_COLOR_TAG0);
				pRtf = qstrcpy(pRtf, szHashString);
				pRtf = qstrcpy(pRtf, HASH_RTF_RETURNCODE);

				// HTML Text
				pHtml = qstrcpy(pHtml, szHashString);
				pHtml = qstrcpy(pHtml, HASH_HTML_RETURNCODE);
			}
		}
		break;
	case ID_FILELIST_LISTMENU1_EXTCOPY5:
		TCharToCharConv2(tagMainWindow1.tagHashThread1.cpHashName, szHashName);

		while ((nItem = ListView_GetNextItem(tagFileListWindow1.hList, nItem, LVNI_SELECTED)) != -1)
		{
			const tagHashThread_FileRecode* cptagHashFile_Recode = HashThread_GetFileRecode_Core(&tagMainWindow1.tagHashThread1, nItem);
			if (cptagHashFile_Recode->dwFileLastError == 0)
			{
				CharSizeToBase64ToTCharv2((char*)cptagHashFile_Recode->pFileHashByte, HashThread_GetHashLen(cptagHashFile_Recode->dwFileHashType), szHashBuf);

				// CF_TEXT
				pDst = qtcscpy(pDst, tagMainWindow1.tagHashThread1.cpHashName);
				pDst = qtcscpy(pDst, _T(HASH_NAME_SPACE));
				pDst = qtcscpy(pDst, szHashBuf);
				pDst = qtcscpy(pDst, _T(HASH_TEXT_RETURNCODE));

				TCharToCharConv2(szHashBuf, szHashString);

				// RTF Text
				pRtf = qstrcpy(pRtf, HASH_RTF_COLOR_TAG1);
				pRtf = qstrcpy(pRtf, szHashName);
				pRtf = qstrcpy(pRtf, HASH_RTF_COLOR_TAG0);
				pRtf = qstrcpy(pRtf, HASH_NAME_SPACE);
				pRtf = qstrcpy(pRtf, szHashString);
				pRtf = qstrcpy(pRtf, HASH_RTF_RETURNCODE);

				// HTML Text
				pHtml = qstrcpy(pHtml, HASH_HTML_COLOR_TAG0);
				pHtml = qstrcpy(pHtml, cpHtmlColorTag[nHashColorType[dwHashType]]);
				pHtml = qstrcpy(pHtml, HASH_HTML_COLOR_TAG1);
				pHtml = qstrcpy(pHtml, szHashName);
				pHtml = qstrcpy(pHtml, HASH_HTML_COLOR_TAG2);
				pHtml = qstrcpy(pHtml, HASH_NAME_SPACE);
				pHtml = qstrcpy(pHtml, szHashString);
				pHtml = qstrcpy(pHtml, HASH_HTML_RETURNCODE);
			}
		}
		break;
	case ID_FILELIST_LISTMENU1_4:
		while ((nItem = ListView_GetNextItem(tagFileListWindow1.hList, nItem, LVNI_SELECTED)) != -1)
		{
			char* pBuf1 = (char*)szBuf;
			char* pBuf2 = (char*)szBuf + 4096;
			const tagHashThread_FileRecode* cptagHashFile_Recode = HashThread_GetFileRecode_Core(&tagMainWindow1.tagHashThread1, nItem);
			pDst = qtcscpy(pDst, cptagHashFile_Recode->szFileName);
			pDst = qtcscpy(pDst, _T(HASH_TEXT_RETURNCODE));

			// RTF Text
			TCharToCharConv2(cptagHashFile_Recode->szFileName, pBuf1);
			CharToRtfEncodeChar(pBuf1, pBuf2);
			pRtf = qstrcpy(pRtf, HASH_RTF_COLOR_TAG0);
			pRtf = qstrcpy(pRtf, pBuf1);
			pRtf = qstrcpy(pRtf, HASH_RTF_RETURNCODE);

			// HTML Text
			TCharToUtf8CharConv2(cptagHashFile_Recode->szFileName, pBuf1);
			pHtml = qstrcpy(pHtml, pBuf1);
			pHtml = qstrcpy(pHtml, HASH_HTML_RETURNCODE);
		}
		break;
	case ID_FILELIST_LISTMENU1_EXTCOPY1:
		while ((nItem = ListView_GetNextItem(tagFileListWindow1.hList, nItem, LVNI_SELECTED)) != -1)
		{
			char* pBuf1 = (char*)szBuf;
			char* pBuf2 = (char*)szBuf + 4096;
			const tagHashThread_FileRecode* cptagHashFile_Recode = HashThread_GetFileRecode_Core(&tagMainWindow1.tagHashThread1, nItem);
			const TCHAR* pFileName = GetTCharToFileName(cptagHashFile_Recode->szFileName);

			pDst = qtcscpy(pDst, pFileName);
			pDst = qtcscpy(pDst, _T(HASH_TEXT_RETURNCODE));

			// RTF Text
			TCharToCharConv2(pFileName, pBuf1);
			CharToRtfEncodeChar(pBuf1, pBuf2);
			pRtf = qstrcpy(pRtf, HASH_RTF_COLOR_TAG0);
			pRtf = qstrcpy(pRtf, pBuf1);
			pRtf = qstrcpy(pRtf, HASH_RTF_RETURNCODE);

			// HTML Text
			TCharToUtf8CharConv2(pFileName, pBuf1);
			pHtml = qstrcpy(pHtml, pBuf1);
			pHtml = qstrcpy(pHtml, HASH_HTML_RETURNCODE);
		}
		break;
	case ID_FILELIST_LISTMENU1_EXTCOPY2:
		while ((nItem = ListView_GetNextItem(tagFileListWindow1.hList, nItem, LVNI_SELECTED)) != -1)
		{
			char* pBuf1 = (char*)szBuf;
			char* pBuf2 = (char*)szBuf + 4096;
			const tagHashThread_FileRecode* cptagHashFile_Recode = HashThread_GetFileRecode_Core(&tagMainWindow1.tagHashThread1, nItem);
			qtcscpy(tagFileListWindow1.pStBuf, cptagHashFile_Recode->szFileName);
			PathRemoveFileSpec(tagFileListWindow1.pStBuf);

			pDst = qtcscpy(pDst, tagFileListWindow1.pStBuf);
			pDst = qtcscpy(pDst, _T(HASH_TEXT_RETURNCODE));

			TCharToCharConv2(tagFileListWindow1.pStBuf, pBuf1);

			// RTF Text
			CharToRtfEncodeChar(pBuf1, pBuf2);
			pRtf = qstrcpy(pRtf, HASH_RTF_COLOR_TAG0);
			pRtf = qstrcpy(pRtf, pBuf1);
			pRtf = qstrcpy(pRtf, HASH_RTF_RETURNCODE);

			// HTML Text
			TCharToUtf8CharConv2(tagFileListWindow1.pStBuf, pBuf1);
			pHtml = qstrcpy(pHtml, pBuf1);
			pHtml = qstrcpy(pHtml, HASH_HTML_RETURNCODE);
		}
		break;
	case ID_FILELIST_LISTMENU1_EXTCOPY3:
		while ((nItem = ListView_GetNextItem(tagFileListWindow1.hList, nItem, LVNI_SELECTED)) != -1)
		{
			char* pBuf1 = (char*)szBuf;
			char* pBuf2 = (char*)szBuf + 4096;
			const tagHashThread_FileRecode* cptagHashFile_Recode = HashThread_GetFileRecode_Core(&tagMainWindow1.tagHashThread1, nItem);
			FileSizeText2ToTChar2(cptagHashFile_Recode->llFileSize, szHashBuf);

			pDst = qtcscpy(pDst, szHashBuf);
			pDst = qtcscpy(pDst, _T(HASH_TEXT_RETURNCODE));

			// RTF Text
			TCharToCharConv2(szHashBuf, pBuf1);
			CharToRtfEncodeChar(pBuf1, pBuf2);
			pRtf = qstrcpy(pRtf, HASH_RTF_COLOR_TAG0);
			pRtf = qstrcpy(pRtf, pBuf2);
			pRtf = qstrcpy(pRtf, HASH_RTF_RETURNCODE);

			// HTML Text
			TCharToUtf8CharConv2(szHashBuf, pBuf1);
			pHtml = qstrcpy(pHtml, pBuf1);
			pHtml = qstrcpy(pHtml, HASH_HTML_RETURNCODE);
		}
		break;
	case ID_FILELIST_LISTMENU1_EXTCOPY4:
		while ((nItem = ListView_GetNextItem(tagFileListWindow1.hList, nItem, LVNI_SELECTED)) != -1)
		{
			const tagHashThread_FileRecode* cptagHashFile_Recode = HashThread_GetFileRecode_Core(&tagMainWindow1.tagHashThread1, nItem);

			if (cptagHashFile_Recode->dwFileLastError == 0)
			{
				char* pBuf1 = (char*)szBuf;
				char* pBuf2 = (char*)szBuf + 4096;
				TCHAR* pFileName;

				if (dwHashFileOutToCopyFolderMode == 0) {
					pFileName = (TCHAR*)cptagHashFile_Recode->szFileName;
				}
				else
				{
					pFileName = szBuf3;
					if (!PathRelativePathTo(pFileName, pFilePath, FILE_ATTRIBUTE_DIRECTORY, cptagHashFile_Recode->szFileName, FILE_ATTRIBUTE_ARCHIVE)) {
						pFileName = (TCHAR*)cptagHashFile_Recode->szFileName;
					}
				}

				HashThread_GetString(szHashBuf, cptagHashFile_Recode->pFileHashByte, cptagHashFile_Recode->dwFileHashType, ~dwAppFrag & APP_HASHOUT_LOWER);
				pDst = qtcscpy(pDst, szHashBuf);
				pDst = qtcscpy(pDst, _T(HASH_TEXT_SPACE));
				pDst = qtcscpy(pDst, pFileName);
				pDst = qtcscpy(pDst, _T(HASH_TEXT_RETURNCODE));

				TCharToCharConv2(szHashBuf, szHashString);

				// RTF Text
				TCharToCharConv2(pFileName, pBuf1);
				CharToRtfEncodeChar(pBuf1, pBuf2);
				pRtf = qstrcpy(pRtf, HASH_RTF_COLOR_TAG0);
				pRtf = qstrcpy(pRtf, szHashString);
				pRtf = qstrcpy(pRtf, HASH_TEXT_SPACE);
				pRtf = qstrcpy(pRtf, pBuf2);
				pRtf = qstrcpy(pRtf, HASH_RTF_RETURNCODE);

				// HTML Text
				TCharToUtf8CharConv2(pFileName, pBuf1);
				pHtml = qstrcpy(pHtml, szHashString);
				pHtml = qstrcpy(pHtml, HASH_TEXT_SPACE);
				pHtml = qstrcpy(pHtml, pBuf1);
				pHtml = qstrcpy(pHtml, HASH_HTML_RETURNCODE);
			}
		}
		break;
	}

	//改行コードの削除
	if (pDst - pBuf >= 2) {
		pDst -= 2;
	}
	*pDst = '\0';

	pRtf -= sizeof(HASH_RTF_RETURNCODE) - 1;
	pRtf = qstrcpy(pRtf, HASH_RTF_FOOTER);

	pHtml = qstrcpy(pHtml, HASH_HTML_FOOTER);

	SetClipboardText2(hWnd, pBuf, pRtfBuf, pHtmlBuf);
	free(pBuf);
	free(pRtfBuf);
	free(pHtmlBuf);
	return TRUE;
}

VOID FileListDlg_GetDpi(VOID)
{
	int i = 0;

	ImageList_Destroy(tagFileListWindow1.hListViewImg);
	for (i = 0; i < SIZEOF_NUM(tagFileListWindow1.hListViewIcon); i++) {
		DeleteObject(tagFileListWindow1.hListViewIcon[i]);
	}

	tagFileListWindow1.hListViewImg = ImageList_Create((int)(16. * tagMainWindow1.dDpiSize), (int)(16. * tagMainWindow1.dDpiSize), ILC_COLOR32 | ILC_MASK, 3, 0);
	ListView_SetImageList(tagFileListWindow1.hList, tagFileListWindow1.hListViewImg, LVSIL_SMALL);
	for (i = 0; i < SIZEOF_NUM(tagFileListWindow1.hListViewIcon); i++)
	{
		tagFileListWindow1.hListViewIcon[i] = LoadIcon(tagMainWindow1.hDllInst, MAKEINTRESOURCE(IDI_FILELIST_ICON1 + (size_t)i));
		ImageList_AddIcon(tagFileListWindow1.hListViewImg, tagFileListWindow1.hListViewIcon[i]);
	}
	SendMessage(tagFileListWindow1.hList, WM_SETFONT, (WPARAM)tagMainWindow1.hFont[0], 0);
	SendMessage(tagFileListWindow1.hGroup[0], WM_SETFONT, (WPARAM)tagMainWindow1.hFont[0], 0);
	for (i = 0; i < SIZEOF_NUM(tagFileListWindow1.hButton); i++) {
		SendMessage(tagFileListWindow1.hButton[i], WM_SETFONT, (WPARAM)tagMainWindow1.hFont[0], 0);
	}
	for (i = 0; i < SIZEOF_NUM(tagFileListWindow1.hRadio); i++) {
		SendMessage(tagFileListWindow1.hRadio[i], WM_SETFONT, (WPARAM)tagMainWindow1.hFont[0], 0);
	}
	FileListDlg_SetWindowMove(FALSE);
	return;
}

int FileListDlg_MessageBox(HWND hWnd, LPCTSTR lpText, LPCTSTR lpCaption, UINT uType)
{
	int nRet = 0;

	// ウィンドウロックします。
	FILELIST_WINDOW_LOCK;

	nRet = MessageBox(hWnd, lpText, lpCaption, uType);

	// ウィンドウロックの解除します。
	FILELIST_WINDOW_UNLOCK;

	return nRet;
}

VOID FileListDlg_SetHashType(VOID)
{
	FileListDlg_SetWindowMove(FALSE);
	return;
}

VOID FileListDlg_SetWindowMove(BOOL nIsDefaultMove)
{
	WINDOWPLACEMENT wndPlace = { 0 };
	RECT rc = { 0 };
	const int nDefWindowSize = nHashStringWidth[0];
	const int nSetWindowSize = nHashStringWidth[tagMainWindow1.tagHashThread1.dwHashType];
	const int nDiffWindowSize = nHashStringWidth[tagMainWindow1.dwHashTypeBack];
	int nSetWindowRectX = MDIVX(FILELISTDLG_X) + nSetWindowSize - nDefWindowSize;
	int nDiffWindowRectX = MDIVX(FILELISTDLG_X) + nDiffWindowSize - nDefWindowSize;

	IF_UNLIKELY(tagFileListWindow1.hWnd == NULL) {
		return;
	}

	tagFileListWindow1.dwDialogRect = nSetWindowSize - nDefWindowSize;

	wndPlace.length = sizeof(WINDOWPLACEMENT);
	GetWindowPlacement(tagFileListWindow1.hWnd, &wndPlace);
	switch (wndPlace.showCmd)
	{
	case SW_SHOWMAXIMIZED:
		rc = wndPlace.rcNormalPosition;
		break;
	default:
		GetWindowRect(tagFileListWindow1.hWnd, &rc);
	}

	if (nIsDefaultMove)
	{
		rc.bottom = rc.bottom - rc.top;
		rc.top = 0;
		rc.right = rc.right - rc.left;
		rc.left = 0;
		nSetWindowRectX = rc.right;
	}
	nDiffWindowRectX = rc.right - rc.left - nDiffWindowRectX;

#ifdef _DEBUG
	OutputDebugString(_T("FileListDlg: FileListDlg_SetWindowMove()\r\n"));
	STPRINTF_FUNC(szDebugText, _T("nSetWindowRectX = %d;\r\nrc = { %d, %d, %d, %d };\r\nnDiffWindowRectX = %d;\r\n"), nSetWindowRectX, rc.left, rc.top, rc.right, rc.bottom, nDiffWindowRectX);
	OutputDebugString(szDebugText);
#endif

	MoveWindow(tagFileListWindow1.hWnd, rc.left, rc.top, nSetWindowRectX + nDiffWindowRectX, rc.bottom - rc.top, TRUE);
	return;
}

DWORD FileListDlg_DropFile(HDROP hdropFile)
{
	DWORD dwRet = (DWORD)-1;
	DWORD dwCount = 0;
	DWORD dwFileCount = HashThread_GetCountItem_Core(&tagMainWindow1.tagHashThread1);
	BOOL nIsHashFileRet = 0;
	unsigned int i = 0;

	tagMainWindow1.bIsEmptyFolder = 0;
	dwCount = DragQueryFile(hdropFile, 0xFFFFFFFF, NULL, NULL);

	for (i = 0; i < dwCount; i++)
	{
		IF_UNLIKELY(dwFileCount >= FILE_MAX_COUNTSIZE)
		{
			LoadString(tagMainWindow1.hInst, IDS_OVERFILE1, tagFileListWindow1.pStBuf, MAX_STRINGTABLE);
			MessageFormat(tagFileListWindow1.pBuf, 1024, tagFileListWindow1.pStBuf, tagMainWindow1.pFile);
			FileListDlg_MessageBox(tagFileListWindow1.hWnd, tagFileListWindow1.pBuf, tagMainWindow1.pTitle, MB_ICONHAND);
			dwRet = FILE_MAX_COUNTSIZE;
			break;
		}

		dwRet = DragQueryFile(hdropFile, i, tagMainWindow1.pFile, MAX_PATH_SIZE - 1);
		IF_LIKELY(dwRet)
		{
			tagMainWindow1.tagHashThread1.nIsFileNoCheck = 0;
			nIsHashFileRet = GetHashFilePath();

			if (dwAppFrag & APP_FOLDEROPEN) {
				dwRet = FileListDlg_AddFolder();
			}
			else if (nIsHashFileRet != FALSE) {
				dwRet = FileListDlg_AddHashFile();
			}
			else {
				dwRet = FileListDlg_AddFile();
			}

			IF_UNLIKELY(dwRet == (DWORD)-1) {
				break;
			}
			dwFileCount = HashThread_GetCountItem_Core(&tagMainWindow1.tagHashThread1);
		}
	}

	if (dwFileCount > 0) {
		qtcscpy(tagMainWindow1.pOfn1Buf, HashThread_GetFileRecode_Core(&tagMainWindow1.tagHashThread1, dwFileCount - 1)->szFileName);
	}

	DragFinish(hdropFile);
	return dwRet;
}

VOID FileListDlg_FileMenu(HWND hWnd, DWORD dwItem, POINT *pt)
{
	DWORD dwMenuState = 0;
	MENUITEMINFO mii = { sizeof(MENUITEMINFO), MIIM_ID | MIIM_STATE, 0, MFS_DEFAULT, ID_FILELIST_LISTMENU1_1, 0 };
	const tagHashThread_FileRecode* cptagHashFile_Recode = HashThread_GetFileRecode_Core(&tagMainWindow1.tagHashThread1, dwItem);

	SetMenuItemInfo(tagFileListWindow1.hPupMenu[0], ID_FILELIST_LISTMENU1_1, FALSE, &mii);
	dwMenuState = dwAppFrag & APP_HASHTHREAD || (tagMainWindow1.dwAddFileModeFileCount != 0 && dwItem < tagMainWindow1.dwAddFileModeFileCount) ? (MF_GRAYED) : (MF_ENABLED);
	EnableMenuItem(tagFileListWindow1.hPupMenu[0], ID_FILELIST_LISTMENU1_5, dwMenuState);
	dwMenuState = cptagHashFile_Recode->dwFileLastError == 0 ? MF_ENABLED : MF_GRAYED;
	EnableMenuItem(tagFileListWindow1.hPupMenu[0], ID_FILELIST_LISTMENU1_3, dwMenuState);
	EnableMenuItem(tagFileListWindow1.hPupMenu[0], ID_FILELIST_LISTMENU1_7, dwMenuState);
	EnableMenuItem(tagFileListWindow1.hPupMenu[0], ID_MAINSUBMENU1_EXTCOPY4, dwMenuState);
	EnableMenuItem(tagFileListWindow1.hPupMenu[0], ID_MAINSUBMENU1_EXTCOPY5, dwMenuState);
	EnableMenuItem(tagFileListWindow1.hPupMenu[0], 6, MF_BYPOSITION | dwMenuState);

	switch (cptagHashFile_Recode->dwFileLastError)
	{
	case ERROR_FILE_NOT_FOUND:
	case ERROR_PATH_NOT_FOUND:
	case ERROR_TOO_MANY_OPEN_FILES:
		dwMenuState = (MF_GRAYED);
		break;
	default:
		dwMenuState = (MF_ENABLED);
	}

	EnableMenuItem(tagFileListWindow1.hPupMenu[0], ID_FILELIST_LISTMENU1_6, dwMenuState);
	FileListDlg_MenuShow(hWnd, tagFileListWindow1.hPupMenu[0], pt);
	return;
}

VOID FileListDlg_NoFileMenu(HWND hWnd, POINT *pt)
{
	DWORD dwClipCopy = 0;
	DWORD dwMenuState = 0;
	DWORD dwFileCount = HashThread_GetCountItem_Core(&tagMainWindow1.tagHashThread1);

	if (dwAppFrag & APP_HASHTHREAD)
	{
		MessageBeep(MB_ICONERROR);
		return;
	}

	dwMenuState = MF_ENABLED;
	EnableMenuItem(tagFileListWindow1.hPupMenu[1], ID_FILELIST_LISTMENU2_1, dwMenuState);
	EnableMenuItem(tagFileListWindow1.hPupMenu[1], ID_FILELIST_LISTMENU2_2, dwMenuState);
	EnableMenuItem(tagFileListWindow1.hPupMenu[1], ID_FILELIST_LISTMENU2_3, dwMenuState);

	dwMenuState = dwFileCount == 0 ? (MF_GRAYED) : (MF_ENABLED);
	EnableMenuItem(tagFileListWindow1.hPupMenu[1], ID_FILELIST_LISTMENU2_5, dwMenuState);

	if (OpenClipboard(tagFileListWindow1.hWnd))
	{
		HDROP hdropFile = NULL;

		hdropFile = (HDROP)GetClipboardData(CF_HDROP);
		if (hdropFile != NULL) {
			dwClipCopy = 1;
		}
		CloseClipboard();
	}
	dwMenuState = dwClipCopy == 0 ? (MF_GRAYED) : (MF_ENABLED);
	EnableMenuItem(tagFileListWindow1.hPupMenu[1], ID_FILELIST_LISTMENU2_4, dwMenuState);
	FileListDlg_MenuShow(hWnd, tagFileListWindow1.hPupMenu[1], pt);
	return;
}

VOID FileListDlg_MenuShow(HWND hWnd, HMENU hMenu, POINT *pt)
{
	DrawMenuBar(hWnd);
	TrackPopupMenu(hMenu, TPM_LEFTALIGN | TPM_LEFTBUTTON, pt->x, pt->y, 0, hWnd, NULL);
	return;
}

VOID FileListDlg_FileDelete(VOID)
{
	DWORD dwCount = 0;
	DWORD dwSelectItem = 0;
	DWORD dwAddModeFileCount = 0;
	DWORD dwFileCount = HashThread_GetCountItem_Core(&tagMainWindow1.tagHashThread1);

	for (dwCount = tagFileListWindow1.tagFileListItem1.dwListSelectCount; dwCount > 0; dwCount--)
	{
		dwSelectItem = *(tagFileListWindow1.tagFileListItem1.dwListSelectItem + dwCount - 1);

		if (tagMainWindow1.dwAddFileModeFileCount != 0 && dwSelectItem < tagMainWindow1.dwAddFileModeFileCount) {
			dwAddModeFileCount++;
		}
		else
		{
			ListView_DeleteItem(tagFileListWindow1.hList, dwSelectItem);
			HashThread_DeleteItemBuffer(&tagMainWindow1.tagHashThread1, dwSelectItem);
		}
	}

	if (dwAddModeFileCount > 0)
	{
		LoadString(tagMainWindow1.hInst, IDS_FILELIST_DELETE1, tagFileListWindow1.pStBuf, MAX_STRINGTABLE);
		FileListDlg_MessageBox(tagFileListWindow1.hWnd, tagFileListWindow1.pStBuf, tagMainWindow1.pTitle, MB_ICONHAND);
	}

	if (dwFileCount == 0) {
		FileListDlg_Clear(TRUE, TRUE);
	}
	else {
		dwAppFrag |= APP_FILELISTBOX_EDIT;
	}

	if (tagMainWindow1.dwOpenItem >= dwFileCount != 0) {
		tagMainWindow1.dwOpenItem = dwFileCount - 1;
	}
	return;
}

LRESULT CALLBACK FileListDlg_SortProc(LPARAM lParam1, LPARAM lParam2, LPARAM lParam3)
{
	NMLISTVIEW* pNMLV = (NMLISTVIEW*)lParam3;
	int SubItem = pNMLV->iSubItem;
	int nItemScore1 = 0;
	int nItemScore2 = 0;
	__int64 llRet = 0;
	__int64 llItem1 = 0;
	__int64 llItem2 = 0;
	static LV_FINDINFO lvf = { 0 };
	static TCHAR szText1[MAX_LOADSTRING] = { 0 };
	static TCHAR szText2[MAX_LOADSTRING] = { 0 };
	static size_t nItem1;
	static size_t nItem2;
	__int64 nRet = 0;
	tagHashThread_FileRecode* ptagHashThread_Item1 = NULL;
	tagHashThread_FileRecode* ptagHashThread_Item2 = NULL;

#ifdef _DEBUG
	TCHAR* szSubItem[] = { (TCHAR*)_T("ファイル名"), (TCHAR*)_T("サイズ"), (TCHAR*)_T("Hash")};
#endif

	lvf.flags  = LVFI_PARAM;
	lvf.lParam = lParam1;
	nItem1 = ListView_FindItem(tagFileListWindow1.hList, -1, &lvf);

	lvf.lParam = lParam2;
	nItem2 = ListView_FindItem(tagFileListWindow1.hList, -1, &lvf);

	ptagHashThread_Item1 = HashThread_GetFileRecode_Core(&tagMainWindow1.tagHashThread1, (DWORD)nItem1);
	ptagHashThread_Item2 = HashThread_GetFileRecode_Core(&tagMainWindow1.tagHashThread1, (DWORD)nItem2);

#ifdef _DEBUG
	STPRINTF_FUNC(szDebugText, _T("FileList: File_SortProc(): ソートの項目[%s]: Item1[%lld(%lld)], Item2[%lld(%lld)]\r\n"), szSubItem[SubItem], (unsigned __int64)lParam1, (unsigned __int64)nItem1, (unsigned __int64)lParam2, (unsigned __int64)nItem2);
	OutputDebugString(szDebugText);
#endif

	if (ptagHashThread_Item1->dwHashFileHashLine != HASHFILE_NOLINE) {
		nItemScore1 += ptagHashThread_Item1->nHashFileCmp == 0 ? 10000 : 1000;
	}

	if (ptagHashThread_Item2->dwHashFileHashLine != HASHFILE_NOLINE) {
		nItemScore2 += ptagHashThread_Item2->nHashFileCmp == 0 ? 10000 : 1000;
	}

	if (ptagHashThread_Item1->dwFileLastError == 0) {
		nItemScore1 += 100;
	}
	else
	{
		if (ptagHashThread_Item2->dwFileLastError == (DWORD)-1 || ptagHashThread_Item1->dwFileLastError > ptagHashThread_Item2->dwFileLastError) {
			nItemScore1 += 10;
		}
	}
	if (ptagHashThread_Item2->dwFileLastError == 0) {
		nItemScore2 += 100;
	}
	else
	{
		if (ptagHashThread_Item1->dwFileLastError == (DWORD)-1 || ptagHashThread_Item2->dwFileLastError > ptagHashThread_Item1->dwFileLastError) {
			nItemScore2 += 10;
		}
	}

	switch (SubItem)
	{
	case 0:
	case 2:
	case 3:
		ListView_GetItemText(tagFileListWindow1.hList, nItem1, SubItem, szText1, sizeof(szText1));
		szText1[MAX_LOADSTRING - 1] = '\0';
		ListView_GetItemText(tagFileListWindow1.hList, nItem2, SubItem, szText2, sizeof(szText2));
		szText2[MAX_LOADSTRING - 1] = '\0';

#ifdef _DEBUG
		STPRINTF_FUNC(szDebugText, _T("FileList: File_SortProc(): 比較文字列: \"%10s\", \"%10s\"\r\n"), szText1, szText2);
		OutputDebugString(szDebugText);
#endif

		nRet = _tcscmp(szText1, szText2);

		if (nRet > 0)
		{
			nItemScore1--;
			nItemScore2++;
		}
		else if (nRet < 0)
		{
			nItemScore1++;
			nItemScore2--;
		}
		break;
	case 1:
		llItem1 = ptagHashThread_Item1->llFileSize;
		llItem2 = ptagHashThread_Item2->llFileSize;

		if (llItem1 != llItem2) {
			llRet = llItem1 - llItem2;
		}

#ifdef _DEBUG
		STPRINTF_FUNC(szDebugText, _T("FileList: File_SortProc(): 比較数値: %lld B, %lld B\r\n"), llItem1, llItem2);
		OutputDebugString(szDebugText);
#endif

		if (llRet > 0)
		{
			nItemScore1--;
			nItemScore2++;
		}
		else if (llRet < 0)
		{
			nItemScore1++;
			nItemScore2--;
		}
		break;
	}

	if (nItemScore1 != nItemScore2) {
		nRet = (__int64)nItemScore1 - nItemScore2;
	}

	if (nFileSortSubNo[SubItem]) {
		nRet *= -1;
	}

#ifdef _DEBUG
	STPRINTF_FUNC(szDebugText, _T("FileList: FileListDlg_SortProc(): ソート結果[%2s]: %I64d\r\n"), nFileSortSubNo[SubItem] ? _T("昇順") : _T("降順"), nRet);
	OutputDebugString(szDebugText);
#endif

	return (int)nRet;
}

VOID FileListDlg_Init(VOID)
{
	tagFileListWindow1.hWnd = CreateDialog(tagMainWindow1.hInst, MAKEINTRESOURCE(IDD_FILELIST), tagMainWindow1.hWnd, (DLGPROC)FileListDlg_WndProc);
	return;
}

VOID FileListDlg(VOID)
{
	if (tagFileListWindow1.hWnd == NULL)
	{
		memset(&nFileSortSubNo, 1, sizeof(nFileSortSubNo));
		FileListDlg_Init();

		if (tagFileListWindow1.hWnd != NULL)
		{

#ifdef _DEBUG
			OutputDebugString(_T("FileList: FileListDlg()\r\n"));
#endif
			ShowWindow(tagFileListWindow1.hWnd, SW_SHOW);
			UpdateWindow(tagFileListWindow1.hWnd);

		}
	}
	else {
		SetForegroundWindow(tagFileListWindow1.hWnd);
	}
	return;
}

VOID FileListDlg_ListView(DWORD dwStart, BOOL nIsDelete)
{
	DWORD dwCount = 0;
	DWORD dwListCount = ListView_GetItemCount(tagFileListWindow1.hList);
	DWORD dwFileCount = HashThread_GetCountItem_Core(&tagMainWindow1.tagHashThread1);;

	if (dwFileCount == 0 || dwFileCount > tagFileListWindow1.tagFileListItem1.dwListBufSize)
	{
		DWORD ___dwFileCount = dwFileCount;
		DWORD ___dwBufSize = tagFileListWindow1.tagFileListItem1.dwListBufSize;
		DWORD* ___dwNewItem = NULL;
		DWORD* ___dwOldItem = tagFileListWindow1.tagFileListItem1.dwListSelectItem;

		if (tagMainWindow1.tagHashThread1.tagMultiFile->dwMaxBufSize > FILE_MAX_COUNTSIZE)
		{
			tagMainWindow1.tagHashThread1.tagMultiFile->dwMaxBufSize = dwListCount = FILE_MAX_COUNTSIZE;
			if (dwStart > FILE_MAX_COUNTSIZE) {
				dwStart = FILE_MAX_COUNTSIZE - 1;
			}
		}
		else
		{
			if (___dwBufSize == 0)
			{
				___dwBufSize = FILE_DEF_COUNTSIZE;
				tagFileListWindow1.tagFileListItem1.dwListSelectItem = ___dwOldItem = NULL;
			}

			// 大きめにバッファのサイズを計算する
			while (___dwBufSize < ___dwFileCount)
			{
				if (___dwBufSize < 4096) {
					___dwBufSize *= FILE_DEF_COUNTSIZE;
				}
				else {
					___dwBufSize *= 2;
				}
			}

			___dwNewItem = (DWORD*)realloc(___dwOldItem, ___dwBufSize * sizeof(DWORD));
			IF_UNLIKELY(___dwNewItem == NULL)
			{
				free(___dwOldItem);
				LoadString(tagMainWindow1.hInst, IDS_FILELIST_MALLOC_FAIL, tagFileListWindow1.pStBuf, MAX_STRINGTABLE);
				MessageFormat(tagFileListWindow1.pBuf, MAX_STRINGTABLE, tagFileListWindow1.pStBuf, _T("tagFileListWindow1.tagFileListItem1.dwListSelectItem"));
				FileListDlg_MessageBox(tagFileListWindow1.hWnd, tagFileListWindow1.pBuf, tagMainWindow1.pTitle, MB_ICONHAND);
				if (tagFileListWindow1.hWnd != NULL) {
					DestroyWindow(tagFileListWindow1.hWnd);
				}
				return;
			}
			tagFileListWindow1.tagFileListItem1.dwListSelectItem = ___dwNewItem;
			tagFileListWindow1.tagFileListItem1.dwListBufSize = ___dwBufSize;
		}
	}

	// リストの削除
	if (nIsDelete != 0)
	{
		ListView_DeleteAllItems(tagFileListWindow1.hList);
		dwStart = 0;
	}

	for (dwCount = dwStart; dwCount < dwFileCount; dwCount++) {
		FileListDlg_SetListView(dwCount, (dwCount + 1) > dwListCount ? TRUE : FALSE);
	}

	if (HashThread_GetCurrentItem_Core(&tagMainWindow1.tagHashThread1) > 0)
	{
		const DWORD dwI = dwFileCount - 1;
		ListView_EnsureVisible(tagFileListWindow1.hList, dwI, FALSE);
	}
	LoadString(tagMainWindow1.hInst, IDS_FILELIST_STATUS_TEXT1, tagFileListWindow1.pStBuf, MAX_STRINGTABLE);
	MessageFormat(tagFileListWindow1.pBuf, 1024, tagFileListWindow1.pStBuf, dwFileCount);
	SendMessage(tagFileListWindow1.hStatus, SB_SETTEXT, SB_SIMPLEID, (WPARAM)tagFileListWindow1.pBuf);
	return;
}

VOID FileListDlg_SetListView(DWORD dwItem, BOOL nIsAdd)
{
	LV_ITEM lvItem = { 0 };
	const tagHashThread_FileRecode* cptagHashFile_Recode = HashThread_GetFileRecode_Core(&tagMainWindow1.tagHashThread1, dwItem);

	lvItem.mask = LVIF_TEXT | LVIF_IMAGE | LVIF_PARAM;
	lvItem.pszText = GetTCharToFileName(cptagHashFile_Recode->szFileName);
	lvItem.iItem = dwItem;
	lvItem.lParam = dwItem;

	if (nIsAdd != FALSE) {
		ListView_InsertItem(tagFileListWindow1.hList, &lvItem);
	}
	else {
		ListView_SetItemText(tagFileListWindow1.hList, dwItem, lvItem.iSubItem, lvItem.pszText);
	}

	STRFORMATKBSIZE_MACRO(cptagHashFile_Recode->llFileSize, tagFileListWindow1.pBuf, MAX_LOADSTRING);
	lvItem.mask = LVIF_TEXT;
	lvItem.pszText = tagFileListWindow1.pBuf;
	lvItem.iItem = dwItem;
	lvItem.iSubItem = 1;
	ListView_SetItem(tagFileListWindow1.hList, &lvItem);

	if (dwItem <= HashThread_GetCurrentItem_Core(&tagMainWindow1.tagHashThread1) && cptagHashFile_Recode->dwFileLastError != (DWORD)-1)
	{
		if (cptagHashFile_Recode->dwFileLastError == 0)
		{
			TCHAR szHashBuf[HASH_LOADSTRING];

			HashThread_GetString(szHashBuf, cptagHashFile_Recode->pFileHashByte, cptagHashFile_Recode->dwFileHashType, ~dwAppFrag & APP_HASHOUT_LOWER);
			MessageFormat(tagFileListWindow1.pFileChangeHash, 1024, _T("%1: %2"), HashThread_GetHashName(cptagHashFile_Recode->dwFileHashType), szHashBuf);
		}
		else {
			GetLastError_String(tagFileListWindow1.pFileChangeHash, cptagHashFile_Recode->dwFileLastError);
		}
		lvItem.pszText = tagFileListWindow1.pFileChangeHash;
		lvItem.iSubItem = 2;
		ListView_SetItem(tagFileListWindow1.hList, &lvItem);
	}

	if (nIsAdd == FALSE) {
		ListView_Update(tagFileListWindow1.hList, dwItem);
	}
	return;
}

VOID FileListDlg_Clear(BOOL nIsFilePathClear, BOOL nIsClearFunc)
{
	dwAppFrag &= ~APP_FILELISTBOX_EDIT;

	if (nIsClearFunc) {
		Clear(FALSE);
	}

	tagFileListWindow1.dwNewFile = 0;
	tagFileListWindow1.dwFileCount = 0;
	tagFileListWindow1.nIsListLock = TRUE;
	ListView_DeleteAllItems(tagFileListWindow1.hList);
	tagFileListWindow1.nIsListLock = FALSE;
	LoadString(tagMainWindow1.hInst, IDS_FILELIST_STATUS_TEXT1, tagFileListWindow1.pStBuf, MAX_STRINGTABLE);
	MessageFormat(tagFileListWindow1.pBuf, 1024, tagFileListWindow1.pStBuf, 0);
	SendMessage(tagFileListWindow1.hStatus, SB_SETTEXT, SB_SIMPLEID, (WPARAM)tagFileListWindow1.pBuf);
	EnableWindow(tagFileListWindow1.hButton[0], FALSE);
	EnableWindow(tagFileListWindow1.hButton[1], FALSE);
	EnableWindow(tagFileListWindow1.hButton[2], TRUE);
	EnableWindow(tagFileListWindow1.hRadio[0], TRUE);
	EnableWindow(tagFileListWindow1.hRadio[1], TRUE);
	EnableWindow(tagFileListWindow1.hRadio[2], TRUE);
	EnableWindow(tagFileListWindow1.hButton[3], FALSE);
	SetFocus(tagFileListWindow1.hButton[2]);

	if (nIsFilePathClear != 0)
	{
		*tagMainWindow1.pHashFile = '\0';
		*tagMainWindow1.pFile = '\0';
	}
	return;
}

VOID FileListDlg_FileOpen(VOID)
{
	// ウィンドウロックします。
	FILELIST_WINDOW_LOCK;

	// 最初のファイル名以降はゼロクリアする。
	qtcscpy(tagMainWindow1.pTempBuf, tagMainWindow1.pOfn1Buf);
	memset(tagMainWindow1.pOfn1Buf, NULL, (MAX_PATH_SIZE * 256) * sizeof(TCHAR));
	qtcscpy(tagMainWindow1.pOfn1Buf, tagMainWindow1.pTempBuf);

	if (GetOpenFileName(&tagMainWindow1.ofnFileName[0]))
	{
		TCHAR* pFileTextPos = NULL; 
		BOOL nIsHashFileRet = 0;
		DWORD dwErrCount = 0;//ファイルのパスが長いと _tfullpath() に失敗してこの値が増えていきます。

		// OpenFileNameの解析
		pFileTextPos = tagMainWindow1.pOfn1Buf + tagMainWindow1.ofnFileName[0].nFileOffset;

		while (1)
		{
			tagMainWindow1.tagHashThread1.nIsFileNoCheck = 0;
			if (_tfullpath(tagMainWindow1.pFile, pFileTextPos, MAX_PATH_SIZE) != NULL)
			{
				nIsHashFileRet = GetHashFilePath();
				if (nIsHashFileRet != FALSE) {
					FileListDlg_AddHashFile();
				}
				else {
					FileListDlg_AddFile();
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
		}

		if (dwErrCount == 0)
		{
			tagHashThread_FileRecode* ptagHashFile_Recode = HashThread_GetFileRecode_Core(&tagMainWindow1.tagHashThread1, HashThread_GetCountItem_Core(&tagMainWindow1.tagHashThread1) - 1);

#ifndef _NODLL
			// 複数選択時に次回のフォルダー表示がおかしくならないように最後のファイルにしておきます。
			_tcscpy_s(tagMainWindow1.pOfn1Buf, MAX_PATH_SIZE, ptagHashFile_Recode->szFileName);
			_tcscpy_s(tagMainWindow1.pFile, MAX_PATH_SIZE, tagMainWindow1.pOfn1Buf);
#else
			// szFileName が初期化されていない場合に tagMainWindow1 の構造体を破壊してしまうため終端文字を追加します。
			ptagHashFile_Recode->szFileName[MAX_PATH_SIZE - 1] = '\0';
			// 複数選択時に次回のフォルダー表示がおかしくならないように最後のファイルにしておきます。
			qtcscpy(tagMainWindow1.pOfn1Buf, ptagHashFile_Recode->szFileName);
			qtcscpy(tagMainWindow1.pFile, tagMainWindow1.pOfn1Buf);
#endif
		}
		else
		{
			LoadString(tagMainWindow1.hInst, IDS_FILE_OPEN3, tagMainWindow1.pStBuf, MAX_STRINGTABLE);
			MessageFormat(tagMainWindow1.pBuf, 2048, tagMainWindow1.pStBuf, tagMainWindow1.pOfn1Buf);
			MainWindow_MessageBox(tagMainWindow1.hWnd, tagMainWindow1.pBuf, tagMainWindow1.pTitle, MB_ICONHAND);
		}
	}

	// ウィンドウロックの解除します。
	FILELIST_WINDOW_UNLOCK;
	return;
}

DWORD FileListDlg_AddFile(VOID)
{
	DWORD dwFileCount = HashThread_GetCountItem_Core(&tagMainWindow1.tagHashThread1);
	DWORD dwRet = 0;

	dwRet = HashThread_MultiFile_Init(&tagMainWindow1.tagHashThread1, tagMainWindow1.pFile, tagFileListWindow1.dwNewFile != 0 ? TRUE : FALSE);
	IF_UNLIKELY(dwRet == (DWORD)-1)
	{
		FileRecodeFail(tagFileListWindow1.hWnd);
		return (DWORD)-1;
	}

	if (dwFileCount == dwRet)
	{
		LoadString(tagMainWindow1.hInst, IDS_FILE_OPEN2, tagFileListWindow1.pStBuf, MAX_STRINGTABLE);
		MessageFormat(tagFileListWindow1.pBuf, 1024, tagFileListWindow1.pStBuf, tagMainWindow1.pFile);
		FileListDlg_MessageBox(tagFileListWindow1.hWnd, tagFileListWindow1.pBuf, tagMainWindow1.pTitle, MB_ICONHAND);
	}
	else
	{
		dwAppFrag |= APP_FILELISTBOX_EDIT;
		tagFileListWindow1.dwNewFile++;
	}

	FileListDlg_ListView(dwRet - 1, FALSE);
	EnableWindow(tagFileListWindow1.hButton[0], TRUE);
	EnableWindow(tagFileListWindow1.hButton[1], TRUE);
	EnableWindow(tagFileListWindow1.hRadio[0], TRUE);
	EnableWindow(tagFileListWindow1.hRadio[1], TRUE);
	EnableWindow(tagFileListWindow1.hRadio[2], TRUE);
	EnableWindow(tagFileListWindow1.hButton[3], FALSE);
	return dwRet;
}

VOID FileListDlg_FolderOpen(VOID)
{
	BOOL nRet = 0;

	LoadString(tagMainWindow1.hInst, IDS_FOLDER_OPEN1, tagFileListWindow1.pStBuf, MAX_STRINGTABLE);
	// ウィンドウロックします。
	FILELIST_WINDOW_LOCK;

	nRet = iFileOpenFolder(tagFileListWindow1.hWnd, tagMainWindow1.tagHashThread1.tagMultiFile->szCurrentFolder, tagFileListWindow1.pStBuf);

	// ウィンドウロックの解除します。
	FILELIST_WINDOW_UNLOCK;

	if (nRet)
	{
		qtcscpy(tagMainWindow1.pFile, tagMainWindow1.tagHashThread1.tagMultiFile->szCurrentFolder);
		tagMainWindow1.tagHashThread1.nIsFileNoCheck = 0;
		GetHashFilePath();
		FileListDlg_AddFolder();
	}
	return;
}

DWORD FileListDlg_AddFolder(VOID)
{
	DWORD dwFileCount = HashThread_GetCountItem_Core(&tagMainWindow1.tagHashThread1);
	DWORD dwRet = 0;

	dwAppFrag |= APP_FOLDEROPEN | APP_FOLDERINIT | APP_FILELISTBOX_EDIT;

	EnableWindow(tagFileListWindow1.hButton[0], FALSE);
	EnableWindow(tagFileListWindow1.hButton[1], FALSE);
	EnableWindow(tagFileListWindow1.hButton[2], FALSE);
	EnableWindow(tagFileListWindow1.hButton[3], FALSE);
	EnableWindow(tagFileListWindow1.hRadio[0], FALSE);
	EnableWindow(tagFileListWindow1.hRadio[1], FALSE);
	EnableWindow(tagFileListWindow1.hRadio[2], FALSE);

	dwRet = HashThread_Folder_Init(&tagMainWindow1.tagHashThread1, tagMainWindow1.pFile, tagFileListWindow1.dwNewFile != 0);
	if (dwRet == (DWORD)-1)
	{
		FileRecodeFail(tagFileListWindow1.hWnd);
		return (DWORD)-1;
	}

	if (dwFileCount == dwRet)
	{
		LoadString(tagMainWindow1.hInst, IDS_FOLDER_OPEN3, tagFileListWindow1.pStBuf, MAX_STRINGTABLE);
		MessageFormat(tagFileListWindow1.pBuf, 1024, tagFileListWindow1.pStBuf, tagMainWindow1.pFile);
		FileListDlg_MessageBox(tagFileListWindow1.hWnd, tagFileListWindow1.pBuf, tagMainWindow1.pTitle, MB_ICONHAND);
	}
	return dwRet;
}

VOID FileListDlg_HashFileOpen(VOID)
{
	// ウィンドウロックします。
	FILELIST_WINDOW_LOCK;

	tagMainWindow1.ofnFileName[1].Flags &= ~(OFN_ENABLEHOOK | OFN_ENABLETEMPLATE);

	if (GetOpenFileName(&tagMainWindow1.ofnFileName[1]))
	{
		tagMainWindow1.tagHashThread1.nIsFileNoCheck = 0;
		qtcscpy(tagMainWindow1.pFile, tagMainWindow1.pHashFile);
		GetHashFilePath();
		FileListDlg_AddHashFile();
	}
	else
	{
		// ウィンドウロックの解除します。
		FILELIST_WINDOW_UNLOCK;
	}

	SetFocus(tagFileListWindow1.hButton[0]);
	return;
}

DWORD FileListDlg_AddHashFile(VOID)
{
	TCHAR szOption[10] = _T("");
	TCHAR* pOption = szOption;
	DWORD dwFileCount = 0;
	DWORD dwRet = 0;

	dwAppFrag |= APP_FILELISTBOX_EDIT;
	dwAppFrag &= ~APP_FOLDEROPEN;

	if (dwAppFrag & APP_HASHFILE_ADDMODE) {
		*pOption++ = 'a';
	}
	*pOption = '\0';

	dwFileCount = HashThread_GetCountItem_Core(&tagMainWindow1.tagHashThread1);
	dwRet = HashThread_HashFile_Init(&tagMainWindow1.tagHashThread1, tagMainWindow1.pFile, tagFileListWindow1.dwNewFile, szOption);

	// ウィンドウロックの解除します。
	FILELIST_WINDOW_UNLOCK;

	IF_UNLIKELY(dwRet == (DWORD)-1)
	{
		FileRecodeFail(tagFileListWindow1.hWnd);
		return (DWORD)-1;
	}
	else if ((dwRet - dwFileCount) > 0)
	{
		tagFileListWindow1.dwNewFile++;
		tagMainWindow1.tagHashThread1.nIsFileNoCheck = 1;

		if (tagFileListWindow1.hWnd)
		{
			if (~dwAppFrag & APP_HASHFILE_ADDMODE) {
				EnableWindow(tagFileListWindow1.hButton[0], TRUE);
			}
			EnableWindow(tagFileListWindow1.hButton[1], TRUE);
			EnableWindow(tagFileListWindow1.hRadio[0], TRUE);
			EnableWindow(tagFileListWindow1.hRadio[1], TRUE);
			EnableWindow(tagFileListWindow1.hRadio[2], TRUE);
			EnableWindow(tagFileListWindow1.hButton[3], FALSE);
		}
	}
	else if (tagFileListWindow1.hWnd)
	{
		DWORD dwError = GetLastError();

		if (dwError == 0)
		{
			if (FileToFileSize(tagMainWindow1.pFile) > 0)
			{
				if (dwFileCount > 0) {
					LoadString(tagMainWindow1.hInst, IDS_HASHFILE_OPEN4, tagFileListWindow1.pStBuf, MAX_STRINGTABLE);
				}
				else {
					LoadString(tagMainWindow1.hInst, IDS_HASHFILE_OPEN1, tagFileListWindow1.pStBuf, MAX_STRINGTABLE);
				}
			}
			else {
				LoadString(tagMainWindow1.hInst, IDS_HASHFILE_OPEN2, tagFileListWindow1.pStBuf, MAX_STRINGTABLE);
			}
			MessageFormat(tagFileListWindow1.pBuf, 1024, tagFileListWindow1.pStBuf, tagMainWindow1.cpHashName, tagMainWindow1.pFile);
		}
		else
		{
			LoadString(tagMainWindow1.hInst, IDS_HASHFILE_OPEN3, tagFileListWindow1.pStBuf, MAX_STRINGTABLE);
			GetLastError_String(tagMainWindow1.pTempBuf, dwError);
			MessageFormat(tagFileListWindow1.pBuf, 1024, tagFileListWindow1.pStBuf, tagMainWindow1.cpHashName, tagMainWindow1.pHashFile, dwError, tagMainWindow1.pTempBuf);
		}

		FileListDlg_MessageBox(tagFileListWindow1.hWnd, tagFileListWindow1.pBuf, tagMainWindow1.pTitle, MB_OK | MB_ICONERROR);
	}
	return dwRet;
}

VOID FileRecodeFail(HWND hWnd)
{
	IF_UNLIKELY(tagFileListWindow1.pStBuf == NULL) {
		return;
	}

	LoadString(tagMainWindow1.hInst, IDS_FILELIST_MALLOC_FAIL, tagFileListWindow1.pStBuf, MAX_STRINGTABLE);
	MessageFormat(tagFileListWindow1.pBuf, MAX_STRINGTABLE, tagFileListWindow1.pStBuf, _T("pHashThread->MultiFile->ptagFileRecode"));
	MessageBox(hWnd, tagFileListWindow1.pBuf, tagMainWindow1.pTitle, MB_ICONHAND);

	if (hWnd != NULL) {
		DestroyWindow(hWnd);
	}
	return;
}
