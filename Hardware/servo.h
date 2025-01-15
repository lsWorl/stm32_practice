#ifndef __SERVO_H
#define __SERVO_H

#include "stm32f10x.h"

void Servo_Init(TIM_TypeDef* TIMx, uint16_t Channel);
void Servo_SetAngle(TIM_TypeDef* TIMx, uint16_t Channel, uint8_t angle);

#endif
