.code

; void crc32c_asm(unsigned int crc, const unsigned char* buf, size_t len);
crc32c_asm proc
	push	rsi						;	//	RAX, RCX, RDX 以外は退避する
	push	rbx						;	//	RBX は盲点なので注意->原因不明の強制終了に
									;	//	Cソースコード
									;	//	厳密な型がないので基本的にDWORD (unsigned int) です。
									;	unsigned int *next, crc0, *end, end1, temp, ret;
	mov		rsi, rdx				;	next = buf;
	mov		ebx, ecx				;	crc0 = *crc;
	not		ebx						;	crc0 ^= (unsigned int)-1;
	mov		rdx, r8					;	end1 = *len;
	mov		rax, rdx				;	temp = end1;
	and		rax, 15					;	temp &= 15;
	mov		rcx, rdx				;	end = (unsigned int*)end1;
	sub		rcx, rax				;	end -= temp;
	add		rcx, rsi				;	end += next;
	cmp		rsi, rcx				;	ret = (next != end);
	jae		crc32_asm1_u32_end		;	while (ret)
crc32_asm_u32_start:				;
	crc32	rbx, qword ptr [rsi]	;		crc0 = _mm_crc32_u64((unsigned int)(crc0), *next);
	add		rsi, 8					;		next += 8;
	cmp		rsi, rcx				;		ret = (next != end);
	jb		crc32_asm_u32_start		;	}
crc32_asm1_u32_end:					;
	test	rax, rax				;	ret = (temp != 0)
	je		crc32_asm1_u8_end
crc32_asm_u8_start:					;	while (ret) {
	lea		rsi, [rsi + 1]			;		next++;
	crc32	ebx, byte ptr [esi - 1]	;		crc0 = __mm_crc32_u8((unsigned int)(crc0), (unsigned char)*(next - 1);
	sub		rax, 1					;		temp--;
	jne		crc32_asm_u8_start		;	}
crc32_asm1_u8_end:
	xor		ebx, 0ffffffffh			;	crc0 ^= (unsigned int)-1;
	mov		eax, ebx				;	return crc0;
	pop		rbx						;	// RBXの復元
	pop		rsi						;	// RSIの復元
	ret								;
crc32c_asm endp
end
