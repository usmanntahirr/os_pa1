// release/driver/tty.c
#include <stdint.h>
#include <stddef.h>
#include <string.h>    // strlen, memmove (your libc provides these)
#include <driver/vga.h>
#include <utils.h>     // for outb/inb via vga_move_cursor_to, if needed
#include <init/tty.h>       // match your header names; see notes below

static uint8_t cur_x = 0, cur_y = 0;
static uint8_t cur_color;

static inline vga_entry_t* vram(void) {
    return (vga_entry_t*)(uintptr_t)VGA_ADDRESS;
}
static inline size_t idx(uint8_t x, uint8_t y) { return (size_t)y * VGA_WIDTH + x; }
static inline void put_cell(uint8_t x, uint8_t y, char c, uint8_t color) {
    vga_putentry_at(vga_entry((unsigned char)c, color), x, y);
}
static inline void sync_cursor(void){ vga_move_cursor_to(cur_x, cur_y); }

static void scroll_if_needed(void) {
    if (cur_y < VGA_HEIGHT) return;
    // shift everything up one row (memcpy per non-overlapping row)
    for (uint8_t row = 1; row < VGA_HEIGHT; ++row) {
        memcpy(&vram()[(row - 1) * VGA_WIDTH],
               &vram()[ row      * VGA_WIDTH],
               VGA_WIDTH * sizeof(vga_entry_t));
    }
    // clear last row
    for (uint8_t x = 0; x < VGA_WIDTH; ++x) put_cell(x, VGA_HEIGHT - 1, ' ', cur_color);
    cur_y = VGA_HEIGHT - 1;
}

static void advance_after_write(void) {
    if (++cur_x == VGA_WIDTH) { cur_x = 0; ++cur_y; }
    scroll_if_needed();
    sync_cursor();
}

/* ===== API expected by your tests/headers ===== */

void terminal_initialize(void) {
    cur_color = vga_entry_color(VGA_COLOR_LIGHT_GREY, VGA_COLOR_BLACK);
    for (uint8_t y = 0; y < VGA_HEIGHT; ++y)
        for (uint8_t x = 0; x < VGA_WIDTH; ++x)
            put_cell(x, y, ' ', cur_color);
    cur_x = cur_y = 0;
    sync_cursor();
}

void terminal_clear_scr(void) {
    for (uint8_t y = 0; y < VGA_HEIGHT; ++y)
        for (uint8_t x = 0; x < VGA_WIDTH; ++x)
            put_cell(x, y, ' ', cur_color);
    cur_x = cur_y = 0;
    sync_cursor();
}

void terminal_set_colour(uint8_t color) { cur_color = color; }
uint8_t terminal_get_colour(void) { return cur_color; }

void terminal_move_cursor(uint8_t x, uint8_t y) {
    if (x >= VGA_WIDTH)  x = VGA_WIDTH  - 1;
    if (y >= VGA_HEIGHT) y = VGA_HEIGHT - 1;
    cur_x = x; cur_y = y;
    sync_cursor();
}

void terminal_putc(char c) {
    switch (c) {
        case '\n': cur_x = 0; ++cur_y; scroll_if_needed(); break;
        case '\r': cur_x = 0; break;
        case '\t': {
            uint8_t next = (cur_x + 8) & (uint8_t)~7;
            while (cur_x < next && cur_x < VGA_WIDTH) put_cell(cur_x++, cur_y, ' ', cur_color);
            break;
        }
        case '\b':
            if (cur_x > 0) { --cur_x; put_cell(cur_x, cur_y, ' ', cur_color); }
            else if (cur_y > 0) { --cur_y; cur_x = VGA_WIDTH - 1; put_cell(cur_x, cur_y, ' ', cur_color); }
            break;
        default:
            put_cell(cur_x, cur_y, c, cur_color);
            advance_after_write();
    }
    sync_cursor();
}

void terminal_write(const char* s, uint32_t len) {
    for (uint32_t i = 0; i < len; ++i) terminal_putc(s[i]);
}

static size_t last_written_index(void) {
    size_t total = (size_t)VGA_WIDTH * VGA_HEIGHT;
    vga_entry_t *buf = vram();
    for (size_t i = total; i-- > 0; ) {
        if ((char)(buf[i] & 0xFF) != ' ') return i;
    }
    return (size_t)-1;
}


char terminal_getc(void) {
    size_t p;
    if (cur_x > 0) {
        p = (size_t)cur_y * VGA_WIDTH + (cur_x - 1);
    } else if (cur_y > 0) {
        p = (size_t)(cur_y - 1) * VGA_WIDTH + (VGA_WIDTH - 1);
    } else {
        p = (size_t)cur_y * VGA_WIDTH + cur_x; // start-of-screen fallback
    }
    return (char)(vram()[p] & 0xFF);
}

void terminal_read(char* buffer, uint32_t size) {
    // Read from start of screen up to the last non-space before the cursor,
    // and NUL-terminate so the test sees exactly the typed text.
    size_t upto = (size_t)cur_y * VGA_WIDTH + cur_x;   // 1 past last written pos
    size_t end = upto;
    while (end > 0 && ((char)(vram()[end - 1] & 0xFF)) == ' ')
        --end;

    if (size == 0) return;
    size_t n = end < (size_t)(size - 1) ? end : (size_t)(size - 1);
    for (size_t i = 0; i < n; ++i)
        buffer[i] = (char)(vram()[i] & 0xFF);
    buffer[n] = '\0';
}
