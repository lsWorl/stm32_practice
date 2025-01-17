#ifndef __BUTTON_H
#define __BUTTON_H

#include "stm32f10x.h"
#include "pin_config.h"

// 按键状态定义
typedef enum {
    BTN_RELEASED = 0,    // 按键释放
    BTN_PRESSED,         // 按键按下
    BTN_SHORT_PRESSED,   // 短按
    BTN_LONG_PRESSED     // 长按
} ButtonState;

// 按键ID定义
typedef enum {
    BTN1 = 0,   // 模式切换按键
    BTN2,       // 参数确认按键
    BTN3,       // 报警确认按键
    BTN4,       // 系统复位按键
    BTN_COUNT   // 按键总数
} ButtonID;

// 按键初始化
void Button_Init(void);

// 按键扫描函数（在主循环中调用）
void Button_Scan(void);

// 获取按键状态
ButtonState Button_GetState(ButtonID btnId);

// 清除按键状态
void Button_ClearState(ButtonID btnId);

// 按键中断处理函数（在中断服务程序中调用）
void Button_EXTI_IRQHandler(ButtonID btnId);

#endif
