K_DIR := /lib/modules/$(shell uname -r)/build
PWD := $(shell pwd)
obj-m += pru_stopwatch.o

all:
	mkdir -p $(K_DIR)
	make -C $(K_DIR) M=$(PWD) modules

install:
	$(MAKE) -C $(K_DIR) SUBDIRS=$(PWD) modules_install

clean:
	make -C $(K_DIR) M=$(PWD) clean

