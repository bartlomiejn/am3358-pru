UENVS = setup/boot/uEnv.txt
BOOT_DIR = /boot
PHONY := pre_reboot
pre_reboot:
	ifneq ($(EUID),0)
		@echo "Please run as root user"
		@exit 1
	endif
	cp $(UENVS) $(BOOT_DIR)

POST_REBOOT = setup/post_reboot.sh
PHONY += post_reboot
post_reboot:
	sh $(POST_REBOOT)
	sudo apt-get update
	sudo apt-get install linux-headers-$(shell uname -r)

PRU0_SRC = firmware/pru0.c
PRU0_OBJECT = output/pru0.object
LINKER_CMD_SRC = firmware/AM335x_PRU.cmd
INCLUDES = \
	--include_path=include \
	--include_path=include/am335x \
	--include_path=/usr/share/ti/cgt-pru/include
LIBS = --library=lib/rpmsg_lib.lib --library=libc.a
CFLAGS = -v3 --endian=little --hardware_mac=on --run_linker --ram_model
PHONY += build_firmware
build_firmware: $(PRU0_SRC) $(LINKER_CMD_SRC)
	mkdir -p output
	clpru $(CFLAGS) $(INCLUDES) -fe $(PRU0_OBJECT) $(PRU0_SRC)
	clpru -z $(LINKER_CMD_SRC)  $(FLAGS) $(LIBS) -o output/am335x-pru0-fw $(PRU0_OBJECT)

DRIVER_KO = pru_stopwatch.ko
PHONY += setup_driver
setup_driver:
	cd module && $(MAKE)
	sudo insmod $(DRIVER_KO)

PHONY += remove_driver
remove_driver:
	sudo rmmod $(DRIVER_KO)
	cd module && $(MAKE) clean

PHONY += clean
clean:
	rm -rf output/*

.PHONY: $(PHONY)
