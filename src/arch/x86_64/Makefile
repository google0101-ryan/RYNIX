all:
	find . -name \*.c -exec gcc -c {} -fno-pie -fno-pic -m64 -I./ -I../../libc \;
	ld -T linker.ld -melf_x86_64 -o kernel.elf *.o
	cp kernel.elf ../../../iso/boot