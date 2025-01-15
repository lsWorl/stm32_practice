#include "gpio.h"
#include "stm32f10x.h"

/**
  * @brief  GPIO初始化
  * @param  GPIOx: GPIO端口
  * @param  GPIO_Pin: GPIO引脚
  * @param  GPIO_Mode: GPIO模式
  * @param  GPIO_Speed: GPIO速度
  * @retval None
  */
void GPIO_Init_Pin(GPIO_TypeDef* GPIOx, uint16_t GPIO_Pin, GPIOMode_TypeDef GPIO_Mode, GPIOSpeed_TypeDef GPIO_Speed)
{
    GPIO_InitTypeDef GPIO_InitStruct;
    
    /* 使能GPIO时钟 */
    if(GPIOx == GPIOA)
        RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);
    else if(GPIOx == GPIOB)
        RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);
    else if(GPIOx == GPIOC)
        RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC, ENABLE);
    else if(GPIOx == GPIOD)
        RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOD, ENABLE);
    else if(GPIOx == GPIOE)
        RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOE, ENABLE);
    
    /* 配置GPIO */
    GPIO_InitStruct.GPIO_Pin = GPIO_Pin;
    GPIO_InitStruct.GPIO_Mode = GPIO_Mode;
    GPIO_InitStruct.GPIO_Speed = GPIO_Speed;
    GPIO_Init(GPIOx, &GPIO_InitStruct);
}

/**
  * @brief  设置GPIO引脚状态
  * @param  GPIOx: GPIO端口
  * @param  GPIO_Pin: GPIO引脚
  * @param  PinState: 引脚状态
  * @retval None
  */
void GPIO_Set_Pin(GPIO_TypeDef* GPIOx, uint16_t GPIO_Pin, uint8_t PinState)
{
    if(PinState)
        GPIOx->BSRR = GPIO_Pin;   //设置引脚为高电平
    else
        GPIOx->BRR = GPIO_Pin;    //设置引脚为低电平
}

/**
  * @brief  读取GPIO引脚状态
  * @param  GPIOx: GPIO端口
  * @param  GPIO_Pin: GPIO引脚
  * @retval 引脚状态
  */
uint8_t GPIO_Read_Pin(GPIO_TypeDef* GPIOx, uint16_t GPIO_Pin)
{
    return (GPIOx->IDR & GPIO_Pin) ? 1 : 0;
}

/**
  * @brief  GPIO引脚翻转
  * @param  GPIOx: GPIO端口
  * @param  GPIO_Pin: GPIO引脚
  * @retval None
  */
void GPIO_Toggle_Pin(GPIO_TypeDef* GPIOx, uint16_t GPIO_Pin)
{
    GPIOx->ODR ^= GPIO_Pin;
}
