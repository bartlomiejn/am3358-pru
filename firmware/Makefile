PRU_CGT_ROOT = /usr/share/ti/cgt-pru/
PRU_RPMSG_ROOT = /usr/lib/ti/pru-software-support-package/
ARM_LINUX_ROOT = /usr/include/arm-linux-gnueabihf/
OUTPUT_ROOT = output/
CFLAGS = -v3 --endian=little --symdebug:none -Os -O4 -pm
INCLUDES = \
	--include_path=$(PRU_CGT_ROOT)include \
	--include_path=$(PRU_RPMSG_ROOT)include \
	--include_path=$(PRU_RPMSG_ROOT)include/am335x \
	--include_path=$(ARM_LINUX_ROOT)
PRU0_SRC = pru0.c
PRU0_OBJECT = $(OUTPUT_ROOT)pru0.object
LFLAGS = --map_file=output/pru0.map
LINKER_CMD_SRC = AM335x_PRU.cmd
LIBS = -l$(PRU_RPMSG_ROOT)lib/rpmsg_lib.lib -l$(PRU_CGT_ROOT)lib/libc.a
PRU0_FIRMWARE = am335x-pru0-fw

PHONY := pru0
pru0: $(PRU0_SRC) $(LINKER_CMD_SRC)
	mkdir -p output
	clpru $(CFLAGS) $(INCLUDES) -fe $(PRU0_OBJECT) $(PRU0_SRC)
	clpru -z $(LFLAGS) $(LINKER_CMD_SRC) -i $(CGT_PRU_ROOT)lib -o \
		$(OUTPUT_ROOT)$(PRU0_FIRMWARE) $(PRU0_OBJECT) $(LIBS)

FIRMWARE_DIR = /lib/firmware/
PHONY += install
install:
	# TODO: Add root check
	rm $(FIRMWARE_DIR)$(PRU0_FIRMWARE)
	cp $(OUTPUT_ROOT)$(PRU0_FIRMWARE) $(FIRMWARE_DIR)
	echo 'stop' > /sys/class/remoteproc/remoteproc1/state
	echo 'am335x-pru0-fw' > /sys/class/remoteproc/remoteproc1/firmware
	echo 'start' > /sys/class/remoteproc/remoteproc1/state

PHONY += clean
clean:
	rm output/*

.PHONY: $(PHONY)
