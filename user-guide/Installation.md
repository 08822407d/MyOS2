This Kernel acquires UEFI bootloader supporting [Multiboot2 Spec](https://www.gnu.org/software/grub/manual/multiboot2/multiboot.html), that means the bootloader should be placed at the path designated by [UEFI Spec](https://uefi.org/specifications) ('/EFI/BOOT/BOOTX64.EFI' to your boot partition, note that all the directory and the efi file names should be **CAPITAL/UPPERCASE**).

The other binaries - kernel and initd - should be placed in '/boot' to your boot partition, the 'sh' can be placed any where in the same partition (obviously you need change it's path in 'initd' source code).

---

**ChangeLog**

04-16 2024

    Create this doc
