#ifndef __DEFINES_H_GUARD
#define __DEFINES_H_GUARD
#include "../extern/stdint-gcc.h"

#define asm_start extern "C" {
#define asm_end }
#define asml extern "C"

#define KERN_CHAR char

#define KERN_NAMESPACE_BEGIN namespace Core {
#define KERN_NAMESPACE_END }

#define ASM_NAMESPACE_BEGIN namespace ASM {
#define ASM_NAMESPACE_END }

using size_t = uint_fast64_t;

#define INT32_LENGTH 9
#define INT64_LENGTH 19

#define PRINTF_CONVERSION_BUFFER 32
//For 64-bit printf support
//#define PRINTF_PTR_64_BITS

#endif // !__DEFINES_H_GUARD

