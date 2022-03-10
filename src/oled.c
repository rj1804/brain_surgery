#include <stdint.h>
#include <ctype.h>
#include <stdio.h>
#include <string.h>

#include "main.h"
#include "paint.h"
#include "spi.h"
#include "uart.h"

#define SSD1306_LOW_COL		0x00	
#define SSD1306_HIGH_COL	0x10	
#define SSD1306_MEMORY_MODE	0x20	
#define SSD1306_COL_ADDRESS	0x21	
#define SSD1306_PAGE_ADDRESS	0x22
	
#define SSD1306_STARTLINE	0x40

#define SSD1306_CONTRAST	0x81
#define SSD1306_CHRG_PUMP	0x8D

#define SSD1306_REMAP0		0xA0	
#define SSD1306_REMAP127	0xA1	

#define SSD1306_RAM_ON		0xA4
#define SSD1306_ALL_ON		0xA5
#define SSD1306_NORMAL		0xA6
#define SSD1306_INVERSE		0xA7
#define SSD1306_MPX_RATIO	0xA8
#define SSD1306_OFF		0xAE
#define SSD1306_ON		0xAF

#define SSD1306_SCAN_NRM	0xC0
#define SSD1306_SCAN_REV	0xC8

#define SSD1306_OFFSET		0xD3
#define SSD1306_CLK_DIV		0xD5
#define SSD1306_PRECHARGE	0xD9

#define SSD1306_COM_PINS_HW	0xDA
#define SSD1306_VCOMH_LEVEL	0xDB

#define SSD1306_NOP		0xE3

#define SSD1306_R_HSCROLL_SETUP	0x26
#define SSD1306_L_HSCROLL_SETUP	0x27
#define SSD1306_RHVSCROLL_SETUP	0x29
#define SSD1306_DEACT_SCROLL	0x2e
#define SSD1306_ACTIVATE_SCROLL	0x2f
#define SSD1306_SET_VSCROLLAREA	0xA3

uint8_t fb[FB_SIZE] = { 0 };

#define LCD_PORT      	GPIOB
#define GPIO_PIN_CS     GPIO_PIN_12
#define GPIO_PIN_DC     GPIO_PIN_14

#define GPIO_PIN_RST	GPIO_PIN_8

static void write_command( uint8_t command )
{
	HAL_GPIO_WritePin(LCD_PORT, GPIO_PIN_DC, GPIO_PIN_RESET);

	HAL_GPIO_WritePin(LCD_PORT, GPIO_PIN_CS, GPIO_PIN_RESET);
	spi_xfer((uint8_t*)&command, 1);
	HAL_GPIO_WritePin(LCD_PORT, GPIO_PIN_CS, GPIO_PIN_SET);
}

void oled_update_all(void)
{
	HAL_GPIO_WritePin(LCD_PORT, GPIO_PIN_CS, GPIO_PIN_RESET);

	uint8_t *f = fb;
	
	uint8_t i = 0;
	for(i = 0; i < 8; i++ ) {
		HAL_GPIO_WritePin(LCD_PORT, GPIO_PIN_DC, GPIO_PIN_RESET);
		uint8_t cmd[3] = { 0xb0, 0x10, 0x02 };
		cmd[0] += i;
		spi_xfer(cmd, 3);
		HAL_GPIO_WritePin(LCD_PORT, GPIO_PIN_DC, GPIO_PIN_SET);
		spi_xfer(f, 128);
		f += 128;
	}

	HAL_GPIO_WritePin(LCD_PORT, GPIO_PIN_CS, GPIO_PIN_SET);
}

void oled_clear( void )
{
	memset( fb, invert ? 0xFF : 0, FB_SIZE);
}

void oled_init( void )
{
xprintf("[OLED] init\n");
	spi_init();

	GPIO_InitTypeDef GPIO_InitStruct = {};

	__HAL_RCC_GPIOB_CLK_ENABLE();
	__HAL_RCC_GPIOD_CLK_ENABLE();

	// PB12 CS
	// PB14 DC
	// PD8 	RST

	GPIO_InitStruct.Pin       = GPIO_PIN_12 | GPIO_PIN_14;
	GPIO_InitStruct.Mode      = GPIO_MODE_OUTPUT_PP;
	GPIO_InitStruct.Pull      = GPIO_NOPULL;
	GPIO_InitStruct.Speed     = GPIO_SPEED_FREQ_LOW;
	HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

	// RESET
	GPIO_InitStruct.Pin       = GPIO_PIN_RST;
	GPIO_InitStruct.Mode      = GPIO_MODE_OUTPUT_PP;
	GPIO_InitStruct.Pull      = GPIO_NOPULL;
	GPIO_InitStruct.Speed     = GPIO_SPEED_FREQ_LOW;

	HAL_GPIO_Init(GPIOD, &GPIO_InitStruct);
	HAL_GPIO_WritePin(GPIOD, GPIO_PIN_RST, GPIO_PIN_SET);

	// set cs
	HAL_GPIO_WritePin(LCD_PORT, GPIO_PIN_CS, GPIO_PIN_SET);

	// RESET OLED
	HAL_GPIO_WritePin(GPIOD, GPIO_PIN_RST, GPIO_PIN_RESET);
	HAL_Delay(1);
	HAL_GPIO_WritePin(GPIOD, GPIO_PIN_RST, GPIO_PIN_SET);


	write_command(SSD1306_OFF);
	write_command(SSD1306_LOW_COL   | 0x00);	
	write_command(SSD1306_HIGH_COL  | 0x00);	
	write_command(SSD1306_STARTLINE | 0x00);

	write_command(SSD1306_CONTRAST);	
	write_command(0x80);

	write_command(SSD1306_REMAP127);
	write_command(SSD1306_SCAN_REV);
	write_command(SSD1306_NORMAL);

	write_command(SSD1306_MPX_RATIO);
	write_command(0x3F);
	
	write_command(SSD1306_OFFSET);
	write_command(0x01);
	
	write_command(SSD1306_CLK_DIV);
	write_command(0x80);
	
	write_command(SSD1306_PRECHARGE);
	write_command(0xF1);
	
	write_command(SSD1306_COM_PINS_HW);
	write_command(0x12);
	
	write_command(SSD1306_VCOMH_LEVEL);
	write_command(0x40);
	
	write_command(SSD1306_MEMORY_MODE);
	write_command(0x10);
	
	write_command(SSD1306_CHRG_PUMP);
	write_command(0x14);
	
	write_command(SSD1306_RAM_ON);
	
	write_command(SSD1306_NORMAL);
	write_command(SSD1306_ON);

	write_command(SSD1306_COL_ADDRESS);
	write_command(0); 
	write_command(LCD_WIDTH - 1);

	write_command(SSD1306_PAGE_ADDRESS);
	write_command(0);	
	write_command(7);

	oled_update_all();

}
