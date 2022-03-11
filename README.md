## intro

**brain surgery** is a replacement boot loader firmware for the B* ***BRAINS*** eurorack module.

see it in action: https://twitter.com/tripo1804/status/1501535014583877636

**brain surgery** replaces the original bootloader in the CPU internal FLASH memory, it does **NOT** touch the actual module firmware which is stored in an external SPI FLASH chip

in order to install **brain surgery** you need to completely wipe the internal FLASH and thus set the ROP (read out protection) bits in the option ROM back to "no protection"

if needed the original FLASH boot loader can be re-installed - maybe make a copy before you erase it :wink:

## **building**

- you need an ARM GCC, e.g. https://developer.arm.com/tools-and-software/open-source-software/developer-tools/gnu-toolchain/gnu-rm/downloads/9-2020-q2-update
- set the **TCHAIN_PATH** environment variable or prepend it to the make commands:
  - e.g. ```TCHAIN_PATH=/opt/gcc-arm-none-eabi-9-2020-q2-update/bin make```

## **connecting a programmer**
- connect an STLINK (or another SWD capable debugger) to the header pins on the back of the PCB (maybe solder some first)
- the pinout is from top to bottom:
  - 3V3
  - SWCLK
  - GND
  - SWDIO
  - NRST
  - UART7 RX (it's RX, but it can be used as TX as well which is what we do) 
- the makefile uses OPENOCD (https://openocd.org/) but you could also use the official STM32CubeProgrammer (https://www.st.com/en/development-tools/stm32cubeprog.html)

## **erasing the original bootloader**
- **NOTE: do this only if you know exactly what you are doing!!!**
- to erase the FLASH run: ```make erase```
- (you only need to do this once, afterwards you can just flash a new bootloader as below)

## **installing the new bootloader**
- to program the new boot loader: ```make program```

## **using the bootloader**
- if you do nothing it will show a nice animation, then load and start the original firmware
- if you connect a USB cable and hold the **left** button while you power on the module the boot loader will expose a USB MSC (mass storage class) device
- note that there is no filesystem, it's a raw drive
- to read the QSPI flash: ```dd if=/dev/sgX of=brain.dump```
- to write a file to the QSPI flash: ```dd if=file.bin of=/dev/sgX```
- **replace the X in /dev/sgX with the appropriate drive, make sure you dont kill any of your HDDs or SSDs :wink:**
- **the QSPI size is 2MB (16Mbit), the stock firmware code takes about 128k, BUT there is other non-code data at other offsets in the FLASH, so don't just send a 2MB file without making a backup**
- if you are done with flashing, press the **right** button to start the firmware (or power cycle the module)
- if you have STLINK v2.1 or V3 you can also read some serial debug logs on the USB UART, otherwise you need something like an FTDI USD to serial converter... (115200, 8n1)

## **have fun!**

proceed at your own risk, yet remember: the module is cheap, fun is invaluable

**MAKE LOVE NOT WAR**
