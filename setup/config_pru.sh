#!/bin/bash

### BEGIN INIT INFO
# Provides:          config_pru
# Required-Start:    $all
# Required-Stop:     $all
# Default-Start:     2 3 4 5
# Default-Stop:      0 1 6
# Short-Description: Configure pins and set firmware for the PRU
# Description:       Sets P8_11, P8_15, P8_12, P8_16 and runs PRU with firmware
### END INIT INFO

config-pin P8_11 pruout
config-pin P8_15 pruin
config-pin P8_12 pruout
config-pin P8_16 pruin
echo am335x-pru0-fw > sys/class/remoteproc/remoteproc1/firmware
echo start > sys/class/remoteproc/remoteproc1/state
