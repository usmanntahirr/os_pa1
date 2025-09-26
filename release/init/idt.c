#include <stdint.h>
#include <string.h>        // memset
#include <init/idt.h>

static idt_entry_t idt[IDT_MAX_DESCRIPTORS];
static idt_ptr_t   idtr;

static inline void lidt(const idt_ptr_t* p) {
    __asm__ __volatile__("lidt %0" : : "m"(*p));
}

// ISR/IRQ stubs from isr.s
extern void isr0(void);  extern void isr1(void);  extern void isr2(void);  extern void isr3(void);
extern void isr4(void);  extern void isr5(void);  extern void isr6(void);  extern void isr7(void);
extern void isr8(void);  extern void isr9(void);  extern void isr10(void); extern void isr11(void);
extern void isr12(void); extern void isr13(void); extern void isr14(void); extern void isr15(void);
extern void isr16(void); extern void isr17(void); extern void isr18(void); extern void isr19(void);
extern void isr20(void); extern void isr21(void); extern void isr22(void); extern void isr23(void);
extern void isr24(void); extern void isr25(void); extern void isr26(void); extern void isr27(void);
extern void isr28(void); extern void isr29(void); extern void isr30(void); extern void isr31(void);

extern void irq0(void);  extern void irq1(void);  extern void irq2(void);  extern void irq3(void);
extern void irq4(void);  extern void irq5(void);  extern void irq6(void);  extern void irq7(void);
extern void irq8(void);  extern void irq9(void);  extern void irq10(void); extern void irq11(void);
extern void irq12(void); extern void irq13(void); extern void irq14(void); extern void irq15(void);

void idt_set_gate(uint8_t num, uint32_t base, uint16_t sel, uint8_t flags) {
    idt[num].base_low  = (uint16_t)(base & 0xFFFF);
    idt[num].sel       = sel;
    idt[num].zero      = 0;
    idt[num].flags     = flags;
    idt[num].base_high = (uint16_t)((base >> 16) & 0xFFFF);
}

void idt_init(void) {
    memset(idt, 0, sizeof(idt));
    idtr.limit = (uint16_t)(sizeof(idt_entry_t) * IDT_MAX_DESCRIPTORS - 1);
    idtr.base  = (uint32_t)&idt[0];

    const uint8_t gate = IDT_FLAG_PRESENT | IDT_FLAG_DPL0 | IDT_FLAG_INT_GATE_32;

    // CPU exceptions 0..31
    idt_set_gate(0,  (uint32_t)isr0,  KERNEL_CODE_SELECTOR, gate);
    idt_set_gate(1,  (uint32_t)isr1,  KERNEL_CODE_SELECTOR, gate);
    idt_set_gate(2,  (uint32_t)isr2,  KERNEL_CODE_SELECTOR, gate);
    idt_set_gate(3,  (uint32_t)isr3,  KERNEL_CODE_SELECTOR, gate);
    idt_set_gate(4,  (uint32_t)isr4,  KERNEL_CODE_SELECTOR, gate);
    idt_set_gate(5,  (uint32_t)isr5,  KERNEL_CODE_SELECTOR, gate);
    idt_set_gate(6,  (uint32_t)isr6,  KERNEL_CODE_SELECTOR, gate);
    idt_set_gate(7,  (uint32_t)isr7,  KERNEL_CODE_SELECTOR, gate);
    idt_set_gate(8,  (uint32_t)isr8,  KERNEL_CODE_SELECTOR, gate);
    idt_set_gate(9,  (uint32_t)isr9,  KERNEL_CODE_SELECTOR, gate);
    idt_set_gate(10, (uint32_t)isr10, KERNEL_CODE_SELECTOR, gate);
    idt_set_gate(11, (uint32_t)isr11, KERNEL_CODE_SELECTOR, gate);
    idt_set_gate(12, (uint32_t)isr12, KERNEL_CODE_SELECTOR, gate);
    idt_set_gate(13, (uint32_t)isr13, KERNEL_CODE_SELECTOR, gate);
    idt_set_gate(14, (uint32_t)isr14, KERNEL_CODE_SELECTOR, gate);
    idt_set_gate(15, (uint32_t)isr15, KERNEL_CODE_SELECTOR, gate);
    idt_set_gate(16, (uint32_t)isr16, KERNEL_CODE_SELECTOR, gate);
    idt_set_gate(17, (uint32_t)isr17, KERNEL_CODE_SELECTOR, gate);
    idt_set_gate(18, (uint32_t)isr18, KERNEL_CODE_SELECTOR, gate);
    idt_set_gate(19, (uint32_t)isr19, KERNEL_CODE_SELECTOR, gate);
    idt_set_gate(20, (uint32_t)isr20, KERNEL_CODE_SELECTOR, gate);
    idt_set_gate(21, (uint32_t)isr21, KERNEL_CODE_SELECTOR, gate);
    idt_set_gate(22, (uint32_t)isr22, KERNEL_CODE_SELECTOR, gate);
    idt_set_gate(23, (uint32_t)isr23, KERNEL_CODE_SELECTOR, gate);
    idt_set_gate(24, (uint32_t)isr24, KERNEL_CODE_SELECTOR, gate);
    idt_set_gate(25, (uint32_t)isr25, KERNEL_CODE_SELECTOR, gate);
    idt_set_gate(26, (uint32_t)isr26, KERNEL_CODE_SELECTOR, gate);
    idt_set_gate(27, (uint32_t)isr27, KERNEL_CODE_SELECTOR, gate);
    idt_set_gate(28, (uint32_t)isr28, KERNEL_CODE_SELECTOR, gate);
    idt_set_gate(29, (uint32_t)isr29, KERNEL_CODE_SELECTOR, gate);
    idt_set_gate(30, (uint32_t)isr30, KERNEL_CODE_SELECTOR, gate);
    idt_set_gate(31, (uint32_t)isr31, KERNEL_CODE_SELECTOR, gate);

    // Remapped IRQs at 0x20..0x2F
    idt_set_gate(0x20, (uint32_t)irq0,  KERNEL_CODE_SELECTOR, gate);
    idt_set_gate(0x21, (uint32_t)irq1,  KERNEL_CODE_SELECTOR, gate);
    idt_set_gate(0x22, (uint32_t)irq2,  KERNEL_CODE_SELECTOR, gate);
    idt_set_gate(0x23, (uint32_t)irq3,  KERNEL_CODE_SELECTOR, gate);
    idt_set_gate(0x24, (uint32_t)irq4,  KERNEL_CODE_SELECTOR, gate);
    idt_set_gate(0x25, (uint32_t)irq5,  KERNEL_CODE_SELECTOR, gate);
    idt_set_gate(0x26, (uint32_t)irq6,  KERNEL_CODE_SELECTOR, gate);
    idt_set_gate(0x27, (uint32_t)irq7,  KERNEL_CODE_SELECTOR, gate);
    idt_set_gate(0x28, (uint32_t)irq8,  KERNEL_CODE_SELECTOR, gate);
    idt_set_gate(0x29, (uint32_t)irq9,  KERNEL_CODE_SELECTOR, gate);
    idt_set_gate(0x2A, (uint32_t)irq10, KERNEL_CODE_SELECTOR, gate);
    idt_set_gate(0x2B, (uint32_t)irq11, KERNEL_CODE_SELECTOR, gate);
    idt_set_gate(0x2C, (uint32_t)irq12, KERNEL_CODE_SELECTOR, gate);
    idt_set_gate(0x2D, (uint32_t)irq13, KERNEL_CODE_SELECTOR, gate);
    idt_set_gate(0x2E, (uint32_t)irq14, KERNEL_CODE_SELECTOR, gate);
    idt_set_gate(0x2F, (uint32_t)irq15, KERNEL_CODE_SELECTOR, gate);

    lidt(&idtr);
}
