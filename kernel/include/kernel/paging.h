#ifndef _KERNEL_PAGING_H
#define _KERNEL_PAGING_H

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>

#define PAGE_SIZE      0x1000     // 4 KiB page size
#define TABLE_ENTRIES  1024

// Common
#define ENTRY_PRESENT   (1 << 0)  // PDE: When not set, MMU ignores remaining bits
#define ENTRY_RW        (1 << 1)
#define ENTRY_USER      (1 << 2)
#define ENTRY_PWT       (1 << 3)
#define ENTRY_PCD       (1 << 4)
#define ENTRY_ACCESSED  (1 << 5)
#define ENTRY_DIRTY     (1 << 6)  // PDE: Unused when PS=0
#define ENTRY_GLOBAL    (1 << 8)  // PDE: Unused when PS=0
// PDE
#define PDE_PS        (1 << 7)    // When set, PDE points directly to a 4 MiB page (no PT)
#define PDE_AVL       (0x7 << 9)
#define PDE_PAT       (1 << 12)
// PTE
#define PTE_PAT       (1 << 7)
#define PTE_AVL       (0x7 << 9)
 
extern uint32_t read_cr0();
extern void write_cr0(uint32_t value);
extern uint32_t read_cr3();
extern void write_cr3(uint32_t page_directory);

void init_page_table();
bool verify_paging();

#endif