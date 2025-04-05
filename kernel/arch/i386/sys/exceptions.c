#include <kernel/exceptions.h>

void handle_page_fault() {
  uint32_t faulting_address;
  __asm__ volatile("mov %%cr2, %0" : "=r"(faulting_address));

  printf("Faulting address: 0x%#\n", faulting_address);
}