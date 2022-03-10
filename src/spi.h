#ifndef _SPI_H
#define _SPI_H

int spi_init(void);
int spi_xfer(uint8_t *tbuf, int cnt);

#endif

