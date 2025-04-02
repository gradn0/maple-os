#include <kernel/paging.h>
#include <kernel/kprint.h>

// Since we are setting PS=0, the address should be 4 KiB aligned
uint32_t page_directory[TABLE_ENTRIES] __attribute__((aligned(4096)));
uint32_t page_table[TABLE_ENTRIES] __attribute__((aligned(4096)));

void init_page_table() {
  // Map the first page table ( first 4 MiB)
  uint64_t address = 0x0;
  for (size_t i = 0; i < TABLE_ENTRIES; i++) {
    page_table[i] = (address | ENTRY_PRESENT | ENTRY_RW);
    address += PAGE_SIZE;
  }

  // Set the first entry in the PDT to point to it
  page_directory[0] = ((uint32_t)page_table | ENTRY_PRESENT | ENTRY_RW);

  // Fill the remaining page directories
  for (size_t i = 1; i < TABLE_ENTRIES; i++) {
    page_directory[i] = (0 | ENTRY_RW);
  }

  // Enable paging
  write_cr3(page_directory);
  write_cr0(read_cr0() | 0x80000000); // flip bit 31 to enable paging

  printf("[INFO] Page directory loaded at: 0x%#\n", page_directory);
  if (read_cr0() & 0x80000000) {
    klog("Successfully enabled paging\n");
  }
}