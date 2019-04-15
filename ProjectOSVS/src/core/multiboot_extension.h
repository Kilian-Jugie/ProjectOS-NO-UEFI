#ifndef __MULTIBOOT_EXTENSION_H_GUARD_
#define __MULTIBOOT_EXTENSION_H_GUARD_
#include "../extra/multiboot.h"

struct multiboot_drives_map_entry {
	multiboot_uint32_t size;
	multiboot_uint8_t number;
	multiboot_uint8_t mode;
	multiboot_uint16_t cylinders;
	multiboot_uint8_t heads;
	multiboot_uint8_t sectors;
	//TODO: Add port support ? (useless ?)
};


#endif // !__MULTIBOOT_EXTENSION_H_GUARD_
