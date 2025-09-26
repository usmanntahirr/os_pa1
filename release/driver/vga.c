#include <driver/vga.h>
#include <utils.h>   // inb/outb prototypes (byte I/O to ports)

/*
 * Coordinate convention:
 *   x = column [0..VGA_WIDTH-1]
 *   y = row    [0..VGA_HEIGHT-1]
 * Linear index in text buffer = y * VGA_WIDTH + x
 */

static inline uint16_t vga_index(uint8_t x, uint8_t y) {
    return (uint16_t)((uint16_t)y * (uint16_t)VGA_WIDTH + (uint16_t)x);
}

uint8_t vga_entry_color(enum vga_color fg, enum vga_color bg) {
    // High nibble = background, low nibble = foreground
    return (uint8_t)(((bg & 0x0F) << 4) | (fg & 0x0F));
}

vga_entry_t vga_entry(unsigned char uc, uint8_t color) {
    // Low byte = ASCII char, high byte = color
    return (vga_entry_t)((vga_entry_t)uc | ((vga_entry_t)color << 8));
}

void vga_move_cursor_to(uint8_t x, uint8_t y) {
    // Clamp to screen to avoid invalid positions
    if (x >= VGA_WIDTH)  x = VGA_WIDTH  - 1;
    if (y >= VGA_HEIGHT) y = VGA_HEIGHT - 1;

    uint16_t pos = vga_index(x, y);

    // Set hardware cursor: write high byte, then low byte via CRTC
    outb(VGA_HARDWARE_CURSOR_CMD_SETHIGH, VGA_CRTC_INDEX_PORT);
    outb((uint8_t)(pos >> 8),             VGA_CRTC_DATA_PORT);

    outb(VGA_HARDWARE_CURSOR_CMD_SETLOW,  VGA_CRTC_INDEX_PORT);
    outb((uint8_t)(pos & 0xFF),           VGA_CRTC_DATA_PORT);
}

void vga_putentry_at(vga_entry_t entry, uint8_t x, uint8_t y) {
    if (x >= VGA_WIDTH || y >= VGA_HEIGHT) return;  // stay inside the 80x25 buffer

    // Write to text buffer at 0xB8000 (each entry = 2 bytes)
    volatile vga_entry_t* buf = (volatile vga_entry_t*)(uintptr_t)VGA_ADDRESS;
    buf[vga_index(x, y)] = entry;
}

vga_entry_t* vga_get_screen_buffer(void) {
    // Return a typed pointer to the start of the VGA text buffer
    return (vga_entry_t*)(uintptr_t)VGA_ADDRESS;
}
