#include "main.h"
#include "uart.h"
#include "system.h"
#include "qspi_flash.h"
#include "oled.h"
#include "paint.h"
#include "usbd.h"

#include <string.h>

void Error_Handler(void)
{
xprintf("\n\n\nOOPS!\n");
	while ( 1 );
}

#define MAIN_START_ADDR 0x024000000

typedef void (*pFunction)(void);

static uint32_t chk32( uint8_t *data, int size ) 
{
	uint32_t chk = 0; 
	int i; 
	for(i = 0; i < size; i++ ) {
		chk += *(data++); 
	} 
	return chk;
}

static int start_main(void) 
{
	uint32_t *pmain = (uint32_t*)MAIN_START_ADDR;
	uint32_t head[4];
	
	qspi_read_data( 0, (uint8_t*)head, 16 );

	xprintf("[main] %08X %08x %08X %08X\n", head[0], head[1], head[2], head[3]);

	if( head[0] > 0x40000 ) {
xprintf("main too large!\n");
		return 1;	
	}

	qspi_read_data( 16, (uint8_t*)pmain, head[0] );

	uint32_t chksum = chk32( (uint8_t*)pmain, head[0] );

	if( head[1] != chksum ) {
xprintf("checksum mismatch!\n");
		return 2;	
	}
	 
	uint32_t main_stack = pmain[0];
	uint32_t reset_vect = pmain[1];

	xprintf("[main] stack %08X  reset %08x\n", main_stack, reset_vect);

	xprintf("[main] booting app...\n\n\n\n");

	SCB_DisableDCache();
	SCB_DisableICache();

	SysTick->CTRL &= ~SysTick_CTRL_ENABLE_Msk;

	HAL_RCC_DeInit();

	SysTick->CTRL = 0;
	SysTick->LOAD = 0;
	SysTick->VAL = 0;

	pFunction applicationEntry = (pFunction) reset_vect;

	__set_MSP(main_stack);

	applicationEntry();
	
	return 0;
}

#define DELAY 5

#define DELAY2 3

static void oled_welcome(void)
{
	paint_set_color( COLOR_FULL );
	
	int i;
	oled_clear();
	for(i = 0; i < 128; i++) {
		paint_draw_vline(i, 0, 63 );
		oled_update_all();
		HAL_Delay(DELAY);	
	}

	paint_set_color( COLOR_NO );

	for(i = 0; i < 64; i++) {
		paint_draw_hline(0, 127, i );
		oled_update_all();
		HAL_Delay(DELAY);	
	}

	paint_set_color( COLOR_FULL );

	oled_clear();
	paint_draw_rect(0, 0, 128, 64);
	for(i = 0; i < 128; i++) {
		paint_draw_line(i, 0, 64, 32 );
		oled_update_all();
		HAL_Delay(DELAY2);	
	}

	for(i = 0; i < 64; i++) {
		paint_draw_line(127, i, 64, 32 );
		oled_update_all();
		HAL_Delay(DELAY2);	
	}

	for(i = 127; i > 0; i--) {
		paint_draw_line(i, 63, 64, 32 );
		oled_update_all();
		HAL_Delay(DELAY2);	
	}

	for(i = 63; i > 0; i--) {
		paint_draw_line(0, i, 64, 32 );
		oled_update_all();
		HAL_Delay(DELAY2);	
	}

	HAL_Delay(500);	
	paint_set_color( COLOR_NO );
	paint_draw_fillrect(4,4, 128 - 8, 64 - 8);

	paint_set_color( COLOR_FULL );

	paint_draw_text_size(16, 14, 2, "Make Love");
	paint_draw_text_size(32, 34, 2, "Not War");
	oled_update_all();
	HAL_Delay(2000);	

	for(i = 0; i < 128; i++) {
		paint_set_color( COLOR_FULL );
		paint_draw_rect(i/2, i/4, 128 - i, 64 - i / 2);
		oled_update_all();
		HAL_Delay(1);	
		paint_set_color( COLOR_NO );
		paint_draw_rect(i/2, i/4, 128 - i, 64 - i / 2);
	}
	oled_clear();
	oled_update_all();
	HAL_Delay(1000);	
}

int main(void)
{
	system_MPU_config();

	system_cache_config();

	HAL_Init();

	system_clock_config();

	UART_init();

	xprintf("\n\nbrain surgery in progress!\n\n");
	
	xprintf("SYSCLK %d  HCLK %d  PCLK1 %d  PCLK2 %d\n\n", 
			(unsigned int)HAL_RCC_GetSysClockFreq(), 
			(unsigned int)HAL_RCC_GetHCLKFreq(),
			(unsigned int)HAL_RCC_GetPCLK1Freq(),
			(unsigned int)HAL_RCC_GetPCLK2Freq() );

	oled_init();

	qspi_flash_init();

	GPIO_InitTypeDef GPIO_InitStruct = {0};
	__GPIOD_CLK_ENABLE();

	GPIO_InitStruct.Pin  = GPIO_PIN_9 | GPIO_PIN_10;
	GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
	GPIO_InitStruct.Pull = GPIO_PULLUP;
	HAL_GPIO_Init(GPIOD, &GPIO_InitStruct);

	if( !HAL_GPIO_ReadPin(GPIOD, GPIO_PIN_10) ) {
		usbd_init();
	}
	
	oled_welcome();

	if( usbd_connected() ) {
		oled_clear();
		paint_set_color( COLOR_FULL );
		paint_draw_text_size(0, 16, 2, "USB");
		paint_draw_text_size(0, 32, 2, "connected...");
		paint_draw_text_size(0, 56, 1, "press RIGHT button to EXIT");
		oled_update_all();
	}

	uint32_t next = 0;
	while (usbd_connected() && HAL_GPIO_ReadPin(GPIOD, GPIO_PIN_9)) {
		uint32_t tick = HAL_GetTick();
		if( tick > next ) {
			next = tick + 100;
//xprintf(".%d%d", HAL_GPIO_ReadPin(GPIOD, GPIO_PIN_9), HAL_GPIO_ReadPin(GPIOD, GPIO_PIN_10) ); 
		}
	}

	oled_clear();
	oled_update_all();

	int ret;
	if( (ret = start_main()) ) {
		paint_set_color( COLOR_FULL );
		paint_draw_text_size(0, 16,  2, "BOOT FAILED");
		paint_draw_text_size(0, 32, 1, ret == 1 ? "size too large!" : "checksum mismatch!");
		oled_update_all();

		xprintf("\nboot failed!\n\n");
	
		while(1);
	}
}
