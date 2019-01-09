PHONY := pre_reboot
pre_reboot:
	ifneq ($(EUID),0)
		@echo "Please run as root user"
		@exit 1
	endif
	cp setup/boot/uEnv.txt /boot/
	reboot

PHONY += post_reboot
post_reboot:
	sh setup/post_reboot.sh

.PHONY: $(PHONY)
