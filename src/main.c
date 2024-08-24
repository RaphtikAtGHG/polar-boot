#include <efi/efi.h>

EFI_STATUS __polar_boot_main(EFI_HANDLE ImageHandle, EFI_SYSTEM_TABLE *SystemTable) {
    SystemTable->ConOut->OutputString(SystemTable->ConOut, L"Hello, world!\r\n");
    for (;;) {asm volatile("hlt");}
}