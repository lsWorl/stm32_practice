#ifndef __DELAY_H
#define __DELAY_H

#include "stm32f10x.h"

void delay_init(void);
void delay_ms(uint16_t ms);
void delay_us(uint32_t us);

#endif 
