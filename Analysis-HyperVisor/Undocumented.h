#pragma once
#include "includes.h"

extern "C" void _sgdt(_Out_ void* gdtr);

typedef struct _NT_KPROCESS
{
    DISPATCHER_HEADER Header;
    LIST_ENTRY ProfileListHead;
    ULONG_PTR DirectoryTableBase;
    UCHAR Data[1];
}NT_KPROCESS, * PNT_KPROCESS;

typedef struct _CPUID
{
    int eax;
    int ebx;
    int ecx;
    int edx;
} CPUID, * PCPUID;

typedef union _MSR
{
    struct
    {
        ULONG Low;
        ULONG High;
    };

    ULONG64 Content;
} MSR, * PMSR;


typedef UINT32 IA32_MSR_ADDRESS;

typedef union
{
    struct
    {
        /**
         * [Bits 31:0] Indicate the allowed 0-settings of these controls. VM entry allows control X to be 0 if bit X in the MSR is
         * cleared to 0; if bit X in the MSR is set to 1, VM entry fails if control X is 0.
         */
        UINT64 Allowed0Settings : 32;
#define IA32_VMX_TRUE_CTLS_ALLOWED_0_SETTINGS_BIT                    0
#define IA32_VMX_TRUE_CTLS_ALLOWED_0_SETTINGS_FLAG                   0xFFFFFFFF
#define IA32_VMX_TRUE_CTLS_ALLOWED_0_SETTINGS_MASK                   0xFFFFFFFF
#define IA32_VMX_TRUE_CTLS_ALLOWED_0_SETTINGS(_)                     (((_) >> 0) & 0xFFFFFFFF)

        /**
         * [Bits 63:32] Indicate the allowed 1-settings of these controls. VM entry allows control X to be 1 if bit 32+X in the MSR
         * is set to 1; if bit 32+X in the MSR is cleared to 0, VM entry fails if control X is 1.
         */
        UINT64 Allowed1Settings : 32;
#define IA32_VMX_TRUE_CTLS_ALLOWED_1_SETTINGS_BIT                    32
#define IA32_VMX_TRUE_CTLS_ALLOWED_1_SETTINGS_FLAG                   0xFFFFFFFF00000000
#define IA32_VMX_TRUE_CTLS_ALLOWED_1_SETTINGS_MASK                   0xFFFFFFFF
#define IA32_VMX_TRUE_CTLS_ALLOWED_1_SETTINGS(_)                     (((_) >> 32) & 0xFFFFFFFF)
    };

    UINT64 Flags;
} IA32_VMX_TRUE_CTLS_REGISTER;

typedef struct _GUEST_REGS
{
    ULONG64 rax; // 0x00         // NOT VALID FOR SVM
    ULONG64 rcx;
    ULONG64 rdx; // 0x10
    ULONG64 rbx;
    ULONG64 rsp; // 0x20         // rsp is not stored here on SVM
    ULONG64 rbp;
    ULONG64 rsi; // 0x30
    ULONG64 rdi;
    ULONG64 r8; // 0x40
    ULONG64 r9;
    ULONG64 r10; // 0x50
    ULONG64 r11;
    ULONG64 r12; // 0x60
    ULONG64 r13;
    ULONG64 r14; // 0x70
    ULONG64 r15;
} GUEST_REGS, * PGUEST_REGS;

union VmxRegmentDescriptorAccessRight {
    unsigned int all;
    struct {
        unsigned type : 4;        //!< [0:3]
        unsigned system : 1;      //!< [4]
        unsigned dpl : 2;         //!< [5:6]
        unsigned present : 1;     //!< [7]
        unsigned reserved1 : 4;   //!< [8:11]
        unsigned avl : 1;         //!< [12]
        unsigned l : 1;           //!< [13] Reserved (except for CS) 64-bit mode
        unsigned db : 1;          //!< [14]
        unsigned gran : 1;        //!< [15]
        unsigned unusable : 1;    //!< [16] Segment unusable
        unsigned reserved2 : 15;  //!< [17:31]
    } fields;
};

struct Idtr {
    unsigned short limit;
    ULONG_PTR base;
};
using Gdtr = Idtr;

union SegmentDescriptor {
    ULONG64 all;
    struct {
        ULONG64 limit_low : 16;
        ULONG64 base_low : 16;
        ULONG64 base_mid : 8;
        ULONG64 type : 4;
        ULONG64 system : 1;
        ULONG64 dpl : 2;
        ULONG64 present : 1;
        ULONG64 limit_high : 4;
        ULONG64 avl : 1;
        ULONG64 l : 1;  //!< 64-bit code segment (IA-32e mode only)
        ULONG64 db : 1;
        ULONG64 gran : 1;
        ULONG64 base_high : 8;
    } fields;
};

struct SegmentDesctiptorX64 {
    SegmentDescriptor descriptor;
    ULONG32 base_upper32;
    ULONG32 reserved;
};

union SegmentSelector {
    unsigned short all;
    struct {
        unsigned short rpl : 2;  //!< Requested Privilege Level
        unsigned short ti : 1;   //!< Table Indicator
        unsigned short index : 13;
    } fields;
};

typedef union SEGMENT_ATTRIBUTES
{
    USHORT UCHARs;
    struct
    {
        USHORT TYPE : 4; /* 0;  Bit 40-43 */
        USHORT S : 1;    /* 4;  Bit 44 */
        USHORT DPL : 2;  /* 5;  Bit 45-46 */
        USHORT P : 1;    /* 7;  Bit 47 */

        USHORT AVL : 1; /* 8;  Bit 52 */
        USHORT L : 1;   /* 9;  Bit 53 */
        USHORT DB : 1;  /* 10; Bit 54 */
        USHORT G : 1;   /* 11; Bit 55 */
        USHORT GAP : 4;

    } Fields;
} SEGMENT_ATTRIBUTES;

typedef struct SEGMENT_SELECTOR
{
    USHORT             SEL;
    SEGMENT_ATTRIBUTES ATTRIBUTES;
    ULONG32            LIMIT;
    ULONG64            BASE;
} SEGMENT_SELECTOR, * PSEGMENT_SELECTOR;

typedef struct _SEGMENT_DESCRIPTOR
{
    USHORT LIMIT0;
    USHORT BASE0;
    UCHAR  BASE1;
    UCHAR  ATTR0;
    UCHAR  LIMIT1ATTR1;
    UCHAR  BASE2;
} SEGMENT_DESCRIPTOR, * PSEGMENT_DESCRIPTOR;