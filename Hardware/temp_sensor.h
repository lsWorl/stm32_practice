#ifndef __TEMP_SENSOR_H
#define __TEMP_SENSOR_H

#include "stm32f10x.h"
#include <math.h>

// 温度传感器参数
#define TEMP_SENSOR_BETA 3950.0f  // NTC热敏电阻B值
#define TEMP_SENSOR_R0   10000.0f // 25°C时的电阻值
#define TEMP_SENSOR_T0   298.15f  // 25°C in Kelvin

// 函数声明
void TempSensor_Init(void);
uint16_t TempSensor_GetRawValue(void);
float TempSensor_GetTemperature(void);

#endif
