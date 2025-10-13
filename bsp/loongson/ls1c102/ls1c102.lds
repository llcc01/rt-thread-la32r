/* 
 * loongson software
 */
OUTPUT_FORMAT("elf32-loongarch")

OUTPUT_ARCH(loongarch)
ENTRY(_start)
MEMORY
{
    /* flash(R) : ORIGIN = 0x1c000000, LENGTH = 256K */
    /* flash(R) : ORIGIN = 0x1c004000, LENGTH = 240K */
    flash(R) : ORIGIN = 0xa0000000, LENGTH = 1M
    /* dram1(RW) : ORIGIN = _RAM1, LENGTH = 10K */
    /* dram2(RW) : ORIGIN = _RAM2, LENGTH = 6K */
    /* dram1(RW) : ORIGIN = _RAM1, LENGTH = 16K */
    /* dram2(RW) : ORIGIN = _RAM2, LENGTH = 16K */
    dram1(RW) : ORIGIN = _RAM1, LENGTH = 1M
    dram2(RW) : ORIGIN = _RAM2, LENGTH = 1M
}
SECTIONS
{
	.text :
    {
        . = ALIGN(4);
        *(.startup)

        . = ALIGN(0x1000);
        __eentry_start = .;
        KEEP(*(.exc_vectors))
        __eentry_end = .;

        . = ALIGN(8);
        *(.text)
        *(.text.*)
        *(.rodata)
        *(.rodata.*)
        *(.rodata1)
        *(.rodata1.*)

        /* section information for finsh shell */
        . = ALIGN(4);
        __fsymtab_start = .;
        KEEP(*(FSymTab))
        __fsymtab_end = .;
        . = ALIGN(4);
        __vsymtab_start = .;
        KEEP(*(VSymTab))
        __vsymtab_end = .;
        . = ALIGN(4);

        
        . = ALIGN(4);
        __rt_init_start = .;
        KEEP(*(SORT(.rti_fn*)))
        __rt_init_end = .;
        . = ALIGN(4);

        . = ALIGN(4);
        __rt_utest_tc_tab_start = .;
        KEEP(*(UtestTcTab))
        __rt_utest_tc_tab_end = .;
        . = ALIGN(16);
    } > flash
    
    _sidata = LOADADDR(.data);

    . = ALIGN(16);
	.data :
    {
        _sdata = .;
        . = ALIGN(4);
        *(.got) 
        . = ALIGN(4);
        *(.got.plt) 
        . = ALIGN(4);
        *(.data)
        *(.data.*)

        *(.data1)
        *(.data1.*)

        . = ALIGN(8);
        *(.sdata)
        *(.sdata.*)
        _edata = .;
    } > dram1 AT > flash
    
    .sbss :
    {
        __bss_start = .;
        *(.sbss)
        *(.sbss.*)
        *(.dynsbss)
        *(.scommon)
    } > dram1

    .bss :
    {
        *(.bss)
        *(.bss.*)
        *(.dynbss)
        *(COMMON)
        __bss_end = .;
    } > dram1

    .stack :
    {
        . = ALIGN(8);
        _system_stack_start = .;

        /* 2kb system stack */
        /* . = . + 0x800; */

        /* 64KB system stack */
        . = . + 0x10000;

        _system_stack = .;
    } > dram1

    .heap :
    {
        . = ALIGN(8);
        _system_heap = .;

        /* 6kb system heap */
        /* . = . + 0x1800; */

        /* 32kb system heap */
        /* . = . + 0x8000; */

        /* 1MB system heap*/
        . = . + 0x100000;

        _system_heap_end = .;
    } > dram2

    _end = .;

}
