#include <stdint.h>
#include <stddef.h>

// Mock hardware definitions
#define NIC_BASE_ADDR 0xFE000000
#define NIC_REG_TX    0x00  // Transmit register offset
#define NIC_REG_RX    0x04  // Receive register offset
#define NIC_REG_STATUS 0x08 // Status register offset

// Function prototypes
void nic_init(void);
void nic_send_packet(const void *data, size_t len);
size_t nic_receive_packet(void *buffer, size_t buffer_len);

// Utility functions to access hardware
static inline void write_register(uintptr_t reg, uint32_t value) {
    *((volatile uint32_t *)(NIC_BASE_ADDR + reg)) = value;
}

static inline uint32_t read_register(uintptr_t reg) {
    return *((volatile uint32_t *)(NIC_BASE_ADDR + reg));
}

// NIC initialization
void nic_init(void) {
    // Reset and initialize NIC hardware
    write_register(NIC_REG_STATUS, 0x1); // Example: Reset NIC
    // Add more configuration as needed
}

// Sending a packet
void nic_send_packet(const void *data, size_t len) {
    // Check for valid input
    if (data == NULL || len == 0) return;

    // Write data to transmit buffer
    for (size_t i = 0; i < len; i += 4) {
        uint32_t word = *((const uint32_t *)((const uint8_t *)data + i));
        write_register(NIC_REG_TX, word);
    }
}

// Receiving a packet
size_t nic_receive_packet(void *buffer, size_t buffer_len) {
    if (!buffer || buffer_len == 0) return 0;

    size_t received = 0;
    while (received < buffer_len) {
        uint32_t word = read_register(NIC_REG_RX);
        if (word == 0) break; // No more data
        *((uint32_t *)((uint8_t *)buffer + received)) = word;
        received += 4;
    }

    return received;
}
