# pru-stopwatch

## Overview
- This project uses `BeagleBone Black rev C` with `Debian 9.5 2018-10-07 4GB SD IOT`
- Three cables are required:
    - Ethernet
    - FTDI to TTL ([like the one here](https://www.ftdichip.com/Support/Documents/DataSheets/Cables/DS_TTL-232R_RPi.pdf))
    - USB Mini Type-B to Type-A cable
- An SD card of at least 4GB size

## Installation instructions (macOS)
1. Retrieve the raw disk filename for your SD card
    - Plug in the SD card in
    - `diskutil list`
    - Plug it out
    - `diskutil list`
    - The missing disk name is your SD card - it should be something similar to `/dev/diskX`
    - Accompanying raw disk filename should be `/dev/rdiskX` where X is the same number from the step above
    - Make sure the number is right in order to not damage anything on your computer in following steps
2. Download and setup Debian
    - `wget https://debian.beagleboard.org/images/bone-debian-9.5-iot-armhf-2018-10-07-4gb.img.xz`
3. Copy Debian to the SD card
    - Clear the card: `sudo dd if=/dev/zero of=/dev/rdiskX bs=8192`
    - `dd` won't give you any indication that it's working, but it's working - just leave it running for a while and don't interrupt anything
    - Copy the image to the card: `xzcat bone-debian-9.5-iot-armhf-2018-10-07-4gb.img.xz | sudo dd of=/dev/rdiskX`
4. Insert the SD card into the card slot on the BBB
5. While holding the boot button, which is the one next to the SD card slot, power it up using the Mini Type-B cable
6. [Connect the FTDI to TTL cable to the serial port](https://elinux.org/Beagleboard:BeagleBone_Black_Serial)
7. Connect to the BBB
    - `sudo screen /dev/tty.usbserial-* 115200`
    - Default credentials: `debian` / `temppwd`

(Further steps will be added here)
