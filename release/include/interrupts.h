#include <init/interrupts.h>
#pragma once
#include <stdint.h>

void interrupts_init(void);     // remap PIC, unmask needed IRQs
void interrupts_enable(void);   // sti
void interrupts_disable(void);  // cli

// Some tests use these names; provide aliases:
static inline void enable_interrupts(void)  { interrupts_enable();  }
static inline void disable_interrupts(void) { interrupts_disable(); }

// EOI helper if tests poke it:
void pic_eoi(uint8_t irq);
