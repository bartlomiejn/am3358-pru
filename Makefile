DTC=dtc
INPUT_TYPE=dts
OUTPUT_TYPE=dtb
INPUT_FILE=dtbs/am335x-boneblack.dts
OUTPUT_FILE=output/am335x-boneblack.dtb

.PHONY: dtbs clean

dtbs: $(DTS_FILE)
	mkdir -p output
	$(DTC) -@ -I $(INPUT_TYPE) -O $(OUTPUT_TYPE) -o $(OUTPUT_FILE) $(INPUT_FILE)

clean:
	rm * output
