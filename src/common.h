#ifndef __COMMON_H__
#define __COMMON_H__

#include <efi/efi.h>

extern EFI_HANDLE IH;
extern EFI_SYSTEM_TABLE *ST;

void __polar_boot_common(EFI_HANDLE IHs, EFI_SYSTEM_TABLE *STs);

#endif // __COMMON_H__