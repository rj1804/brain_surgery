#include "usbd_core.h"
#include "usbd_desc_MSC.h"
#include "usbd_conf.h"
#include "usbd.h"

#define USB_VID 0x1397	
#define USB_PID 0x123c


#define LANGID_STRING        	0x409
#define CONFIGURATION_STRING 	"MSC Config"
#define INTERFACE_STRING     	"MSC Interface"
#define SERIAL_STRING     	"12345678"

#define USB_PRODUCT_STRING       "BrainSurgeon FreshBrains"
#define USB_MANUFACTURER_STRING  "BrainSurgeon"

/** USB standard device descriptor. */
static __ALIGN_BEGIN const uint8_t DeviceDesc[USB_LEN_DEV_DESC] __ALIGN_END = {
	0x12,			/*bLength */
	USB_DESC_TYPE_DEVICE,	/*bDescriptorType */
	0x00,			/*bcdUSB */
	0x02,
	0x00,			/*bDeviceClass */
	0x00,			/*bDeviceSubClass */
	0x00,			/*bDeviceProtocol */
	USB_MAX_EP0_SIZE,	/*bMaxPacketSize */
	LOBYTE(USB_VID),	/*idVendor */
	HIBYTE(USB_VID),	/*idVendor */
	LOBYTE(USB_PID),	/*idDevice */
	HIBYTE(USB_PID),	/*idDevice */
	0x00,			/*bcdDevice rel. 2.00 */
	0x02,
	USBD_IDX_MFC_STR,	/*Index of manufacturer  string */
	USBD_IDX_PRODUCT_STR,	/*Index of product string */
	USBD_IDX_SERIAL_STR,	/*Index of serial number string */
	USBD_MAX_NUM_CONFIGURATION	/*bNumConfigurations */
};

static __ALIGN_BEGIN const uint8_t LangIDDesc[USB_LEN_LANGID_STR_DESC] __ALIGN_END = {
	USB_LEN_LANGID_STR_DESC,
	USB_DESC_TYPE_STRING,
	LOBYTE(LANGID_STRING),
	HIBYTE(LANGID_STRING)
};

static __ALIGN_BEGIN uint8_t StrDesc[USBD_MAX_STR_DESC_SIZ] __ALIGN_END;

static uint8_t *DeviceDescriptor(USBD_SpeedTypeDef speed, uint16_t *length)
{
	*length = sizeof(DeviceDesc);
	return (uint8_t*) DeviceDesc;
}

static uint8_t *LangIDStrDescriptor(USBD_SpeedTypeDef speed, uint16_t *length)
{
	*length = sizeof(LangIDDesc);
	return (uint8_t*) LangIDDesc;
}

static uint8_t *ProductStrDescriptor(USBD_SpeedTypeDef speed, uint16_t *length)
{
	USBD_GetString((uint8_t *) USB_PRODUCT_STRING, StrDesc, length);
	return StrDesc;
}

static uint8_t *ManufacturerStrDescriptor(USBD_SpeedTypeDef speed, uint16_t *length)
{
	USBD_GetString((uint8_t *) USB_MANUFACTURER_STRING, StrDesc, length);
	return StrDesc;
}

static uint8_t *SerialStrDescriptor(USBD_SpeedTypeDef speed, uint16_t *length)
{
	USBD_GetString((uint8_t *) SERIAL_STRING, StrDesc, length);
	return StrDesc;
}

static uint8_t *ConfigStrDescriptor(USBD_SpeedTypeDef speed, uint16_t *length)
{
	USBD_GetString((uint8_t *) CONFIGURATION_STRING, StrDesc, length);
	return StrDesc;
}

static uint8_t *InterfaceStrDescriptor(USBD_SpeedTypeDef speed, uint16_t *length)
{
	USBD_GetString((uint8_t *) INTERFACE_STRING, StrDesc, length);
	return StrDesc;
}

const USBD_DescriptorsTypeDef usbd_desc_MSC = {
	DeviceDescriptor, 
	LangIDStrDescriptor, 
	ManufacturerStrDescriptor, 
	ProductStrDescriptor, 
	SerialStrDescriptor, 
	ConfigStrDescriptor,
	InterfaceStrDescriptor,
};

