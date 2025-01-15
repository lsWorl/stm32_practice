#ifndef __PWM_H
#define __PWM_H

#include "stm32f10x.h"

void PWM_Init(TIM_TypeDef* TIMx, uint16_t Channel, uint16_t Period, uint16_t Pulse);
void PWM_SetDuty(TIM_TypeDef* TIMx, uint16_t Channel, uint16_t Pulse);

#endif
