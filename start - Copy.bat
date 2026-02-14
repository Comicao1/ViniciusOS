@echo off
setlocal enabledelayedexpansion

set OUT=output
set OBJ=%OUT%\obj
set BIN=%OUT%\bin

echo Cleaning...
rmdir /s /q "%OUT%" 2>nul
mkdir "%OBJ%"
mkdir "%BIN%"

echo Assembling bootloader...
nasm -f bin source/boot.asm -o "%BIN%\boot.bin"

echo Assembling 2nd stage...
nasm -f bin source/main.asm -o "%BIN%\main.bin"

echo Assembling isr_wrappers...
nasm -f elf32 source/headers/idt/isr_wrappers.asm -o "%OBJ%\isr_wrappers.o"

echo Compiling kernel C files...
i686-elf-gcc -m32 -ffreestanding -c source/kernel.c -o "%OBJ%\kernel.o"
i686-elf-gcc -m32 -ffreestanding -c source/headers/idt/idt.c -o "%OBJ%\idt.o"
i686-elf-gcc -m32 -ffreestanding -c source/headers/gdt/gdt.c -o "%OBJ%\gdt.o"
i686-elf-gcc -m32 -ffreestanding -c source/headers/gdt/gdt.c -o "%OBJ%\memory.o"
i686-elf-gcc -m32 -ffreestanding -c source/headers/text/fonts.c -o "%OBJ%\fonts.o"
i686-elf-gcc -m32 -ffreestanding -c source/headers/console/console.c -o "%OBJ%\console.o"
i686-elf-gcc -m32 -ffreestanding -c source/headers/memory/memory.c -o "%OBJ%\memory.o"

echo Linking kernel ELF...
i686-elf-ld -T source/link.ld -m elf_i386 ^
    "%OBJ%\kernel.o" ^
    "%OBJ%\idt.o" ^
    "%OBJ%\gdt.o" ^
    "%OBJ%\isr_wrappers.o" ^
    "%OBJ%\fonts.o" ^
    "%OBJ%\console.o" ^
    "%OBJ%\memory.o" ^
    -o "%BIN%\kernel.elf"

echo Converting kernel to binary...
i686-elf-objcopy -O binary "%BIN%\kernel.elf" "%BIN%\kernel.bin"

echo Padding kernel...
python pad.py "%BIN%\kernel.bin"

echo Padding model...
python pad_joselia.py models\cubo.jos 10240

echo Creating floppy image...
copy /b "%BIN%\boot.bin" + "%BIN%\main.bin" + "%BIN%\kernel.bin" + models\cubo.jos "%OUT%\floppy.img" >nul

echo Running QEMU...
qemu-system-i386 -accel tcg,thread=single -cpu max -m 256 ^
    -drive format=raw,file="%OUT%\floppy.img",index=0,if=floppy ^
    -d int,cpu_reset,guest_errors ^
    -D log.txt ^
    -no-reboot ^
    -no-shutdown

pause
