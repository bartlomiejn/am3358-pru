PHONY := pre_reboot
C_SOURCES = src/pru0.c

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

PHONY += build_sources
build_sources:
	clpru -fr=output --c99 $(C_SOURCES)

.PHONY: $(PHONY)
