#ifndef _KERNEL_INIT_H
#define _KERNEL_INIT_H

#include <kernel/stdio.h>

void initialize_gdt();
void initialize_idt();

#endif