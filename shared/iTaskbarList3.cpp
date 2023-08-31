#include "iTaskbarList3.h"
#include "c20.h"


#if _MSC_VER > 1400

/*
	pTask3->ActivateTab(hWnd);
	pTask3->AddRef();
	pTask3->AddTab(hWnd);
	pTask3->DeleteTab(hWnd);
	pTask3->HrInit();
	pTask3->MarkFullscreenWindow(hWnd, BOOL hFullscreen);
	//pTask3->QueryInterface();
	pTask3->RegisterTab(hwndTab, hwndMDI);
	pTask3->SetActiveAlt(hWnd);
	pTask3->SetOverlayIcon(hWnd, hIcon, LPCWSTR pszDescription);
	pTask3->SetTabActive(hwndTab, hwndMDI, dwReserved);
	pTask3->SetTabOrder(hwndTab, hwndInsertBefore);
	pTask3->SetThumbnailClip(hWnd, RECT * prcClip);
	pTask3->SetThumbnailTooltip(hWnd, LPCWSTR pszTip);
	pTask3->ThumbBarAddButtons(hWnd, UINT cButtons, LPTHUMBBUTTON pButton);
	pTask3->ThumbBarSetImageList(hWnd, HIMAGELIST himl);
	pTask3->ThumbBarUpdateButtons(hWnd, UINT cButtons, LPTHUMBBUTTON pButton);
	pTask3->UnregisterTab(hwndTab);
*/

//#pragma auto_inline(off)
HRESULT WINAPI iTaskbarList3_CoCreateInstance(ITASKBARLIST3** pTask3)
{
	IF_LIKELY(*pTask3 == NULL) {
		return CoCreateInstance(CLSID_TaskbarList, NULL, CLSCTX_INPROC_SERVER, IID_PPV_ARGS(pTask3));
	}
	return E_ABORT;
}

ULONG WINAPI iTaskbarList3_Release(ITASKBARLIST3* pTask3)
{
	IF_LIKELY(pTask3) {
		return pTask3->Release();
	}
	return 0;
}

HRESULT WINAPI iTaskbarList3_SetProgressState(ITASKBARLIST3* pTask3, HWND hWnd, size_t tbpFlags)
{
	IF_LIKELY(pTask3) {
		return pTask3->SetProgressState(hWnd, (TBPFLAG)tbpFlags);
	}
	return E_POINTER;
}

HRESULT WINAPI iTaskbarList3_SetProgressValue(ITASKBARLIST3* pTask3, HWND hWnd, size_t nCompleted)
{
	IF_LIKELY(pTask3) {
		return pTask3->SetProgressValue(hWnd, nCompleted, 100);
	}
	return E_POINTER;
}

HRESULT WINAPI iTaskbarList3_SetOverlayIcon(ITASKBARLIST3* pTask3, HWND hWnd, HICON hIcon, LPCWSTR pszDescription)
{
	IF_LIKELY(pTask3) {
		return pTask3->SetOverlayIcon(hWnd, hIcon, pszDescription);
	}
	return E_POINTER;
}

HRESULT WINAPI iTaskbarList3_DeleteOverlayIcon(ITASKBARLIST3* pTask3, HWND hWnd)
{
	IF_LIKELY(pTask3)
	{
		LPCWSTR _NULL_STRING = L"";

#pragma warning(push)
#pragma warning(disable: 6387)
		return pTask3->SetOverlayIcon(hWnd, NULL, _NULL_STRING);	// C6387
#pragma warning(pop)
	}
	return E_POINTER;
}
//#pragma auto_inline(on)
#endif
