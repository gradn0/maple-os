#include <kernel/stdio.h>

#include <kernel/tty.h>
#include <kernel/gdt.h>
#include <kernel/idt.h>
#include <kernel/kprint.h>
#include <kernel/kbd.h>
#include <kernel/multiboot.h>

enum memory_type_e {
  MEMORY_AVAILABLE =              1,
  MEMORY_RESERVED =               2,
  MEMORY_ACPI_RECLAIMABLE =       3,
  MEMORY_NVS =                    4,
  MEMORY_BADRAM =                 5,
};

void print_memory(multiboot_info_t *mbi) {
  multiboot_memory_map_t *mmap = mbi->mmap_addr;
  for (uint8_t i = 0; i < 15; i++) {
		printf(
      "ADDR: 0x%#, SIZE: %d, LEN: %d, TYPE: 0x%#\n",
      mmap[i].addr, mmap[i].size, mmap[i].len, mmap[i].type
    );
	}
}

void kernel_main(void* mbi_addr, unsigned int magic) {
  multiboot_info_t *mbi = (multiboot_info_t *)mbi_addr;

	terminal_init();

  if (magic != 0x2BADB002) {
    kerror("GRUB: Unexpected magic number");
  }

  printf("Bootloader: %s\n\n", (char *)mbi->boot_loader_name);

	gdt_init();
	idt_init();

  init_page_table();

  printf("\nWelcome to Maple OS!\n");

  while(1) {
    char buffer[10];
    scanf(buffer, 10);
    printf("%s", buffer);
  }

	while (1);
}
