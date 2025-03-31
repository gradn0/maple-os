#include <kernel/kprint.h>

#include "vga.h"

void panic(const char* str) {
	kerror("KERNEL PANIC:");
	kerror(str);
	abort();
}

void kerror(const char* str) {
	uint8_t prev_trm_color = terminal_getcolor();
	terminal_setcolor(VGA_COLOR_LIGHT_CYAN);
	printf("%s\n", str);
	terminal_setcolor(prev_trm_color);
}

void kwarning(const char* str) {
	uint8_t prev_trm_color = terminal_getcolor();
	terminal_setcolor(VGA_COLOR_RED);
	printf("%s\n", str);
	terminal_setcolor(prev_trm_color);
}

void klog(const char* str) {
  printf("[INFO] %s\n", str);
}