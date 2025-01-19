#ifndef __SERVO_H
#define __SERVO_H

#include "stm32f10x.h"
#include "pin_config.h"

// 舵机参数定义
#define SERVO_PERIOD     20000   // PWM周期（单位：us）
#define SERVO_MIN_PULSE  500     // 最小脉冲宽度（0度位置）
#define SERVO_MAX_PULSE  2500    // 最大脉冲宽度（180度位置）
#define SERVO_MID_PULSE  1500    // 中间脉冲宽度（90度位置）

// 舵机角度范围
#define SERVO_MIN_ANGLE  0       // 最小角度
#define SERVO_MAX_ANGLE  180     // 最大角度

// 舵机函数
void Servo_Init(void);                   // 初始化舵机
void Servo_SetAngle(uint8_t angle);      // 设置舵机角度（0-180度）
void Servo_SetPulse(uint16_t pulse);     // 设置舵机脉冲宽度（500-2500us）
uint8_t Servo_GetAngle(void);            // 获取当前舵机角度
void Servo_Reset(void);                  // 复位舵机到中间位置（90度）

#endif
