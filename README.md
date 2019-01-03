# pru-stopwatch

## Overview
- This project uses `BeagleBone Black rev C` with `Debian 9.5 2018-10-07 4GB SD IOT`
- Three cables are required:
    - Ethernet
    - FTDI to TTL [like the one here](https://www.ftdichip.com/Support/Documents/DataSheets/Cables/DS_TTL-232R_RPi.pdf)
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
2. Download Debian
    - `wget https://debian.beagleboard.org/images/bone-debian-9.5-iot-armhf-2018-10-07-4gb.img.xz`
3. Copy Debian to the SD card
    - Replace the X from commands below with your SD card number from step 1
    - Clear the card: `sudo dd if=/dev/zero of=/dev/rdiskX bs=8192`
    - Copy the image to the card: `xzcat bone-debian-9.5-iot-armhf-2018-10-07-4gb.img.xz | sudo dd of=/dev/rdiskX`
4. Insert the SD card into the card slot on the BBB
5. While holding the boot button, power it up using the Mini Type-B cable
6. [Connect the FTDI to TTL cable to the serial port](https://elinux.org/Beagleboard:BeagleBone_Black_Serial), the pins that are of interest to you are J1_1, J1_4 and J1_5 which are GND, RX and TX respectively
7. Connect using:
    - `sudo screen /dev/tty.usbserial-* 115200`
    - Default credentials: `debian` / `temppwd`
8. Setup the board:
    - Edit `/boot/uEnv.txt`
    - Set the DTB to `dtb=am335x-boneblack.dtb`
    - Disable all overlays by setting all `disable_uboot_overlay_*=1` - like `disable_uboot_overlay_emmc=1`, `disable_uboot_overlay_audio=1` etc.
    - In this distribution you should be on the 4.14.x kernel by default, but make sure by checking `sudo /opt/scripts/tools/version.sh | grep kernel`
    - Under `PRUSS OPTIONS` comment all `uboot_overlay_pru` except for the `(4.14.x-ti kernel)` one which should stay uncommented
    - Save the file and `sudo reboot`
9. Clone this repository into the `home` directory: 
    - `cd ~; git clone https://github.com/bartlomiejn/pru-stopwatch`

(Further steps will be added here)
