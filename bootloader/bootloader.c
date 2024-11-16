#include <efi.h>
#include <efilib.h>

EFI_STATUS efi_main(EFI_HANDLE ImageHandle, EFI_SYSTEM_TABLE *SystemTable) {
    InitializeLib(ImageHandle, SystemTable);

    // Clear screen
    SystemTable->ConOut->ClearScreen(SystemTable->ConOut);

    Print(L"Starting 64-bit OS bootloader...\n");

    // Locate and load kernel
    EFI_FILE_PROTOCOL *kernel;
    EFI_STATUS status = LoadKernel(ImageHandle, SystemTable, &kernel);
    if (EFI_ERROR(status)) {
        Print(L"Error loading kernel: %r\n", status);
        return status;
    }
    Print(L"Kernel loaded successfully.\n");

    // Exit boot services
    EFI_MEMORY_DESCRIPTOR *memoryMap = NULL;
    UINTN mapSize = 0, mapKey, descSize;
    UINT32 descVersion;
    SystemTable->BootServices->GetMemoryMap(&mapSize, memoryMap, &mapKey, &descSize, &descVersion);
    memoryMap = AllocatePool(mapSize);
    SystemTable->BootServices->GetMemoryMap(&mapSize, memoryMap, &mapKey, &descSize, &descVersion);
    SystemTable->BootServices->ExitBootServices(ImageHandle, mapKey);

    // Enter long mode and jump to kernel
    void (*kernel_entry)() = (void*)KERNEL_LOAD_ADDRESS;
    kernel_entry();

    return EFI_SUCCESS;
}

EFI_STATUS LoadKernel(EFI_HANDLE ImageHandle, EFI_SYSTEM_TABLE *SystemTable, EFI_FILE_PROTOCOL **kernel) {
    EFI_LOADED_IMAGE_PROTOCOL *loadedImage;
    EFI_SIMPLE_FILE_SYSTEM_PROTOCOL *fileSystem;
    EFI_FILE_PROTOCOL *rootDir;

    // Get the loaded image protocol
    SystemTable->BootServices->HandleProtocol(ImageHandle, &gEfiLoadedImageProtocolGuid, (void**)&loadedImage);

    // Get the filesystem
    SystemTable->BootServices->HandleProtocol(loadedImage->DeviceHandle, &gEfiSimpleFileSystemProtocolGuid, (void**)&fileSystem);

    // Open the root directory
    fileSystem->OpenVolume(fileSystem, &rootDir);

    // Open the kernel file
    rootDir->Open(rootDir, kernel, L"kernel.bin", EFI_FILE_MODE_READ, 0);

    return EFI_SUCCESS;
}
