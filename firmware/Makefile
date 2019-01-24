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
	$(HARDWARE_DIR)switch1.c $(HARDWARE_DIR)switch2.c \
	$(SOFTWARE_DIR)cycle_timer.c $(SOURCE_ROOT)utils.c \
	$(GPI_DIR)debounced_gpi.c
PRU0_SRC = \
	$(SOFTWARE_DIR)rpmsg.c \
	$(CYCLE_COUNTER_DIR)pru_cycle_counter.c \
	$(GPI_DIR)gpi_p8_15.c $(GPI_DIR)gpi_p8_16.c \
	$(GPO_DIR)gpo_p8_11.c $(GPO_DIR)gpo_p8_12.c \
	$(SOURCE_ROOT)pru0.c
CFLAGS = -v3 --endian=little -fs=$(OUTPUT_ROOT) -fr=$(OUTPUT_ROOT)
INCLUDES = \
	--include_path=$(SOURCE_ROOT) \
	--include_path=$(HARDWARE_DIR) \
	--include_path=$(SOFTWARE_DIR) \
	--include_path=$(PRU_CGT_ROOT)include \
	--include_path=$(PRU_RPMSG_ROOT)include \
	--include_path=$(PRU_RPMSG_ROOT)include/am335x \
	--include_path=$(ARM_LINUX_ROOT)
LINKER_CMD_SRC = $(SOURCE_ROOT)AM335x_PRU.cmd
OBJ_FILES = $(shell find $(OUTPUT_ROOT) -name '*.obj')
LIBS = -l$(PRU_RPMSG_ROOT)lib/rpmsg_lib.lib -l$(PRU_CGT_ROOT)lib/libc.a
PRU0_FW = am335x-pru0-fw
PRU0_FW_OUTPUT = $(OUTPUT_ROOT)$(PRU0_FW)
PHONY := pru0
pru0: $(PRU0_SRC) $(SHARED_SRC) $(LINKER_CMD_SRC)
	mkdir -p output
	clpru $(CFLAGS) $(INCLUDES) $(PRU0_SRC) $(SHARED_SRC)
	clpru -z $(LINKER_CMD_SRC) -i $(CGT_PRU_ROOT)lib -o $(PRU0_FW_OUTPUT) \
		$(OBJ_FILES) $(LIBS)

TESTS_ROOT = tests/
DOUBLES_DIR = $(TESTS_ROOT)doubles/
TESTS_SRC = \
	$(DOUBLES_DIR)mock_cycle_counter.c \
	$(DOUBLES_DIR)mock_gpi.c $(DOUBLES_DIR)mock_gpo.c \
	$(DOUBLES_DIR)mock_cycle_timer.c $(DOUBLES_DIR)mock_debounced_gpi.c \
	$(TESTS_ROOT)switch1_tests.c $(TESTS_ROOT)switch2_tests.c \
	$(TESTS_ROOT)cycle_timer_tests.c $(TESTS_ROOT)debounced_gpi_tests.c \
	$(TESTS_ROOT)main.c
TESTS_INCLUDE =  \
	-I $(ARM_LINUX_ROOT) \
	-I $(SOURCE_ROOT) \
	-I $(HARDWARE_DIR) \
	-I $(SOFTWARE_DIR)
PHONY += tests
tests: $(SHARED_SRC) $(TESTS_SRC)
	mkdir -p output
	gcc $(TESTS_INCLUDE) $(SHARED_SRC) $(TESTS_SRC) -o \
		$(OUTPUT_ROOT)unit_tests
	./$(OUTPUT_ROOT)unit_tests

PRU0 = remoteproc1
PRU0_DEV_STATE = /sys/class/remoteproc/$(PRU0)/state
PRU0_DEV_FIRMWARE = /sys/class/remoteproc/$(PRU0)/firmware
FIRMWARE_ROOT = /lib/firmware/
PHONY += install
install:
	# TODO: Add root check
	@rm -f $(FIRMWARE_ROOT)$(PRU0_FW)
	@cp $(PRU0_FW_OUTPUT) $(FIRMWARE_ROOT)$(PRU0_FW)
	@config-pin P8_11 pruout
	@config-pin P8_15 pruin
	@config-pin P8_12 pruout
	@config-pin P8_16 pruin
	@if [ $(shell cat $(PRU0_DEV_STATE)) = running ]; then \
		echo stop > $(PRU0_DEV_STATE); \
	fi
	@echo am335x-pru0-fw > $(PRU0_DEV_FIRMWARE)
	@echo start > $(PRU0_DEV_STATE)

PHONY += clean
clean:
	rm output/*

.PHONY: $(PHONY)
