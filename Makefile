all:
	make -C src/arch/x86_64
	grub-mkrescue iso -o os.iso
