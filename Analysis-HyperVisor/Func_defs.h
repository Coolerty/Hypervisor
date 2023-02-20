#pragma once
#include "Common.h"
#include "Undocumented.h"

//Driver.cpp
NTSTATUS DriverEntry(PDRIVER_OBJECT DriverObject, PUNICODE_STRING RegistryPath);
void DrvUnload(PDRIVER_OBJECT Obj);

//Assembly
extern "C" segment_selector  GetCs(void);
extern "C" segment_selector  GetDs(void);
extern "C" segment_selector  GetEs(void);
extern "C" segment_selector  GetSs(void);
extern "C" segment_selector  GetFs(void);
extern "C" segment_selector  GetGs(void);
extern "C" segment_selector  GetLdtr(void);
extern "C" segment_selector  GetTr(void);
extern "C"  unsigned __int32 __load_ar(unsigned __int16 segment_selector);
extern "C" unsigned char inline __invept(unsigned long Type, void* Descriptors);


//Asm Exit Handler
extern "C" void AsmVmexitHandler(void);
extern "C" bool Launch(void);

//Exit Handler
extern "C" bool VmexitHandler(pguest_registers GuestRegs);
extern "C" void VmResumeFunc(void);

//Ept.cpp
unsigned char Invept(uint32_t InveptType, invept_descriptor* Descriptor);
unsigned char InveptSingleConext(uint64_t EptPointer);
unsigned char InveptSingleConext();


//VmxRegion.cpp
bool FreeRegions();
bool AllocateRegions(vcpu* Vpcu);

//Vmcs.cpp
bool SetupVmcs(vcpu * Vcpu);

//Vmx.cpp
bool Start(vcpu* Vcpu);

//Util.cpp
uint64_t FindSystemDirectoryTableBase();
int MathPower(int Base, int Exp);
uint64_t PhysicalAddressToVirtualAddress(uint64_t PhysicalAddress);
uint64_t VirtualAddressToPhysicalAddress(PVOID VirtualAddress);
uint64_t segment_base(segment_descriptor_register_64 const& gdtr, segment_selector const selector);
unsigned __int32 access_right(unsigned __int16 segment_selector);
uint64_t VmxRead(IN uint64_t Field);
VOID DumpGuestState();
extern "C" uint64_t VmxReturnStackPointerForVmxoff();
extern "C" uint64_t VmxReturnInstructionPointerForVmxoff();