#ifndef __BUTTON_H
#define __BUTTON_H

#include "stm32f10x.h"
#include "pin_config.h"

// 按键逻辑状态定义
typedef enum {
    BTN_IDLE = 0,       // 空闲状态
    BTN_DEBOUNCE,       // 消抖状态
    BTN_SHORT_PRESS,    // 短按状态
    BTN_LONG_PRESS      // 长按状态
} ButtonState;

// 按键编号定义
typedef enum {
    KEY_MODE = 0,   // 模式切换按键 (BTN1)
    KEY_CONFIRM,    // 确认按键 (BTN2)
    KEY_ALARM,      // 报警确认按键 (BTN3)
    KEY_RESET,      // 复位按键 (BTN4)
    KEY_COUNT       // 按键总数
} KeyID;

// 按键初始化
void Button_Init(void);

// 按键扫描函数（在主循环中调用）
void Button_Scan(void);

// 获取按键状态
ButtonState Button_GetState(KeyID key);

// 清除按键状态
void Button_ClearState(KeyID key);

#endif
