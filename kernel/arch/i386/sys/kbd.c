#include <kernel/kbd.h>
#include <kernel/io.h>

#include <stdbool.h>

const keycode_t keymap[128] = {
  {0,   0},
  {27,  27},    // Esc
  {'1', '!'},  
  {'2', '@'},  
  {'3', '#'},  
  {'4', '$'},  
  {'5', '%'},  
  {'6', '^'},  
  {'7', '&'},  
  {'8', '*'},  
  {'9', '('},  
  {'0', ')'},  
  {'-', '_'},  
  {'=', '+'},  
  {'\b','\b'},  // 0x0E Backspace
  {'\t','\t'},  // 0x0F Tab
  {'q', 'Q'},  
  {'w', 'W'},  
  {'e', 'E'},  
  {'r', 'R'},  
  {'t', 'T'},  
  {'y', 'Y'},  
  {'u', 'U'},  
  {'i', 'I'},  
  {'o', 'O'},  
  {'p', 'P'},  
  {'[', '{'},  
  {']', '}'},  
  {'\n','\n'}, // 0x1C Enter
  {0,   0},    // 0x1D Left Ctrl
  {'a', 'A'},  
  {'s', 'S'},  
  {'d', 'D'},  
  {'f', 'F'},  
  {'g', 'G'},  
  {'h', 'H'},  
  {'j', 'J'},  
  {'k', 'K'},  
  {'l', 'L'},  
  {';', ':'},  
  {'\'','"'},  
  {'`', '~'},  
  {0,   0},     // 0x2A Left Shift
  {'\\','|'},  
  {'z', 'Z'},  
  {'x', 'X'},  
  {'c', 'C'},  
  {'v', 'V'},  
  {'b', 'B'},  
  {'n', 'N'},  
  {'m', 'M'},  
  {',', '<'},  
  {'.', '>'},  
  {'/', '?'},  
  {0,   0},    // 0x36 Right Shift
  {'*', '*'},  // 0x37 Keypad *
  {0,   0},    // 0x38 Left Alt
  {' ', ' '},  // 0x39 Space
  {0,   0},    // 0x3A Caps Lock
};

// Helper map for setting the modifier flags given a scan code
typedef struct {uint8_t scancode; uint8_t mod_shift; bool toggled; } mod_map_t;
static const mod_map_t mod_map[SUPPORTED_MODS] = {
  {SCANCODE_CTRL,         MOD_CTRL,   false }, 
  {SCANCODE_LEFT_SHIFT,   MOD_SHIFT,  false }, 
  {SCANCODE_RIGHT_SHIFT,  MOD_SHIFT,  false }, 
  {SCANCODE_ALT,          MOD_ALT,    false }, 
  {SCANCODE_CAPS,         MOD_CAPS,   true }
};


static uint8_t current_modifiers = 0;
static key_event_t last_key_event = {0};

static void update_modifiers(uint8_t scancode, bool is_released) {
  for (size_t i = 0; i <= SUPPORTED_MODS; ++i) {
    if (mod_map[i].scancode == scancode) {
      if (mod_map[i].toggled) {
        if (!is_released) {
          current_modifiers ^= mod_map[i].mod_shift;  // Toggle modifiers like caps lock
        }
      }
      else if (is_released) {
        current_modifiers &= ~mod_map[i].mod_shift;   // Clear "held down" modifiers
      } else {
        current_modifiers |= mod_map[i].mod_shift;    // Set modifier
      }
    }
  }
}

void handle_keyboard_irq() {
  uint8_t scancode = inb(PS2_DATA);
  key_event_t key_event;

  // When a key is released, bit 7 is set
  bool is_released = scancode & 0x80;
  scancode = is_released ? scancode & 0x7F : scancode;
  update_modifiers(scancode, is_released);

  // Set scancode and keycode
  key_event.scancode = scancode;
  bool use_shifted = (current_modifiers & MOD_SHIFT) || (current_modifiers & MOD_CAPS);
  key_event.keycode = use_shifted ? keymap[scancode].shifted : keymap[scancode].normal;

  // Set flags
  if (is_released) {
    key_event.flags = current_modifiers | RELEASED;
  } else {
    key_event.flags = current_modifiers;
  }

  last_key_event = key_event;
}

key_event_t get_key_event() {
  key_event_t k = last_key_event;
  last_key_event.scancode = 0;
  last_key_event.keycode = 0;
  last_key_event.flags = 0;
  return k;
}