#include "includes.h"
#include "Common.h"
#include "ia32.hpp"
#include "Func_defs.h"

bool
AllcoateVmxOn(vcpu* Vpcu)
{
	PAGED_CODE()
	ia32_vmx_basic_register VmxBasic = { 0 };
	PHYSICAL_ADDRESS PhysicalMax = { 0 };
	UINT64 dummy = 0;
	PhysicalMax.QuadPart = MAXULONG64;

	Vpcu->VMXONRegion = (UINT64)MmAllocateContiguousMemory(VMXON_SIZE, PhysicalMax);

	if (Vpcu->VMXONRegion == NULL) {

		DBG_LOG("Couldn't allocate Vmxon");
		return false;
	}

	RtlZeroMemory((PVOID)Vpcu->VMXONRegion, VMXON_SIZE);

	VmxBasic.flags = __readmsr(IA32_VMX_BASIC);
	dummy = Vpcu->VMXONRegion;

	*(UINT64*)dummy = VmxBasic.vmcs_revision_id;

	Vpcu->PhysicalVMXONRegion = VirtualAddressToPhysicalAddress((PVOID)Vpcu->VMXONRegion);


	DBG_LOG("Vmx On Region Virtual Address : 0x%llx", Vpcu->VMXONRegion);
	DBG_LOG("Vmx On Region Physical Address : 0x%llx", Vpcu->PhysicalVMXONRegion);

	return true;
}

bool AllocateVmcs(vcpu* Vpcu)
{
	PAGED_CODE()
	ia32_vmx_basic_register VmxBasic = { 0 };
	PHYSICAL_ADDRESS PhysicalMax = { 0 };
	UINT64 dummy = 0;
	PhysicalMax.QuadPart = MAXULONG64;

	Vpcu->VMCSRegion = (UINT64)MmAllocateContiguousMemory(VMCS_SIZE, PhysicalMax);

	if (Vpcu->VMCSRegion == NULL) {

		DBG_LOG("Couldn't allocate Vmcs");
		return false;
	}

	RtlZeroMemory((PVOID)Vpcu->VMCSRegion, VMXON_SIZE);

	VmxBasic.flags = __readmsr(IA32_VMX_BASIC);
	dummy = Vpcu->VMCSRegion;

	*(UINT64*)dummy = VmxBasic.vmcs_revision_id;

	Vpcu->PhysicalVMCSRegion = VirtualAddressToPhysicalAddress((PVOID)Vpcu->VMCSRegion);

	DBG_LOG("Vmcs Region Virtual Address : 0x%llx", Vpcu->VMCSRegion);
	DBG_LOG("Vmcs Region Physical Address : 0x%llx", Vpcu->PhysicalVMCSRegion);

	return true;
}

bool AllocateVmmStack(vcpu* Vpcu)
{
	PAGED_CODE()
	// Allocate stack for the VM Exit Handler.
	Vpcu->VMMStack = (UINT64)ExAllocatePool2(POOL_FLAG_NON_PAGED, VMM_STACK_SIZE, (ULONG)POOL_TAG);

	if (Vpcu->VMMStack == NULL)
	{
		DBG_LOG("Insufficient memory in allocationg Vmm stack");
		return false;
	}
	RtlZeroMemory((PVOID)Vpcu->VMMStack, VMM_STACK_SIZE);

	DBG_LOG("Vmm Stack for logical processor : 0x%llx\n", Vpcu->VMMStack);

	return true;
}


bool AllocateMsrBitmap(vcpu * Vpcu)
{
	PAGED_CODE()
	// Allocate memory for MSRBitMap
	Vpcu->MsrBitmap = (UINT64)ExAllocatePool2(POOL_FLAG_NON_PAGED, PAGE_SIZE, (ULONG)POOL_TAG);  // should be aligned

	if (Vpcu->MsrBitmap == NULL)
	{
		DBG_LOG("Insufficient memory in allocationg Msr bitmaps");
		return false;
	}

	RtlZeroMemory((PVOID)Vpcu->MsrBitmap, PAGE_SIZE);

	Vpcu->PhysicalMsrBitmap = (UINT64)VirtualAddressToPhysicalAddress((PVOID)Vpcu->MsrBitmap);

	DBG_LOG("Msr Bitmap Virtual Address : 0x%llx", Vpcu->MsrBitmap);
	DBG_LOG("Msr Bitmap Physical Address : 0x%llx", Vpcu->PhysicalMsrBitmap);



	return true;
}


bool AllocateRegions(vcpu* Vpcu) {
	DBG_LOG("Allocating...");

	if (!AllcoateVmxOn(Vpcu))
		return false;

	if (!AllocateVmcs(Vpcu))
		return false;

	if (!AllocateMsrBitmap(Vpcu))
		return false;

	if (!AllocateVmmStack(Vpcu))
		return false;


	return true;
}



bool FreeVmcs(vcpu* Vpcu) {
	PAGED_CODE()
	
	MmFreeContiguousMemory((PVOID)Vpcu->VMCSRegion);

	return true;
}

bool FreeVmxOn(vcpu* Vpcu) {
	PAGED_CODE()
	
	MmFreeContiguousMemory((PVOID)Vpcu->VMXONRegion);

	return true;
}

bool FreeMsrBitmap(vcpu* Vpcu) {

	PAGED_CODE()

	ExFreePoolWithTag((PVOID)Vpcu->MsrBitmap, (ULONG)POOL_TAG);

	return true;
}

bool FreeVmmStack(vcpu* Vpcu) {

	PAGED_CODE()

	ExFreePoolWithTag((PVOID)Vpcu->VMMStack, (ULONG)POOL_TAG);

	return true;
}


bool FreeRegions() {
	DBG_LOG("Freeing...\n");

	vcpu* Vpcu;
	int index = KeGetCurrentProcessorNumber();
	Vpcu = &Hv.vcpus[index];


	if (!FreeVmxOn(Vpcu))
		return false;

	if (!FreeVmcs(Vpcu))
		return false;

	if (!FreeMsrBitmap(Vpcu))
		return false;

	if (!FreeVmmStack(Vpcu))
		return false;

	return true;
}