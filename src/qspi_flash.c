#include "main.h"
#include "qspi_flash.h"
#include "uart.h"

#include <stdlib.h>
#include <ctype.h>
#include <string.h>

#define PAGE_SIZE       	   256

#define QSPI_FLASH_SIZE            21 // 2MBytes

#define CMD_PAGE_PROGRAM	  0x02
#define CMD_READ		  0x03
#define CMD_FAST_READ		  0x0B
#define CMD_WRITE_DISABLE	  0x04
#define CMD_READ_STATUS_REG	  0x05
#define CMD_WRITE_ENABLE	  0x06
#define CMD_PROGRAM_EXECUTE       0x10
#define CMD_PAGE_DATA_READ	  0x13
#define CMD_WRITE_STATUS_REG      0x1F
#define CMD_FAST_READ_DUAL_OUTPUT 0x3B
#define CMD_LOAD_PROGRAM_DATA     0x84
#define CMD_READ_ID		  0x9F
#define CMD_SECTOR_ERASE          0x20
#define CMD_BLOCK_ERASE           0xD8
#define CMD_FAST_READ_DUAL_IO     0xBB
#define CMD_QUAD_INOUT_FAST_READ  0xEB
#define CMD_RESET 		  0xFF

#define DBG if(0)

static QSPI_HandleTypeDef hqspi;

#ifdef __cplusplus
extern "C"
#endif
void QUADSPI_IRQHandler(void)
{
xprintf("q!");
	HAL_QSPI_IRQHandler(&hqspi);
}

void HAL_QSPI_RxCpltCallback(QSPI_HandleTypeDef *hqspi)
{
xprintf("r!");
}

void HAL_QSPI_TxCpltCallback(QSPI_HandleTypeDef *hqspi)
{
xprintf("t!");
}

void HAL_QSPI_CmdCpltCallback(QSPI_HandleTypeDef *hqspi)
{
xprintf("c!");
}

void HAL_QSPI_StatusMatchCallback(QSPI_HandleTypeDef *hqspi)
{
xprintf("s!");
}

static uint32_t read_id(void)
{
	QSPI_CommandTypeDef command;

	command.Instruction	  = CMD_READ_ID;
	command.InstructionMode   = QSPI_INSTRUCTION_1_LINE;
	command.AddressMode	  = QSPI_ADDRESS_NONE;
	command.AlternateByteMode = QSPI_ALTERNATE_BYTES_NONE;
	command.DataMode	  = QSPI_DATA_1_LINE;
	command.NbData            = 3;
	command.DummyCycles	  = 0;
	command.DdrMode	          = QSPI_DDR_MODE_DISABLE;
	command.DdrHoldHalfCycle  = QSPI_DDR_HHC_ANALOG_DELAY;
	command.SIOOMode	  = QSPI_SIOO_INST_EVERY_CMD;

	if (HAL_QSPI_Command(&hqspi, &command, HAL_QPSI_TIMEOUT_DEFAULT_VALUE) != HAL_OK) {
		Error_Handler();
	}

	uint8_t id[3];
	if (HAL_QSPI_Receive(&hqspi, id, HAL_QPSI_TIMEOUT_DEFAULT_VALUE) != HAL_OK) {
		Error_Handler();
	}

//	xprintf("%02X %02X %02X\n", id[0], id[1], id[2]);
	
	return (id[0] << 16) | (id[1] << 8) | id[2];
}

static uint8_t sr_cmd[3] = { 0x05, 0x35, 0x15 };

static uint8_t read_status(int num)
{
	QSPI_CommandTypeDef command;

	command.Instruction	  = sr_cmd[num];
	command.InstructionMode   = QSPI_INSTRUCTION_1_LINE;
	command.AddressMode	  = QSPI_ADDRESS_NONE;
	command.AlternateByteMode = QSPI_ALTERNATE_BYTES_NONE;
	command.DataMode	  = QSPI_DATA_1_LINE;
	command.NbData            = 1;
	command.DummyCycles	  = 0;
	command.DdrMode           = QSPI_DDR_MODE_DISABLE;
	command.DdrHoldHalfCycle  = QSPI_DDR_HHC_ANALOG_DELAY;
	command.SIOOMode	  = QSPI_SIOO_INST_EVERY_CMD;

	if (HAL_QSPI_Command(&hqspi, &command, HAL_QPSI_TIMEOUT_DEFAULT_VALUE) != HAL_OK) {
		Error_Handler();
	}

	uint8_t reg;
	if (HAL_QSPI_Receive(&hqspi, &reg, HAL_QPSI_TIMEOUT_DEFAULT_VALUE) != HAL_OK) {
		Error_Handler();
	}
	return reg;	
}

static void wait_status_busy(void)
{
	while( read_status( 0 ) & 0x01 ) {
//xprintf(".");	
	}
}

static void dump_status(void) 
{
xprintf("[st %02X  %02X  %02X]\n", read_status(0), read_status(1), read_status(2) );
}

static void write_enable(void)
{
DBG xprintf("write_enable\n");
	QSPI_CommandTypeDef command;

	command.Instruction       = CMD_WRITE_ENABLE;
	command.InstructionMode   = QSPI_INSTRUCTION_1_LINE;
	command.AddressMode       = QSPI_ADDRESS_NONE;
	command.AlternateByteMode = QSPI_ALTERNATE_BYTES_NONE;
	command.DataMode          = QSPI_DATA_NONE;
	command.DummyCycles       = 0;
	command.DdrMode           = QSPI_DDR_MODE_DISABLE;
	command.DdrHoldHalfCycle  = QSPI_DDR_HHC_ANALOG_DELAY;
	command.SIOOMode          = QSPI_SIOO_INST_EVERY_CMD;

	if (HAL_QSPI_Command(&hqspi, &command, HAL_QPSI_TIMEOUT_DEFAULT_VALUE) != HAL_OK) {
		Error_Handler();
	}
}

static void write_disable(void)
{
xprintf("write_disable\n");
	QSPI_CommandTypeDef command;

	command.Instruction       = CMD_WRITE_DISABLE;
	command.InstructionMode   = QSPI_INSTRUCTION_1_LINE;
	command.AddressMode       = QSPI_ADDRESS_NONE;
	command.AlternateByteMode = QSPI_ALTERNATE_BYTES_NONE;
	command.DataMode          = QSPI_DATA_NONE;
	command.DummyCycles       = 0;
	command.DdrMode           = QSPI_DDR_MODE_DISABLE;
	command.DdrHoldHalfCycle  = QSPI_DDR_HHC_ANALOG_DELAY;
	command.SIOOMode          = QSPI_SIOO_INST_EVERY_CMD;

	if (HAL_QSPI_Command(&hqspi, &command, HAL_QPSI_TIMEOUT_DEFAULT_VALUE) != HAL_OK) {
		Error_Handler();
	}
}

static uint32_t block_erase(uint32_t address)
{
DBG xprintf("block_erase %08X\n", address);
	QSPI_CommandTypeDef command;

	command.Instruction	  = CMD_BLOCK_ERASE;
	command.InstructionMode   = QSPI_INSTRUCTION_1_LINE;
	command.AddressSize       = QSPI_ADDRESS_24_BITS;
	command.AddressMode	  = QSPI_ADDRESS_1_LINE;

	command.Address           = address & 0x00FFFFFF;

	command.AlternateByteMode = QSPI_ALTERNATE_BYTES_NONE;
	command.DataMode	  = QSPI_DATA_NONE;
	command.NbData            = 0;
	command.DummyCycles	  = 0;
	command.DdrMode	          = QSPI_DDR_MODE_DISABLE;
	command.DdrHoldHalfCycle  = QSPI_DDR_HHC_ANALOG_DELAY;
	command.SIOOMode	  = QSPI_SIOO_INST_EVERY_CMD;


	if (HAL_QSPI_Command(&hqspi, &command, HAL_QPSI_TIMEOUT_DEFAULT_VALUE) != HAL_OK) {
		Error_Handler();
	}

	wait_status_busy();

DBG xprintf("done\n");

	return 0;
}

static uint32_t sector_erase(uint32_t address)
{
DBG xprintf("sector_erase %08X\n", address);
	QSPI_CommandTypeDef command;

	command.Instruction	  = CMD_SECTOR_ERASE;
	command.InstructionMode   = QSPI_INSTRUCTION_1_LINE;
	command.AddressSize       = QSPI_ADDRESS_24_BITS;
	command.AddressMode	  = QSPI_ADDRESS_1_LINE;

	command.Address           = address & 0x00FFFFFF;

	command.AlternateByteMode = QSPI_ALTERNATE_BYTES_NONE;
	command.DataMode	  = QSPI_DATA_NONE;
	command.NbData            = 0;
	command.DummyCycles	  = 0;
	command.DdrMode	          = QSPI_DDR_MODE_DISABLE;
	command.DdrHoldHalfCycle  = QSPI_DDR_HHC_ANALOG_DELAY;
	command.SIOOMode	  = QSPI_SIOO_INST_EVERY_CMD;

	if (HAL_QSPI_Command(&hqspi, &command, HAL_QPSI_TIMEOUT_DEFAULT_VALUE) != HAL_OK) {
		Error_Handler();
	}

	wait_status_busy();

DBG xprintf("done\n");

	return 0;
}

static void reset(void)
{
	QSPI_CommandTypeDef command;

	command.Instruction	  = CMD_RESET;
	command.InstructionMode   = QSPI_INSTRUCTION_1_LINE;
	command.AddressMode	  = QSPI_ADDRESS_NONE;
	command.AlternateByteMode = QSPI_ALTERNATE_BYTES_NONE;
	command.DataMode	  = QSPI_DATA_NONE;
	command.DummyCycles	  = 0;
	command.DdrMode	          = QSPI_DDR_MODE_DISABLE;
	command.DdrHoldHalfCycle  = QSPI_DDR_HHC_ANALOG_DELAY;
	command.SIOOMode	  = QSPI_SIOO_INST_EVERY_CMD;

	if (HAL_QSPI_Command(&hqspi, &command, HAL_QPSI_TIMEOUT_DEFAULT_VALUE) != HAL_OK)
	{
		Error_Handler();
	}
}

static void read_data_one_line(uint32_t addr, uint8_t *buf, int size)
{
	QSPI_CommandTypeDef s_command;

	s_command.InstructionMode   = QSPI_INSTRUCTION_1_LINE;
	s_command.Instruction	    = CMD_READ;
	s_command.AddressMode	    = QSPI_ADDRESS_1_LINE;
	s_command.AddressSize	    = QSPI_ADDRESS_24_BITS;
	s_command.AlternateByteMode = QSPI_ALTERNATE_BYTES_NONE;
	s_command.DataMode	    = QSPI_DATA_1_LINE;
	s_command.DummyCycles	    = 0;
	s_command.DdrMode	    = QSPI_DDR_MODE_DISABLE;
	s_command.DdrHoldHalfCycle  = QSPI_DDR_HHC_ANALOG_DELAY;
	s_command.SIOOMode	    = QSPI_SIOO_INST_EVERY_CMD;

 	s_command.Address           = addr & 0x00ffffff;
	s_command.NbData            = size;

	if (HAL_QSPI_Command(&hqspi, &s_command, HAL_QPSI_TIMEOUT_DEFAULT_VALUE) != HAL_OK) {
		Error_Handler();
	}

	if (HAL_QSPI_Receive(&hqspi, buf, 1234) != HAL_OK) {
		Error_Handler();
	}
}

static void page_program(uint32_t addr, const uint8_t *buf)
{
DBG xprintf("page_program %08X\n", addr);
	QSPI_CommandTypeDef s_command;

	s_command.InstructionMode   = QSPI_INSTRUCTION_1_LINE;
	s_command.Instruction	    = CMD_PAGE_PROGRAM;
	s_command.AddressMode	    = QSPI_ADDRESS_1_LINE;
	s_command.AddressSize	    = QSPI_ADDRESS_24_BITS;
	s_command.AlternateByteMode = QSPI_ALTERNATE_BYTES_NONE;
	s_command.DataMode	    = QSPI_DATA_1_LINE;
	s_command.DummyCycles	    = 0;
	s_command.DdrMode	    = QSPI_DDR_MODE_DISABLE;
	s_command.DdrHoldHalfCycle  = QSPI_DDR_HHC_ANALOG_DELAY;
	s_command.SIOOMode	    = QSPI_SIOO_INST_EVERY_CMD;

 	s_command.Address           = addr & 0x00ffffff;
	s_command.NbData            = 256;

	if (HAL_QSPI_Command(&hqspi, &s_command, HAL_QPSI_TIMEOUT_DEFAULT_VALUE) != HAL_OK) {
		Error_Handler();
	}

	if (HAL_QSPI_Transmit(&hqspi, (uint8_t*)buf, 1234) != HAL_OK) {
		Error_Handler();
	}

	wait_status_busy();
DBG xprintf("done\n");
}

int qspi_flash_init(void)
{
xprintf("[QSPI] init\n");

	RCC_PeriphCLKInitTypeDef PeriphClkInitStruct = {0};
	PeriphClkInitStruct.PeriphClockSelection = RCC_PERIPHCLK_QSPI;
	PeriphClkInitStruct.QspiClockSelection   = RCC_QSPICLKSOURCE_D1HCLK;
	if (HAL_RCCEx_PeriphCLKConfig(&PeriphClkInitStruct) != HAL_OK) {
		Error_Handler();
	}

	__HAL_RCC_QSPI_CLK_ENABLE();

	__HAL_RCC_GPIOB_CLK_ENABLE();
	__HAL_RCC_GPIOD_CLK_ENABLE();
	__HAL_RCC_GPIOE_CLK_ENABLE();

	/**QUADSPI GPIO Configuration
	PE2     ------> QUADSPI_BK1_IO2
	PB2     ------> QUADSPI_CLK
	PB10     ------> QUADSPI_BK1_NCS
	PD11     ------> QUADSPI_BK1_IO0
	PD12     ------> QUADSPI_BK1_IO1
	PD13     ------> QUADSPI_BK1_IO3
	*/
	GPIO_InitTypeDef GPIO_InitStruct = {0};
	GPIO_InitStruct.Pin       = GPIO_PIN_2;
	GPIO_InitStruct.Mode      = GPIO_MODE_AF_PP;
	GPIO_InitStruct.Pull      = GPIO_NOPULL;
	GPIO_InitStruct.Speed     = GPIO_SPEED_FREQ_HIGH;
	GPIO_InitStruct.Alternate = GPIO_AF9_QUADSPI;
	HAL_GPIO_Init(GPIOE, &GPIO_InitStruct);

	GPIO_InitStruct.Pin       = GPIO_PIN_2|GPIO_PIN_10;
	HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

	GPIO_InitStruct.Pin       = GPIO_PIN_11|GPIO_PIN_12|GPIO_PIN_13;
	HAL_GPIO_Init(GPIOD, &GPIO_InitStruct);
    
	hqspi.Instance                = QUADSPI;
	hqspi.Init.ClockPrescaler     = 2; // 2 = 100MHz
	hqspi.Init.FifoThreshold      = 4;
	hqspi.Init.SampleShifting     = QSPI_SAMPLE_SHIFTING_HALFCYCLE;
	hqspi.Init.FlashSize	      = QSPI_FLASH_SIZE;
	hqspi.Init.ChipSelectHighTime = QSPI_CS_HIGH_TIME_1_CYCLE;
	hqspi.Init.ClockMode          = QSPI_CLOCK_MODE_0;
	hqspi.Init.FlashID            = QSPI_FLASH_ID_1;
	hqspi.Init.DualFlash          = QSPI_DUALFLASH_DISABLE;

        HAL_QSPI_DeInit(&hqspi);
        if (HAL_QSPI_Init(&hqspi) != HAL_OK) {
xprintf("[QSPI] cannot init HW!\n");
		return 1;
        }

	reset();

	uint32_t id = read_id();
xprintf("[QSPI] ID: %08X\n", id);

	return 0;
}

#define SECTOR 4096
#define PAGE   256

static void sector_write(uint32_t addr, const uint8_t *data )
{
	int size = SECTOR;
	
	while( size > 0 ) {
//xprintf("addr %06X\n", addr);
		write_enable();
		page_program( addr, data );
		addr += PAGE;
		data += PAGE;
		size -= PAGE;
	}
}

void qspi_write_data(uint32_t dst, const uint8_t *src, int size)
{
DBG xprintf("qspi_write %08X/%d > %06x\n", src, size, dst);
	uint8_t data  [SECTOR];
	uint8_t verify[SECTOR];

	while( size > 0 ) {
		int to_copy = MIN( size, SECTOR);
DBG xprintf("%08x -> %06x  %d\n", src, dst, to_copy);

		memset(data, 0, SECTOR );
		memcpy(data, src, to_copy );

#if 1
		write_enable();
		sector_erase( dst );
	
		write_enable();
		sector_write( dst, data );
#endif
		read_data_one_line( dst, verify, SECTOR );

		if( memcmp(data, verify, SECTOR) ) {
xprintf("error! %06X\n", dst);
//			while(1);
		}
	
		src  += SECTOR;
		dst  += SECTOR;
		size -= to_copy;
	}
}

void qspi_read_data(uint32_t addr, uint8_t *dst, int size)
{
	read_data_one_line(addr, dst, size);
}
