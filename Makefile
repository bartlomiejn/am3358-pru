DTC=dtc
INPUT_TYPE=dts
OUTPUT_TYPE=dtb
INPUT_FILE=dtbs/AM335X-PRU-STOPWATCH.dtso
OUTPUT_FILE=output/AM335X-PRU-STOPWATCH.dtbo

.PHONY: dtbs clean

dtbs: $(DTS_FILE)
	mkdir -p output
	$(DTC) -@ -I $(INPUT_TYPE) -O $(OUTPUT_TYPE) -o $(OUTPUT_FILE) $(INPUT_FILE)

clean:
	rm * output
