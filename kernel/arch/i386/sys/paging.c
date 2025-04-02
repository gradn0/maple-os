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

  if (verify_paging) {
    klog("Successfully set up paging");
  }
}

bool verify_paging() {
  // Write to an address and read it back to verify contents
  uint32_t* test_addr = (uint32_t*)0x1000;
  *test_addr = 0xDEADBEEF;
  
  uint32_t read_value = *test_addr;
  
  if (read_value != 0xDEADBEEF) {
    kerror("Paging: Failed basic memory access check");
    return false;
  }

  // Verify CR0 bit and page directory address
  if (!read_cr0() & 0x80000000) {
    kerror("Paging: CR0 bit was not set");
    return false;
  }

  if (!read_cr3() == (uint32_t)page_directory) {
    kerror("Paging: CR3 does not point to a valid page directory");
    return false;
  }
}