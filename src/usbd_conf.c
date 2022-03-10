#include "main.h"
#include "uart.h"

#include "usbd.h"
#include "usbd_def.h"
#include "usbd_core.h"

PCD_HandleTypeDef hpcd_FS;

#ifdef __cplusplus
extern "C"
#endif
void OTG_FS_IRQHandler(void)
{
//xprintf("!");
	HAL_PCD_IRQHandler(&hpcd_FS);
}

void HAL_PCD_MspInit(PCD_HandleTypeDef * hpcd)
{
	GPIO_InitTypeDef GPIO_InitStruct = {0};

	if (hpcd->Instance == USB_OTG_FS) {
xprintf("[PCD ] hw init FS!\n");

		RCC_PeriphCLKInitTypeDef PeriphClkInitStruct = { 0 };
		PeriphClkInitStruct.PeriphClockSelection =  RCC_PERIPHCLK_USB;
		// PLL3 
		PeriphClkInitStruct.PLL3.PLL3M      = 8;
		PeriphClkInitStruct.PLL3.PLL3N      = 384;
		PeriphClkInitStruct.PLL3.PLL3P      = 2;
		PeriphClkInitStruct.PLL3.PLL3Q      = 8;
		PeriphClkInitStruct.PLL3.PLL3R      = 2;	
		PeriphClkInitStruct.PLL3.PLL3RGE    = RCC_PLL3VCIRANGE_0;
//		PeriphClkInitStruct.PLL3.PLL3VCOSEL = RCC_PLL3VCOWIDE;
		PeriphClkInitStruct.PLL3.PLL3FRACN  = 0;
		PeriphClkInitStruct.UsbClockSelection  = RCC_USBCLKSOURCE_PLL3;
		if (HAL_RCCEx_PeriphCLKConfig(&PeriphClkInitStruct) != HAL_OK) {
			while(1);
		}
	
		// Enable the GPIOA clock
		__GPIOA_CLK_ENABLE();

		GPIO_InitStruct.Pin       = GPIO_PIN_11 |GPIO_PIN_12;
		GPIO_InitStruct.Mode      = GPIO_MODE_AF_PP;
		GPIO_InitStruct.Pull      = GPIO_NOPULL;
		GPIO_InitStruct.Speed     = GPIO_SPEED_FREQ_HIGH;
		GPIO_InitStruct.Alternate = GPIO_AF10_OTG1_FS;
		HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

		GPIO_InitStruct.Pin       = GPIO_PIN_9;
		GPIO_InitStruct.Mode      = GPIO_MODE_INPUT;
		GPIO_InitStruct.Pull      = GPIO_NOPULL;
		HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

		__HAL_RCC_USB_OTG_FS_CLK_ENABLE();

		/* Set USB FS Interrupt priority */
		HAL_NVIC_SetPriority(OTG_FS_IRQn, 1, 0);

		/* Enable USB FS Interrupt */
		HAL_NVIC_EnableIRQ(OTG_FS_IRQn);
	}
}

/**
  * @brief  De-Initializes the PCD MSP.
  * @param  hpcd: PCD handle
  * @retval None
  */
void HAL_PCD_MspDeInit(PCD_HandleTypeDef *hpcd)
{
	if (hpcd->Instance == USB_OTG_FS) {
		// Disable USB FS Clock 
		__HAL_RCC_USB_OTG_FS_CLK_DISABLE();
		HAL_NVIC_DisableIRQ(OTG_FS_IRQn);
	}
}

void HAL_PCD_SetupStageCallback(PCD_HandleTypeDef * hpcd)
{
	USBD_LL_SetupStage((USBD_HandleTypeDef *) hpcd->pData, (uint8_t *) hpcd->Setup);
}

void HAL_PCD_DataOutStageCallback(PCD_HandleTypeDef * hpcd, uint8_t epnum)
{
	USBD_LL_DataOutStage((USBD_HandleTypeDef *) hpcd->pData, epnum, hpcd->OUT_ep[epnum].xfer_buff);
}

void HAL_PCD_DataInStageCallback(PCD_HandleTypeDef * hpcd, uint8_t epnum)
{
	USBD_LL_DataInStage((USBD_HandleTypeDef *) hpcd->pData, epnum, hpcd->IN_ep[epnum].xfer_buff);
}

void HAL_PCD_SOFCallback(PCD_HandleTypeDef * hpcd)
{
	USBD_LL_SOF((USBD_HandleTypeDef *) hpcd->pData);
}

void HAL_PCD_ResetCallback(PCD_HandleTypeDef * hpcd)
{
	USBD_SpeedTypeDef speed = USBD_SPEED_FULL;

	/* Set USB current speed. */
	switch (hpcd->Init.speed) {
	case PCD_SPEED_HIGH:
		speed = USBD_SPEED_HIGH;
		break;
	case PCD_SPEED_FULL:
		speed = USBD_SPEED_FULL;
		break;

	default:
		speed = USBD_SPEED_FULL;
		break;
	}
	USBD_LL_SetSpeed((USBD_HandleTypeDef *) hpcd->pData, speed);

	/* Reset Device. */
	USBD_LL_Reset((USBD_HandleTypeDef *) hpcd->pData);
}

void HAL_PCD_SuspendCallback(PCD_HandleTypeDef * hpcd)
{
xprintf("\n[PCD ] Suspend\n");
	/* Inform USB library that core enters in suspend Mode */
	USBD_LL_Suspend((USBD_HandleTypeDef *) hpcd->pData);

	/*Enter in STOP mode */
	if (hpcd->Init.low_power_enable) {
		/* Set SLEEPDEEP bit and SleepOnExit of Cortex System Control Register */
		SCB->SCR |= (uint32_t) ((uint32_t) (SCB_SCR_SLEEPDEEP_Msk | SCB_SCR_SLEEPONEXIT_Msk));
	}
	usbd_disconnect();
}

void HAL_PCD_ResumeCallback(PCD_HandleTypeDef * hpcd)
{
xprintf("[PCD ] Resume\n");
	USBD_LL_Resume((USBD_HandleTypeDef *) hpcd->pData);
//	usbd_connect();
}

void HAL_PCD_ISOOUTIncompleteCallback(PCD_HandleTypeDef * hpcd, uint8_t epnum)
{
	USBD_LL_IsoOUTIncomplete((USBD_HandleTypeDef *) hpcd->pData, epnum);
}

void HAL_PCD_ISOINIncompleteCallback(PCD_HandleTypeDef * hpcd, uint8_t epnum)
{
	USBD_LL_IsoINIncomplete((USBD_HandleTypeDef *) hpcd->pData, epnum);
}

void HAL_PCD_ConnectCallback(PCD_HandleTypeDef * hpcd)
{
xprintf("[PCD ] Connect\n");
//	usbd_connect();
	USBD_LL_DevConnected((USBD_HandleTypeDef *) hpcd->pData);
}

void HAL_PCD_DisconnectCallback(PCD_HandleTypeDef * hpcd)
{
xprintf("[PCD ] Disconnect\n");
	usbd_disconnect();
	USBD_LL_DevDisconnected((USBD_HandleTypeDef *) hpcd->pData);
}


USBD_StatusTypeDef USBD_LL_Init(USBD_HandleTypeDef * pdev)
{
	if (pdev->id == DEVICE_FS) {
xprintf("[PCD ] USBD_LL_Init FS!\n");
		// Link The driver to the stack 
		hpcd_FS.pData  = pdev;
		pdev->pData = &hpcd_FS;

		hpcd_FS.Instance                 = USB_OTG_FS;
		hpcd_FS.Init.dev_endpoints       = 9;
		hpcd_FS.Init.speed               = PCD_SPEED_FULL;
		hpcd_FS.Init.dma_enable          = DISABLE;
		hpcd_FS.Init.ep0_mps             = 64;
		hpcd_FS.Init.phy_itface          = PCD_PHY_EMBEDDED;
		hpcd_FS.Init.Sof_enable          = DISABLE;
		hpcd_FS.Init.low_power_enable    = DISABLE;
		hpcd_FS.Init.lpm_enable          = DISABLE;
		hpcd_FS.Init.vbus_sensing_enable = ENABLE;
		hpcd_FS.Init.use_dedicated_ep1   = DISABLE;
		hpcd_FS.Init.use_external_vbus   = DISABLE;
		
		int ret;
		if ((ret = HAL_PCD_Init(&hpcd_FS)) != HAL_OK) {
xprintf("[PCD ] USBD_LL_Init FS ERR! %d\n", ret);
//			while(1);
			return USBD_FAIL;
		}

		HAL_PCDEx_SetRxFiFo(&hpcd_FS, 0x80);

		HAL_PCDEx_SetTxFiFo(&hpcd_FS, 0, 0x40);		// control
		HAL_PCDEx_SetTxFiFo(&hpcd_FS, 1, 0x80);		
	}
	return USBD_OK;
}

USBD_StatusTypeDef USBD_LL_DeInit(USBD_HandleTypeDef * pdev)
{
	HAL_StatusTypeDef hal_status = HAL_OK;
	USBD_StatusTypeDef usb_status = USBD_OK;

	hal_status = HAL_PCD_DeInit((PCD_HandleTypeDef*)pdev->pData);

	switch (hal_status) {
	case HAL_OK:
		usb_status = USBD_OK;
		break;
	case HAL_ERROR:
		usb_status = USBD_FAIL;
		break;
	case HAL_BUSY:
		usb_status = USBD_BUSY;
		break;
	case HAL_TIMEOUT:
		usb_status = USBD_FAIL;
		break;
	default:
		usb_status = USBD_FAIL;
		break;
	}
	return usb_status;
}

USBD_StatusTypeDef USBD_LL_Start(USBD_HandleTypeDef * pdev)
{
	HAL_StatusTypeDef hal_status = HAL_OK;
	USBD_StatusTypeDef usb_status = USBD_OK;

	hal_status = HAL_PCD_Start((PCD_HandleTypeDef*)pdev->pData);

	switch (hal_status) {
	case HAL_OK:
		usb_status = USBD_OK;
		break;
	case HAL_ERROR:
		usb_status = USBD_FAIL;
		break;
	case HAL_BUSY:
		usb_status = USBD_BUSY;
		break;
	case HAL_TIMEOUT:
		usb_status = USBD_FAIL;
		break;
	default:
		usb_status = USBD_FAIL;
		break;
	}
	return usb_status;
}

USBD_StatusTypeDef USBD_LL_Stop(USBD_HandleTypeDef * pdev)
{
	HAL_StatusTypeDef hal_status = HAL_OK;
	USBD_StatusTypeDef usb_status = USBD_OK;

	hal_status = HAL_PCD_Stop((PCD_HandleTypeDef*)pdev->pData);

	switch (hal_status) {
	case HAL_OK:
		usb_status = USBD_OK;
		break;
	case HAL_ERROR:
		usb_status = USBD_FAIL;
		break;
	case HAL_BUSY:
		usb_status = USBD_BUSY;
		break;
	case HAL_TIMEOUT:
		usb_status = USBD_FAIL;
		break;
	default:
		usb_status = USBD_FAIL;
		break;
	}
	return usb_status;
}

USBD_StatusTypeDef USBD_LL_OpenEP(USBD_HandleTypeDef * pdev, uint8_t ep_addr, uint8_t ep_type, uint16_t ep_mps)
{
	HAL_StatusTypeDef hal_status = HAL_OK;
	USBD_StatusTypeDef usb_status = USBD_OK;

	hal_status = HAL_PCD_EP_Open((PCD_HandleTypeDef*)pdev->pData, ep_addr, ep_mps, ep_type);

	switch (hal_status) {
	case HAL_OK:
		usb_status = USBD_OK;
		break;
	case HAL_ERROR:
		usb_status = USBD_FAIL;
		break;
	case HAL_BUSY:
		usb_status = USBD_BUSY;
		break;
	case HAL_TIMEOUT:
		usb_status = USBD_FAIL;
		break;
	default:
		usb_status = USBD_FAIL;
		break;
	}
	return usb_status;
}

USBD_StatusTypeDef USBD_LL_CloseEP(USBD_HandleTypeDef * pdev, uint8_t ep_addr)
{
	HAL_StatusTypeDef hal_status = HAL_OK;
	USBD_StatusTypeDef usb_status = USBD_OK;

	hal_status = HAL_PCD_EP_Close((PCD_HandleTypeDef*)pdev->pData, ep_addr);

	switch (hal_status) {
	case HAL_OK:
		usb_status = USBD_OK;
		break;
	case HAL_ERROR:
		usb_status = USBD_FAIL;
		break;
	case HAL_BUSY:
		usb_status = USBD_BUSY;
		break;
	case HAL_TIMEOUT:
		usb_status = USBD_FAIL;
		break;
	default:
		usb_status = USBD_FAIL;
		break;
	}
	return usb_status;
}

USBD_StatusTypeDef USBD_LL_FlushEP(USBD_HandleTypeDef * pdev, uint8_t ep_addr)
{
	HAL_StatusTypeDef hal_status = HAL_OK;
	USBD_StatusTypeDef usb_status = USBD_OK;

	hal_status = HAL_PCD_EP_Flush((PCD_HandleTypeDef*)pdev->pData, ep_addr);

	switch (hal_status) {
	case HAL_OK:
		usb_status = USBD_OK;
		break;
	case HAL_ERROR:
		usb_status = USBD_FAIL;
		break;
	case HAL_BUSY:
		usb_status = USBD_BUSY;
		break;
	case HAL_TIMEOUT:
		usb_status = USBD_FAIL;
		break;
	default:
		usb_status = USBD_FAIL;
		break;
	}
	return usb_status;
}

USBD_StatusTypeDef USBD_LL_StallEP(USBD_HandleTypeDef * pdev, uint8_t ep_addr)
{
	HAL_StatusTypeDef hal_status = HAL_OK;
	USBD_StatusTypeDef usb_status = USBD_OK;

	hal_status = HAL_PCD_EP_SetStall((PCD_HandleTypeDef*)pdev->pData, ep_addr);

	switch (hal_status) {
	case HAL_OK:
		usb_status = USBD_OK;
		break;
	case HAL_ERROR:
		usb_status = USBD_FAIL;
		break;
	case HAL_BUSY:
		usb_status = USBD_BUSY;
		break;
	case HAL_TIMEOUT:
		usb_status = USBD_FAIL;
		break;
	default:
		usb_status = USBD_FAIL;
		break;
	}
	return usb_status;
}

USBD_StatusTypeDef USBD_LL_ClearStallEP(USBD_HandleTypeDef * pdev, uint8_t ep_addr)
{
	HAL_StatusTypeDef hal_status = HAL_OK;
	USBD_StatusTypeDef usb_status = USBD_OK;

	hal_status = HAL_PCD_EP_ClrStall((PCD_HandleTypeDef*)pdev->pData, ep_addr);

	switch (hal_status) {
	case HAL_OK:
		usb_status = USBD_OK;
		break;
	case HAL_ERROR:
		usb_status = USBD_FAIL;
		break;
	case HAL_BUSY:
		usb_status = USBD_BUSY;
		break;
	case HAL_TIMEOUT:
		usb_status = USBD_FAIL;
		break;
	default:
		usb_status = USBD_FAIL;
		break;
	}
	return usb_status;
}

uint8_t USBD_LL_IsStallEP(USBD_HandleTypeDef * pdev, uint8_t ep_addr)
{
	PCD_HandleTypeDef *hpcd = (PCD_HandleTypeDef *) pdev->pData;

	if ((ep_addr & 0x80) == 0x80) {
		return hpcd->IN_ep[ep_addr & 0x7F].is_stall;
	} else {
		return hpcd->OUT_ep[ep_addr & 0x7F].is_stall;
	}
}

USBD_StatusTypeDef USBD_LL_SetUSBAddress(USBD_HandleTypeDef * pdev, uint8_t dev_addr)
{
	HAL_StatusTypeDef hal_status = HAL_OK;
	USBD_StatusTypeDef usb_status = USBD_OK;

	hal_status = HAL_PCD_SetAddress((PCD_HandleTypeDef*)pdev->pData, dev_addr);

	switch (hal_status) {
	case HAL_OK:
		usb_status = USBD_OK;
		break;
	case HAL_ERROR:
		usb_status = USBD_FAIL;
		break;
	case HAL_BUSY:
		usb_status = USBD_BUSY;
		break;
	case HAL_TIMEOUT:
		usb_status = USBD_FAIL;
		break;
	default:
		usb_status = USBD_FAIL;
		break;
	}
	return usb_status;
}

USBD_StatusTypeDef USBD_LL_Transmit(USBD_HandleTypeDef * pdev, uint8_t ep_addr, uint8_t * pbuf, uint16_t size)
{
	HAL_StatusTypeDef hal_status = HAL_OK;
	USBD_StatusTypeDef usb_status = USBD_OK;

	hal_status = HAL_PCD_EP_Transmit((PCD_HandleTypeDef*)pdev->pData, ep_addr, pbuf, size);

	switch (hal_status) {
	case HAL_OK:
		usb_status = USBD_OK;
		break;
	case HAL_ERROR:
		usb_status = USBD_FAIL;
		break;
	case HAL_BUSY:
		usb_status = USBD_BUSY;
		break;
	case HAL_TIMEOUT:
		usb_status = USBD_FAIL;
		break;
	default:
		usb_status = USBD_FAIL;
		break;
	}
	return usb_status;
}

USBD_StatusTypeDef USBD_LL_PrepareReceive(USBD_HandleTypeDef * pdev, uint8_t ep_addr, uint8_t * pbuf, uint16_t size)
{
	HAL_StatusTypeDef hal_status = HAL_OK;
	USBD_StatusTypeDef usb_status = USBD_OK;

	hal_status = HAL_PCD_EP_Receive((PCD_HandleTypeDef*)pdev->pData, ep_addr, pbuf, size);

	switch (hal_status) {
	case HAL_OK:
		usb_status = USBD_OK;
		break;
	case HAL_ERROR:
		usb_status = USBD_FAIL;
		break;
	case HAL_BUSY:
		usb_status = USBD_BUSY;
		break;
	case HAL_TIMEOUT:
		usb_status = USBD_FAIL;
		break;
	default:
		usb_status = USBD_FAIL;
		break;
	}
	return usb_status;
}

uint32_t USBD_LL_GetRxDataSize(USBD_HandleTypeDef * pdev, uint8_t ep_addr)
{
	return HAL_PCD_EP_GetRxCount((PCD_HandleTypeDef *) pdev->pData, ep_addr);
}

void USBD_LL_Delay(uint32_t Delay)
{
	HAL_Delay(Delay);
}

