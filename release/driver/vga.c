// made this file as mentioned in the manual
#include "driver/vga.h"
#include "utils.h"  // for outb



// unit8_t -> unsigned int 8 bits long (1 byte)
// enum has range 0-15, thus 4 bits max
uint8_t vga_entry_color(enum vga_color fg, enum vga_color bg)
{
    // combines foreground and background colour into 1 byte
    // top 4 bits represent the background color, while the low 4 bits represent the FG colors
    uint8_t left = (bg & 0x0F) << 4; // take lower 4 bits of bg and shift 4 places to the left
    uint8_t right = (fg & 0x0F);
    uint8_t final_byte = left | right; // bitwise or to concatenate the 2
    return final_byte;
}

// Note that vga entry t is a simple typedef for the type uint16_t (defined in vga.h).
vga_entry_t vga_entry(unsigned char uc, uint8_t color)
{
    // Byte 0 : ASCII character code
    // Byte 1 : BG + FG colors

    vga_entry_t byte_0 = (vga_entry_t)uc; // typecasting needed becasue "unsigned char" and "unit8_t" are both less than the output of 16 bits
    vga_entry_t byte_1 = (vga_entry_t)color << 8;
    return byte_1 | byte_0;
}

void vga_move_cursor_to(uint8_t x, uint8_t y) //-> manipulates registers
{
    // vga width = 80
    // vga height = 25 // vga size = 80 * 25
    // vga entry size =  2
    // total buffer size = 80 *25 * 2 = 4000

    if (x >= VGA_WIDTH || y >= VGA_HEIGHT)
    {
        return; // Exit if out of bounds
    }

    // total no of chars till position (row,col) is =  starting position + (row * vga width) + colounm -> converting 2d array into 1d
    // low byte(0x3D4) -> command
    // high byte(0x3D5) -> data

    uint16_t cursor_position = ((uint16_t)y * VGA_WIDTH) + x;
    uint8_t cursor_position_low = (uint8_t)(cursor_position & 0xFF);
    uint8_t cursor_position_high = (uint8_t)(cursor_position >> 8);

    outb(VGA_HARDWARE_CURSOR_CMD_SETLOW, VGA_CRTC_INDEX_PORT);
    outb(cursor_position_low, VGA_CRTC_DATA_PORT);

    outb(VGA_HARDWARE_CURSOR_CMD_SETHIGH, VGA_CRTC_INDEX_PORT);
    outb(cursor_position_high, VGA_CRTC_DATA_PORT);
}

void vga_putentry_at (vga_entry_t entry, uint8_t x, uint8_t y)
{
    if (x >= VGA_WIDTH || y >= VGA_HEIGHT) return;
    vga_entry_t* vga_array = (vga_entry_t*)(uintptr_t)VGA_ADDRESS;
    vga_array[(y*(VGA_WIDTH))+x]= entry;
}

vga_entry_t* vga_get_screen_buffer (void)
{
    return (vga_entry_t*)(uintptr_t)VGA_ADDRESS;
}