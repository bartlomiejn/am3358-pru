#!/bin/bash

config-pin P8_11 pruout
config-pin P8_15 pruin
config-pin P8_12 pruout
config-pin P8_16 pruin
echo am335x-pru0-fw > sys/class/remoteproc/remoteproc1/firmware
echo start > sys/class/remoteproc/remoteproc1/state
