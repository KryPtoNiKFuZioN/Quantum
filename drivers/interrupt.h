#ifndef INTERRUPT_H
#define INTERRUPT_H

#include <stdint.h>

#define IRQ1 33 // IRQ1 for keyboard

typedef void (*interrupt_handler_t)(void);

void register_interrupt_handler(uint8_t irq, interrupt_handler_t handler);

#endif
