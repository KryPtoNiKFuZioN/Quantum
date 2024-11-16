#include "usb.h"
#include "xhci.h"
#include "pci.h"

void usb_init() {
    // Locate xHCI controller via PCI
    pci_device_t xhci_controller;
    if (pci_find_device(PCI_CLASS_SERIAL, PCI_SUBCLASS_USB, &xhci_controller)) {
        printf("xHCI Controller found: Vendor=0x%x, Device=0x%x\n", xhci_controller.vendor_id, xhci_controller.device_id);

        // Initialize xHCI controller
        if (xhci_controller.prog_if == 0x30) { // xHCI programming interface
            xhci_init(&xhci_controller);
        } else {
            printf("Unsupported USB controller interface: 0x%x\n", xhci_controller.prog_if);
        }
    } else {
        printf("No xHCI Controller found.\n");
    }
}
