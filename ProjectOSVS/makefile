export CXX=bash /mnt/d/cross/bin/i686-elf-g++ -c
export CXXFLAGS=-ffreestanding -O2 -Wall -Wextra -fno-exceptions -fpermissive -fno-rtti
export LD=/mnt/d/cross/bin/i686-elf-g++ -T src/extra/linker.ld -o bin/os.bin -ffreestanding -O2 -nostdlib
export LDFLAGS=-lgcc -Xlinker -zmuldefs

all: os

os: tmp/boot.o bin/os.bin
	cp bin/os.bin bin/isodir/boot/os.bin
	cp src/extra/grub.cfg bin/isodir/boot/grub/grub.cfg
	#To add: bin to iso
	#grub-mkrescue -o bin/os.iso bin/isodir &> log/grub_mkrescue.log
	
tmp/boot.o:
	/mnt/d/cross/bin/i686-elf-cpp src/extra/boot.S | /mnt/d/cross/bin/i686-elf-as -o tmp/boot.o -nostartfiles -nostdlib

.PHONY: tmp/kernel.o tmp/boot.o bin/os.bin

tmp/kernel.o:
	/mnt/d/cross/bin/i686-elf-g++ -c $(wildcard src/**/*.cpp) -ffreestanding -O2 -Wall -Wextra -fno-exceptions -fpermissive -fno-rtti;
	mv *.o tmp/
bin/os.bin: tmp/kernel.o
	/mnt/d/cross/bin/i686-elf-g++ -T src/extra/linker.ld -o bin/os.bin -ffreestanding -O2 -nostdlib $(wildcard tmp/*.o) -lgcc -Xlinker -zmuldefs

clean:
	rm -rf $(wildcard tmp/*.*)

mrproper: clean
	rm bin/os.iso
	rm -rf $(wildcard bin/**/*.bin)
