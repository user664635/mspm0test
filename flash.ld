ENTRY(reset)
MEMORY{
	flash	: org = 0x00000000, len = 128k
	sram	: org = 0x20000000, len = 32k
}

SECTIONS{
	.text : {
		KEEP(*(.text.ivt))
		*(.text*)
	} > flash

	.data : {
		*(.data*)
	} > sram

	.bss : {
		*(.bss*)
	} > sram
	stack_top = ORIGIN(sram) + LENGTH(sram);
}
