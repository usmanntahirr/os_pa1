#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>

#include <driver/keyboard.h>
#include <utils.h>
#include <interrupts.h>

//! a ring buffer to store input characters 
#define KBD_RING_BUF_SIZE 32

//! private data

static KBD_ENTRY _kbd_ring_buffer[KBD_RING_BUF_SIZE];	// this buffer only stores the valid keycodes
static uint32_t  _kbd_ring_buffer_head = 0; // head of the ring buffer
static uint32_t  _kbd_ring_buffer_tail = 0; // tail of the ring buffer

//! private functions for ring buffer management
static bool 	 _kbd_ring_buffer_full(void);
static bool 	 _kbd_ring_buffer_empty(void);
static void 	 _kbd_ring_buffer_push(KBD_KEYCODE key);
static KBD_ENTRY _kbd_ring_buffer_pop(void);

//! private function prototypes


//! standard keyboard scancode set. array index represents the make code
static const KBD_KEYCODE _kbd_scancode_set[] = {
	KEY_UNKNOWN, KEY_ESCAPE,									// 0x00 - 0x01
	KEY_1, KEY_2, KEY_3, KEY_4, KEY_5,							// 0x02 - 0x06
	KEY_6, KEY_7, KEY_8, KEY_9, KEY_0,							// 0x07 - 0x0b
	KEY_MINUS, KEY_EQUAL, KEY_BACKSPACE, KEY_TAB,				// 0x0c - 0x0f
	KEY_Q, KEY_W, KEY_E, KEY_R, KEY_T,							// 0x10 - 0x14
	KEY_Y, KEY_U, KEY_I, KEY_O, KEY_P,							// 0x15 - 0x19
	KEY_LEFTBRACKET, KEY_RIGHTBRACKET, KEY_RETURN, KEY_LCTRL,   // 0x1a - 0x1d
	KEY_A, KEY_S, KEY_D, KEY_F, KEY_G,							// 0x1e - 0x22
	KEY_H, KEY_J, KEY_K, KEY_L, KEY_SEMICOLON,					// 0x23 - 0x27
	KEY_QUOTE, KEY_GRAVE, KEY_LSHIFT, KEY_BACKSLASH, 			// 0x28 - 0x2b
	KEY_Z, KEY_X, KEY_C, KEY_V, KEY_B, KEY_N, KEY_M,			// 0x2c - 0x32
	KEY_COMMA, KEY_DOT, KEY_SLASH, KEY_RSHIFT,					// 0x33 - 0x36
	KEY_KP_ASTERISK, KEY_RALT, KEY_SPACE, KEY_CAPSLOCK,			// 0x37 - 0x3a
	KEY_F1, KEY_F2, KEY_F3, KEY_F4, KEY_F5, 					// 0x3b - 0x3f
	KEY_F6, KEY_F7, KEY_F8, KEY_F9, KEY_F10,					// 0x40 - 0x44
	KEY_KP_NUMLOCK, KEY_SCROLLLOCK, KEY_HOME,					// 0x45 - 0x47
	KEY_KP_8, KEY_PAGEUP,										// 0x48 - 0x49
	KEY_KP_2, KEY_KP_3, KEY_KP_0, KEY_KP_DECIMAL,				// 0x50 - 0x53
	KEY_UNKNOWN, KEY_UNKNOWN, KEY_UNKNOWN, 						// 0x54 - 0x56
	KEY_F11, KEY_F12											// 0x57 - 0x58
};

#define KBD_SCANCODE_BREAK 0x80 // break code offset

//! check if the keyboard ring buffer is full
bool _kbd_ring_buffer_full (void) {
	return ((_kbd_ring_buffer_head + 1) % KBD_RING_BUF_SIZE) == _kbd_ring_buffer_tail;
}
//! check if the keyboard ring buffer is empty
bool _kbd_ring_buffer_empty (void) {
	return _kbd_ring_buffer_head == _kbd_ring_buffer_tail;
}

//! push a key to the keyboard ring buffer, to be retrieved by higher level functions
void _kbd_ring_buffer_push(KBD_KEYCODE key) {
	if (_kbd_ring_buffer_full()) {
		return; // buffer is full, drop the key
	}
	_kbd_ring_buffer[_kbd_ring_buffer_head].keycode = key; // push the key to the buffer
	_kbd_ring_buffer[_kbd_ring_buffer_head].ascii   = kbd_keycode_to_ascii(key); // store ascii translation
	_kbd_ring_buffer_head = (_kbd_ring_buffer_head + 1) % KBD_RING_BUF_SIZE; // update head
}

//! pop a key from the keyboard ring buffer, to be retrieved by higher level functions
KBD_ENTRY _kbd_ring_buffer_pop (void) {
	if (_kbd_ring_buffer_empty()) {
		return (KBD_ENTRY) {.keycode=KEY_UNKNOWN, .ascii='\0'}; // buffer is empty, return invalid key
	}
	KBD_ENTRY key = _kbd_ring_buffer[_kbd_ring_buffer_tail]; // pop the key from the buffer
	_kbd_ring_buffer_tail = (_kbd_ring_buffer_tail + 1) % KBD_RING_BUF_SIZE; // update tail
	return key;
}

//! translates a keycode to its ascii representation, taking into account the
//! state of the modifier keys (shift, capslock)
char kbd_keycode_to_ascii (KBD_KEYCODE key) {

	// TODO : start your implementation here
}