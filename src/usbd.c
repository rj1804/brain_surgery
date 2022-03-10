#include "main.h"
#include "uart.h"
#include "oled.h"
#include "paint.h"

#include "usbd.h"
#include "usbd_core.h"

#include "usbd_desc_MSC.h"
#include "usbd_msc.h"
#include "usbd_storage.h"

static USBD_HandleTypeDef hUSBD;
USBD_HandleTypeDef* pUSBD = &hUSBD;

static int connected = 0;

#define MSC_MEDIA_PACKET  61440

ALIGN_32BYTES( static uint8_t bot_data[MSC_MEDIA_PACKET]; )

int usbd_init(void)
{
xprintf("[usbd] init\n");
	memset( &hUSBD, 0, sizeof(hUSBD) );
	
	if ( USBD_Init(&hUSBD, (USBD_DescriptorsTypeDef*)&usbd_desc_MSC, DEVICE_FS) ) {
		return 1;
	}
	USBD_RegisterClass(&hUSBD, (USBD_ClassTypeDef*)&USBD_MSC);
	USBD_MSC_RegisterStorage(&hUSBD, (USBD_StorageTypeDef*)&usbd_storage_fops, bot_data, sizeof(bot_data) );
	USBD_Start(&hUSBD);

	return 0;
}

void usbd_deinit(void)
{
xprintf("[usbd] stop\n");
	USBD_Stop(&hUSBD);
xprintf("[usbd] deinit\n");

	USBD_DeInit(&hUSBD);
}

int usbd_connected(void)
{
	return connected;
}

void usbd_connect(void)
{
xprintf("[usbd] connect\n");
	connected = 1;
}

void usbd_disconnect(void)
{
xprintf("[usbd] disconnect\n");
	connected = 0;
}

void usbd_eject(void)
{
xprintf("[usbd] eject\n");
	connected = 0;
}
