ENTRY(reset)
MEMORY{
	flash	: org = 0x00000000, len = 128k
	sram	: org = 0x20000000, len = 32k
}
	stack_top = ORIGIN(sram) + LENGTH(sram);
SECTIONS{
	.text : {
		KEEP(*(.text.ivt))
		*(.text*)
	} > flash

	ldata = .;
	.data : AT (ldata) {
		sdata = .;
		*(.data*)
		edata = .;
	} > sram

	.bss : {
		*(.bss*)
	} > sram
	/DISCARD/ : { *(.ARM.exidx*) }
}
