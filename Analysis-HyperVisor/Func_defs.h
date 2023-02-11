#pragma once
#include "Common.h"
#include "Undocumented.h"

//Driver.cpp
NTSTATUS DriverEntry(PDRIVER_OBJECT DriverObject, PUNICODE_STRING RegistryPath);
void DrvUnload(PDRIVER_OBJECT Obj);

//Assembly
extern "C" segment_selector  GetCs(VOID);
extern "C" segment_selector  GetDs(VOID);
extern "C" segment_selector  GetEs(VOID);
extern "C" segment_selector  GetSs(VOID);
extern "C" segment_selector  GetFs(VOID);
extern "C" segment_selector  GetGs(VOID);
extern "C" segment_selector  GetLdtr(VOID);
extern "C" segment_selector  GetTr(VOID);
extern "C" ULONG64 GetIdtBase(VOID);
extern "C" USHORT GetIdtLimit(VOID);
extern "C" ULONG64 GetGdtBase(VOID);
extern "C" USHORT GetGdtLimit(VOID);
extern "C" ULONG_PTR GetAccessRightsByte(ULONG_PTR segment_selector);
extern "C" bool Launch();

//Asm Exit Handler
extern "C" void AsmVmexitHandler(VOID); 

//Exit Handler
extern "C" bool VmexitHandler(GUEST_REGS * GuestRegs);
extern "C" void VmResumeFunc(void);

//VmxRegion.cpp
bool FreeRegions(vcpu* Vpcu);
bool AllocateRegions(vcpu* Vpcu);

//Vmcs.cpp
bool SetupVmcs(vcpu * Vcpu);

//Vmx.cpp
bool Start(vcpu* Vcpu);

//Util.cpp
UINT64 FindSystemDirectoryTableBase();
int MathPower(int Base, int Exp);
UINT64 PhysicalAddressToVirtualAddress(UINT64 PhysicalAddress);
UINT64 VirtualAddressToPhysicalAddress(PVOID VirtualAddress);
uint64_t segment_base(segment_descriptor_register_64 const& gdtr, segment_selector const selector);
ULONG access_right(USHORT Segment_Selector);
UINT64 VmxRead(IN UINT64 Field);
VOID DumpGuestState();