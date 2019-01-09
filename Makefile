PHONY := pre_reboot
pre_reboot:
	@if ! [ "$(shell id -u)" = 0 ];then
    	@echo "You are not root, run as root please"
    	exit 1
	fi
	cp /setup/uboot/uEnv.txt /boot/
	reboot

PHONY += post_reboot
post_reboot:
	sh /setup/post_reboot.sh

.PHONY: $(PHONY)
