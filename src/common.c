#include <common.h>

EFI_HANDLE IH;
EFI_SYSTEM_TABLE *ST;

void __polar_boot_common(EFI_HANDLE IHs, EFI_SYSTEM_TABLE *STs) {
    IH = IHs;
    ST = STs;

    ST->ConOut->ClearScreen(ST->ConOut);
    ST->ConOut->OutputString(ST->ConOut, L"Loaded common boot function\r\n");
}
