#include <kernel/stdio.h>

#include <kernel/tty.h>
#include <kernel/gdt.h>
#include <kernel/kprint.h>
#include <kernel/kbd.h>

void kernel_main(void) {
	terminal_init();

	gdt_init();
	idt_init();

  printf("Welcome to Maple OS!\n");

  while(1) {
    char* buffer[10];
    scanf(&buffer, 10);
    printf("%s", buffer);
  }

	while (1);
}
