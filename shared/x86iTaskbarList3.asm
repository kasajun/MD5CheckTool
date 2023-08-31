.486
.model flat, stdcall

; include C:\masm32\include\ole32.inc
CoCreateInstance PROTO STDCALL :DWORD,:DWORD,:DWORD,:DWORD,:DWORD

.data
;	COMDAT __GUID_ea1afb91_9e28_4b86_90e9_9e9f8a5eefaf
CONST	SEGMENT
__GUID_ea1afb91_9e28_4b86_90e9_9e9f8a5eefaf DD 0ea1afb91H
	DW	09e28H
	DW	04b86H
	DB	090H
	DB	0e9H
	DB	09eH
	DB	09fH
	DB	08aH
	DB	05eH
	DB	0efH
	DB	0afH
CONST	ENDS
;	_CLSID_TaskbarList
CONST	SEGMENT
_CLSID_TaskbarList DB 044h
	DB	0f3H
	DB	0fdH
	DB	056H
	DB	06dH
	DB	0fdH
	DB	0d0H
	DB	011H
	DB	095H
	DB	08aH
	DB	000H
	DB	060H
	DB	097H
	DB	0c9H
	DB	0a0H
	DB	090H
CONST	ENDS
;	_NULL_STRING
CONST	SEGMENT
_NULL_STRING DD 0
CONST	ENDS

.code

; HRESULT WINAPI iTaskbarList3_CoCreateInstance(ITaskbarList3** pTask3)
; {
iTaskbarList3_CoCreateInstance proc pTask3:dword
	mov	ecx, dword ptr [pTask3]
	cmp	dword ptr [ecx], 0
	jne	FUNC_END
;	 if (*pTask3 == NULL)
;	 {
	push	ecx
	push	offset __GUID_ea1afb91_9e28_4b86_90e9_9e9f8a5eefaf
	push	1
	push	0
	push	offset _CLSID_TaskbarList
	call	CoCreateInstance
	ret
;		 return CoCreateInstance(CLSID_TaskbarList, NULL, CLSCTX_INPROC_SERVER, IID_PPV_ARGS(pTask3));
;	 }
FUNC_END:
	mov	eax, 080004004h
	ret
;	 return E_ABORT;
iTaskbarList3_CoCreateInstance endp
; }

; HRESULT WINAPI iTaskbarList3_Release(ITaskbarList3* pTask3)
iTaskbarList3_Release proc pTask3:dword
	mov	ecx, dword ptr [pTask3]
	test	ecx, ecx
	je	FUNC_END
;	 if (pTask3)
;	 {
	mov	eax, dword ptr [ecx]
	push	ecx
	call	dword ptr [eax + 8]
	ret
;		 return pTask3->Release();
;	 }
FUNC_END:
	mov	eax, 0
	ret
;	 return 0;
iTaskbarList3_Release endp
; }

; HRESULT WINAPI iTaskbarList3_SetProgressValue(ITaskbarList3* pTask3, HWND hWnd, size_t nCompleted)
; {
iTaskbarList3_SetProgressValue proc pTask3:dword, hWnd:dword, nCompleted:dword
	mov	ecx, dword ptr [pTask3]
	mov	edx, dword ptr [hWnd]
	test	ecx, ecx
	je	FUNC_END
;	 if (pTask3)
;	 {
	mov	eax, dword ptr [ecx]
	push	0
	push	064h
	push	0
	push	dword ptr [nCompleted]
	push	edx
	push	ecx
	call	dword ptr [eax + 024h]
	ret
;		 return pTask3->SetProgressValue(hWnd, nCompleted, 100);
;	 }
FUNC_END:
	mov	eax, 080004003h
	ret
;	 return E_POINTER;
iTaskbarList3_SetProgressValue endp
; }

; HRESULT WINAPI iTaskbarList3_SetProgressState(ITaskbarList3* pTask3, HWND hWnd, size_t tbpFlags)
; {
iTaskbarList3_SetProgressState proc pTask3:dword, hWnd:dword, tbpFlags:dword
	mov	ecx, dword ptr [pTask3]
	mov	edx, dword ptr [hWnd]
	test	ecx, ecx
	je	FUNC_END
;	 if (pTask3)
;	 {
	push	dword ptr [tbpFlags]
	mov	eax, dword ptr [ecx]
	push	edx
	push	ecx
	call	dword ptr [eax + 028h]
	ret
;		 return pTask3->SetProgressState(hWnd, (TBPFLAG)tbpFlags);
;	 }
FUNC_END:
	mov	eax, 080004003h
	ret
;	 return E_POINTER;
iTaskbarList3_SetProgressState endp
; }

; HRESULT WINAPI iTaskbarList3_SetOverlayIcon(TASKBARLIST3* pTask3, HWND hWnd, HICON hIcon, LPCWSTR pszDescription)
; {
iTaskbarList3_SetOverlayIcon proc pTask3:dword, hWnd:dword, hIcon:dword, pszDescription:dword
	mov	ecx, dword ptr [pTask3]
	mov	edx, dword ptr [hWnd]
	test	ecx, ecx
	je	FUNC_END
;	 if (pTask3)
;	 {
	mov	eax, dword ptr [ecx]
	push	dword ptr [pszDescription]
	push	dword ptr [hIcon]
	push	edx
	push	ecx
	call	dword ptr [eax + 048h]
	ret
;		 return pTask3->SetOverlayIcon(hWnd, hIcon, pszDescription);
;	 }
FUNC_END:
	mov	eax, 080004003h
	ret
;	 return E_POINTER;
iTaskbarList3_SetOverlayIcon endp
; }

; HRESULT WINAPI iTaskbarList3_DeleteOverlayIcon(TASKBARLIST3* pTask3, HWND hWnd, HICON hIcon, LPCWSTR pszDescription)
; {
iTaskbarList3_DeleteOverlayIcon proc pTask3:dword, hWnd:dword
	mov	ecx, dword ptr [pTask3]
	mov	edx, dword ptr [hWnd]
	test	ecx, ecx
	je	FUNC_END
;	 if (pTask3)
;	 {
	mov	eax, dword ptr [ecx]
	push	offset _NULL_STRING
	push	0
	push	edx
	push	ecx
	call	dword ptr [eax + 048h]
	ret
;		 return pTask3->SetOverlayIcon(hWnd, NULL, L"");
;	 }
FUNC_END:
	mov	eax, 080004003h
	ret
;	 return E_POINTER;
iTaskbarList3_DeleteOverlayIcon endp
; }

end
