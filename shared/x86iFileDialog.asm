.486
.model flat, stdcall

; include C:\masm32\include\ole32.inc
CoCreateInstance PROTO STDCALL :DWORD,:DWORD,:DWORD,:DWORD,:DWORD
extern SHCreateItemFromParsingName_Func:dword

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
iFileOpenDialog_CoCreateInstance proc pFileDlg:dword
	mov	ecx, dword ptr [pFileDlg]
	cmp	dword ptr [ecx], 0
	jne	FUNC_END
;	 if (*pFileDlg == NULL)
;	 {
	push	ecx
	push	offset __GUID_d57c7288_d4ad_4768_be02_9d969532d960
	push	1
	push	0
	push	offset _CLSID_FileOpenDialog
	call	CoCreateInstance
	ret
;		 return CoCreateInstance(CLSID_FileOpenDialog, NULL, CLSCTX_INPROC_SERVER, IID_PPV_ARGS(pFileDlg));
;	 }
FUNC_END:
	mov	eax, 080004004h
	ret
;	 return E_ABORT;
iFileOpenDialog_CoCreateInstance endp
; }

; HRESULT WINAPI ShellItem_Release(IShellItem* pShellItem)
; {
ShellItem_Release proc pShellItem:dword
	mov	ecx, dword ptr [pShellItem]
	test	ecx, ecx
	je	FUNC_END
;	 if (pShellItem)
;	 {
	mov	eax, dword ptr [ecx]
	push	ecx
	call	dword ptr [eax + 8]
	ret
;		 return pShellItem->Release();
;	 }
FUNC_END:
	mov	eax, 0
	ret
;	 return 0;
ShellItem_Release endp
; }

; ULONG WINAPI iFileOpenDialog_Release(IFILEDIALOG* pFileDlg)
; {
iFileOpenDialog_Release proc pFileDlg:dword
	mov	ecx, dword ptr [pFileDlg]
	test	ecx, ecx
	je	FUNC_END
;	 if (pFileDlg)
;	 {
	mov	eax, dword ptr [ecx]
	push	ecx
	call	dword ptr [eax + 8]
	ret
;		 return pFileDlg->Release();
;	 }
FUNC_END:
	mov	eax, 0
	ret
;	 return 0;
iFileOpenDialog_Release endp
; }

; HRESULT WINAPI iFileOpenDialog_SetDefaultFolder(IFileOpenDialog* pFileDlg, IShellItem* psi)
; {
iFileOpenDialog_SetDefaultFolder proc pFileDlg:dword, psi:dword
	mov	ecx, dword ptr [pFileDlg]
	mov	edx, dword ptr [psi]
	test	ecx, ecx
	je	FUNC_END
;	 if (pFileDlg)
;	 {
	mov	eax, dword ptr [ecx]
	push	edx
	push	ecx
	call	dword ptr [eax + 02Ch]
	ret
;		 return pFileDlg->SetDefaultFolder(psi);
;	 }
FUNC_END:
	mov	eax, 080004003h
	ret
;	 return E_POINTER;
iFileOpenDialog_SetDefaultFolder endp
; }

; HRESULT WINAPI iFileOpenDialog_GetFileName(IFILEDIALOG* pFileDlg, LPWSTR **pszName)
; {
iFileOpenDialog_GetFileName proc pFileDlg:dword, pszName:dword
	mov	ecx, dword ptr [pFileDlg]
	mov	edx, dword ptr [pszName]
	test	ecx, ecx
	je	FUNC_END
;	 if (pFileDlg)
;	 {
	push	dword ptr [edx]
	mov	eax, dword ptr [ecx]
	push	ecx
	call	dword ptr [eax + 040h]
	ret
;		 return pFileDlg->GetFileName(pszName);
;	 }
FUNC_END:
	mov	eax, 080004003h
	ret
;	 return E_POINTER;
iFileOpenDialog_GetFileName endp
; }

; HRESULT WINAPI iFileOpenDialog_SetFileName(IFILEDIALOG* pFileDlg, LPCWSTR pszName)
; {
iFileOpenDialog_SetFileName proc pFileDlg:dword, pszName:dword
	mov	ecx, dword ptr [pFileDlg]
	mov	edx, dword ptr [pszName]
	test	ecx, ecx
	je	FUNC_END
;	 if (pFileDlg)
;	 {
	mov	eax, dword ptr [ecx]
	push	edx
	push	ecx
	call	dword ptr [eax + 03Ch]
	ret
;		 return pFileDlg->SetFileName(pszName);
;	 }
FUNC_END:
	mov	eax, 080004003h
	ret
;	 return E_POINTER;
iFileOpenDialog_SetFileName endp
; }

; HRESULT WINAPI iFileOpenDialog_GetOptions(IFileOpenDialog* pFileDlg, FILEOPENDIALOGOPTIONS *pfos)
; {
iFileOpenDialog_GetOptions proc pFileDlg:dword, pfos:dword
	mov	ecx, dword ptr [pFileDlg]
	mov	edx, dword ptr [pfos]
	test	ecx, ecx
	je	FUNC_END
;	 if (pFileDlg)
;	 {
	mov	eax, dword ptr [ecx]
	push	edx
	push	ecx
	call	dword ptr [eax + 028h]
	ret
;		 return pFileDlg->GetOptions(pfos);
;	 }
FUNC_END:
	mov	eax, 080004003h
	ret
;	 return E_POINTER;
iFileOpenDialog_GetOptions endp
; }

; HRESULT WINAPI iFileOpenDialog_SetOptions(IFileOpenDialog* pFileDlg, FILEOPENDIALOGOPTIONS fos)
; {
iFileOpenDialog_SetOptions proc pFileDlg:dword, fos:dword
	mov	eax, dword ptr [pFileDlg]
	mov	edx, dword ptr [fos]
	test	ecx, ecx
	je	FUNC_END
;	 if (pFileDlg)
;	 {
	mov	eax, dword ptr [ecx]
	push	edx
	push	ecx
	call	dword ptr [eax + 024h]
	ret
;		 return pFileDlg->SetOptions(fos);
;	 }
FUNC_END:
	mov	eax, 080004003h
	ret
;	 return E_POINTER;
iFileOpenDialog_SetOptions endp
; }

; HRESULT WINAPI iFileOpenDialog_Show(IFileOpenDialog* pFileDlg, HWND hwndOwner)
; {
iFileOpenDialog_Show proc pFileDlg:dword, hwndOwner:dword
	mov	ecx, dword ptr [pFileDlg]
	mov	edx, dword ptr [hwndOwner]
	test	ecx, ecx
	je	FUNC_END
;	 if (pFileDlg)
;	 {
	mov	eax, dword ptr [ecx]
	push	edx
	push	ecx
	call	dword ptr [eax + 00Ch]
	ret
;		 return pFileDlg->Show(hwndOwner);
;	 }
FUNC_END:
	mov	eax, 080004003h
	ret
;	 return E_POINTER;
iFileOpenDialog_Show endp
; }

; HRESULT WINAPI iFileOpenDialog_GetResult(IFileOpenDialog* pFileDlg, IShellItem** ppsi)
; {
iFileOpenDialog_GetResult proc pFileDlg:dword, ppsi:dword
	mov	ecx, dword ptr [pFileDlg]
	mov	edx, dword ptr [ppsi]
	test	ecx, ecx
	je	FUNC_END
;	 if (pFileDlg)
;	 {
	mov	eax, dword ptr [ecx]
	push	edx
	push	ecx
	call	dword ptr [eax + 050h]
	ret
;		 return pFileDlg->GetResult(ppsi);
;	 }
FUNC_END:
	mov	eax, 080004003h
	ret
;	 return E_POINTER;
iFileOpenDialog_GetResult endp
; }

; HRESULT WINAPI ShellItem_GetDisplayName(IShellItem* pShellItem, SIGDN sigdnName, LPWSTR* ppszName)
; {
ShellItem_GetDisplayName proc pShellItem:dword, sigdnName:dword, ppszName:dword
	mov	ecx, dword ptr [pShellItem]
	test	ecx, ecx
	je	FUNC_END
;	 if (pShellItem)
;	 {
	push	dword ptr [ppszName]
	mov	eax, dword ptr [ecx]
	push	dword ptr [sigdnName]
	push	ecx
	call	dword ptr [eax + 014h]
	ret
;		 return pShellItem->GetDisplayName(sigdnName, ppszName);
;	 }
FUNC_END:
	mov	eax, 080004003h
	ret
;	 return E_POINTER;
ShellItem_GetDisplayName endp
; }

; HRESULT WINAPI ShellItem_SHCreateItemFromParsingName(PCWSTR pszPath, IShellItem* ppv)
; {
ShellItem_SHCreateItemFromParsingName proc pszPath:dword, ppv:dword
	mov	ecx, dword ptr [pszPath]
	mov	edx, dword ptr [ppv]
	push	edx
	push	offset __GUID_43826d1e_e718_42ee_bc55_a1e261c37bfe
	push	0
	push	dword ptr [ecx]
	call	dword ptr [SHCreateItemFromParsingName_Func]
	ret
;	 return SHCreateItemFromParsingName_Func(pszPath, NULL, IID_PPV_ARGS(&ppv));
ShellItem_SHCreateItemFromParsingName endp
; }

end
