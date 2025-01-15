#include "gpio.h"
#include "stm32f10x.h"

/**
  * @brief  GPIO��ʼ��
  * @param  GPIOx: GPIO�˿�
  * @param  GPIO_Pin: GPIO����
  * @param  GPIO_Mode: GPIOģʽ
  * @param  GPIO_Speed: GPIO�ٶ�
  * @retval None
  */
void GPIO_Init_Pin(GPIO_TypeDef* GPIOx, uint16_t GPIO_Pin, GPIOMode_TypeDef GPIO_Mode, GPIOSpeed_TypeDef GPIO_Speed)
{
    GPIO_InitTypeDef GPIO_InitStruct;
    
    /* ʹ��GPIOʱ�� */
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
    
    /* ����GPIO */
    GPIO_InitStruct.GPIO_Pin = GPIO_Pin;
    GPIO_InitStruct.GPIO_Mode = GPIO_Mode;
    GPIO_InitStruct.GPIO_Speed = GPIO_Speed;
    GPIO_Init(GPIOx, &GPIO_InitStruct);
}

/**
  * @brief  ����GPIO����״̬
  * @param  GPIOx: GPIO�˿�
  * @param  GPIO_Pin: GPIO����
  * @param  PinState: ����״̬
  * @retval None
  */
void GPIO_Set_Pin(GPIO_TypeDef* GPIOx, uint16_t GPIO_Pin, uint8_t PinState)
{
    if(PinState)
        GPIOx->BSRR = GPIO_Pin;   //��������Ϊ�ߵ�ƽ
    else
        GPIOx->BRR = GPIO_Pin;    //��������Ϊ�͵�ƽ
}

/**
  * @brief  ��ȡGPIO����״̬
  * @param  GPIOx: GPIO�˿�
  * @param  GPIO_Pin: GPIO����
  * @retval ����״̬
  */
uint8_t GPIO_Read_Pin(GPIO_TypeDef* GPIOx, uint16_t GPIO_Pin)
{
    return (GPIOx->IDR & GPIO_Pin) ? 1 : 0;
}

/**
  * @brief  GPIO���ŷ�ת
  * @param  GPIOx: GPIO�˿�
  * @param  GPIO_Pin: GPIO����
  * @retval None
  */
void GPIO_Toggle_Pin(GPIO_TypeDef* GPIOx, uint16_t GPIO_Pin)
{
    GPIOx->ODR ^= GPIO_Pin;
}
