#ifndef __ADC_H
#define __ADC_H

#include "stm32f10x.h"

void ADC_Init(void);
uint16_t ADC_GetValue(uint8_t channel);

#endif
