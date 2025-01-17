#ifndef __LED_H
#define __LED_H

#include "stm32f10x.h"
#include "pin_config.h"

// LED控制函数
void LED_Init(void);
void LED_On(uint16_t LED_PIN);
void LED_Off(uint16_t LED_PIN);
void LED_Toggle(uint16_t LED_PIN);

// LED状态控制
void LED_AllOn(void);
void LED_AllOff(void);
void LED_SystemRunning(void);    // LED1指示系统运行
void LED_DataCollecting(void);   // LED2指示数据采集
void LED_SettingMode(void);      // LED3指示设置模式
void LED_AlarmStatus(void);      // LED4指示报警状态

#endif
