*brain surgery* is a replacement boot loader firmware for the *B* Brains eurorack module.

*brain surgery* replaces the original bootloader in the CPU internal flash, it does not touch the actual module firmware which is stored in an SPI FLASH chip

in order to flash *brain surgery* you need to wipe the internal FLASH and thus set the ROP (read out protection) bits in the option ROM back to "no protection"

if needed the original FLASH boot loader can be re-installed (maybe make a copy...)
