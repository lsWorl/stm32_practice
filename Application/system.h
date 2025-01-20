#ifndef __SYSTEM_H
#define __SYSTEM_H

#include "stm32f10x.h"
#include "led.h"
#include "OLED.h"
#include "beeper.h"
#include "light_sensor.h"
#include "temp_sensor.h"
#include "mpu6050.h"
#include "encoder.h"
#include "servo.h"
#include "button.h"
// 系统工作模式定义
typedef enum {
    SYS_MODE_MONITOR = 0,    // 监测模式：显示所有传感器数据
    SYS_MODE_SETTING,        // 设置模式：调整参数
    SYS_MODE_DEBUG           // 调试模式：测试单个模块
} SystemMode;

// 系统状态定义
typedef struct {
    SystemMode mode;         // 当前工作模式
    uint8_t isAlarming;     // 是否处于报警状态
    uint8_t isInitialized;  // 是否已初始化
    
    // 传感器数据
    float temperature;      // 温度值
    uint8_t lightLevel;     // 光照等级
    MPU6050_Attitude attitude;  // 姿态数据
    
    // 设置参数
    float tempThreshold;    // 温度报警阈值
    uint8_t servoAngle;     // 舵机角度
} SystemState;

// 系统函数
void System_Init(void);                    // 系统初始化
void System_Process(void);                 // 系统主处理函数
void System_SetMode(SystemMode mode);      // 设置系统模式
void System_UpdateDisplay(void);           // 更新显示
void System_CheckAlarm(void);             // 检查报警条件
void System_HandleInput(void);             // 处理用户输入
void Servo_Running(void);                   // 超过阈值，则转动舵机
// 获取系统状态
SystemState* System_GetState(void);

#endif
