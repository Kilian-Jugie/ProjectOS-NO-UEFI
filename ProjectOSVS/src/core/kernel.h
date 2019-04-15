#ifndef __KERNEL_H_GUARD
#define __KERNEL_H_GUARD
#include "defines.h"
#include "cpu.h"

#define KERNEL_WRITE_FATAL(kernel, ...) (kernel).printf("FATAL KERNEL ERROR: "); (kernel).printf(__VA_ARGS__)

KERN_NAMESPACE_BEGIN
	class Kernel {
	public:

		void init(unsigned long magic, unsigned long addr);
		void initCpu();
		void initMemory();
		void initTerminal();
		void handleGRUB(unsigned long magic, unsigned long addr);

		Terminal<KERN_CHAR>& getTerm() { return m_Terminal; }

		void printf(const char*, ...);

		

	private:
		Terminal<KERN_CHAR> m_Terminal;
		Cpu m_CPU;
	};
KERN_NAMESPACE_END

#endif