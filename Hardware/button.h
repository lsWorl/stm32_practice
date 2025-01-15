#ifndef __BUTTON_H
#define __BUTTON_H

#include "stm32f10x.h"

// 按钮编号定义
#define BUTTON1 1
#define BUTTON2 2
#define BUTTON3 3
#define BUTTON4 4

// 按钮状态定义
#define BUTTON_PRESSED  1
#define BUTTON_RELEASED 0

// 函数声明
void Button_Init(void);
uint8_t Button_GetState(uint8_t button);

#endif
