PUBLIC GetCs
PUBLIC GetDs
PUBLIC GetEs
PUBLIC GetSs
PUBLIC GetFs
PUBLIC GetGs
PUBLIC GetLdtr
PUBLIC GetTr
PUBLIC AsmVmexitHandler
PUBLIC	GetAccessRightsByte
PUBLIC	Launch

EXTERN VmexitHandler:PROC
EXTERN VmResumeFunc:PROC

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

GetAccessRightsByte PROC
    lar rax, rcx
    ret
GetAccessRightsByte ENDP

Launch PROC
  ; set VMCS_GUEST_RSP to the current value of RSP
  mov rax, 681Ch
  vmwrite rax, rsp

  ; set VMCS_GUEST_RIP to the address of <successful_launch>
  mov rax, 681Eh
  mov rdx, successful_launch
  vmwrite rax, rdx

  vmlaunch

  ; if we reached here, then we failed to launch
  xor al, al
  ret
  
successful_launch:
  hlt   
  mov al, 1
  ret
Launch ENDP	

AsmVmexitHandler PROC
    push 0  ; Maybe unaligned stack

    pushfq

    push r15
    push r14
    push r13
    push r12
    push r11
    push r10
    push r9
    push r8        
    push rdi
    push rsi
    push rbp
    push rbp	; rsp
    push rbx
    push rdx
    push rcx
    push rax	

	mov rcx, rsp		; Fast call argument to PGUEST_REGS
	sub	rsp, 28h		; Free some space for Shadow Section
	call	VmexitHandler
	add	rsp, 28h		; Restore the state

	RestoreState:
	pop rax
    pop rcx
    pop rdx
    pop rbx
    pop rbp		; rsp
    pop rbp
    pop rsi
    pop rdi 
    pop r8
    pop r9
    pop r10
    pop r11
    pop r12
    pop r13
    pop r14
    pop r15

    popfq

	sub rsp, 0100h      ; to avoid error in future functions
	call    VmResumeFunc

AsmVmexitHandler ENDP


END