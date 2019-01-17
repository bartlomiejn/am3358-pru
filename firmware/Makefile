PRU_CGT_ROOT = /usr/share/ti/cgt-pru/
PRU_RPMSG_ROOT = /usr/lib/ti/pru-software-support-package/
ARM_LINUX_ROOT = /usr/include/arm-linux-gnueabihf/
OUTPUT_ROOT = output/
CFLAGS = -v3 --endian=little
INCLUDES = \
	--include_path=$(PRU_CGT_ROOT)include \
	--include_path=$(PRU_RPMSG_ROOT)include \
	--include_path=$(PRU_RPMSG_ROOT)include/am335x \
	--include_path=$(ARM_LINUX_ROOT)
PRU0_SRC = pru0.c rpmsg.c switch1.c switch2.c debouncer.c
LINKER_CMD_SRC = AM335x_PRU.cmd
LIBS = -l$(PRU_RPMSG_ROOT)lib/rpmsg_lib.lib -l$(PRU_CGT_ROOT)lib/libc.a
PRU0_FIRMWARE = am335x-pru0-fw

PHONY := pru0
pru0: $(PRU0_SRC) $(LINKER_CMD_SRC)
	mkdir -p output
	clpru $(CFLAGS) $(INCLUDES) $(PRU0_SRC)
	clpru -z $(LINKER_CMD_SRC) -i $(CGT_PRU_ROOT)lib -o \
		$(OUTPUT_ROOT)$(PRU0_FIRMWARE) $(patsubst %.c,%.obj,$(PRU0_SRC)) $(LIBS)

PRU0 = remoteproc1
PRU0_DEV_STATE = /sys/class/remoteproc/$(PRU0)/state
PRU0_DEV_FIRMWARE = /sys/class/remoteproc/$(PRU0)/firmware
FIRMWARE_ROOT = /lib/firmware/
PHONY += install
install:
	# TODO: Add root check
	rm -f $(FIRMWARE_ROOT)$(PRU0_FIRMWARE)
	cp $(OUTPUT_ROOT)$(PRU0_FIRMWARE) $(FIRMWARE_ROOT)$(PRU0_FIRMWARE)
	echo stop > $(PRU0_DEV_STATE)
	echo am335x-pru0-fw > $(PRU0_DEV_FIRMWARE)
	echo start > $(PRU0_DEV_STATE)

PHONY += clean
clean:
	rm *.obj
	rm output/*

.PHONY: $(PHONY)
