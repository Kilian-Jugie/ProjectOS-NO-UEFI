#include "terminal.h"
#include "vgautils.h"
#include "kernel.h"
#include "string.h"
#include "asm.h"
#include "multiboot_extension.h"



asml void kernel_main(unsigned long magic, unsigned long addr) {
	Core::Kernel kern;
	kern.init(magic, addr);
}

KERN_NAMESPACE_BEGIN

void Kernel::init(unsigned long magic, unsigned long addr) {
	initTerminal(); //No memory informations needed 
	printf("Terminal initialized !\nGetting GRUB informations...\n");
	handleGRUB(magic, addr);

	initMemory();
	
	initCpu();
}

/* Check if the bit BIT in FLAGS is set. */
#define CHECK_FLAG(flags,bit)   ((flags) & (1 << (bit)))

void Kernel::handleGRUB(unsigned long magic, unsigned long addr) {
	multiboot_info_t* _mbi;
	
	if (magic != MULTIBOOT_BOOTLOADER_MAGIC) {
		KERNEL_WRITE_FATAL(*this, "magic number is invalid !");
		return;
	}

	_mbi = (multiboot_info_t*)addr;

	this->printf("flags = 0x%x\n", _mbi->flags);

	//Valid memory check
	if (CHECK_FLAG(_mbi->flags, 0))
		printf("mem_lower = %uKB, mem_upper = %uKB\n", _mbi->mem_lower, _mbi->mem_upper);

	//Valid boot device
	if (CHECK_FLAG(_mbi->flags, 1))
		printf("boot_device = 0x%x\n", _mbi->boot_device);

	//Valid modules
	if (CHECK_FLAG(_mbi->flags, 3)) {
		multiboot_module_t *mod;
		int i;

		printf("mods_count = %u, mods_addr = 0x%x\n",
			_mbi->mods_count, _mbi->mods_addr);
		for (i = 0, mod = (multiboot_module_t *)_mbi->mods_addr; i < _mbi->mods_count; i++, mod++)
			printf(" mod_start = 0x%x, mod_end = 0x%x, cmdline = %s\n", mod->mod_start,
				mod->mod_end, (char *)mod->cmdline);
	}

	if (CHECK_FLAG(_mbi->flags, 4) && CHECK_FLAG(_mbi->flags, 5)) {
		KERNEL_WRITE_FATAL(*this, "multiboot bit 4 and 5 are set !\n");
		return;
	}

	//Sym tab
	if (CHECK_FLAG(_mbi->flags, 4)) {
		multiboot_aout_symbol_table_t *multiboot_aout_sym = &(_mbi->u.aout_sym);

		printf("multiboot_aout_symbol_table: tabsize = 0x%0x, strsize = 0x%x, addr = 0x%x\n",
			multiboot_aout_sym->tabsize, multiboot_aout_sym->strsize, multiboot_aout_sym->addr);
	}

	//ELF header
	if (CHECK_FLAG(_mbi->flags, 5)) {
		multiboot_elf_section_header_table_t *multiboot_elf_sec = &(_mbi->u.elf_sec);

		printf("multiboot_elf_sec: num = %u, size = 0x%x, addr = 0x%x, shndx = 0x%x\n",
			multiboot_elf_sec->num, multiboot_elf_sec->size, multiboot_elf_sec->addr, multiboot_elf_sec->shndx);
	}

	//mmap_
	if (CHECK_FLAG(_mbi->flags, 6)) {
		multiboot_memory_map_t* mmap;

		printf("mmap_addr = 0x%x, mmap_length = 0x%x\n",
			_mbi->mmap_addr, _mbi->mmap_length);
		for (mmap = (multiboot_memory_map_t *)_mbi->mmap_addr;
			(unsigned long)mmap < _mbi->mmap_addr + _mbi->mmap_length;
			mmap = (multiboot_memory_map_t *)((unsigned long)mmap
				+ mmap->size + sizeof(mmap->size)))
			printf(" size = 0x%x, base_addr = 0x%x%08x,"
				" length = 0x%x%08x, type = 0x%x\n", mmap->size,
				(unsigned)(mmap->addr >> 32),
				(unsigned)(mmap->addr & 0xffffffff),
				(unsigned)(mmap->len >> 32),
				(unsigned)(mmap->len & 0xffffffff),
				(unsigned)mmap->type);
	}

	//drives
	if (CHECK_FLAG(_mbi->flags, 7)) {
		multiboot_drives_map_entry* dmap;
		printf("dmap_addr = 0x%x, dmap_length = 0x%x\n", _mbi->drives_addr, _mbi->drives_length);
		for(dmap = (multiboot_drives_map_entry*)_mbi->drives_addr;
			(multiboot_uint32_t)dmap<_mbi->drives_length;
			dmap = (multiboot_drives_map_entry*)((multiboot_uint32_t)dmap + dmap->size + sizeof(dmap->size))) {
			printf(" size = 0x%x, number = %u, mode = %u, cylinders = %u, heads = % u, sectors = %u\n",
				dmap->size, dmap->number, dmap->mode, dmap->cylinders, dmap->heads, dmap->sectors);
		}
	}

	if (CHECK_FLAG(_mbi->flags, 8)) {
		printf("config_table = 0x%x\n", _mbi->config_table);
	}

	if (CHECK_FLAG(_mbi->flags, 9)) {
		printf("boot_loader_name = %s\n", (char*)_mbi->boot_loader_name);
	}

	//Advanced Power Managment
	if (CHECK_FLAG(_mbi->flags, 10)) {
		//Deprecated ! -> ACPI
		printf("apm_table = 0x%x\n", _mbi->apm_table);
		multiboot_apm_info* apmi = (multiboot_apm_info*)_mbi->apm_table;
		printf("apm_version = 0x%x, ", apmi->version);
		//a continuer si besoin
	}

	//VESA BIOS Extension
	if (CHECK_FLAG(_mbi->flags, 11)) {
		//A ne pas utiliser : (dernier standard : 1998 : v3.0)
		//Utilisable qu'en mode réel + res max < fhd
		//Seule utilitée : initialisation de la CG (?)
		printf("vbe_control_info = 0x%x, ", _mbi->vbe_control_info);
		printf("vbe_mode_info = 0x%x, ", _mbi->vbe_mode_info);
		printf("vbe_mode = 0x%x, ", _mbi->vbe_mode);
		printf("vbe_interface_seg = 0x%x, ", _mbi->vbe_interface_seg);
		printf("vbe_interface_off = 0x%x, ", _mbi->vbe_interface_off);
		printf("vbe_interface_len = 0x%x\n", _mbi->vbe_interface_len);
	}

	if (CHECK_FLAG(_mbi->flags, 12)) {
		printf("framebuffer_addr = 0x%x, ", _mbi->framebuffer_addr);
		printf("framebuffer_pitch = 0x%x, ", _mbi->framebuffer_pitch);
		printf("framebuffer_width = 0x%x, ", _mbi->framebuffer_width);
		printf("framebuffer_height = 0x%x, ", _mbi->framebuffer_height);
		printf("framebuffer_bpp = 0x%x, ", _mbi->framebuffer_bpp);
		printf("framebuffer_type = 0x%x\n", _mbi->framebuffer_type);
	}
		
	this->getTerm().write("GRUB informations correctly handled !\n");
}

void Kernel::printf(const char* format, ...) {
	char** arg = (char**)& format;
	int c;
	++arg;

	while ((c = *format++) != 0) {
		if (c != '%') this->getTerm().putchar(c);
		else {
			c = *format++;
			switch (c) {
			case '%':
				this->getTerm().putchar('%');
				break;
			case 'c': {
				this->getTerm().putchar((*arg)[0]);
				++arg;
			}break;
			case 'x': {
				char cBuf[PRINTF_CONVERSION_BUFFER];
				long long* nb = (long long*)arg;
				cstring::lltoa(cBuf, 16, *nb);
				unsigned int chI(0u);
				while (cBuf[chI]) {
					this->getTerm().putchar(cBuf[chI]);
					++chI;
				}
				++arg;
			}break;
			case 'l': {
				c = *format++;
				switch (c)
				{
				case 'l': {
					char cBuf[PRINTF_CONVERSION_BUFFER];
					long long* nb = (long long*)arg;
					cstring::lltoa(cBuf, 10, *nb);
					unsigned int chI(0u);
					while (cBuf[chI]) {
						this->getTerm().putchar(cBuf[chI]);
						++chI;
					}
					++arg;
				}break;
				default:
					//Long
					c = *format--;
					break;
				}
			}break;
			case 'i':
			case 'd': {
				char cBuf[PRINTF_CONVERSION_BUFFER];
				int* nb = (int*)arg;
				cstring::itoa(cBuf, 10, *nb);
				unsigned int chI(0u);
				while (cBuf[chI]) {
					this->getTerm().putchar(cBuf[chI]);
					++chI;
				}
				++arg;
			}break;
#ifndef PRINTF_PTR_64_BITS
			case 'p':
#else
			case 'p': { //Same as ull
				char cBuf[PRINTF_CONVERSION_BUFFER];
				unsigned long long* nb = (unsigned long long*)arg;
				ulltoa(cBuf, 16, *nb);
				unsigned int chI(0u);
				while (cBuf[chI]) {
					this->getTerm().putchar(cBuf[chI]);
					++chI;
				}
				++arg;
			}break;
#endif // !PRINTF_PTR_64_BITS
			case 'u': {
				c = *format++;
				switch (c) {
				case 'l': {
					c = *format++;
					switch (c) {
					case 'l': { //Unsigned long long
						char cBuf[PRINTF_CONVERSION_BUFFER];
						unsigned long long* nb = (unsigned long long*)arg;
						cstring::ulltoa(cBuf, 10, *nb);
						unsigned int chI(0u);
						while (cBuf[chI]) {
							this->getTerm().putchar(cBuf[chI]);
							++chI;
						}
						++arg;
					}break;
					default:
						c = *format--;
						//Unsigned long
						break;
					}
				}break;
				default: { //Unsigned
					c = *format--;
					char cBuf[PRINTF_CONVERSION_BUFFER];
					unsigned int* nb = (unsigned int*)arg;
					cstring::utoa(cBuf, 10, *nb);
					unsigned int chI(0u);
					while (cBuf[chI]) {
						this->getTerm().putchar(cBuf[chI]);
						++chI;
					}
					++arg;
				} break;
				}
			}break;
			case 's': {
				unsigned int chI(0u);
				while ((*arg)[chI]) {
					this->getTerm().putchar((*arg)[chI]);
					++chI;
				}
				++arg;
			}
			default:
				break;
			}
		}
	}
}

void Kernel::initCpu() {
	union {
		char txt[4];
		unsigned int r[4];
	} buffer = { .txt = {0} };

	unsigned int id;
	unsigned int* __eax, *__ebx, *__ecx, *__edx;
	__cpuid(0, id, buffer.r[0], buffer.r[2], buffer.r[1]);

	getTerm().write(buffer.txt);
	getTerm().write("\n");

	//ASM::reg_t buf[4];
	//ASM::cpuid(buf, 0);
	//getTerm().write(buf)

	/*__get_cpuid(1, __eax, __ebx, __ecx, __edx);

	char dispBuffer[32];
	itoa(*__eax, dispBuffer);
	dispBuffer[11] = '\0';
	getTerm().write(dispBuffer);

	__cpuid(0x80000002, id, buffer.r[0], buffer.r[1], buffer.r[2]);

	getTerm().write(buffer.txt);
	getTerm().write("\n");

	

	__cpuid(0x80000004, id, buffer.r[0], buffer.r[1], buffer.r[2]);

	getTerm().write(buffer.txt);
	getTerm().write("\n");

	__cpuid(0x80000003, id, buffer.r[0], buffer.r[1], buffer.r[2]);

	getTerm().write(buffer.txt);
	getTerm().write("\n");*/
}

void Kernel::initMemory() {

}

void Kernel::initTerminal() {
	m_Terminal.init(Core::vga_utils::VGA_WIDTH, Core::vga_utils::VGA_HEIGHT);
}
KERN_NAMESPACE_END