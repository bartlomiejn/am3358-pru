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

FIRMWARE_SRC = firmware/pru0.c
LINKER_CMD_SRC = firmware/AM335x_PRU.cmd
INCLUDE_PATHS = \
	--include_path=include \
	--include_path=include/am335x \
	--include_path=/usr/share/ti/cgt-pru/lib \
	--include_path=/usr/share/ti/cgt-pru/include
RPMSG_LIB = --library=lib/rpmsg_lib.lib
PHONY += setup_firmware
setup_firmware: $(FIRMWARE_SRC)
	mkdir -p output
	clpru \
		-fr=output $(FIRMWARE_SRC) $(LINKER_CMD_SRC) $(INCLUDE_PATHS) \
		--run_linker $(RPMSG_LIB)

DRIVER_KO = pru_stopwatch.ko
PHONY += setup_driver
setup_driver:
	cd module && $(MAKE)
	sudo insmod $(DRIVER_KO)

PHONY += remove_driver
remove_driver:
	sudo rmmod $(DRIVER_KO)
	cd module && $(MAKE) clean

.PHONY: $(PHONY)
