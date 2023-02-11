#include "includes.h"
#include "Common.h"
#include "Func_defs.h"

hypervisor Hv = { 0 };

NTSTATUS DriverEntry(PDRIVER_OBJECT DriverObject, PUNICODE_STRING RegistryPath) {
	UNREFERENCED_PARAMETER(DriverObject);
	UNREFERENCED_PARAMETER(RegistryPath);

	PAGED_CODE();


	DBG_LOG("Hyper-V Entry loaded");

	DriverObject->DriverUnload = DrvUnload;

	__try
	{
		memset(&Hv, 0, sizeof(Hv));

		Hv.vcpu_count = KeQueryActiveProcessorCount(0);

		auto const ArraySize = sizeof(vcpu) * Hv.vcpu_count;

		Hv.vcpus = static_cast<vcpu*>(ExAllocatePool2(POOL_FLAG_NON_PAGED, ArraySize, (ULONG)POOL_TAG));
		
		if (!Hv.vcpus) {
			DBG_LOG("Failed to allocate VCPUs.\n");
			return STATUS_UNSUCCESSFUL;
		}

		memset(Hv.vcpus, 0, ArraySize);

		DBG_LOG("Allocated %u VCPUs (0x%zX bytes).\n", Hv.vcpu_count, ArraySize);

		KAFFINITY AffinityMask;

		//Allocate everything needed
		for (int i = 0; i < Hv.vcpu_count; i++) {
			DBG_LOG("--------------------------- Allocating %d ---------------------------", i);

			AffinityMask = MathPower(2, i);

			KeSetSystemAffinityThread(AffinityMask);
			//Allocate Regions
			if (!AllocateRegions(&Hv.vcpus[i])) {
				DrvUnload(DriverObject);
			}

		}

		for (int i = 0; i < Hv.vcpu_count; i++) {
			DBG_LOG("--------------------------- Starting %d ---------------------------", i);

			AffinityMask = MathPower(2, i);

			KeSetSystemAffinityThread(AffinityMask);

			//Start
			if (!Start(&Hv.vcpus[i])) {
				DrvUnload(DriverObject);
			}
			if (!Launch()) {
				DBG_LOG("Vm Launch failed");
				DrvUnload(DriverObject);
			}
		}


	}
	__except (GetExceptionCode())
	{
		DBG_LOG("Failed ya Sucka %u", GetExceptionCode());
	}


	return STATUS_SUCCESS;
}

void DrvUnload(PDRIVER_OBJECT Obj) {

	int NumofProcessors = KeQueryActiveProcessorCount(0);

	KAFFINITY AffinityMask;


	for (int i = 0; i < NumofProcessors; i++) {
		DBG_LOG("Processor Num: %d", i);

		AffinityMask = MathPower(2, i);

		KeSetSystemAffinityThread(AffinityMask);

		//Stop Vmx
		if (Hv.vcpus[i].ExecutedVmxon == true) {
			__vmx_off();
		}

		FreeRegions(&Hv.vcpus[i]);

	}

	ExFreePoolWithTag(Hv.vcpus, (ULONG)POOL_TAG);

	DBG_LOG("Finished");

	UNREFERENCED_PARAMETER(Obj);
}
