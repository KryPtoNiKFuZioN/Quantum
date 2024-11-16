#include "ps2.h"

void ps2_initialize() {
    // Flush output buffer
    while (inb(PS2_CMD_PORT) & 0x01) {
        inb(PS2_DATA_PORT);
    }

    // Disable devices
    outb(PS2_CMD_PORT, 0xAD); // Disable keyboard
    outb(PS2_CMD_PORT, 0xA7); // Disable mouse

    // Enable keyboard
    outb(PS2_CMD_PORT, PS2_CMD_ENABLE_KEYBOARD);
}

void ps2_write(uint8_t command) {
    // Wait until the input buffer is empty
    while (inb(PS2_CMD_PORT) & 0x02);
    outb(PS2_DATA_PORT, command);
}

uint8_t ps2_read() {
    // Wait until the output buffer is full
    while (!(inb(PS2_CMD_PORT) & 0x01));
    return inb(PS2_DATA_PORT);
}
