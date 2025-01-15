#ifndef __LIGHT_SENSOR_H
#define __LIGHT_SENSOR_H

#include "stm32f10x.h"

// ADC�ֱ���
#define ADC_RESOLUTION 4096  // 12-bit ADC

// ��������
void LightSensor_Init(void);
uint16_t LightSensor_GetValue(void);

#endif
