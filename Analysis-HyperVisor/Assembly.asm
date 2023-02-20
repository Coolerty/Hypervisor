PUBLIC GetCs
PUBLIC GetDs
PUBLIC GetEs
PUBLIC GetSs
PUBLIC GetFs
PUBLIC GetGs
PUBLIC GetLdtr
PUBLIC GetTr
PUBLIC	__load_ar 
PUBLIC __invept 

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

__invept PROC ; Return Value should be 0, 1 is failed with Status, 2 is failed without Status

	invept	rcx, oword ptr [rdx]
	jz @FailedWithStatus
	jc @FailedWithoutStatus
	xor rax, rax
	ret

	@FailedWithStatus:
		mov	rax, 1 ;
		ret

	@FailedWithoutStatus:
		mov rax, 2
		ret
__invept ENDP

END