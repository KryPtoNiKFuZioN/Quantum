#include "keyboard.h"
#include "ps2.h"
#include "interrupt.h"

#define KEYBOARD_BUFFER_SIZE 128

static char key_buffer[KEYBOARD_BUFFER_SIZE];
static int buffer_head = 0;
static int buffer_tail = 0;

// US keyboard layout map for scan codes
static const char scancode_to_ascii[128] = {
    0,  27, '1', '2', '3', '4', '5', '6', '7', '8', // 0-9
    '9', '0', '-', '=', '\b', '\t', 'q', 'w', 'e', 'r', 't', // 10-19
    'y', 'u', 'i', 'o', 'p', '[', ']', '\n', 0, 'a', // 20-29
    's', 'd', 'f', 'g', 'h', 'j', 'k', 'l', ';', '\'', '`', // 30-39
    0, '\\', 'z', 'x', 'c', 'v', 'b', 'n', 'm', ',', '.', '/', // 40-49
    0, '*', 0, ' ', 0, 0, 0, 0, 0, 0, // 50-59
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, // 60-69
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, // 70-79
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0  // 80-89
};

void keyboard_interrupt_handler() {
    uint8_t scancode = inb(0x60); // Read scan code from PS/2 port

    // Check for valid scan code and convert to ASCII
    if (scancode < 128) {
        char ascii = scancode_to_ascii[scancode];
        if (ascii) {
            // Add to the buffer
            key_buffer[buffer_head] = ascii;
            buffer_head = (buffer_head + 1) % KEYBOARD_BUFFER_SIZE;
        }
    }
}

void keyboard_init() {
    // Initialize the PS/2 controller
    ps2_initialize();

    // Register interrupt handler (IRQ1 is keyboard interrupt)
    register_interrupt_handler(IRQ1, keyboard_interrupt_handler);

    // Enable keyboard interrupts in the PS/2 controller
    ps2_write(PS2_CMD_ENABLE_KEYBOARD);
    Printf("Keyboard driver initialized.\n");
}

char keyboard_read() {
    // Wait until there is data in the buffer
    while (buffer_head == buffer_tail);

    // Read a character from the buffer
    char c = key_buffer[buffer_tail];
    buffer_tail = (buffer_tail + 1) % KEYBOARD_BUFFER_SIZE;
    return c;
}
