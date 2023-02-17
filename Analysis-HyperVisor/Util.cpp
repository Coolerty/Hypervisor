#include "includes.h"
#include "Undocumented.h"
#include "Func_defs.h"

/* Converts Virtual Address to Physical Address */
UINT64 VirtualAddressToPhysicalAddress(PVOID VirtualAddress)
{
	return MmGetPhysicalAddress(VirtualAddress).QuadPart;
}

/* Converts Physical Address to Virtual Address */
UINT64 PhysicalAddressToVirtualAddress(UINT64 PhysicalAddress)
{
	PHYSICAL_ADDRESS PhysicalAddr = { 0 };
	PhysicalAddr.QuadPart = PhysicalAddress;

	return reinterpret_cast<UINT64>(MmGetVirtualForPhysical(PhysicalAddr));
}

/* Find cr3 of system process*/
UINT64 FindSystemDirectoryTableBase()
{
	// Return CR3 of the system process.
	NT_KPROCESS* SystemProcess = (NT_KPROCESS*)(PsInitialSystemProcess);
	return SystemProcess->DirectoryTableBase;
}

/* Power function in order to computer address for MSR bitmaps */
int MathPower(int Base, int Exp) {

	int result;

	result = 1;

	for (;;)
	{
		if (Exp & 1)
		{
			result *= Base;
		}
		Exp >>= 1;
		if (!Exp)
		{
			break;
		}
		Base *= Base;
	}
	return result;
}

uint64_t segment_base(
	segment_descriptor_register_64 const& gdtr,
	segment_selector const selector) {
	// null selector
	if (selector.index == 0)
		return 0;

	// fetch the segment descriptor from the gdtr
	auto const descriptor = reinterpret_cast<segment_descriptor_64*>(
		gdtr.base_address + static_cast<uint64_t>(selector.index) * 8);

	// 3.3.4.5
	// calculate the segment base address
	auto base_address =
		(uint64_t)descriptor->base_address_low |
		((uint64_t)descriptor->base_address_middle << 16) |
		((uint64_t)descriptor->base_address_high << 24);

	// 3.3.5.2
	// system descriptors are expanded to 16 bytes for ia-32e
	if (descriptor->descriptor_type == SEGMENT_DESCRIPTOR_TYPE_SYSTEM)
		base_address |= (uint64_t)descriptor->base_address_upper << 32;

	return base_address;
}


unsigned __int32 access_right(unsigned __int16 segment_selector)
{
    __segment_selector_t selector;
    __segment_access_rights_t vmx_access_rights;

    selector.flags = segment_selector;

    //
    // Check for null selector use, if found set access right to unusable
    // and return. Otherwise, get access rights, modify format, return the
    // segment access rights.
    //
    if (selector.table == 0
        && selector.index == 0)
    {
        vmx_access_rights.flags = 0;
        vmx_access_rights.unusable = TRUE;
        return vmx_access_rights.flags;
    }

    //
    // Use our custom intrinsic to store our access rights, and
    // remember that the first byte of the access rights returned
    // are not used in VMX access right format.
    //
    vmx_access_rights.flags = (__load_ar(segment_selector) >> 8);
    vmx_access_rights.unusable = 0;
    vmx_access_rights.reserved0 = 0;
    vmx_access_rights.reserved1 = 0;

    return vmx_access_rights.flags;
}

UINT64
VmxRead(
	IN UINT64 Field
)
{
	unsigned char result;
	UINT64 fieldValue;

	result = __vmx_vmread(Field, &fieldValue);
	if (result != 0)
	{
		fieldValue = 0;
	}
	return fieldValue;
}

VOID
DumpGuestState(
)
{
    //
    // 16-Bit Guest-State Fields
    //
    DBG_LOG("Guest ES Selector              = %016llx", VmxRead(VMCS_GUEST_ES_SELECTOR));
    DBG_LOG("Guest CS Selector              = %016llx", VmxRead(VMCS_GUEST_CS_SELECTOR));
    DBG_LOG("Guest SS Selector              = %016llx", VmxRead(VMCS_GUEST_SS_SELECTOR));
    DBG_LOG("Guest DS Selector              = %016llx", VmxRead(VMCS_GUEST_DS_SELECTOR));
    DBG_LOG("Guest FS Selector              = %016llx", VmxRead(VMCS_GUEST_FS_SELECTOR));
    DBG_LOG("Guest GS Selector              = %016llx", VmxRead(VMCS_GUEST_GS_SELECTOR));
    DBG_LOG("Guest LDTR Selector            = %016llx", VmxRead(VMCS_GUEST_LDTR_SELECTOR));
    DBG_LOG("Guest TR Selector              = %016llx", VmxRead(VMCS_GUEST_TR_SELECTOR));
    DBG_LOG("Guest interrupt status         = %016llx", VmxRead(VMCS_GUEST_INTERRUPT_STATUS));
    DBG_LOG("PML index                      = %016llx", VmxRead(VMCS_GUEST_PML_INDEX));

    //
    // 64-Bit Guest-State Fields
    //
    DBG_LOG("VMCS link pointer              = %016llx", VmxRead(VMCS_GUEST_VMCS_LINK_POINTER));
    DBG_LOG("Guest IA32_DEBUGCTL            = %016llx", VmxRead(VMCS_GUEST_DEBUGCTL));
    DBG_LOG("Guest IA32_PAT                 = %016llx", VmxRead(VMCS_GUEST_PAT));
    DBG_LOG("Guest IA32_EFER                = %016llx", VmxRead(VMCS_GUEST_EFER));
    DBG_LOG("Guest IA32_PERF_GLOBAL_CTRL    = %016llx", VmxRead(VMCS_GUEST_PERF_GLOBAL_CTRL));
    DBG_LOG("Guest PDPTE0                   = %016llx", VmxRead(VMCS_GUEST_PDPTE0));
    DBG_LOG("Guest PDPTE1                   = %016llx", VmxRead(VMCS_GUEST_PDPTE1));
    DBG_LOG("Guest PDPTE2                   = %016llx", VmxRead(VMCS_GUEST_PDPTE2));
    DBG_LOG("Guest PDPTE3                   = %016llx", VmxRead(VMCS_GUEST_PDPTE3));
    DBG_LOG("Guest IA32_BNDCFGS             = %016llx", VmxRead(VMCS_GUEST_BNDCFGS));
    DBG_LOG("Guest IA32_RTIT_CTL            = %016llx", VmxRead(VMCS_GUEST_RTIT_CTL));

    //
    // 32-Bit Guest-State Fields
    //
    DBG_LOG("Guest ES Limit                 = %016llx", VmxRead(VMCS_GUEST_ES_LIMIT));
    DBG_LOG("Guest CS Limit                 = %016llx", VmxRead(VMCS_GUEST_CS_LIMIT));
    DBG_LOG("Guest SS Limit                 = %016llx", VmxRead(VMCS_GUEST_SS_LIMIT));
    DBG_LOG("Guest DS Limit                 = %016llx", VmxRead(VMCS_GUEST_DS_LIMIT));
    DBG_LOG("Guest FS Limit                 = %016llx", VmxRead(VMCS_GUEST_FS_LIMIT));
    DBG_LOG("Guest GS Limit                 = %016llx", VmxRead(VMCS_GUEST_GS_LIMIT));
    DBG_LOG("Guest LDTR Limit               = %016llx", VmxRead(VMCS_GUEST_LDTR_LIMIT));
    DBG_LOG("Guest TR Limit                 = %016llx", VmxRead(VMCS_GUEST_TR_LIMIT));
    DBG_LOG("Guest GDTR limit               = %016llx", VmxRead(VMCS_GUEST_GDTR_LIMIT));
    DBG_LOG("Guest IDTR limit               = %016llx", VmxRead(VMCS_GUEST_IDTR_LIMIT));
    DBG_LOG("Guest ES access rights         = %016llx", VmxRead(VMCS_GUEST_ES_ACCESS_RIGHTS));
    DBG_LOG("Guest CS access rights         = %016llx", VmxRead(VMCS_GUEST_CS_ACCESS_RIGHTS));
    DBG_LOG("Guest SS access rights         = %016llx", VmxRead(VMCS_GUEST_SS_ACCESS_RIGHTS));
    DBG_LOG("Guest DS access rights         = %016llx", VmxRead(VMCS_GUEST_DS_ACCESS_RIGHTS));
    DBG_LOG("Guest FS access rights         = %016llx", VmxRead(VMCS_GUEST_FS_ACCESS_RIGHTS));
    DBG_LOG("Guest GS access rights         = %016llx", VmxRead(VMCS_GUEST_GS_ACCESS_RIGHTS));
    DBG_LOG("Guest LDTR access rights       = %016llx", VmxRead(VMCS_GUEST_LDTR_ACCESS_RIGHTS));
    DBG_LOG("Guest TR access rights         = %016llx", VmxRead(VMCS_GUEST_TR_ACCESS_RIGHTS));
    DBG_LOG("Guest interruptibility state   = %016llx", VmxRead(VMCS_GUEST_INTERRUPTIBILITY_STATE));
    DBG_LOG("Guest activity state           = %016llx", VmxRead(VMCS_GUEST_ACTIVITY_STATE));
    DBG_LOG("Guest SMBASE                   = %016llx", VmxRead(VMCS_GUEST_SMBASE));
    DBG_LOG("Guest IA32_SYSENTER_CS         = %016llx", VmxRead(VMCS_GUEST_SYSENTER_CS));
    DBG_LOG("VMX-preemption timer value     = %016llx", VmxRead(VMCS_GUEST_VMX_PREEMPTION_TIMER_VALUE));

    //
    // Natural-Width Guest-State Fields
    //
    DBG_LOG("Guest CR0                      = %016llx", VmxRead(VMCS_GUEST_CR0));
    DBG_LOG("Guest CR3                      = %016llx", VmxRead(VMCS_GUEST_CR3));
    DBG_LOG("Guest CR4                      = %016llx", VmxRead(VMCS_GUEST_CR4));
    DBG_LOG("Guest ES Base                  = %016llx", VmxRead(VMCS_GUEST_ES_BASE));
    DBG_LOG("Guest CS Base                  = %016llx", VmxRead(VMCS_GUEST_CS_BASE));
    DBG_LOG("Guest SS Base                  = %016llx", VmxRead(VMCS_GUEST_SS_BASE));
    DBG_LOG("Guest DS Base                  = %016llx", VmxRead(VMCS_GUEST_DS_BASE));
    DBG_LOG("Guest FS Base                  = %016llx", VmxRead(VMCS_GUEST_FS_BASE));
    DBG_LOG("Guest GS Base                  = %016llx", VmxRead(VMCS_GUEST_GS_BASE));
    DBG_LOG("Guest LDTR base                = %016llx", VmxRead(VMCS_GUEST_LDTR_BASE));
    DBG_LOG("Guest TR base                  = %016llx", VmxRead(VMCS_GUEST_TR_BASE));
    DBG_LOG("Guest GDTR base                = %016llx", VmxRead(VMCS_GUEST_GDTR_BASE));
    DBG_LOG("Guest IDTR base                = %016llx", VmxRead(VMCS_GUEST_IDTR_BASE));
    DBG_LOG("Guest RSP                      = %016llx", VmxRead(VMCS_GUEST_RSP));
    DBG_LOG("Guest RIP                      = %016llx", VmxRead(VMCS_GUEST_RIP));
    DBG_LOG("Guest RFLAGS                   = %016llx", VmxRead(VMCS_GUEST_RFLAGS));
    DBG_LOG("Guest pending debug exceptions = %016llx", VmxRead(VMCS_GUEST_PENDING_DEBUG_EXCEPTIONS));
    DBG_LOG("Guest IA32_SYSENTER_ESP        = %016llx", VmxRead(VMCS_GUEST_SYSENTER_ESP));
    DBG_LOG("Guest IA32_SYSENTER_EIP        = %016llx", VmxRead(VMCS_GUEST_SYSENTER_EIP));
}

UINT64
VmxReturnStackPointerForVmxoff()
{
    return Hv.vcpus[KeGetCurrentProcessorNumber()].Offstate->GuestRsp;
}

UINT64
VmxReturnInstructionPointerForVmxoff()
{
    return Hv.vcpus[KeGetCurrentProcessorNumber()].Offstate->GuestRip;
}

void RestoreRegisters() {
    ULONG64 FsBase;
    ULONG64 GsBase;
    ULONG64 GdtrBase;
    ULONG64 GdtrLimit;
    ULONG64 IdtrBase;
    ULONG64 IdtrLimit;

    __vmx_vmread(VMCS_GUEST_FS_BASE, &FsBase);
    __writemsr(IA32_FS_BASE, FsBase);

    __vmx_vmread(VMCS_GUEST_GS_BASE, &GsBase);
    __writemsr(IA32_GS_BASE, GsBase);

    __vmx_vmread(VMCS_GUEST_GDTR_BASE, &GdtrBase);
    __vmx_vmread(VMCS_GUEST_GDTR_LIMIT, &GdtrLimit);

    ReloadGdtr((PVOID)GdtrBase, (unsigned long)GdtrLimit);

    // Restore IDTR
    __vmx_vmread(VMCS_GUEST_IDTR_BASE, &IdtrBase);
    __vmx_vmread(VMCS_GUEST_IDTR_LIMIT, &IdtrLimit);

    ReloadIdtr((PVOID)IdtrBase, (unsigned long)IdtrLimit);


}