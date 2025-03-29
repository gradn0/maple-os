#ifndef _KERNEL_KBD_H
#define _KERNEL_KBD_H

#include <stdint.h>

typedef struct {
  uint8_t normal;
  uint8_t shifted;
} keycode_t;

#define KEY_EVENT_BUFFER_SIZE 128
#define SUPPORTED_MODS        5

// Modifier scancodes
#define SCANCODE_CTRL         0x1D
#define SCANCODE_LEFT_SHIFT   0x2A
#define SCANCODE_RIGHT_SHIFT  0x36
#define SCANCODE_ALT          0x38
#define SCANCODE_CAPS         0x3A

// Key event flags
#define RELEASED    (1 << 0)
#define MOD_CTRL    (1 << 1)
#define MOD_SHIFT   (1 << 2)
#define MOD_ALT     (1 << 3)
#define MOD_CAPS    (1 << 4)

typedef struct {
  uint8_t scancode;
  uint8_t keycode;
  uint8_t flags;
} key_event_t;

// PS/2 IO ports
#define PS2_DATA      0x60    // Read/write device or controller
#define PS2_STATUS    0x64    // Read status of the controller
#define PSW_COMMAND   0x64    // Write commands to the controller (not device)

void handle_keyboard_irq();   
key_event_t get_key_event();

#endif