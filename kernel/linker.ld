INCLUDE memory.ld

ENTRY(_start)

/*Pass this symbols via CMake*/
MAGIC = 0x1BADB002;
FLAGS = 0;
CHECKSUM = -(MAGIC + FLAGS);

SECTIONS
{
    /DISCARD/ :
    {
        *(.fini)
        *(.comment)
    }

    /* It's a raw solution. */
    /* Automatic generation of filling .multiboot section needs. */
    .multiboot ALIGN(START_ADDR, CONSTANT(MAXPAGESIZE)) : ALIGN(CONSTANT(MAXPAGESIZE))
    {
        LONG(MAGIC);
        LONG(FLAGS);
        LONG(CHECKSUM)
    } > REGION_MULTIBOOT

    .text ALIGN(CONSTANT(MAXPAGESIZE)) : ALIGN(CONSTANT(MAXPAGESIZE))
    {
        _text = .;
        *(.text*)
        *(.init)
        . = ALIGN(CONSTANT(MAXPAGESIZE));
        _etext = .;
    } > REGION_TEXT

    .rodata :
    {
        _rodata = .;
        *(.rodata*)
        HIDDEN(__CTOR_LIST__ = .);
        *(.ctors)
        HIDDEN(__CTOR_END__ = .);
        HIDDEN(__DTOR_LIST__ = .);
        *(.dtors)
        HIDDEN(__DTOR_END__ = .);
        _erodata = .;
    } > REGION_RODATA

    .data :
    {
        _data = .;
        *(.data)
        _edata = .;
    } > REGION_DATA

    .bss :
    {
        _bss = .;
        *(.bss)
        *(COMMON)
        _ebss = .;
    } > REGION_BSS

    .stack ALIGN(CONSTANT(MAXPAGESIZE)) : ALIGN(CONSTANT(MAXPAGESIZE))
    {
        _stack = ABSOLUTE(.);
        . += 4 * CONSTANT(MAXPAGESIZE);
        _estack = ABSOLUTE(.) - 1;
    } > REGION_DATA

    .kheap ALIGN(CONSTANT(MAXPAGESIZE)) : ALIGN(CONSTANT(MAXPAGESIZE))
    {
        _kheap = ABSOLUTE(.);
        . += CONSTANT(MAXPAGESIZE);
        _ekheap = ABSOLUTE(.) - 1;
    } > REGION_DATA

    .eh_frame :
    {
        _eh_frame = .;
        *(.eh_frame)
        _e_eh_frame = .;
    } > REGION_COMMON

}
