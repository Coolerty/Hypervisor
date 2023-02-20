#include "includes.h"
#include "Common.h"
#include "ia32.hpp"
#include "Func_defs.h"
#include "Undocumented.h"



unsigned char Invept(uint32_t InveptType, invept_descriptor* Descriptor) {

	if (!Descriptor) {
		invept_descriptor ZeroDescriptor = { 0 };
		Descriptor = &ZeroDescriptor;
	}
	
	return __invept(InveptType, Descriptor);
}

unsigned char InveptSingleConext(uint64_t EptPointer) {

	invept_descriptor Descriptor = { 0 };
	Descriptor.ept_pointer = EptPointer;

	return Invept(1, &Descriptor); // Invept Single Context in Ept Cache Table
}

unsigned char InveptSingleConext() {

	return Invept(1, 0); // Invept All Context in Ept Cache Table
}