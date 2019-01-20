PRU_CGT_ROOT = /usr/share/ti/cgt-pru/
PRU_RPMSG_ROOT = /usr/lib/ti/pru-software-support-package/
ARM_LINUX_ROOT = /usr/include/arm-linux-gnueabihf/
OUTPUT_ROOT = output/
SOURCE_ROOT = source/
HARDWARE_DIR = $(SOURCE_ROOT)hardware/
SOFTWARE_DIR = $(SOURCE_ROOT)software/
CYCLE_COUNTER_DIR = $(HARDWARE_DIR)cycle_counter/
GPI_DIR = $(HARDWARE_DIR)gpi/
GPO_DIR = $(HARDWARE_DIR)gpo/
SHARED_SRC = \
	$(CYCLE_COUNTER_DIR)cycle_counter.h \
	$(GPI_DIR)gpi.h \
	$(GPO_DIR)gpo.h \
	$(HARDWARE_DIR)switch1.c $(HARDWARE_DIR)switch1.h \
	$(HARDWARE_DIR)switch2.c $(HARDWARE_DIR)switch2.h \
	$(SOFTWARE_DIR)debouncer.c $(SOFTWARE_DIR)debouncer.h \
	$(SOURCE_ROOT)utils.h
PRU0_SRC = \
	$(SOURCE_ROOT)pru0.c \
	$(SOFTWARE_DIR)rpmsg.c $(SOFTWARE_DIR)rpmsg.h \
	$(CYCLE_COUNTER_DIR)pru_cycle_counter.c \
	$(CYCLE_COUNTER_DIR)pru_cycle_counter.h \
	$(GPI_DIR)gpi_p8_15.c $(GPI_DIR)gpi_p8_15.h \
	$(GPI_DIR)gpi_p8_21.c $(GPI_DIR)gpi_p8_21.h \
	$(GPO_DIR)gpi_p8_11.c $(GPO_DIR)gpi_p8_11.h \
	$(GPO_DIR)gpi_p8_20.c $(GPO_DIR)gpi_p8_20.h
CFLAGS = -v3 --endian=little
INCLUDES = \
	--include_path=$(PRU_CGT_ROOT)include \
	--include_path=$(PRU_RPMSG_ROOT)include \
	--include_path=$(PRU_RPMSG_ROOT)include/am335x \
	--include_path=$(ARM_LINUX_ROOT)
LINKER_CMD_SRC = $(SOURCE_ROOT)AM335x_PRU.cmd
LIBS = -l$(PRU_RPMSG_ROOT)lib/rpmsg_lib.lib -l$(PRU_CGT_ROOT)lib/libc.a
PRU0_FIRMWARE = am335x-pru0-fw

PHONY := pru0
pru0: $(PRU0_SRC) $(SHARED_SRC) $(LINKER_CMD_SRC)
	mkdir -p output
	clpru $(CFLAGS) $(INCLUDES) $(PRU0_SRC)
	clpru -z $(LINKER_CMD_SRC) -i $(CGT_PRU_ROOT)lib -o \
		$(OUTPUT_ROOT)$(PRU0_FIRMWARE) $(patsubst %.c,%.obj,$(PRU0_SRC)) \
		$(patsubst %.c,%.obj,$(SHARED_SRC)) $(LIBS)

TESTS_ROOT = tests/
DOUBLES_DIR = $(TESTS_ROOT)
TESTS_SRC = \
	$(DOUBLES_DIR)mock_cycle_counter.c $(DOUBLES_DIR)mock_cycle_counter.h \
	$(DOUBLES_DIR)mock_gpi.c $(DOUBLES_DIR)mock_gpi.h \
	$(DOUBLES_DIR)mock_gpo.c $(DOUBLES_DIR)mock_gpo.h \
	$(TESTS_ROOT)switch1_tests.c $(TESTS_ROOT)switch1_tests.h \
	$(TESTS_ROOT)switch2_tests.c $(TESTS_ROOT)switch2_tests.h \
	$(TESTS_ROOT)unit_test.h
TESTS_INCLUDE = --include_path=$(ARM_LINUX_ROOT)
PHONY += tests
tests: $(SHARED_SRC) $(TESTS_SRC)
	mkdir -p output
	gcc $(CFLAGS) $(TESTS_INCLUDE) $(SHARED_SRC) $(TESTS_SRC) -o \
		$(OUTPUT_ROOT)unit_tests
	./$(OUTPUT_ROOT)unit_tests

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
