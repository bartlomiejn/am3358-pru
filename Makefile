EUID := $(shell id -u -r)

PHONY := is_root
is_root:
ifneq ($(EUID),0)
	@echo "This recipe requires root access."
	@exit 1
endif

PHONY += firmware
firmware: is_root
	cd firmware && $(MAKE) pru0
	cd firmware && $(MAKE) install

PHONY += driver
driver: is_root
	cd driver && $(MAKE)
	cd driver && $(MAKE) install

PHONY += tests
tests:
	cd firmware && $(MAKE) tests

PHONY += install
install: is_root
	@cp setup/boot/uEnv.txt /boot/
	@cp setup/config_pru.sh /etc/init.d/
	@chmod +x /etc/init.d/config_pru.sh
	@apt-get update
	@apt-get install linux-headers-$(shell uname -r)
	$(MAKE) driver
	$(MAKE) firmware
	reboot

.PHONY: $(PHONY)
