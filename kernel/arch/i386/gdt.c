#include <kernel/gdt.h>
#include <kernel/init.h> 
#include <kernel/kprint.h>

typedef struct gdt_pointer
{
  uint16_t limit;
  uint32_t firstEntryAddr;
} __attribute__ ((packed)) gdtr_t;

static uint64_t gdtEntries[NUM_GDT_ENTRIES];
gdtr_t gdtr;

uint64_t gdt_encode(uint32_t base, uint32_t limit, uint16_t flag)
{
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

extern void FlushGDT(uint32_t);

void initialize_gdt() {
	gdtr.limit = (sizeof(uint64_t) * NUM_GDT_ENTRIES) - 1u;
	gdtr.firstEntryAddr = (uint32_t)&gdtEntries;

    // Kernel segments
	gdtEntries[0u] = gdt_encode(0, 0, 0);
    gdtEntries[1u] = gdt_encode(0, 0x03FFFFFF, (GDT_CODE_PL0));
    gdtEntries[2u] = gdt_encode(0, 0x03FFFFFF, (GDT_DATA_PL0)); 
	
	// User segments
    gdtEntries[3u] = gdt_encode(0x04000000, 0x03FFFFFF, (GDT_CODE_PL3));
    gdtEntries[4u] = gdt_encode(0x08000000, 0x03FFFFFF, (GDT_DATA_PL3));

	gdt_flush((uint32_t)&gdtr);
}
