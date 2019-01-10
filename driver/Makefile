K_DIR := /lib/modules/$(shell uname -r)/build
PWD := $(shell pwd)
obj-m += prusw.o

all:
	mkdir -p $(K_DIR)
	make -C $(K_DIR) M=$(PWD) modules
	sh ./reload_module.sh

install:
	# TODO: Root check
	$(MAKE) -C $(K_DIR) SUBDIRS=$(PWD) modules_install
	cp rules/99-prusw.rules /etc/udev/rules.d

clean:
	make -C $(K_DIR) M=$(PWD) clean

