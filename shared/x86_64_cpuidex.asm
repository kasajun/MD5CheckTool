.code
__cpuidex proc
	push 	rsi
	mov		esi, ecx
	mov		eax, edx
	mov		ecx, r9d
	cpuid
	mov		dword ptr [esi +  0], eax
	mov		dword ptr [esi +  4], ebx
	mov		dword ptr [esi +  8], ecx
	mov		dword ptr [esi + 12], edx
	pop		rsi
	ret
__cpuidex endp
end
