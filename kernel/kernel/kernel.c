#include <kernel/stdio.h>

#include <kernel/tty.h>
#include <kernel/init.h>
#include <kernel/kprint.h>

void kernel_main(void) {
	terminal_initialize();
	printf("Welcome to Maple OS!\n");

	initialize_gdt();
	initialize_idt();
	while (1);
}
