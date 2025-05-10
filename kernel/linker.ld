ENTRY(_start)

SECTIONS
{
    . = 1M;

    .multiboot :
    {
        _startMultiboot = .;
        *(.multiboot)
        _endMultiboot = .;
    }

    .text BLOCK(4K) : ALIGN(4K)
    {
        _startText = .;
        *(.text)
        _endText = .;
    }

    .rodata BLOCK(4K) : ALIGN(4k)
    {
        _startRodata = .;
        *(.rodata)
        _endRodata = .;
    }

    .data BLOCK(4K) : ALIGN(4K)
    {
        _startData = .;
        *(.data)
        _endData = .;
    }

    .bss BLOCK(16K) : ALIGN(16K)
    {
        _startBss = .;
        *(.bss)
        _endBss = .;
    }

    .kheap BLOCK(4k) : ALIGN(4k)
    {
        _startKheap = .;
        *(.kheap)
        _endKheap = .;
    }

}
