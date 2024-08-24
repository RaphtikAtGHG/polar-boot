#include <efi/efi.h>
#include <common.h>

EFI_STATUS __polar_boot_main(EFI_HANDLE ImageHandle, EFI_SYSTEM_TABLE *SystemTable) {
    SystemTable->ConOut->OutputString(SystemTable->ConOut, L"Hello, world!\r\n");
    __polar_boot_common(ImageHandle, SystemTable);
    SystemTable->ConOut->OutputString(SystemTable->ConOut, L"Couldnt finish shared entry function! Please reboot manually\r\n");
    for (;;) {asm ("hlt");}
}