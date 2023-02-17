PUBLIC GetCs
PUBLIC GetDs
PUBLIC GetEs
PUBLIC GetSs
PUBLIC GetFs
PUBLIC GetGs
PUBLIC GetLdtr
PUBLIC GetTr
PUBLIC	__load_ar 
PUBLIC ReloadGdtr
PUBLIC ReloadIdtr

.code


GetCs PROC

	MOV		rax, cs	
	RET

GetCs ENDP

;------------------------------------------------------------------------

GetDs PROC

	MOV		rax, ds	
	RET

GetDs ENDP

;------------------------------------------------------------------------

GetEs PROC

	MOV		rax, es	
	RET

GetEs ENDP

;------------------------------------------------------------------------

GetSs PROC

	MOV		rax, ss	
	RET

GetSs ENDP

;------------------------------------------------------------------------

GetFs PROC

	MOV		rax, fs	
	RET

GetFs ENDP

;------------------------------------------------------------------------

GetGs PROC

	MOV		rax, gs
	RET

GetGs ENDP

;------------------------------------------------------------------------

GetLdtr PROC

	SLDT	rax
	RET

GetLdtr ENDP

;------------------------------------------------------------------------

GetTr PROC

	STR		rax
	RET

GetTr ENDP

;------------------------------------------------------------------------

__load_ar  PROC
        lar     rax, rcx
        jz      no_error
        xor     rax, rax
no_error:
        ret
__load_ar  ENDP

ReloadGdtr PROC
	push	rcx
	shl		rdx, 48
	push	rdx
	lgdt	fword ptr [rsp+6]	
	pop		rax
	pop		rax
	ret
ReloadGdtr ENDP

;------------------------------------------------------------------------

ReloadIdtr PROC
	push	rcx
	shl		rdx, 48
	push	rdx
	lidt	fword ptr [rsp+6]
	pop		rax
	pop		rax
	ret
ReloadIdtr ENDP

END