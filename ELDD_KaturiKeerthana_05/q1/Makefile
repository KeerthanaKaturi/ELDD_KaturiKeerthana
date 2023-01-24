obj-m += driver1.o
KERN_DIR =/lib/modules/$(shell uname -r)/build/

module:
	make -C $(KERN_DIR) M=$(shell pwd) modules
	gcc -o readfile read_file.c
	gcc -o writefile write_file.c
clean:
	make -C $(KERN_DIR) M=$(shell pwd) clean
