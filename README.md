# ProjectOS-NO-UEFI
Very little kernel using GRUB as bootloader with capabilities :
  - Displaying color text (16 bit) using a printf-like function
  - Collecting GRUB informations and handle them :
    - Memory size
    - Boot device
    - Modules (0 in this project)
    - Symbol table
    - ELF header
    - Memory map
    - Drives
    - Config table
    - Boot loader name
    - APM table (Advanced Power Managment) (Not handled correctly -> deprecated)
    - VBE (VESA BIOS Extension)
    - Framebuffer
  - CPUID-safe functions

Need an i386-elf-g++ cross compiler to be build.

This project has not been made to be released. It's only for testing and learning purpose.
This project is stopped to create a more efficient kernel using UEFI.

Continuation of this project : ProjectOS-UEFI
