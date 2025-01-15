#include "button.h"
#include "stm32f10x_gpio.h"
#include "stm32f10x_rcc.h"
#include "pin_config.h"

void Button_Init(void)
{
    GPIO_InitTypeDef GPIO_InitStructure;
    
    // 使能GPIOB时钟
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);
    
    // 配置按钮引脚为上拉输入
    GPIO_InitStructure.GPIO_Pin = BTN1_PIN | BTN2_PIN | BTN3_PIN | BTN4_PIN;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(BTN_PORT, &GPIO_InitStructure);
}

uint8_t Button_GetState(uint8_t button)
{
    uint16_t pin = 0;
    
    switch(button) {
        case 1: pin = BTN1_PIN; break;
        case 2: pin = BTN2_PIN; break;
        case 3: pin = BTN3_PIN; break;
        case 4: pin = BTN4_PIN; break;
        default: return 0;
    }
    
    return GPIO_ReadInputDataBit(BTN_PORT, pin) == Bit_RESET;
}
