#ifndef __BEEPER_H
#define __BEEPER_H

#include "stm32f10x.h"
#include "pin_config.h"

// 蜂鸣器控制函数
void Beeper_Init(void);        // 初始化蜂鸣器
void Beeper_On(void);          // 打开蜂鸣器
void Beeper_Off(void);         // 关闭蜂鸣器
void Beeper_Toggle(void);      // 切换蜂鸣器状态
void Beeper_Beep(uint16_t ms); // 蜂鸣指定时长（毫秒）

#endif
