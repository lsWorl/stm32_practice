#ifndef __GPIO_H
#define __GPIO_H

#include "stm32f10x.h"

/* GPIO引脚初始化 */
void GPIO_Init_Pin(GPIO_TypeDef* GPIOx, uint16_t GPIO_Pin, GPIOMode_TypeDef GPIO_Mode, GPIOSpeed_TypeDef GPIO_Speed);

/* 设置GPIO引脚状态 */
void GPIO_Set_Pin(GPIO_TypeDef* GPIOx, uint16_t GPIO_Pin, uint8_t PinState);

/* 读取GPIO引脚状态 */
uint8_t GPIO_Read_Pin(GPIO_TypeDef* GPIOx, uint16_t GPIO_Pin);

/* GPIO引脚状态翻转 */
void GPIO_Toggle_Pin(GPIO_TypeDef* GPIOx, uint16_t GPIO_Pin);

#endif
