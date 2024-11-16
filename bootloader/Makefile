TARGET = bootloader.efi
ARCH = x86_64
CFLAGS = -I /path/to/uefi/includes -nostdlib -ffreestanding
LDFLAGS = -Ttext 0x1000 -nostdlib -static

all: $(TARGET)

$(TARGET): bootloader.c
    clang $(CFLAGS) bootloader.c -o $(TARGET) $(LDFLAGS)

clean:
    rm -f $(TARGET)
