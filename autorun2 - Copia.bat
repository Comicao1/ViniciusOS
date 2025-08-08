@echo off
del /f /q "C:\Users\suliv\Desktop\ViniciusOS\output\*" & for /d %%i in ("C:\Users\suliv\Desktop\ViniciusOS\output\*") do rd /s /q "%%i"
nasm -f bin source/boot.asm -o output/boot.o
nasm -f bin source/main.asm -o output/main.o
i686-elf-gcc -m32 -ffreestanding -c source/kernel.c -o output/kernel.o
i686-elf-ld -T source/link.ld -m elf_i386 output/kernel.o -o output/kernel.elf
i686-elf-objcopy -O binary output/kernel.elf output/kernel.bin
python pad.py output\kernel.bin
python pad_joselia.py models\cubo.jos 10240
copy /b output\boot.o + output\main.o + output\kernel.bin + models\cubo.jos output\floppy.img
qemu-system-x86_64 -accel tcg,thread=multi -cpu max -m 256 -drive format=raw,file=output/floppy.img
