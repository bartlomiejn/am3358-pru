PRU_CGT_ROOT = /usr/share/ti/cgt-pru/
PRU_RPMSG_ROOT = /usr/lib/ti/pru-software-support-package/
CFLAGS = -v3 --endian=little --hardware_mac=on
INCLUDES = \
	--include_path=$(PRU_CGT_ROOT)include \
	--include_path=$(PRU_RPMSG_ROOT)include \
	--include_path=$(PRU_RPMSG_ROOT)include/am335x
PRU0_SRC = pru0.c
PRU0_OBJECT = output/pru0.object
LINKER_CMD_SRC = AM335x_PRU.cmd
LIBS = -l$(PRU_RPMSG_ROOT)lib/rpmsg_lib.lib

PHONY := pru0
pru0: $(PRU0_SRC) $(LINKER_CMD_SRC)
	mkdir -p output
	clpru $(CFLAGS) $(INCLUDES) -fe $(PRU0_OBJECT) $(PRU0_SRC)
	clpru -z $(LINKER_CMD_SRC) -i $(CGT_PRU_ROOT)lib -o output/am335x-pru0-fw \
		$(PRU0_OBJECT) $(LIBS)

PHONY += install
install:
	# TODO: Add root check
	cp output/am335x-pru0-fw /lib/firmware
	rmmod -f pru_rproc
	modprobe pru_rproc

PHONY += clean
clean:
	rm *.pp
	rm output/*

.PHONY: $(PHONY)
