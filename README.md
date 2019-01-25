# pru-stopwatch

## Overview
The project consists of two elements:
- A kernel module which allows to access switch information through a sysfs interface available at `/sys/devices/prusw` 
- PRU0 firmware which measures the time for two switches:
    - First: Time between last state change in ms, -1 if it didn't happen
    - Second: Time for last time when the switch was on, -1 if it didn't happen
        
Required hardware and software:
- `BeagleBone Black rev C` 
- `Debian 9.5 2018-10-07 4GB SD IOT` (download link in installation description)
- Ethernet cable
- FTDI to TTL [like the one here](https://www.ftdichip.com/Support/Documents/DataSheets/Cables/DS_TTL-232R_RPi.pdf)
- USB Mini Type-B to Type-A cable
- An SD card of at least 4GB size
- Two switches

## Installation instructions (macOS)
1) Retrieve the raw disk filename for your SD card
    - Plug in the SD card in
    - `diskutil list`
    - Plug it out
    - `diskutil list`
    - The missing disk name is your SD card - it should be something similar to `/dev/diskX`
    - Accompanying raw disk filename should be `/dev/rdiskX` where X is the same number from the step above
2) Download `Debian 9.5 2018-10-07 4GB SD IOT`: `wget https://debian.beagleboard.org/images/bone-debian-9.5-iot-armhf-2018-10-07-4gb.img.xz`
3) Copy Debian to the SD card
    - Replace the X from commands below with your SD card number from step 1
    - Clear the card: `sudo dd if=/dev/zero of=/dev/rdiskX bs=8192`
    - Copy the image to the card: `xzcat bone-debian-9.5-iot-armhf-2018-10-07-4gb.img.xz | sudo dd of=/dev/rdiskX`
4) Insert the SD card into the card slot on the BBB
5) Power it up while holding the boot button
6) [Connect the FTDI to TTL cable to the serial port](https://elinux.org/Beagleboard:BeagleBone_Black_Serial)
7) Access Debian (Credentials: `debian` / `temppwd`): `sudo screen /dev/tty.usbserial-* 115200`
8) `cd /home/debian/; git clone https://github.com/bartlomiejn/pru-stopwatch; cd /home/debian/pru-stopwatch; sudo make install`
10) After reboot, connect first switch to P8_11 and P8_15 and second switch to P8_12 and P8_16

