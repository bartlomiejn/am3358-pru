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
	$(MAKE) build_sources
	$(MAKE) load_firmware

FIRMWARE_SRC = firmware/pru0.c
PHONY += build_firmware
build_firmware:
	clpru -fr=output --c99 $(FIRMWARE_SRC)

PHONY += load_firmware
load_firmware:
	# TODO: Move obj to firmware, load firmware

PHONY += build_module
build_module:
	cd module && $(MAKE)

.PHONY: $(PHONY)
