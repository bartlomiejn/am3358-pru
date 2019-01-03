DTC=dtc
INPUT_TYPE=dts
OUTPUT_TYPE=dtb
INPUT_FILE=dtso/am335x-boneblack-pru-stopwatch-overlay.dts
OUTPUT_FILE=output/am335x-boneblack-pru-stopwatch-overlay.dtb

dtbs: $(DTS_FILE)
	mkdir -p output
	$(DTC) -I $(INPUT_TYPE) -O $(OUTPUT_TYPE) -o $(OUTPUT_FILE) $(INPUT_FILE)
