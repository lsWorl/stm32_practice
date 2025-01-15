#include "led.h"
#include "stm32f10x_gpio.h"
#include "stm32f10x_rcc.h"
#include "pin_config.h"

void LED_Init(void)
{
    GPIO_InitTypeDef GPIO_InitStructure;
    
    // 使能GPIOB时钟
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);
    
    // 配置LED引脚为推挽输出
    GPIO_InitStructure.GPIO_Pin = LED1_PIN | LED2_PIN | LED3_PIN | LED4_PIN;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(LED_PORT, &GPIO_InitStructure);
    
    // 初始化时关闭所有LED，即输出高电平
    GPIO_SetBits(LED_PORT, LED1_PIN | LED2_PIN | LED3_PIN | LED4_PIN);
}

void LED_SetState(uint8_t led, uint8_t state)
{
    uint16_t pin = 0;
    
    switch(led) {
        case 1: pin = LED1_PIN; break;
        case 2: pin = LED2_PIN; break;
        case 3: pin = LED3_PIN; break;
        case 4: pin = LED4_PIN; break;
        default: return;
    }
    
    if(state) {
        GPIO_SetBits(LED_PORT, pin);
    } else {
        GPIO_ResetBits(LED_PORT, pin);
    }
}

void LED_Toggle(uint8_t led)
{
    uint16_t pin = 0;
    
    switch(led) {
        case 1: pin = LED1_PIN; break;
        case 2: pin = LED2_PIN; break;
        case 3: pin = LED3_PIN; break;
        case 4: pin = LED4_PIN; break;
        default: return;
    }
    
    if(GPIO_ReadOutputDataBit(LED_PORT, pin)) {
        GPIO_ResetBits(LED_PORT, pin);
    } else {
        GPIO_SetBits(LED_PORT, pin);
    }
}
