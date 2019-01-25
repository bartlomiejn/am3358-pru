K_DIR := /lib/modules/$(shell uname -r)/build
PWD := $(shell pwd)
obj-m += prusw.o

PHONY = all
all:
	@mkdir -p $(K_DIR)
	make -C $(K_DIR) M=$(PWD) modules

PHONY += install
install:
	$(MAKE) -C $(K_DIR) SUBDIRS=$(PWD) modules_install
	@cp -f prusw.ko /lib/modules/$(shell uname -r)/
	@cp -f prusw.conf /etc/modules-load.d/
	@echo prusw > /etc/modules-load.d/modules.conf

PHONY += clean
clean:
	make -C $(K_DIR) M=$(PWD) clean

.PHONY: $(PHONY)
