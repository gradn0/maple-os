#ifndef _KERNEL_IDT_H
#define _KERNEL_IDT_H

#include <stdint.h>
#include <kernel/io.h>
#include <kernel/kprint.h>

#define IDT_MAX_DESCRIPTORS 256

#define PRIMARY_CTRL 0x20
#define SECONDARY_CTRL 0xA0
#define PRIMARY_DATA 0x21
#define SECONDARY_DATA 0xA1

#define PIC_EOI 0x20
#define IRQ_0 0x20
#define IRQ_8 0x28

extern void idt_load();

void exception_handler(uint32_t err_code);
void idt_init();

typedef struct {
	uint16_t bits_offset_lower;
	uint16_t sel;
	uint8_t zero;
	uint8_t type_attr;
	uint16_t bits_offset_higher;
} __attribute__((packed)) idt_entry_t;

#endif