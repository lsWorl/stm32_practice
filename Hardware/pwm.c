#include "pwm.h"
#include "stm32f10x.h"

/**
  * @brief  PWM初始化
  * @param  TIMx: 定时器端口
  * @param  Channel: PWM通道
  * @param  Period: 周期值
  * @param  Pulse: 脉冲宽度
  * @retval None
  */
void PWM_Init(TIM_TypeDef* TIMx, uint16_t Channel, uint16_t Period, uint16_t Pulse)
{
    GPIO_InitTypeDef GPIO_InitStruct;
    TIM_TimeBaseInitTypeDef TIM_TimeBaseStruct;
    TIM_OCInitTypeDef TIM_OCInitStruct;
    
    /* 使能GPIO和TIM时钟 */
    if(TIMx == TIM2) {
        RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2, ENABLE);
        RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);
        
        /* 配置TIM2通道引脚 */
        if(Channel == TIM_Channel_1) {
            GPIO_InitStruct.GPIO_Pin = GPIO_Pin_0;
        }
        else if(Channel == TIM_Channel_2) {
            GPIO_InitStruct.GPIO_Pin = GPIO_Pin_1;
        }
        else if(Channel == TIM_Channel_3) {
            GPIO_InitStruct.GPIO_Pin = GPIO_Pin_2;
        }
        else if(Channel == TIM_Channel_4) {
            GPIO_InitStruct.GPIO_Pin = GPIO_Pin_3;
        }
    }
    else if(TIMx == TIM3) {
        RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3, ENABLE);
        RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);
        
        /* 配置TIM3通道引脚 */
        if(Channel == TIM_Channel_1) {
            GPIO_InitStruct.GPIO_Pin = GPIO_Pin_6;
        }
        else if(Channel == TIM_Channel_2) {
            GPIO_InitStruct.GPIO_Pin = GPIO_Pin_7;
        }
        else if(Channel == TIM_Channel_3) {
            GPIO_InitStruct.GPIO_Pin = GPIO_Pin_0;  // TIM3_CH3对应PB0
        }
        else if(Channel == TIM_Channel_4) {
            GPIO_InitStruct.GPIO_Pin = GPIO_Pin_1;  // TIM3_CH4对应PB1
        }
    }
    
    GPIO_InitStruct.GPIO_Mode = GPIO_Mode_AF_PP;
    GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(GPIOA, &GPIO_InitStruct);
    
    /* 定时器基本配置 */
    TIM_TimeBaseStruct.TIM_Period = Period - 1;
    TIM_TimeBaseStruct.TIM_Prescaler = 72 - 1;  // 72MHz/72 = 1MHz
    TIM_TimeBaseStruct.TIM_ClockDivision = 0;
    TIM_TimeBaseStruct.TIM_CounterMode = TIM_CounterMode_Up;
    TIM_TimeBaseInit(TIMx, &TIM_TimeBaseStruct);
    
    /* PWM模式配置 */
    TIM_OCInitStruct.TIM_OCMode = TIM_OCMode_PWM1;
    TIM_OCInitStruct.TIM_OutputState = TIM_OutputState_Enable;
    TIM_OCInitStruct.TIM_Pulse = Pulse;
    TIM_OCInitStruct.TIM_OCPolarity = TIM_OCPolarity_High;
    
    switch(Channel) {
        case TIM_Channel_1:
            TIM_OC1Init(TIMx, &TIM_OCInitStruct);
            TIM_OC1PreloadConfig(TIMx, TIM_OCPreload_Enable);
            break;
        case TIM_Channel_2:
            TIM_OC2Init(TIMx, &TIM_OCInitStruct);
            TIM_OC2PreloadConfig(TIMx, TIM_OCPreload_Enable);
            break;
        case TIM_Channel_3:
            TIM_OC3Init(TIMx, &TIM_OCInitStruct);
            TIM_OC3PreloadConfig(TIMx, TIM_OCPreload_Enable);
            break;
        case TIM_Channel_4:
            TIM_OC4Init(TIMx, &TIM_OCInitStruct);
            TIM_OC4PreloadConfig(TIMx, TIM_OCPreload_Enable);
            break;
    }
    
    /* 使能定时器 */
    TIM_Cmd(TIMx, ENABLE);
    TIM_CtrlPWMOutputs(TIMx, ENABLE);
}

/**
  * @brief  设置PWM占空比
  * @param  TIMx: 定时器端口
  * @param  Channel: PWM通道
  * @param  Pulse: 脉冲宽度
  * @retval None
  */
void PWM_SetDuty(TIM_TypeDef* TIMx, uint16_t Channel, uint16_t Pulse)
{
    switch(Channel) {
        case TIM_Channel_1:
            TIMx->CCR1 = Pulse;
            break;
        case TIM_Channel_2:
            TIMx->CCR2 = Pulse;
            break;
        case TIM_Channel_3:
            TIMx->CCR3 = Pulse;
            break;
        case TIM_Channel_4:
            TIMx->CCR4 = Pulse;
            break;
    }
}
