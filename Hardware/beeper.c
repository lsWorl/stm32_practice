#include "beeper.h"
#include "delay.h"

/**
  * @brief  初始化蜂鸣器GPIO
  * @param  None
  * @retval None
  */
void Beeper_Init(void)
{
    GPIO_InitTypeDef GPIO_InitStructure;
    
    // 使能GPIOA时钟
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);
    
    // 配置蜂鸣器引脚为推挽输出
    GPIO_InitStructure.GPIO_Pin = BEEPER_PIN;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(BEEPER_PORT, &GPIO_InitStructure);
    
    // 初始状态为关闭
    Beeper_Off();
}

/**
  * @brief  打开蜂鸣器
  * @param  None
  * @retval None
  */
void Beeper_On(void)
{
    GPIO_ResetBits(BEEPER_PORT, BEEPER_PIN);
}

/**
  * @brief  关闭蜂鸣器
  * @param  None
  * @retval None
  */
void Beeper_Off(void)
{
    GPIO_SetBits(BEEPER_PORT, BEEPER_PIN);
}

/**
  * @brief  切换蜂鸣器状态
  * @param  None
  * @retval None
  */
void Beeper_Toggle(void)
{
    BEEPER_PORT->ODR ^= BEEPER_PIN;
}

/**
  * @brief  蜂鸣器鸣叫指定时长
  * @param  ms: 鸣叫时长(毫秒)
  * @retval None
  */
void Beeper_Beep(uint16_t ms)
{
    Beeper_On();
    delay_ms(ms);
    Beeper_Off();
}
