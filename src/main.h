#ifndef _MAIN_H_
#define _MAIN_H_

#include "stm32h7xx_hal.h"

#define CONFIG_DCACHE

#define MIN(a,b) (((a)<(b))?(a):(b))
#define MAX(a,b) (((a)>(b))?(a):(b))

void Error_Handler(void);

#endif 
