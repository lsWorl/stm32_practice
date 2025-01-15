#ifndef __LIGHT_SENSOR_H
#define __LIGHT_SENSOR_H

#include "stm32f10x.h"

// ADC分辨率
#define ADC_RESOLUTION 4096  // 12-bit ADC

// 函数声明
void LightSensor_Init(void);
uint16_t LightSensor_GetValue(void);

#endif
