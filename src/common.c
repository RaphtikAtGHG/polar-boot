#include <common.h>
#include <libc/string.h>

EFI_HANDLE IH;
EFI_SYSTEM_TABLE *ST;

void output_string(const char *ascii_str) {
    if (ascii_str == NULL) {
        return; // Handle null input
    }

    // Calculate the length of the ASCII string
    size_t len = strlen(ascii_str);

    CHAR16* char16_str;
    // Allocate memory for the CHAR16 string (+1 for the null terminator)
    ST->BootServices->AllocatePool(EfiLoaderData, (len + 1) * sizeof(CHAR16), (void**)&char16_str);
    if (char16_str == NULL) {
        return; // Handle memory allocation failure
    }

    // Convert each ASCII character to CHAR16
    for (size_t i = 0; i < len; i++) {
        char16_str[i] = (CHAR16)ascii_str[i];
    }

    // Add the null terminator
    char16_str[len] = (CHAR16)'\0';

    ST->ConOut->OutputString(ST->ConOut, char16_str);

}

void __polar_boot_common(EFI_HANDLE IHs, EFI_SYSTEM_TABLE *STs) {
    IH = IHs;
    ST = STs;

    output_string("Loaded common boot function\r\n");

    SimpleFile file = sfs_open(L"\\boot\\test.txt");
    if (EFI_ERROR(file.status)) {
        output_string("Failed to open file\r\n");
        return;
    }

    char *buffer = malloc(file.info.PhysicalSize + 1);
    if (buffer == NULL) {
        output_string("Failed to allocate memory for file buffer\r\n");
        return;
    }

    sfs_read(&file, &*buffer);
    if (EFI_ERROR(file.status)) {
        free(buffer);
        output_string("Failed to read file\r\n");
        return;
    }

    buffer[file.info.FileSize] = '\0';

    output_string("\r\n");
    output_string("File contents of ");
    ST->ConOut->OutputString(ST->ConOut, file.info.FileName);
    output_string(":\r\n");
    output_string(buffer);
    output_string("\r\n");

    sfs_close(&file);
    free(buffer);

    output_string("Finished common boot function\r\n");
    for (;;) {asm("hlt");}
}
