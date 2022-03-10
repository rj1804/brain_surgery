#include "main.h"
#include "uart.h"

#include <string.h>

static UART_HandleTypeDef huart;

void UART_putc(uint8_t data)
{
	while (__HAL_UART_GET_FLAG(&huart, UART_FLAG_TXE) == RESET) {
		// wait
	}
	huart.Instance->TDR = data;
}

int UART_getc( void )
{
	unsigned char c;
	int ret = HAL_UART_Receive(&huart, (uint8_t *)&c, 1, 0); 
	if(ret) { 
		return -1;
	}
	return c;
}

/* Definition for USARTx clock resources */
#define USARTx                           UART7
#define USARTx_CLK_ENABLE()              __HAL_RCC_UART7_CLK_ENABLE();
#define USARTx_RX_GPIO_CLK_ENABLE()      __HAL_RCC_GPIOB_CLK_ENABLE()
#define USARTx_TX_GPIO_CLK_ENABLE()      __HAL_RCC_GPIOB_CLK_ENABLE()

/* Definition for USARTx Pins */
#define USARTx_TX_PIN                    GPIO_PIN_4
#define USARTx_TX_GPIO_PORT              GPIOB
#define USARTx_TX_AF                     GPIO_AF11_UART7

#define USARTx_RX_PIN                    GPIO_PIN_3
#define USARTx_RX_GPIO_PORT              GPIOB
#define USARTx_RX_AF                     GPIO_AF11_UART7

int UART_init( void )
{
	GPIO_InitTypeDef  GPIO_InitStruct;

	USARTx_TX_GPIO_CLK_ENABLE();
	USARTx_RX_GPIO_CLK_ENABLE();

	GPIO_InitStruct.Pull	  = GPIO_NOPULL;
	GPIO_InitStruct.Speed	  = GPIO_SPEED_FAST;
	GPIO_InitStruct.Alternate = USARTx_TX_AF;

/*
	GPIO_InitStruct.Pin	  = USARTx_TX_PIN;
	GPIO_InitStruct.Mode	  = GPIO_MODE_AF_PP;
	HAL_GPIO_Init(USARTx_TX_GPIO_PORT, &GPIO_InitStruct);
*/

	GPIO_InitStruct.Pin       = USARTx_RX_PIN;
	GPIO_InitStruct.Alternate = USARTx_RX_AF;

	HAL_GPIO_Init(USARTx_RX_GPIO_PORT, &GPIO_InitStruct);

	USARTx_CLK_ENABLE(); 

	huart.Instance	        = USARTx;
	huart.Init.BaudRate     = DEBUG_BAUDRATE;
	huart.Init.WordLength   = UART_WORDLENGTH_8B;
	huart.Init.StopBits     = UART_STOPBITS_1;
	huart.Init.Parity       = UART_PARITY_NONE;
	huart.Init.HwFlowCtl    = UART_HWCONTROL_NONE;
	huart.Init.Mode	        = UART_MODE_TX_RX;
	huart.Init.OverSampling = UART_OVERSAMPLING_16;
	huart.AdvancedInit.AdvFeatureInit = UART_ADVFEATURE_SWAP_INIT;
	huart.AdvancedInit.Swap = UART_ADVFEATURE_SWAP_ENABLE;
	  
	int ret;
	if((ret = HAL_UART_Init(&huart)) != HAL_OK)
	{
		return ret;
	}

	return HAL_OK;
}
