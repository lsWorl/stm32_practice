#ifndef __GPIO_H
#define __GPIO_H

#include "stm32f10x.h"

/* GPIO���ų�ʼ�� */
void GPIO_Init_Pin(GPIO_TypeDef* GPIOx, uint16_t GPIO_Pin, GPIOMode_TypeDef GPIO_Mode, GPIOSpeed_TypeDef GPIO_Speed);

/* ����GPIO����״̬ */
void GPIO_Set_Pin(GPIO_TypeDef* GPIOx, uint16_t GPIO_Pin, uint8_t PinState);

/* ��ȡGPIO����״̬ */
uint8_t GPIO_Read_Pin(GPIO_TypeDef* GPIOx, uint16_t GPIO_Pin);

/* GPIO����״̬��ת */
void GPIO_Toggle_Pin(GPIO_TypeDef* GPIOx, uint16_t GPIO_Pin);

#endif
