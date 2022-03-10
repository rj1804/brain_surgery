#include "main.h"
#include "uart.h"
#include "usbd_storage.h"
#include "usbd.h"
#include "qspi_flash.h"

#define STORAGE_LUN_NBR    1
#define STORAGE_BLK_SIZ 4096
#define STORAGE_BLK_NBR  512

// USB Mass storage Standard Inquiry Data.

const uint8_t _inquiry_data[] = {	/* 36 */

	/* LUN 0 */
	0x00,
	0x80,
	0x02,
	0x02,
	(STANDARD_INQUIRY_DATA_LEN - 5),
	0x00,
	0x00,
	0x00,
	'B', 'R', 'A', 'I', 'N', 'S', 'U', 'R',	/* Manufacturer : 8 bytes */
	'G', 'E', 'O', 'N', ' ', ' ', ' ', ' ',	/* Product      : 16 Bytes */
	' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ',
	'0', '.', '0', '1'	/* Version      : 4 Bytes */
};

static int8_t _init(uint8_t lun)
{
xprintf("[stor] _init\n");
	usbd_connect();
	return (USBD_OK);
}

static int8_t _get_capacity(uint8_t lun, uint32_t *block_num, uint16_t *block_size)
{
xprintf("[stor] get_capacity\n");
	*block_num  = STORAGE_BLK_NBR;
	*block_size = STORAGE_BLK_SIZ;
	return (USBD_OK);
}

static int8_t _is_ready(uint8_t lun)
{
//xprintf("[stor] is_ready\n");
	return (USBD_OK);
}

static int8_t _is_write_protected(uint8_t lun)
{
//xprintf("[stor] is_write_protected\n");
	return (USBD_OK);
}

static int8_t _read(uint8_t lun, uint8_t *buf, uint32_t blk_addr, uint16_t blk_len)
{
xprintf("[stor] read  %08X  %4d\n", (unsigned int) blk_addr, blk_len);
	qspi_read_data( STORAGE_BLK_SIZ * blk_addr, buf, STORAGE_BLK_SIZ * blk_len );

	return (USBD_OK);
}

static int8_t _write(uint8_t lun, uint8_t *buf, uint32_t blk_addr, uint16_t blk_len)
{
xprintf("[stor] write %08X  %4d\n", (unsigned int)blk_addr, (unsigned int)blk_len);
	qspi_write_data(STORAGE_BLK_SIZ * blk_addr, buf, STORAGE_BLK_SIZ * blk_len);

	return (USBD_OK);
}

static int8_t _get_max_lun(void)
{
xprintf("[stor] get_max_lun\n");
	return (STORAGE_LUN_NBR - 1);
}

static int8_t _start_stop_unit(uint8_t lun, uint8_t *params)
{
xprintf("[stor] start_stop_unit %d\n", params[4]);

	if((params[4] & 0xF3) == 0x02) {
xprintf("[stor] EJECT!\n");
		usbd_eject();
	}
	return 0;
}

const USBD_StorageTypeDef usbd_storage_fops = {
	_init,
	_get_capacity,
	_is_ready,
	_is_write_protected,
	_read,
	_write,
	_get_max_lun,
	_start_stop_unit,
	(int8_t*) _inquiry_data
};
