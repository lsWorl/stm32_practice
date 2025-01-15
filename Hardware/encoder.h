#ifndef __ENCODER_H
#define __ENCODER_H

#include "stm32f10x.h"

void ENCODER_Init(void);
int32_t ENCODER_GetCount(void);

#endif
