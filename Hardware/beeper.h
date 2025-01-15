#ifndef __BEEPER_H
#define __BEEPER_H

#include "stm32f10x.h"

// 蜂鸣器状态定义
#define BEEPER_ON  1
#define BEEPER_OFF 0

// 函数声明
void Beeper_Init(void);
void Beeper_On(void);
void Beeper_Off(void);
void Beeper_Toggle(void);
void Beeper_Beep(uint16_t duration_ms);

#endif
