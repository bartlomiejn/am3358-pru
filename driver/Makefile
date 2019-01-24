K_DIR := /lib/modules/$(shell uname -r)/build
PWD := $(shell pwd)
obj-m += prusw.o

PHONY = all
all:
	mkdir -p $(K_DIR)
	make -C $(K_DIR) M=$(PWD) modules

PHONY += install
install:
	cp prusw.conf /etc/modules-load.d/
	$(MAKE) -C $(K_DIR) SUBDIRS=$(PWD) modules_install
	cp -f prusw.ko /lib/modules/$(shell uname -r)/

PHONY += clean
clean:
	make -C $(K_DIR) M=$(PWD) clean

.PHONY: $(PHONY)
