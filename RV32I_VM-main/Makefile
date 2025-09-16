#export PATH=/opt/riscv/bin:$PATH

clean:
	cd firmware && make clean
run:
	clear
	gcc -o riscv_em riscv_em.c 
	cd firmware && make clean && make kernel #./build_fw.sh
	./riscv_em firmware/kernel.bin #hello_world_fw.bin


hello:
	clear 
	gcc -o riscv_em riscv_em2.c 
	cd app/hello && make clean && make hello
	./riscv_em app/hello/hello.bin #hello_world_fw.bin
