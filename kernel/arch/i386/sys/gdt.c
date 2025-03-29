#include <kernel/gdt.h>
#include <kernel/kprint.h>

static uint64_t gdt[NUM_GDT_ENTRIES];
static gdtr_t gdtr;

static uint64_t gdt_create_entry(uint32_t base, uint32_t limit, uint16_t flag) {
  uint64_t gdt_entry;

  gdt_entry  =  limit       & 0x000F0000;
  gdt_entry |= (flag <<  8) & 0x00F0FF00;
  gdt_entry |= (base >> 16) & 0x000000FF;
  gdt_entry |=  base        & 0xFF000000;

  gdt_entry <<= 32;

  gdt_entry |= base  << 16;                       // set base bits 15:0
  gdt_entry |= limit  & 0x0000FFFF;               // set limit bits 15:0

  return gdt_entry;
}

void gdt_init() {
  klog("Initialising GDT");
  gdtr.limit = (sizeof(uint64_t) * NUM_GDT_ENTRIES) - 1u;
  gdtr.base = (uint32_t)&gdt;

  // Kernel segments
  gdt[0u] = gdt_create_entry(0, 0, 0);
  gdt[1u] = gdt_create_entry(0, 0x03FFFFFF, (GDT_CODE_PL0));
  gdt[2u] = gdt_create_entry(0, 0x03FFFFFF, (GDT_DATA_PL0)); 
	
	// User segments
  gdt[3u] = gdt_create_entry(0x04000000, 0x03FFFFFF, (GDT_CODE_PL3));
  gdt[4u] = gdt_create_entry(0x08000000, 0x03FFFFFF, (GDT_DATA_PL3));

	gdt_flush((uint32_t)&gdtr);
}
