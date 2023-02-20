#pragma once
#include "includes.h"

// Hypervisor Sig is for future vmcall
#define HYPERVISR_SIGNATURE 0x6672643067
#define POOL_TAG  0x6672643067
#define VMXON_SIZE  PAGE_SIZE
#define VMCS_SIZE  PAGE_SIZE
#define VMM_STACK_SIZE  KERNEL_STACK_SIZE
#define MSR_BITMAP_SIZE PAGE_SIZE

extern "C" uint64_t GuestRsp;
extern "C" uint64_t GuestRip;

typedef struct VmxOffState {

	uint64_t GuestRsp;
	uint64_t GuestRip;
};

typedef struct vcpu {

	uint64_t VMXONRegion;                     /* VMA of VMXON region */
	uint64_t PhysicalVMXONRegion;  /* PMA of VMXON region */

	uint64_t VMCSRegion;                      /* VMA of VMCS region */
	uint64_t PhysicalVMCSRegion;   /* PMA of VMCS region */

	uint64_t MsrBitmap;                       /* VMA of MSR bitmap */
	uint64_t PhysicalMsrBitmap;    /* PMA of MSR bitmap */

	uint64_t VMMStack;                        /* VMM stack area */

	VmxOffState* Offstate;

	uint64_t Eptp;

	bool HasLaunched = false;
};


typedef struct hypervisor {
	// dynamically allocated array of vcpus
	uint64_t vcpu_count;
	vcpu* vcpus;
};

extern hypervisor Hv;

