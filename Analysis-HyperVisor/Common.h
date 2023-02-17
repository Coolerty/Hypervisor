#pragma once
#include "includes.h"

// Hypervisor Sig is for future vmcall
#define HYPERVISR_SIGNATURE 0x6672643067
#define POOL_TAG  0x6672643067
#define VMXON_SIZE  PAGE_SIZE
#define VMCS_SIZE  PAGE_SIZE
#define VMM_STACK_SIZE  KERNEL_STACK_SIZE
#define MSR_BITMAP_SIZE PAGE_SIZE

extern "C" UINT64 GuestRsp;
extern "C" UINT64 GuestRip;

typedef struct VmxOffState {

	UINT64 GuestRsp;
	UINT64 GuestRip;
};

typedef struct vcpu {

	UINT64 VMXONRegion;                     /* VMA of VMXON region */
	UINT64 PhysicalVMXONRegion;  /* PMA of VMXON region */

	UINT64 VMCSRegion;                      /* VMA of VMCS region */
	UINT64 PhysicalVMCSRegion;   /* PMA of VMCS region */

	UINT64 MsrBitmap;                       /* VMA of MSR bitmap */
	UINT64 PhysicalMsrBitmap;    /* PMA of MSR bitmap */

	UINT64 VMMStack;                        /* VMM stack area */

	VmxOffState* Offstate;


	bool ExecutedVmxon = false;
};


typedef struct hypervisor {
	// dynamically allocated array of vcpus
	int vcpu_count;
	vcpu* vcpus;
};

extern hypervisor Hv;

