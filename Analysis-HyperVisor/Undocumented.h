#pragma once
#include "includes.h"

extern "C" void _sgdt(_Out_ void* gdtr);
using u8 = unsigned char;
using u16 = unsigned short;
using u32 = unsigned int;
using u64 = unsigned long long;
using u128 = __m128;

using s8 = char;
using s16 = short;
using s32 = int;
using s64 = long long;

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

typedef struct __segment_descriptor_64_t
{
    unsigned __int16 segment_limit_low;
    unsigned __int16 base_low;
    union
    {
        struct
        {
            unsigned __int32 base_middle : 8;
            unsigned __int32 type : 4;
            unsigned __int32 descriptor_type : 1;
            unsigned __int32 dpl : 2;
            unsigned __int32 present : 1;
            unsigned __int32 segment_limit_high : 4;
            unsigned __int32 system : 1;
            unsigned __int32 long_mode : 1;
            unsigned __int32 default_big : 1;
            unsigned __int32 granularity : 1;
            unsigned __int32 base_high : 8;
        };
        unsigned __int32 flags;
    };
    unsigned __int32 base_upper;
    unsigned __int32 reserved;
};

typedef struct __segment_descriptor_32_t
{
    unsigned __int16 segment_limit_low;
    unsigned __int16 base_low;
    union
    {
        struct
        {
            unsigned __int32 base_middle : 8;
            unsigned __int32 type : 4;
            unsigned __int32 descriptor_type : 1;
            unsigned __int32 dpl : 2;
            unsigned __int32 present : 1;
            unsigned __int32 segment_limit_high : 4;
            unsigned __int32 system : 1;
            unsigned __int32 long_mode : 1;
            unsigned __int32 default_big : 1;
            unsigned __int32 granularity : 1;
            unsigned __int32 base_high : 8;
        };
        unsigned __int32 flags;
    };
};

typedef union __segment_selector_t
{
    struct
    {
        unsigned __int16 rpl : 2;
        unsigned __int16 table : 1;
        unsigned __int16 index : 13;
    };
    unsigned __int16 flags;
};

typedef union __segment_access_rights_t
{
    struct
    {
        unsigned __int32 type : 4;
        unsigned __int32 descriptor_type : 1;
        unsigned __int32 dpl : 2;
        unsigned __int32 present : 1;
        unsigned __int32 reserved0 : 4;
        unsigned __int32 available : 1;
        unsigned __int32 long_mode : 1;
        unsigned __int32 default_big : 1;
        unsigned __int32 granularity : 1;
        unsigned __int32 unusable : 1;
        unsigned __int32 reserved1 : 15;
    };
    unsigned __int32 flags;
};

typedef struct _guest_registers
{
    u128 xmm0;
    u128 xmm1;
    u128 xmm2;
    u128 xmm3;
    u128 xmm4;
    u128 xmm5;
    u128 xmm6;
    u128 xmm7;
    u128 xmm8;
    u128 xmm9;
    u128 xmm10;
    u128 xmm11;
    u128 xmm12;
    u128 xmm13;
    u128 xmm14;
    u128 xmm15;

    u64 padding_8b;

    u64 r15;
    u64 r14;
    u64 r13;
    u64 r12;
    u64 r11;
    u64 r10;
    u64 r9;
    u64 r8;
    u64 rbp;
    u64 rdi;
    u64 rsi;
    u64 rdx;
    u64 rcx;
    u64 rbx;
    u64 rax;
} guest_registers, * pguest_registers;