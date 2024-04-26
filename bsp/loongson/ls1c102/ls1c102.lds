/*
 * loongson software
 */
OUTPUT_FORMAT("elf32-loongarch")

OUTPUT_ARCH(loongarch)
ENTRY(_start)
MEMORY
{
    flash(R) : ORIGIN = 0x1c000000, LENGTH = 64K
    iram(RW) : ORIGIN = _RAM_BSS, LENGTH = 4K
    dram(RW) : ORIGIN = _RAM_DATA, LENGTH = 4K
}
SECTIONS
{
	.text	: 
    { 
        . = ALIGN(4);
        *(.text .text.*) 
        . = ALIGN(4);
        _etext = .;
    } > flash
    
	.rodata  : 
    { 
        . = ALIGN(4);
        *(.rodata .rodata.*) 
        . = ALIGN(4);
        _sidata = ALIGN(.,4);
        . = ALIGN(4);
    } > flash
    
	.data  : AT(_sidata)
    {
        _sdata = .;
        . = ALIGN(4);
        *(.got.plt) 
        . = ALIGN(4);
        *(.got) 
        . = ALIGN(4);
        *(.data .data.*) 
        . = ALIGN(4);
        _edata = .;
    } > dram

    .stack :
    {
        . = ALIGN(8);
        _system_stack_start = .;
        . = . + 0x400; /* 1kb system stack */
        _system_stack = .;
    }
    
    .bss :
    {
        _sbss = .; /* BSS */
        . = ALIGN(4);
        *(.sbss)
        . = ALIGN(4);
        *(.bss)
        . = ALIGN(4);
        *(.scommon) 
        . = ALIGN(4);
        *(COMMON) 
        . = ALIGN(4);
        _ebss = . ;	 
    } > iram
}
