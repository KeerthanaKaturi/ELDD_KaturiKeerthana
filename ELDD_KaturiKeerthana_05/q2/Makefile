obj-m += driver2.o
KERN_DIR := /lib/modules/$(shell uname -r)/build/

module:
	make -C $(KERN_DIR) M= $(shell pwd) modules
	gcc -c ioctl_app ioctl_extfile.c
clean:
	make -C $(KERN_DIR) M= $(shell pwd) clean
