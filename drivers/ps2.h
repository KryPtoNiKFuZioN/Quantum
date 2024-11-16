#ifndef PS2_H
#define PS2_H

#include <stdint.h>
#include "io.h"

#define PS2_CMD_PORT 0x64
#define PS2_DATA_PORT 0x60

#define PS2_CMD_ENABLE_KEYBOARD 0xAE

void ps2_initialize();
void ps2_write(uint8_t command);
uint8_t ps2_read();

#endif
