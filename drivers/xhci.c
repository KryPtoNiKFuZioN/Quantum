#include "xhci.h"
#include "pci.h"
#include "io.h"

#define XHCI_CAP_LENGTH       0x00
#define XHCI_OPER_BASE        0x10
#define XHCI_DOORBELL_BASE    0x20
#define XHCI_RUNTIME_BASE     0x30

static uint64_t mmio_base;

void xhci_init(pci_device_t *xhci_controller) {
    // Enable MMIO and bus mastering
    pci_enable_bus_mastering(xhci_controller);

    // Retrieve MMIO base address from BAR0
    mmio_base = pci_read_long(xhci_controller->bus, xhci_controller->device, xhci_controller->function, PCI_BAR0) & ~0xF;

    // Verify MMIO access
    uint8_t cap_length = mmio_read8(mmio_base + XHCI_CAP_LENGTH);
    if (cap_length == 0) {
        printf("Failed to access xHCI MMIO.\n");
        return;
    }
    printf("xHCI Controller MMIO Base: 0x%lx\n", mmio_base);

    // Additional xHCI-specific initialization here (e.g., setup operational registers)

    printf("xHCI Controller initialized.\n");
}
