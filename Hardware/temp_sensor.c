#include "temp_sensor.h"
#include "stm32f10x_adc.h"
#include "stm32f10x_rcc.h"
#include "pin_config.h"

void TempSensor_Init(void)
{
    ADC_InitTypeDef ADC_InitStructure;
    GPIO_InitTypeDef GPIO_InitStructure;
    
    // 使能GPIOA和ADC1时钟
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA | RCC_APB2Periph_ADC1, ENABLE);
    
    // 配置PA1为模拟输入
    GPIO_InitStructure.GPIO_Pin = TEMP_SENSOR_PIN;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AIN;
    GPIO_Init(TEMP_SENSOR_PORT, &GPIO_InitStructure);
    
    // ADC配置
    ADC_InitStructure.ADC_Mode = ADC_Mode_Independent;
    ADC_InitStructure.ADC_ScanConvMode = DISABLE;
    ADC_InitStructure.ADC_ContinuousConvMode = DISABLE;
    ADC_InitStructure.ADC_ExternalTrigConv = ADC_ExternalTrigConv_None;
    ADC_InitStructure.ADC_DataAlign = ADC_DataAlign_Right;
    ADC_InitStructure.ADC_NbrOfChannel = 1;
    ADC_Init(ADC1, &ADC_InitStructure);
    
    // 配置ADC通道
    ADC_RegularChannelConfig(ADC1, TEMP_SENSOR_CHANNEL, 1, ADC_SampleTime_239Cycles5);
    
    // 使能ADC
    ADC_Cmd(ADC1, ENABLE);
    
    // ADC校准
    ADC_ResetCalibration(ADC1);
    while(ADC_GetResetCalibrationStatus(ADC1));
    ADC_StartCalibration(ADC1);
    while(ADC_GetCalibrationStatus(ADC1));
}

uint16_t TempSensor_GetRawValue(void)
{
    // 启动转换
    ADC_SoftwareStartConvCmd(ADC1, ENABLE);
    
    // 等待转换完成
    while(ADC_GetFlagStatus(ADC1, ADC_FLAG_EOC) == RESET);
    
    // 返回转换结果
    return ADC_GetConversionValue(ADC1);
}

float TempSensor_GetTemperature(void)
{
    uint16_t rawValue = TempSensor_GetRawValue();
    float voltage = (rawValue / 4095.0f) * 3.3f;
    
    // 使用NTC热敏电阻的Steinhart-Hart方程计算温度
    float R = 10000.0f * (3.3f / voltage - 1.0f);  // 10kΩ分压电阻
    float T0 = 298.15f;  // 25°C in Kelvin
    float B = 3950.0f;   // B值
    float R0 = 10000.0f; // 25°C时的电阻值
    
    float tempK = 1.0f / (1.0f/T0 + (1.0f/B) * log(R/R0));
    return tempK - 273.15f;  // 转换为摄氏度
}
