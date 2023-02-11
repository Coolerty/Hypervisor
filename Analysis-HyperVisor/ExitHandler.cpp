#include "includes.h"
#include "Common.h"
#include "ia32.hpp"
#include "Func_defs.h"
#include "Undocumented.h"


extern "C" void VmResumeFunc(void) {


	__vmx_vmresume();

	// Vm resume failed

	ULONG64 ErrorCode = 0;
	__vmx_vmread(VMCS_VM_INSTRUCTION_ERROR, &ErrorCode);

	DBG_LOG("VmResume Error : 0x%llx\n", ErrorCode);


	DbgBreakPoint();


}
extern "C" bool VmexitHandler(GUEST_REGS* GuestRegs) {
	UNREFERENCED_PARAMETER(GuestRegs);
    DbgBreakPoint();

	DBG_LOG("Exit Handler called");
	size_t Exitreason;
	__vmx_vmread(VMCS_EXIT_REASON, &Exitreason);

	size_t Qualification = 0;
	__vmx_vmread(VMCS_EXIT_REASON, &Exitreason);


	DBG_LOG("Reason: 0x%x, Qualification: 0x%x",Exitreason & 0xffff, Qualification);
    switch (Exitreason & 0xffff)
    {
	case VMX_EXIT_REASON_ERROR_INVALID_GUEST_STATE:
	{
		DBG_LOG("Sadge");
		DumpGuestState();
		DbgBreakPoint();
	}

    default:
    {
        DbgBreakPoint();
        break;
    }
}
	return true;
}
