#ifndef __TEMP_SENSOR_H
#define __TEMP_SENSOR_H

#include "stm32f10x.h"
#include <math.h>

// �¶ȴ���������
#define TEMP_SENSOR_BETA 3950.0f  // NTC��������Bֵ
#define TEMP_SENSOR_R0   10000.0f // 25��Cʱ�ĵ���ֵ
#define TEMP_SENSOR_T0   298.15f  // 25��C in Kelvin

// ��������
void TempSensor_Init(void);
uint16_t TempSensor_GetRawValue(void);
float TempSensor_GetTemperature(void);

#endif
