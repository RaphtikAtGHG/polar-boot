MAKEFLAGS += --no-print-directory

CXX := clang
LD := ld.lld
CXXFLAGS := -target x86_64-windows-unknown -ffreestanding -fshort-wchar \
           -Wno-unused-command-line-argument -Wno-void-pointer-to-int-cast \
           -Wno-int-to-void-pointer-cast -Wno-int-to-pointer-cast -g -Ilib 

LDFLAGS := -target x86_64-windows-unknown -nostdlib -fuse-ld=lld \
           -Wl,/subsystem:efi_application -Wl,/entry:__polar_boot_main -g


OBJ_DIR := build
BIN_DIR := bin
TARGET := $(BIN_DIR)/violin.efi
IMAGE_NAME := boot.img

.PHONY: all build $(BIN_DIR)/$(IMAGE_NAME) run clean clean-disk

SRC_FILES := $(shell find src -name '*.c')
OBJ_FILES := $(SRC_FILES:src/%.c=$(OBJ_DIR)/%.o)

all: build $(BIN_DIR)/$(IMAGE_NAME)

build: $(TARGET)

$(TARGET): $(OBJ_FILES)
	$(CXX) $(LDFLAGS) -o $@ $(OBJ_FILES)

$(OBJ_DIR)/%.o: src/%.c
	@mkdir -p $(dir $@)
	@$(CXX) $(CXXFLAGS) -c $< -o $@

$(BIN_DIR)/$(IMAGE_NAME): $(TARGET)
	@dd if=/dev/zero of=$(BIN_DIR)/$(IMAGE_NAME) bs=1M count=64
	@mkfs.fat -F 32 -n EFI_SYSTEM $(BIN_DIR)/$(IMAGE_NAME)
	@mkdir -p mnt
	@sudo mount -o loop $(BIN_DIR)/$(IMAGE_NAME) mnt
	@sudo mkdir -p mnt/EFI/BOOT
	@sudo cp $(TARGET) mnt/EFI/BOOT/BOOTX64.EFI
	@sudo umount mnt
	@rm -rf mnt

run: $(BIN_DIR)/$(IMAGE_NAME)
	@qemu-system-x86_64 -drive file=$(BIN_DIR)/$(IMAGE_NAME),format=raw -m 2G \
						-bios /usr/share/OVMF/x64/OVMF.fd -boot order=c

clean:
	
	@rm -rf $(OBJ_DIR) $(BIN_DIR) $(TARGET) $(IMAGE_NAME)
	
clean-disk:
	@sudo umount mnt
	@rm -rf mnt
