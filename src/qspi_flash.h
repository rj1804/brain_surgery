#ifndef _QSPI_FLASH_H_
#define _QSPI_FLASH_H_

int  qspi_flash_init(void); 
void qspi_write_data(uint32_t addr, const uint8_t *src, int size);
void qspi_read_data (uint32_t addr,       uint8_t *dst, int size);

#endif 

