#/bin/bash

set -e

riscv32-unknown-linux-gnu-gcc -march=rv32i -mabi=ilp32 -Wl,-Bstatic,-T,sections.lds,--strip-debug -ffreestanding -nostdlib -o hello_world_fw.elf start.S main.c
riscv32-unknown-linux-gnu-objcopy -O binary hello_world_fw.elf hello_world_fw.bin
riscv32-unknown-linux-gnu-objdump -d hello_world_fw.elf > hello_world_fw.list
# print text section
#riscv32-none-elf-objdump -d hx8kdemo_fw.elf | awk '{print "0x"$2","}'

# dump all
#riscv32-none-elf-objdump -d hx8kdemo_fw.elf 

