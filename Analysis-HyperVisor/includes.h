#pragma once
#pragma warning(disable: 4471 4189 4083 4005 6328 4201 4091 4310)
#include <ntifs.h>
#include <ntddk.h>
#include <wdf.h>
#include <wdm.h>
#include <cstdint>
#include <cstddef>
#include <ntimage.h>
#include <windef.h>
#include <intrin.h>
#include "ia32.hpp"


#define DBG_LOG(fmt, ...) DbgPrintEx(DPFLTR_IHVDRIVER_ID, DPFLTR_ERROR_LEVEL, "[HyperVisor][" __FUNCTION__ "] " fmt "\n", ##__VA_ARGS__)
