#include <kernel/init.h>

#define PRIMARY_CTRL 0x20
#define SECONDARY_CTRL 0xA0
#define PRIMARY_DATA 0x21
#define SECONDARY_DATA 0xA1

#define PIC_EOI 0x20
#define IRQ_0 0x20
#define IRQ_8 0x28

__attribute__((noreturn))
void exception_handler(void);

extern void load_idt();
extern void* isr_stub_table[];

typedef unsigned char bool;
enum { false = 0, true = 1 };

struct IDT_entry_t {
	uint16_t bits_offset_lower;
	uint16_t sel;
	uint8_t zero;
	uint8_t type_attr;
	uint16_t bits_offset_higher;
};

#define IDT_MAX_DESCRIPTORS 256

struct IDT_entry_t IDT[IDT_MAX_DESCRIPTORS];

void set_idt_values(uint8_t _idt, uint32_t irq_addr) {
	IDT[_idt].bits_offset_lower = irq_addr & 0xFFFF;
	IDT[_idt].sel = 0x08;
	IDT[_idt].zero = 0;
	IDT[_idt].type_attr = 0x8E;
	IDT[_idt].bits_offset_higher = (irq_addr & 0xFFFF0000) >> 16;
}

void pic_send_eoi(uint8_t irq) {
  if (irq >= 8) {
    outb(SECONDARY_CTRL, PIC_EOI);
  }
  outb(PRIMARY_CTRL, PIC_EOI);
}

void exception_handler() {
  printf("EXCEPTION\n");
  __asm__ volatile ("cli; hlt");
}

void irq_handler(uint8_t irq) {
  switch(irq) {
    case 33: // Key press
      uint8_t key = inb(0x60);
      printf("Key press: %d\n", key);
    default:
      break;
  }

  pic_send_eoi(irq);
}

void initialize_idt() {
	uint32_t idt_addr;
	uint32_t idt_ptr[2];
	
	//Remap the PIC
  // ICW1
	outb(PRIMARY_CTRL, 0x11);
	outb(SECONDARY_CTRL, 0x11);

  // ICW 2
	outb(PRIMARY_DATA, IRQ_0);
	outb(SECONDARY_DATA, IRQ_8);

  // ICW3
	outb(PRIMARY_DATA, 0x04);
	outb(SECONDARY_DATA, 0x02);

  // ICW4
	outb(PRIMARY_DATA, 0x01);
	outb(SECONDARY_DATA, 0x01);

  // Mask all ports
	outb(PRIMARY_DATA, 0xFD);
	outb(SECONDARY_DATA, 0xFF);

  // Set IDT exeption entries
  for (uint8_t vector = 0; vector < 48; vector++) {
    set_idt_values(vector, isr_stub_table[vector]);
  }
	
	idt_addr = (uint32_t)IDT;
	idt_ptr[0] = (sizeof (struct IDT_entry_t) * 256) + ((idt_addr & 0xffff) << 16);
	idt_ptr[1] = idt_addr >> 16;
	
	load_idt(idt_ptr);
}