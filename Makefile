PHONY := pre_reboot
pre_reboot:
	# TODO: Add root check
	cp setup/boot/uEnv.txt /boot/

PHONY += post_reboot
post_reboot:
	config-pin P8_11 pruout 
	config-pin P8_15 pruin
	config-pin P8_20 pruout 
	config-pin P8_21 pruin
	sudo apt-get update
	sudo apt-get install linux-headers-$(shell uname -r)

PHONY += firmware
firmware:
	cd firmware && $(MAKE) pru0

PHONY += setup_driver
setup_driver:
	cd module && $(MAKE)

PHONY += clean
clean:
	rm -rf output/*

.PHONY: $(PHONY)
