#include <kernel/stdio.h>

#include <kernel/tty.h>
#include <kernel/gdt.h>
#include <kernel/kprint.h>

void kernel_main(void) {
	terminal_initialize();
	printf("Welcome to Maple OS!\n");

	gdt_init();
	idt_init();
	
	while (1);
}
