FIRMWARE_SRC = firmware/pru0.c

PHONY := pre_reboot
pre_reboot:
	ifneq ($(EUID),0)
		@echo "Please run as root user"
		@exit 1
	endif
	cp setup/boot/uEnv.txt /boot/

PHONY += post_reboot
post_reboot:
	sh setup/post_reboot.sh
	$(MAKE) build_sources
	$(MAKE) load_firmware

PHONY += build_firmware
build_firmware:
	clpru -fr=output --c99 $(FIRMWARE_SRC)

PHONY += load_firmware
load_firmware:
	# TODO: Move obj to firmware, load firmware

.PHONY: $(PHONY)
