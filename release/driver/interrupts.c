#include <stdint.h>
#include <utils.h>
#include <init/interrupts.h>
#include <init/pic.h>   // if you have a pic.h header with these; otherwise forward-declare

// If no header is available, uncomment these forward decls:
// void pic_remap(uint8_t off1, uint8_t off2);
// void pic_set_mask(uint8_t irq);
// void pic_clear_mask(uint8_t irq);
// void pic_send_eoi(uint8_t irq);

void interrupts_enable(void)  { __asm__ __volatile__("sti"); }
void interrupts_disable(void) { __asm__ __volatile__("cli"); }

void pic_eoi(uint8_t irq) {    // small wrapper for tests
    pic_send_eoi(irq);
}

void interrupts_init(void) {
    // Standard remap: master=0x20, slave=0x28
    pic_remap(0x20, 0x28);

    // Mask everything first (optional, depending on your tests)
    for (uint8_t i = 0; i < 16; ++i) pic_set_mask(i);

    // Unmask IRQ1 (keyboard); unmask others if your tests need them
    pic_clear_mask(1);
    // If timer needed in tests: pic_clear_mask(0);

    // Do not sti() here—let tests call enable/disable explicitly if they want.
}