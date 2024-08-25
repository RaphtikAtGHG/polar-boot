#include "malloc.h"

void *malloc(size_t size)
{
    void *ptr;
    if (EFI_ERROR(ST->BootServices->AllocatePool(EfiLoaderData, size, &ptr)))
    {
        ST->ConOut->OutputString(ST->ConOut, L"ERROR: Failed to allocate memory\r\n");
        return NULL;
    }

    return ptr;
}

void free(void *ptr)
{
    ST->BootServices->FreePool(ptr);
}

void CopyMem(void *dest, const void *src, size_t size)
{
    // Copy memory from src to dest
    char *d = dest;
    const char *s = src;
    while (size--)
    {
        *d++ = *s++;
    }
}

void *realloc(void *ptr, size_t new_size)
{
    if (!ptr)
        return malloc(new_size);

    void *new_ptr = malloc(new_size);
    if (!new_ptr)
        return NULL;

    CopyMem(new_ptr, ptr, new_size);
    free(ptr);
    return new_ptr;
}