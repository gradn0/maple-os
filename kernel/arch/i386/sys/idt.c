#include <kernel/idt.h>
#include <kernel/kbd.h>

extern void* isr_stub_table[];

static idt_entry_t idt[IDT_MAX_DESCRIPTORS];

static void idt_set_entry(uint8_t i, uint32_t isr) {
	idt[i].bits_offset_lower = isr & 0xFFFF;
	idt[i].sel = 0x08;
	idt[i].zero = 0;
	idt[i].type_attr = 0x8E;
	idt[i].bits_offset_higher = (isr & 0xFFFF0000) >> 16;
}

static void pic_remap() {
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

	// Unmask all ports
	outb(PRIMARY_DATA, 0);
	outb(SECONDARY_DATA, 0);
}

static void pic_send_eoi(uint8_t irq) {
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
    case 33:
      handle_keyboard_irq();
    default:
      break;
  }

  pic_send_eoi(irq);
}

void idt_init() {
  klog("Initialising Interrupts");

	uint32_t idtr[2];
	
  pic_remap();

  // Route stub table entries to the IDT
  for (uint8_t vector = 0; vector < 48; vector++) {
    idt_set_entry(vector, isr_stub_table[vector]);
  }
	
	uint32_t idt_addr = idt;
	idtr[0] = (sizeof (idt_entry_t) * 256) + ((idt_addr & 0xffff) << 16);
	idtr[1] = idt_addr >> 16;
	
	idt_load(idtr);
}