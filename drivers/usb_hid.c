#include "usb.h"

void usb_hid_process_input(uint8_t *data, size_t length) {
    // Simple example: handle keyboard input
    if (length >= 8) {
        uint8_t key = data[2]; // HID usage for keyboard keys
        if (key) {
            printf("Key pressed: 0x%x\n", key);
        }
    }
}
