# PolarBoot Protocol
The polar boot protocol is relativly simple but just enought to boot a functional operating system. 

## Executable 
For a valid PolarBoot kernel image the file **must** be an ELF executable and contain the PolarBoot Header in the `.pb_header` section of the ELF file. The ELF must be valid and be targeted for the boot architecture. The only support architecture will be `x86_64` for a long time. Below is a sketch of the header if the header is invalid the Operating System will not boot!

## Boot header

| Offset | Type      | Name                 |Value                 |
|--------|-----------|----------------------|----------------------|
| 0x00   | uint8 x 5 | Magic                | Must be 0x50424F4F54 |
| 0x05   | uint8     | Target Archictecture | See below            |
| 0x06   | uint64    | Start Adress         | See below            |
| 0x0E   | uint32    | CRC32 Signature      | See below            |

Here are the definitions of what the fields mean:
* **Magic:** The magic value of the PolarBoot protocol must be 0x50424F4F54 (PBOOT in ASCII).
* **Target Architecture:** The architecture of which the kernel is supposted to run on. **MUST BE MATCHING WITH THE ARCH IDENTIFIER IN THE ELF HEADER**. </br>
Supported Values:
    * x86_64: 1.

* **Start Adress:** The start adress of the kernel must loaded at the adress **0xFFFFFFFF80000000**.
* **CRC32 Signature** This field should contain a CRC32 signature of the kernel image.

## Protocol Struct

On successful boot the PolarBoot protocol will return a structure with fields such as the framebuffer, RAM Filesystem, And Memory Map entries.

### Framebuffer

The struct for the framebuffer will be like this:
| Offset | Type               | Name                | Value     |
|--------|--------------------|---------------------|-----------|
| 0x00   | uint64             | Framebuffer Address | See below |
| 0x08   | uint32             | Framebugger Width   | See below |
| 0x0C   | uint32             | Framebuffer Height  | See below |
| 0x10   | uint32             | Framebuffer Pitch   | See below |
| 0x14   | uint32             | Framebuffer BPP     | See below |

The following Values are excepted:
* **Framebufffer Address:** The start address of the framebuffer.
* **Framebuffer Width:** The width of the screen in pixels.
* **Framebuffer Height:** The height of the framebuffer in pixels.
* **Framebuffer Pitch:** The number of bytes per line of the framebuffer (includes padding).
* **Framebuffer BPP:** Bits per pixel, indicating the color depth of each pixel.
### RAM Filesystem

| Offset | Type   | Name                   | Value     |
|--------|--------|------------------------|-----------|
| 0x00   | uint64 | RAM Filesystem Address | See below |
| 0x08   | uint64 | RAM Filesystem Size    | See below |

Here are what the values mean:
* **RAM FS Address:** The start address of the RAM FS tarball.
* **RAM FS Size:** The size of the RAM FS tarball.

### Memory Map

#### Memory Map Entry

Here is a structure of a memory map entry:
| Offset | Type   | Name         | Value     |
|--------|--------|--------------|-----------|
| 0x00   | uint64 | Base Address | See below |
| 0x08   | uint64 | Length       | See below |
| 0x10   | uint32 | Type         | See Below |
| 0x14   | uint32 | Reserved     | 0         |

Here are what the values mean:
* **Base Adress:** The base address of the memmap entry
* **Length:** The length/size of the memmap entry
* **Type:** The type of the memmap entry.
* **Reserved:** Reserved for padding/future use </br>
  Types of memmaps:
    * **FREE:** Free for general purpose use: 0x01
    * **BOOTLOADER RECLAIMABLE:** Reclaimable by the kernel after the bootloader finished: 0x02
    * More will be added soon.

#### Memory Map Table

Here is a structure of the memory map:
| Offset | Type   | Name                     | Value     |
|--------|--------|--------------------------|-----------|
| 0x00   | uint64 | Memory Map Address       | See below |
| 0x08   | uint32 | Memory Map Entry Count   | See below |
| 0x0C   | uint32 | Reserved                 | See below |

Here are what the values mean:
* **Memory Map Address:** The start adress of the memmap entry structs
* **Memory Map Entry Count:** The count of memmap entry structs maximum 64 entries
* **Reserved:** Reserved for padding/future use

### Complete Struct

| Offset | Size       | Name            | Value     |
|--------|------------|-----------------|-----------|
| 0x00   | 24 bytes   | Framebuffer     | See below |
| 0x18   | 16 bytes   | RAM Filesystem  | See below |
| 0x2A   | 1 Kilobyte | Memorymap Table | See below |


Here are what the values mean:
* **Framebuffer:** The framebuffer struct from before
* **RAM Filesystem:** The RAM Filesystem struct from before
* **Memorymap Table:** The memorymap table with the 64 entries though no all needn't be used. 16 bytes per struct x 64 entries = 1024 bytes = 1 Kilobyte

-- END OF SPECIFIC STRUCTS --

Please be aware that the structure MUST be accessed and be available at **0xFFFFFFFF8000DEAD** after the struct has been utilized it can be overwritten

## Config Values

The Polar Boot kernel can be configured by using a configuration file, which the bootloader can control **BUT** the following values **MUST** be present:
* **KERNEL PATH:** The path of the kernel in the filesystem it must be in the EFI System Partition
* **GRAPHICS MODE:** Must be a valid UEFI GOP mode.
* **RAMFS PATH [NULLABLE]:** Can be empty. This will contain the path of the RAM Filesystem tarball.
* **KERNEL COMMAND LINE [NULLABLE]:** Can be empty. This will contain the command line of the kernel. The kernel can interpret it as it wants.

## Info

Polar Boot Protocol Specification Version 1.0.1
(c) 2024 Volartrix Team 
