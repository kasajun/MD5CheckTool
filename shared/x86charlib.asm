.686
.model flat, c
.code

__cpuidex proc
	push	ebp
	mov		ebp, esp
	push	esi
	mov		eax, dword ptr [ebp + 16]
	mov		ecx, dword ptr [ebp + 12]
	cpuid
	mov		esi, dword ptr [ebp + 20]
	mov		dword ptr [esi +  0], eax
	mov		dword ptr [esi +  4], ebx
	mov		dword ptr [esi +  8], ecx
	mov		dword ptr [esi + 12], edx
	pop		esi
	mov		esp, ebp
	pop		ebp
	ret		0
__cpuidex endp
end
