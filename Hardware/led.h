#ifndef __LED_H
#define __LED_H

#include "stm32f10x.h"

// LED编号定义
#define LED1 1
#define LED2 2
#define LED3 3
#define LED4 4

// LED状态定义
#define LED_ON  1
#define LED_OFF 0

// 函数声明
void LED_Init(void);
void LED_SetState(uint8_t led, uint8_t state);
void LED_Toggle(uint8_t led);

#endif
