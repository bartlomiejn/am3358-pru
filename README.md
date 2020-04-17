# am3358-pru

## Overview
The project consists of three elements:
- A kernel module driver with switch information at `/sys/devices/prusw`
- PRU subsystem Cortex-M firmware which measures the time for two switches:
    - First: Time between last state change in ms, -1 if it didn't happen
    - Second: Time for last time when the switch was on, -1 if it didn't happen
- Firmware unit test suite which you can run using `make tests`

Required:
- `BeagleBone Black rev C`
- `Debian 9.5 2018-10-07 4GB SD IOT` (download link in installation description)
- An SD card of at least 4GB size
- Two switches

## HOWTO
1) Flash [Debian 9.5 2018-10-07 4GB SD IOT](https://debian.beagleboard.org/images/bone-debian-9.5-iot-armhf-2018-10-07-4gb.img.xz) to the SD card
3) Power up the BBB while holding the boot button
5) Connect using an FTDI to TTL cable
7) Clone the repo and `sudo make install`
8) After reboot, connect first switch to P8_11 and P8_15 and second switch to P8_12 and P8_16
9) `sudo make run`
