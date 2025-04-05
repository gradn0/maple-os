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

#define PHYS_TO_VIRT(addr) ((void *)((uintptr_t)(addr) + 0xC0000000))

void print_memory(multiboot_info_t *mbi) {
  printf("\nMEMORY MAP\n");

  for (uint32_t offset = 0; offset < mbi->mmap_length; offset += sizeof(mmap_entry_32_t)) {
    mmap_entry_32_t *entry = (mmap_entry_32_t *)PHYS_TO_VIRT(mbi->mmap_addr + offset);
    printf(
      "LOW ADDR: 0x%08x | LOW LEN: %#x | SIZE: %#x | TYPE: %d\n",
      entry->addr_low, entry->len_low, entry->size, entry->type
    );
  }
}

void kernel_main(void* mbi_addr, unsigned int magic) {
  multiboot_info_t *mbi = (multiboot_info_t *)mbi_addr;

	terminal_init();

  if (magic != 0x2BADB002) {
    kerror("GRUB: Unexpected magic number");
  }

	gdt_init();
	idt_init();

  printf("\nWelcome to Maple OS!\n");

  print_memory(mbi);

  while(1) {
    char buffer[10];
    scanf(buffer, 10);
    printf("%s", buffer);
  }

	while (1);
}
