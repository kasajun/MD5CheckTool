extern CoCreateInstance: proc
extern SHCreateItemFromParsingName_Func:qword

.data
;	COMDAT __GUID_d57c7288_d4ad_4768_be02_9d969532d960
CONST	SEGMENT
__GUID_d57c7288_d4ad_4768_be02_9d969532d960 DD 0d57c7288H
	DW	0d4adH
	DW	04768H
	DB	0beH
	DB	02H
	DB	09dH
	DB	096H
	DB	095H
	DB	032H
	DB	0d9H
	DB	060H
CONST	ENDS
;	_CLSID_FileOpenDialog
CONST	SEGMENT
_CLSID_FileOpenDialog DB 09cH
	DB	05aH
	DB	01cH
	DB	0dcH
	DB	08aH
	DB	0e8H
	DB	0deH
	DB	04dH
	DB	0a5H
	DB	0a1H
	DB	060H
	DB	0f8H
	DB	02aH
	DB	020H
	DB	0aeH
	DB	0f7H
CONST	ENDS
;	COMDAT __GUID_43826d1e_e718_42ee_bc55_a1e261c37bfe
CONST	SEGMENT
__GUID_43826d1e_e718_42ee_bc55_a1e261c37bfe DD 043826d1eH
	DW	0e718H
	DW	042eeH
	DB	0bcH
	DB	055H
	DB	0a1H
	DB	0e2H
	DB	061H
	DB	0c3H
	DB	07bH
	DB	0feH
CONST	ENDS

.code

; HRESULT WINAPI iFileOpenDialog_CreateeInstance(IFileOpenDialog** pFileDlg)
; {
iFileOpenDialog_CoCreateInstance proc
	sub	rsp, 56
	cmp	qword ptr [rcx], 0
	jne	short FUNC_END
;	 if (*pFileDlg == NULL)
;	 {
	xor	edx, edx
	mov	qword ptr [rsp + 020h], rcx
	lea	r9, offset __GUID_d57c7288_d4ad_4768_be02_9d969532d960
	lea	rcx, offset _CLSID_FileOpenDialog
	lea	r8d, qword ptr [rdx + 1]
	call	CoCreateInstance
	add	rsp, 56
	ret
;		 return CoCreateInstance(CLSID_FileOpenDialog, NULL, CLSCTX_INPROC_SERVER, IID_PPV_ARGS(pFileDlg));
;	 }
FUNC_END:
	mov	eax, 080004004h
	add	rsp, 56
	ret
;	 return E_ABORT;
iFileOpenDialog_CoCreateInstance endp
; }

; ULONG WINAPI iFileOpenDialog_Release(IFILEDIALOG* pFileDlg)
; {
iFileOpenDialog_Release proc
	test	rcx, rcx
	je	short FUNC_END
;	 if (pFileDlg)
;	 {
	mov	rax, qword ptr [rcx]
	call	qword ptr [rax + 010h]
	ret
;		 return pFileDlg->Release();
;	 }
FUNC_END:
	mov	eax, 0
	ret
;	 return 0;
iFileOpenDialog_Release endp
; }

; HRESULT WINAPI ShellItem_Release(IShellItem* pShellItem)
; {
ShellItem_Release proc
	test	rcx, rcx
	je	short FUNC_END
;	 if (pShellItem)
;	 {
	mov	rax, qword ptr [rcx]
	call	qword ptr [rax + 010h]
	ret
;		 return pShellItem->Release();
;	 }
FUNC_END:
	mov	eax, 0
	ret
;	 return 0;
ShellItem_Release endp
; }

; HRESULT WINAPI iFileOpenDialog_SetDefaultFolder(IFileOpenDialog* pFileDlg, IShellItem* psi)
; {
iFileOpenDialog_SetDefaultFolder proc
	sub	rsp, 56
	test	rcx, rcx
	je	short FUNC_END
;	 if (pFileDlg)
;	 {
	mov	rax,qword ptr [rcx]
	call	qword ptr [rax + 058h]
	add	rsp, 56
	ret
;		 return pFileDlg->SetDefaultFolder(psi);
;	 }
FUNC_END:
	mov	eax, 080004003h
	add	rsp, 56
	ret
;	 return E_POINTER;
iFileOpenDialog_SetDefaultFolder endp
; }

; HRESULT WINAPI iFileOpenDialog_GetFileName(IFILEDIALOG* pFileDlg, LPWSTR **pszName)
; {
iFileOpenDialog_GetFileName proc
	sub	rsp, 56
	test	rcx, rcx
	je	short FUNC_END
;	 if (pFileDlg)
;	 {
	mov	rax, qword ptr [rcx]
	mov	rdx, qword ptr [rdx]
	call	qword ptr [rax + 080h]
	add	rsp, 56
	ret
;		 return pFileDlg->GetFileName(pszName);
;	 }
FUNC_END:
	mov	eax, 080004003h
	add	rsp, 56
	ret
;	 return E_POINTER;
iFileOpenDialog_GetFileName endp
; }

; HRESULT WINAPI iFileOpenDialog_SetFileName(IFILEDIALOG* pFileDlg, LPCWSTR pszName)
; {
iFileOpenDialog_SetFileName proc
	sub	rsp, 56
	test	rcx, rcx
	je	short FUNC_END
;	 if (pFileDlg)
;	 {
	mov	rax, qword ptr [rcx]
	call	qword ptr [rax + 078h]
	add	rsp, 56
	ret
;		 return pFileDlg->SetFileName(pszName);
;	 }
FUNC_END:
	mov	eax, 080004003h
	add	rsp, 56
	ret
;	 return E_POINTER;
iFileOpenDialog_SetFileName endp
; }

; HRESULT WINAPI iFileOpenDialog_GetOptions(IFileOpenDialog* pFileDlg, FILEOPENDIALOGOPTIONS *pfos)
; {
iFileOpenDialog_GetOptions proc
	sub	rsp, 56
	test	rcx, rcx
	je	short FUNC_END
;	 if (pFileDlg)
;	 {
	mov	rax, qword ptr [rcx]
	call	qword ptr [rax + 050h]
	add	rsp, 56
	ret
;		 return pFileDlg->GetOptions(pfos);
;	 }
FUNC_END:
	mov	eax, 080004003h
	add	rsp, 56
	ret
;	 return E_POINTER;
iFileOpenDialog_GetOptions endp
; }

; HRESULT WINAPI iFileOpenDialog_SetOptions(IFileOpenDialog* pFileDlg, FILEOPENDIALOGOPTIONS fos)
; {
iFileOpenDialog_SetOptions proc
	sub	rsp, 56
	test	rcx, rcx
	je	short FUNC_END
;	 if (pFileDlg)
;	 {
	mov	rax, qword ptr [rcx]
	call	qword ptr [rax + 048h]
	add	rsp, 56
	ret
;		 return pFileDlg->SetOptions(fos);
;	 }
FUNC_END:
	mov	eax, 080004003h
	add	rsp, 56
	ret
;	 return E_POINTER;
iFileOpenDialog_SetOptions endp
; }

; HRESULT WINAPI iFileOpenDialog_Show(IFileOpenDialog* pFileDlg, HWND hwndOwner)
; {
iFileOpenDialog_Show proc
	sub	rsp, 56
	test	rcx, rcx
	je	short FUNC_END
;	 if (pFileDlg)
;	 {
	mov	rax, qword ptr [rcx]
	call	qword ptr [rax + 018h]
	add	rsp, 56
	ret
;		 return pFileDlg->Show(hwndOwner);
;	 }
FUNC_END:
	mov	eax, 080004003h
	add	rsp, 56
	ret
;	 return E_POINTER;
iFileOpenDialog_Show endp
; }

; HRESULT WINAPI iFileOpenDialog_GetResult(IFileOpenDialog* pFileDlg, IShellItem** ppsi)
; {
iFileOpenDialog_GetResult proc
	sub	rsp, 56
	test	rcx, rcx
	je	short FUNC_END
;	 if (pFileDlg)
;	 {
	mov	rax, qword ptr [rcx]
	call	qword ptr [rax + 0a0h]
	add	rsp, 56
	ret
;		 return pFileDlg->GetResult(ppsi);
;	 }
FUNC_END:
	mov	eax, 080004003h
	add	rsp, 56
	ret
;	 return E_POINTER;
iFileOpenDialog_GetResult endp
; }

; HRESULT WINAPI ShellItem_GetDisplayName(IShellItem* pShellItem, SIGDN sigdnName, LPWSTR* ppszName)
; {
ShellItem_GetDisplayName proc
	sub	rsp, 56
	test	rcx, rcx
	je	short FUNC_END
;	 if (pShellItem)
;	 {
	mov	rax, qword ptr [rcx]
	call	qword ptr [rax + 028h]
	add	rsp, 56
	ret
;		 return pShellItem->GetDisplayName(sigdnName, ppszName);
;	 }
FUNC_END:
	mov	eax, 080004003h
	add	rsp, 56
	ret
;	 return E_POINTER;
ShellItem_GetDisplayName endp
; }


; HRESULT WINAPI ShellItem_SHCreateItemFromParsingName(PCWSTR pszPath, IShellItem* ppv)
; {
ShellItem_SHCreateItemFromParsingName proc
	sub	rsp, 56
	mov	rcx, qword ptr [rcx]
	lea	r8, offset __GUID_43826d1e_e718_42ee_bc55_a1e261c37bfe
	mov	r9, rdx
	xor	edx, edx
	call	qword ptr SHCreateItemFromParsingName_Func
	add	rsp, 56
	ret
;	 return SHCreateItemFromParsingName_Func(pszPath, NULL, IID_PPV_ARGS(&ppv));
ShellItem_SHCreateItemFromParsingName endp
; }

end
