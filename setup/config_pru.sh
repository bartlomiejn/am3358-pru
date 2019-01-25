#!/bin/bash

### BEGIN INIT INFO
# Provides:          config_pru
# Required-Start:    kmod
# Required-Stop:
# Default-Start:
# Default-Stop:
# Short-Description: Configure pins for the prusw module
# Description:       Sets P8_11, P8_12 to pruout and P8_15, P8_16 to pruin
### END INIT INFO

config-pin P8_11 pruout
config-pin P8_15 pruin
config-pin P8_12 pruout
config-pin P8_16 pruin
