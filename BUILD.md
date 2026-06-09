## Requirements

1. Compiler toolset chose in the toolchain file
    * GCC cross compiler toolchain (arch-elf-tool - i686-elf-g++ for compiler)

    To build and install from source refer to the [documentation](https://gcc.gnu.org/install)

    * Clang cross compiler toolchain
    Clang support cross compile out the box.

    You can download *clang* from your package manager.

    Using *apt*:

    ```bash
    apt-get update && apt-get install clang
    ```

    Using *pacman*:

    ```bash
    pacman -Sy clang
    ```

    Or build from source consider the [documentation](https://clang.llvm.org/get_started.html)

2. Nasm compiler (for x86 only)

    To download from package manager.

    Using *apt*

    ```bash
    apt-get update && apt-get install nasm
    ```

    Using *pacman*

    ```bash
    pacman -Sy nasm
    ```

3. Grub bootloader (only it is currently supported)

    It is reccommended to build grub from source configured target as chose
architecture.

    To build grub from source refer to the [documentation](https://www.gnu.org/software/grub/manual/grub/html_node/Obtaining-and-Building-GRUB.html)

## Kernel configuration
You can configure kernel using cmake -D option. All cmake options build-in are supported

### Options
Not yet available options

## Build instructions

### On your host environment
```
cmake -DCMAKE_TOOLCHAIN_FILE=<FILE> -D<OPTION>=<VALUE> -B build
cmake --build build
# Create iso file using *grub-mkrescue*
# Assume GRUB configured and built to a chose architecture
mkdir -p iso/os
cp build/Objective iso/os
grub-mkrescue -o build/kernel.iso iso
```

After you can create virtual machine or using emulator to run kernel.
For qemu and x86 architecture:
```bash
qemu-system-i386 -cdrom build/kernel.iso
```
