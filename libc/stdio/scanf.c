#include <stddef.h>
#include <kernel/kbd.h>
#include <stdio.h>

void scanf(char* buffer, size_t max_length) {
  size_t i = 0;
  
  while(1) {
    key_event_t e = get_key_event();
    if (e.flags & RELEASED) continue;

    char c = e.keycode;
    if (c == '\0') continue;

    if (c == '\n') {
      buffer[i] = '\0';
      putchar('\n');
      return;
    } else if (c == '\b') {
        if (i > 0) {
          buffer[i--] = '\0';
          putchar('\b');
          putchar(' ');
          putchar('\b');
        }
    } else if (i < max_length - 1) {
      buffer[i++] = c;
      putchar(c);
    }
  }

  return;
}