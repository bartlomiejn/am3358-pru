PHONY := pre_reboot
pre_reboot:
	cp setup/boot/uEnv.txt /boot/
	apt-get update
	apt-get install linux-headers-$(shell uname -r)
	$(MAKE) driver

PHONY += post_reboot
post_reboot:
	config-pin P8_11 pruout
	config-pin P8_15 pruin
	config-pin P8_12 pruout
	config-pin P8_16 pruin
	$(MAKE) firmware

PHONY += firmware
firmware:
	cd firmware
	$(MAKE) pru0
	$(MAKE) install

PHONY += driver
driver:
	cd driver
	$(MAKE)
	$(MAKE) install

.PHONY: $(PHONY)
