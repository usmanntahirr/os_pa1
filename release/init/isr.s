/*******************************************************************************
 * 
 * @file 	isr.h
 * @author  
 * @brief
 * 
 * The stubs for the first 32 CPU exceptions and interrupts + irqs are declared
 *  here.
 * 
 * These are the standard CPU exceptions and interrupts that are defined by the
 *  x86 architecture. These ISRs will be linked externally via the assembly
 *  implementations.
 * 
 * Note that interrupts 0-31 are reserved for CPU exceptions and interrupts, 
 *  and are not used for user-defined interrupts.
 *  Beyond that, you can define your own interrupts in the range 32-255.
 * 
 * **Exception classification:**
 * - Faults: These exceptions can be handled by the operating system, allowing
 *  		 it to recover. Ret address of the instruction that caused the 
 * 			 fault is pushed onto the stack, instead of the next one.
 * - Traps:  A trap is an exception that is reported immediately following
 * 			 the execution of the trapping instruction. Ret address of the next
 * 			 instruction is pushed.
 * - Aborts: An abort is an exception that does not always report the precise
 * 			 location of the instruction that caused the abort. It is used for
 * 			 unrecoverable errors, such as hardware failures. No return address
 * 			 is pushed onto the stack.
 * 
*******************************************************************************/

KERNEL_CODE_SEGMENT = 0x08      /* Kernel code segment offset in the GDT */
KERNEL_DATA_SEGMENT = 0x10      /* Kernel data segment offset in the GDT */

.extern    interrupt_dispatch   /* Declare the external C handler for ISRs */

/* Interrupt handlers entry points will look something like this:
    .globl isr0
isr0:
    pushl $0 # push dummy error code
    pushl $0 # push interrupt number
    jmp isr_common_handler # jump instead of a call


     */


/* We then call the macros to put in place handlers for all exceptions. Note
    that the definitions for all 256 entries will be provided on the per
    need basis. */

// -- ADD THE ASSEMBLY ENTRY POINTS HERE FOR THE 32 EXCEPTIONS -- //

// CPU exceptions


// reserved interrupts from 22 to 31


// The following are the IRQ definitions for the PIC generated interrupts.
// master pic IRQs are from 0 to 7, and slave pic IRQs are from 8 to 15.


// interrupt 0x80 is used for system calls, so we configure the entry for that.

/**
 * @brief Common ISR handler that saves the state, calls the C handler, 
 *          and restores the state. This function is called by all ISRs to
 *          handle the interrupt. It saves the registers, sets up the segment
 *          registers, calls the C handler (interrupt_dispatch), and restores
 *          the state before returning.
 */

.type isr_common_handler, @function
isr_common_handler:
    
    /* 1.save1 the GP registers */
    /* in the order: eax, ecx, edx, ebx, old esp, ebp, esi, edi */
    
    /* 2. save the data segment register */

    /* 3. Call the C handler for the interrupt dispatching */   

    /* clean up the stack and restore the state */

    /* restore the gp registers */

    /* stack cleanup, must be restored to the state before the ISR. Done because
        the iret instruction expects the previously automatically pushed fields:
        ss, eflags, cs, ip. These are pushed automatically by the CPU when an
        interrupt occurs. */
    
    /* Return from the interrupt, restoring the flags and segments */
