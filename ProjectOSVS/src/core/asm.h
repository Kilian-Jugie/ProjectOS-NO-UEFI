#ifndef __ASM_H_GUARD
#define __ASM_H_GUARD
#include "defines.h"
#include "../extern/cpuid.h"

ASM_NAMESPACE_BEGIN

static constexpr uint_fast8_t CPUID_REG_COUNT = 4; //Should NEVER be changed (only if major architectural changes)

using reg_t = uint_fast32_t;

enum class CPUID_RETURN : unsigned int {
	NOERROR,
	ARRAY_TOO_SMALL,
	EAX_INITIAL_TOO_BIG
};

static constexpr const char* CPUID_RETURN_STR[] = {"no error",
								"passed registers array is too small (see CPUID_REG_COUNT for minimal size)",
								"EAX initial value is too big"};

CPUID_RETURN cpuid(reg_t* _reg, reg_t _eax) {
	__cpuid(_eax, _reg[0], _reg[1], _reg[2], _reg[3]);
	return CPUID_RETURN::NOERROR;
}

template<size_t N>CPUID_RETURN safe_cpuid(reg_t (&_reg)[N], reg_t _eax) {
	if (N < CPUID_REG_COUNT)
		return CPUID_RETURN::ARRAY_TOO_SMALL;
	reg_t _maxBasic(0), _none(0);
	//__cpuid(0, _maxBasic, _maxExt, _maxExt, _maxExt);
	if (_eax > _maxBasic)
		return CPUID_RETURN::EAX_INITIAL_TOO_BIG;
	return cpuid(_reg, _eax);
}



ASM_NAMESPACE_END


#endif // !__ASM_H_GUARD
