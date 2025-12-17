CC = i686-elf-g++
LD = ld
ASM = nasm
# test -O2 flag
CFLAGS = -std=c++20 -c -m32 -Wall -ffreestanding -nostdinc -nostdlib -fno-rtti -mgeneral-regs-only
AFLAGS = -f elf32
LDFLAGS = -m elf_i386 $(patsubst %,-L%,$(subst :, ,$(LIBS_PATH))) -lgcc
QEMUFLAGS =

CFILES = $(shell find ./ -type f \( -name \*.cpp -o -name \*.c \))
AFILES = $(shell find ./ -type f \( -iname \*.s -o -name \*.asm \))
LDFILE = ./kernel/linker.ld
SOURCE_FILES = $(CFILES) $(AFILES)
OBJ_FILES = $(addprefix $(BDIR)/, $(addsuffix .o, $(basename $(SOURCE_FILES))))
INCLUDE = ./include
# Bad practice!
ARCHDIR = arch/x86

ifdef DEBUG
	CFLAGS := -g3 $(CFLAGS)
	AFLAGS := -g $(AFLAGS)
	QEMUFLAGS := -s -S $(QEMUFLAGS)
endif

OS_BINARY = $(BDIR)/kernel.bin
BDIR = ./build

.PHONY: install $(SOURCE_FILES)

build: startbuild $(SOURCE_FILES)
	echo $(AFLAGS)
	echo $(CFLAGS)
	@echo "Link object files..."
	$(LD) -o $(OS_BINARY) -L$(ARCHDIR) -T$(LDFILE) $(OBJ_FILES) $(LDFLAGS) 
	@echo "Project was built"

$(CFILES):
	mkdir -p $(BDIR)/$(@D)
	$(CC) $(CFLAGS) -I $(INCLUDE) -o $(BDIR)/$(addsuffix .o, $(basename $@)) $@

$(AFILES):
	mkdir -p $(BDIR)/$(@D)
	$(ASM) $(AFLAGS) -o $(BDIR)/$(addsuffix .o, $(basename $@)) $@

startbuild:
	@echo $(OBJ_FILES)
	rm -rf $(BDIR)
	mkdir $(BDIR)
	@echo "Compile source files..."

install:
	@echo "Creating an iso image of OS"
	mkdir -p ./iso/os/
	cp ./$(BDIR)/kernel.bin ./iso/os/
	grub-mkrescue -o ./$(BDIR)/kernel.iso ./iso

clean:
	rm -rf $(BDIR)
	rm -rf ./iso/os/kernel.bin

run:
	qemu-system-i386 $(QEMUFLAGS) -cdrom ./$(BDIR)/kernel.iso

all: clean build install run
