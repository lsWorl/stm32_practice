#include "servo.h"

// 当前舵机角度
static uint8_t currentAngle = 90;

/**
  * @brief  初始化舵机
  * @param  None
  * @retval None
  */
void Servo_Init(void)
{
    GPIO_InitTypeDef GPIO_InitStructure;
    TIM_TimeBaseInitTypeDef TIM_TimeBaseStructure;
    TIM_OCInitTypeDef TIM_OCInitStructure;
    
    // 使能时钟
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA | RCC_APB2Periph_TIM1, ENABLE);
    
    // 配置GPIO
    GPIO_InitStructure.GPIO_Pin = SERVO_PWM_PIN;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;  // 复用推挽输出
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(SERVO_PORT, &GPIO_InitStructure);
    
    // 配置时基结构体
    // PWM频率 = 72M/(Period+1)/(Prescaler+1)
    // 72000000/20000/72 = 50Hz 1/50 = 20ms
    TIM_TimeBaseStructure.TIM_Period = SERVO_PERIOD - 1;  // 20ms周期
    TIM_TimeBaseStructure.TIM_Prescaler = 72 - 1;         // 72分频
    TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1;
    TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;
    TIM_TimeBaseStructure.TIM_RepetitionCounter = 0;  // ARR寄存器
    TIM_TimeBaseInit(TIM1, &TIM_TimeBaseStructure);
    
    // 配置PWM模式
    TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1;
    TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;
    TIM_OCInitStructure.TIM_OutputNState = TIM_OutputNState_Disable;  // 互补输出被禁用
    TIM_OCInitStructure.TIM_Pulse = SERVO_MID_PULSE;  // 初始化到中间位置
    TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High; 
    TIM_OCInitStructure.TIM_OCNPolarity = TIM_OCNPolarity_Low; //由于互补输出被禁用，此配置在舵机控制中不会生效
    TIM_OCInitStructure.TIM_OCIdleState = TIM_OCIdleState_Set;  //配置主输出通道（TIM1_CH1） 空闲状态时输出高电平
    TIM_OCInitStructure.TIM_OCNIdleState = TIM_OCNIdleState_Reset; //配置 互补输出通道 空闲状态
    TIM_OC1Init(TIM1, &TIM_OCInitStructure);
    
    // 使能预装载
    TIM_OC1PreloadConfig(TIM1, TIM_OCPreload_Enable);
    TIM_ARRPreloadConfig(TIM1, ENABLE);
    
    // 使能TIM1
    TIM_Cmd(TIM1, ENABLE);
    TIM_CtrlPWMOutputs(TIM1, ENABLE); //用于控制是否启用高级定时器的PWM输出
    
    // 复位到中间位置
    Servo_Reset();
}

/**
  * @brief  设置舵机角度
  * @param  angle: 目标角度（0-180度）
  * @retval None
  */
void Servo_SetAngle(uint8_t angle)
{
    uint16_t pulse;
    
    // 限制角度范围
    if(angle > SERVO_MAX_ANGLE)
        angle = SERVO_MAX_ANGLE;
    
    // 角度转换为脉冲宽度
    // pulse = 500 + (angle/180) * 2000
    pulse = SERVO_MIN_PULSE + ((uint32_t)angle * (SERVO_MAX_PULSE - SERVO_MIN_PULSE)) / SERVO_MAX_ANGLE;
    
    // 设置PWM占空比
    TIM_SetCompare1(TIM1, pulse);
    
    // 更新当前角度
    currentAngle = angle;
}

/**
  * @brief  设置舵机脉冲宽度
  * @param  pulse: 脉冲宽度（500-2500us）
  * @retval None
  */
void Servo_SetPulse(uint16_t pulse)
{
    // 限制脉冲宽度范围
    if(pulse < SERVO_MIN_PULSE)
        pulse = SERVO_MIN_PULSE;
    else if(pulse > SERVO_MAX_PULSE)
        pulse = SERVO_MAX_PULSE;
    
    // 设置PWM占空比
    TIM_SetCompare1(TIM1, pulse);
    
    // 更新当前角度
    currentAngle = ((pulse - SERVO_MIN_PULSE) * SERVO_MAX_ANGLE) / (SERVO_MAX_PULSE - SERVO_MIN_PULSE);
}

/**
  * @brief  获取当前舵机角度
  * @param  None
  * @retval uint8_t: 当前角度（0-180度）
  */
uint8_t Servo_GetAngle(void)
{
    return currentAngle;
}

/**
  * @brief  复位舵机到中间位置
  * @param  None
  * @retval None
  */
void Servo_Reset(void)
{
    Servo_SetAngle(90);  // 复位到90度位置
}
