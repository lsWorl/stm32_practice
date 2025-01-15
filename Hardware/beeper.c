#include "beeper.h"
#include "stm32f10x_gpio.h"
#include "stm32f10x_rcc.h"
#include "pin_config.h"

void Beeper_Init(void)
{
    GPIO_InitTypeDef GPIO_InitStructure;
    
    // 使能GPIOB时钟
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);
    
    // 配置PB8为推挽输出
    GPIO_InitStructure.GPIO_Pin = BEEPER_PIN;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(BEEPER_PORT, &GPIO_InitStructure);
    
    // 初始状态关闭蜂鸣器
    GPIO_ResetBits(BEEPER_PORT, BEEPER_PIN);
}

void Beeper_On(void)
{
    GPIO_SetBits(BEEPER_PORT, BEEPER_PIN);
}

void Beeper_Off(void)
{
    GPIO_ResetBits(BEEPER_PORT, BEEPER_PIN);
}

void Beeper_Toggle(void)
{
    GPIO_WriteBit(BEEPER_PORT, BEEPER_PIN, 
        (BitAction)(1 - GPIO_ReadOutputDataBit(BEEPER_PORT, BEEPER_PIN)));
}

void Delay_ms(uint16_t ms)
{
    volatile uint32_t i, j;
    
    for(i = 0; i < ms; i++)
        for(j = 0; j < 1141; j++);
}

void Beeper_Beep(uint16_t duration_ms)
{
    Beeper_On();
    Delay_ms(duration_ms);
    Beeper_Off();
}
