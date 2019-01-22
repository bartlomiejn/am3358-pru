K_DIR := /lib/modules/$(shell uname -r)/build
PWD := $(shell pwd)
obj-m += prusw.o

PHONY = all
all:
	mkdir -p $(K_DIR)
	make -C $(K_DIR) M=$(PWD) modules

PHONY += install
install:
	# TODO: Add root check
	$(MAKE) -C $(K_DIR) SUBDIRS=$(PWD) modules_install
	cp rules/99-prusw.rules /etc/udev/rules.d
	rmmod $(DRIVER_KO)
	insmod $(DRIVER_KO)

PHONY += clean
clean:
	make -C $(K_DIR) M=$(PWD) clean

.PHONY: $(PHONY)
