#include "servo.h"
#include "pwm.h"
#include "stm32f10x.h"

#define SERVO_MIN_PULSE 500   // 0.5ms
#define SERVO_MAX_PULSE 2500  // 2.5ms
#define SERVO_PERIOD 20000    // 20ms

/**
  * @brief  舵机初始化
  * @param  TIMx: 定时器端口
  * @param  Channel: PWM通道
  * @retval None
  */
void Servo_Init(TIM_TypeDef* TIMx, uint16_t Channel)
{
    /* 初始化PWM周期为20ms */
    PWM_Init(TIMx, Channel, SERVO_PERIOD, SERVO_MIN_PULSE);
}

/**
  * @brief  设置舵机角度
  * @param  TIMx: 定时器端口
  * @param  Channel: PWM通道
  * @param  angle: 角度值 (0-180)
  * @retval None
  */
void Servo_SetAngle(TIM_TypeDef* TIMx, uint16_t Channel, uint8_t angle)
{
    uint16_t pulse;
    
    /* 限制角度范围 */
    if(angle > 180) angle = 180;
    
    /* 计算脉冲 */
    pulse = SERVO_MIN_PULSE + (angle * (SERVO_MAX_PULSE - SERVO_MIN_PULSE)) / 180;
    
    /* 设置PWM脉宽 */
    PWM_SetDuty(TIMx, Channel, pulse);
}
