DTC=dtc
INPUT_TYPE=dts
OUTPUT_TYPE=dtb
INPUT_FILE=dtso/am335x-beagleblack-pins-overlay.dts
OUTPUT_FILE=output/am335x-beagleblack-pins-overlay.dtb

dtbs: $(DTS_FILE)
	$(DTC) -I $(INPUT_TYPE) -O $(OUTPUT_TYPE) -o $(OUTPUT_FILE) $(INPUT_FILE)
