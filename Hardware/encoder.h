#ifndef __ENCODER_H
#define __ENCODER_H

#include "stm32f10x.h"
#include "pin_config.h"

// 编码器状态定义
typedef enum {
    ENCODER_IDLE = 0,    // 空闲状态
    ENCODER_CW,          // 顺时针旋转
    ENCODER_CCW,         // 逆时针旋转
    ENCODER_PUSH         // 按下按钮
} EncoderState;

// 编码器计数结构体
typedef struct {
    int16_t count;       // 编码器计数值
    int16_t lastCount;   // 上次的计数值
    uint8_t buttonState; // 按钮状态
} EncoderTypeDef;

// 编码器函数
void Encoder_Init(void);                     // 初始化编码器
EncoderState Encoder_GetState(void);         // 获取编码器状态
int16_t Encoder_GetCount(void);             // 获取编码器计数值
uint8_t Encoder_GetButtonState(void);        // 获取按钮状态
void Encoder_SetCount(int16_t count);       // 设置编码器计数值
void Encoder_ClearCount(void);              // 清除编码器计数值

#endif
