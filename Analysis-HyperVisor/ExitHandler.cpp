#include "includes.h"
#include "Common.h"
#include "ia32.hpp"
#include "Func_defs.h"
#include "Undocumented.h"

extern "C" uint64_t GuestRsp = 0;
extern "C" uint64_t GuestRip = 0;

extern "C" void VmResumeFunc(void) {


	__vmx_vmresume();

	// Vm resume failed

	ULONG64 ErrorCode = 0;
	__vmx_vmread(VMCS_VM_INSTRUCTION_ERROR, &ErrorCode);

	DBG_LOG("VmResume Error : 0x%llx\n", ErrorCode);


	DbgBreakPoint();


}


 void VmxVmxoff() {

	size_t VmexitGuestRsp = 0;
	size_t VmexitGuestRip = 0;
	size_t GuestCr3 = 0;
	size_t Instructionlength = 0;


	__vmx_vmread(VMCS_GUEST_RIP, &VmexitGuestRip);
	__vmx_vmread(VMCS_GUEST_RSP, &VmexitGuestRsp);

	// Read instruction length
	__vmx_vmread(VMCS_VMEXIT_INSTRUCTION_LENGTH, &Instructionlength);

	VmexitGuestRip += Instructionlength;
	GuestRip = VmexitGuestRip;
	GuestRsp = VmexitGuestRsp;

	DBG_LOG("Exit Guest Rip: 0x%x ; Adjusted Rip 0x%x ; Instruction Length: 0x%x" , GuestRip - Instructionlength, GuestRip, Instructionlength);

}



extern "C" bool VmexitHandler(pguest_registers  GuestRegs) {
	UNREFERENCED_PARAMETER(GuestRegs);

	DBG_LOG("Vm Exit on Processor %d", KeGetCurrentProcessorNumber());

	size_t Exitreason;
	__vmx_vmread(VMCS_EXIT_REASON, &Exitreason);

	size_t Qualification = 0;
	__vmx_vmread(VMCS_EXIT_REASON, &Exitreason);


	DBG_LOG("Reason: 0x%x, Qualification: 0x%x", Exitreason & 0xffff, Qualification);

	switch (Exitreason & 0xffff)
	{
	case VMX_EXIT_REASON_ERROR_INVALID_GUEST_STATE:
	{
		DBG_LOG("Sadge");
		DumpGuestState();
		DbgBreakPoint();
	}
	case VMX_EXIT_REASON_EXECUTE_HLT:
	{
		DBG_LOG("Hlt executed!");
		return 0;
	}
	case VMX_EXIT_REASON_EXECUTE_CPUID:
	{
		DBG_LOG("Cpuid executed!");
		
		if (GuestRegs->rax == 0x1337) {
			VmxVmxoff();
			return 1;
		}

		int result[4];
		__cpuid(result, (int)GuestRegs->rax);
		GuestRegs->rax = result[0];
		GuestRegs->rbx = result[1];
		GuestRegs->rcx = result[2];
		GuestRegs->rdx = result[3];

		goto advance_rip;
	}
	case VMX_EXIT_REASON_EXECUTE_VMXOFF:
	{
		VmxVmxoff();
		return 0;
	}
	default:
	{
		DBG_LOG("Not implented exit!");
		break;
	}
	}

	return 0;
advance_rip:
	size_t rip, exec_len;
	__vmx_vmread(VMCS_GUEST_RIP, &rip);
	__vmx_vmread(VMCS_VMEXIT_INSTRUCTION_LENGTH, &exec_len);
	__vmx_vmwrite(VMCS_GUEST_RIP, rip + exec_len);

	DBG_LOG("Exit Guest Rip: 0x%x ; Adjusted Rip 0x%x ; Instruction Length: 0x%x", rip - exec_len, rip, exec_len);
	return 0;
}
