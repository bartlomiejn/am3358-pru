EUID := $(shell id -u -r)

PHONY := is_root
is_root:
ifneq ($(EUID),0)
	@echo "This has to be run as root"
	@exit 1
endif

PHONY += pre_reboot
pre_reboot: is_root
	@cp setup/boot/uEnv.txt /boot/
	@apt-get update
	@apt-get install linux-headers-$(shell uname -r)
	$(MAKE) driver
	reboot

PHONY += post_reboot
post_reboot: is_root
	$(MAKE) firmware

PHONY += firmware
firmware: is_root
	cd firmware && $(MAKE) pru0
	cd firmware && $(MAKE) install

PHONY += driver
driver: is_root
	cd driver && $(MAKE)
	cd driver && $(MAKE) install

.PHONY: $(PHONY)
