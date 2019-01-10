BUILD_DIR = /lib/modules/$(shell uname -r)/build

obj-m+ = prumodule.o

all:
	mkdir -p $(BUILD_DIR)
	make -C $(BUILD_DIR) M=$(PWD) modules

clean:
	make -C $(BUILD_DIR) M=$(PWD) clean
