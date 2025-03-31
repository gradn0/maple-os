#ifndef _KERNEL_KPRINT_H
#define _KERNEL_KPRINT_H

#include <stdio.h>
#include <kernel/tty.h>
#include <stdlib.h>

void panic(const char* str);
void kerror(const char* str);
void kwarning(const char* str);
void klog(const char* str);

#endif