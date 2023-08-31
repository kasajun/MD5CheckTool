extern CoCreateInstance: proc

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

; HRESULT WINAPI iTaskbarList3_CoCreateInstance(IFileOpenDialog** pFileDlg)
; {
iTaskbarList3_CoCreateInstance proc
	sub	rsp, 56
	cmp	qword ptr [rcx], 0
	jne	short FUNC_END
;	 if (*pTask3 == NULL)
;	 {
	xor	edx, edx
	mov	qword ptr [rsp + 020h], rcx
	lea	r9, [__GUID_ea1afb91_9e28_4b86_90e9_9e9f8a5eefaf]
	lea	rcx, [_CLSID_TaskbarList]
	lea	r8d, [rdx + 1]
	call	CoCreateInstance
	add	rsp, 56
	ret
;		 return CoCreateInstance(CLSID_TaskbarList, NULL, CLSCTX_INPROC_SERVER, IID_PPV_ARGS(pTask3));
;	 }
FUNC_END:
	mov	eax, 080004004h
	add	rsp, 56
	ret
;	 return E_ABORT;
iTaskbarList3_CoCreateInstance endp
; }

; HRESULT WINAPI iTaskbarList3_Release(ITaskbarList3* pTask3)
iTaskbarList3_Release proc
	test	rcx, rcx
	je	short FUNC_END
;	 if (pTask3)
;	 {
	mov	rax, qword ptr [rcx]
	call	qword ptr [rax + 010h]
	ret
;		 return pTask3->Release();
;	 }
FUNC_END:
	mov	eax, 0
	ret
;	 return 0;
iTaskbarList3_Release endp
; }

; HRESULT WINAPI iTaskbarList3_SetOverlayIcon(TASKBARLIST3* pTask3, HWND hWnd, HICON hIcon, LPCWSTR pszDescription)
; {
iTaskbarList3_SetOverlayIcon proc
	sub	rsp, 56
	test	rcx, rcx
	je	FUNC_END
;	 if (pTask3)
;	 {
	mov	rax, qword ptr [rcx]
	call	qword ptr [rax + 090h]
	add	rsp, 56
	ret
;		 return pTask3->SetOverlayIcon(hWnd, hIcon, pszDescription);
;	 }
FUNC_END:
	mov	eax, 080004003h
	add	rsp, 56
	ret
;	 return E_POINTER;
iTaskbarList3_SetOverlayIcon endp
; }

; HRESULT WINAPI iTaskbarList3_DeleteOverlayIcon(TASKBARLIST3* pTask3, HWND hWnd)
; {
iTaskbarList3_DeleteOverlayIcon proc
	sub	rsp, 56
	test	rcx, rcx
	je	FUNC_END
;	 if (pTask3)
;	 {
	mov	rax, qword ptr [rcx]
	lea	r9, [_NULL_STRING]
	call	qword ptr [rax + 090h]
	add	rsp, 56
	ret
;		 return pTask3->SetOverlayIcon(hWnd, hIcon, pszDescription);
;	 }
FUNC_END:
	mov	eax, 080004003h
	add	rsp, 56
	ret
;	 return E_POINTER;
iTaskbarList3_DeleteOverlayIcon endp
; }

; HRESULT WINAPI iTaskbarList3_SetProgressState(ITaskbarList3* pTask3, HWND hWnd, size_t tbpFlags)
; {
iTaskbarList3_SetProgressState proc
	sub	rsp, 18
	test	rcx, rcx
	je	short FUNC_END
;	 if (pTask3)
;	 {
	mov	rax, qword ptr [rcx]
	call	qword ptr [rax + 050h]
	add	rsp, 18
	ret
;		 return pTask3->SetProgressState(hWnd, (TBPFLAG)tbpFlags);
;	 }
FUNC_END:
	mov	eax, 080004003h
	add	rsp, 18
	ret
;	 return E_POINTER;
iTaskbarList3_SetProgressState endp
; }

; HRESULT WINAPI iTaskbarList3_SetProgressValue(ITaskbarList3* pTask3, HWND hWnd, size_t nCompleted)
; {
iTaskbarList3_SetProgressValue proc
	sub	rsp, 56
	test	rcx, rcx
	je	short FUNC_END
;	 if (pTask3)
;	 {
	mov	rax, qword ptr [rcx]
	mov	r9d, 064h
	call	qword ptr [rax + 048h]
	add	rsp, 56
	ret
;		 return pTask3->SetProgressValue(hWnd, nCompleted, 100);
;	 }
FUNC_END:
	mov	eax, 080004003h
	add	rsp, 56
	ret
;	 return E_POINTER;
iTaskbarList3_SetProgressValue endp
; }

end
