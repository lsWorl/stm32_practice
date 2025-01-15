#include "temp_sensor.h"
#include "stm32f10x_adc.h"
#include "stm32f10x_rcc.h"
#include "pin_config.h"

void TempSensor_Init(void)
{
    ADC_InitTypeDef ADC_InitStructure;
    GPIO_InitTypeDef GPIO_InitStructure;
    
    // ʹ��GPIOA��ADC1ʱ��
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA | RCC_APB2Periph_ADC1, ENABLE);
    
    // ����PA1Ϊģ������
    GPIO_InitStructure.GPIO_Pin = TEMP_SENSOR_PIN;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AIN;
    GPIO_Init(TEMP_SENSOR_PORT, &GPIO_InitStructure);
    
    // ADC����
    ADC_InitStructure.ADC_Mode = ADC_Mode_Independent;
    ADC_InitStructure.ADC_ScanConvMode = DISABLE;
    ADC_InitStructure.ADC_ContinuousConvMode = DISABLE;
    ADC_InitStructure.ADC_ExternalTrigConv = ADC_ExternalTrigConv_None;
    ADC_InitStructure.ADC_DataAlign = ADC_DataAlign_Right;
    ADC_InitStructure.ADC_NbrOfChannel = 1;
    ADC_Init(ADC1, &ADC_InitStructure);
    
    // ����ADCͨ��
    ADC_RegularChannelConfig(ADC1, TEMP_SENSOR_CHANNEL, 1, ADC_SampleTime_239Cycles5);
    
    // ʹ��ADC
    ADC_Cmd(ADC1, ENABLE);
    
    // ADCУ׼
    ADC_ResetCalibration(ADC1);
    while(ADC_GetResetCalibrationStatus(ADC1));
    ADC_StartCalibration(ADC1);
    while(ADC_GetCalibrationStatus(ADC1));
}

uint16_t TempSensor_GetRawValue(void)
{
    // ����ת��
    ADC_SoftwareStartConvCmd(ADC1, ENABLE);
    
    // �ȴ�ת�����
    while(ADC_GetFlagStatus(ADC1, ADC_FLAG_EOC) == RESET);
    
    // ����ת�����
    return ADC_GetConversionValue(ADC1);
}

float TempSensor_GetTemperature(void)
{
    uint16_t rawValue = TempSensor_GetRawValue();
    float voltage = (rawValue / 4095.0f) * 3.3f;
    
    // ʹ��NTC���������Steinhart-Hart���̼����¶�
    float R = 10000.0f * (3.3f / voltage - 1.0f);  // 10k����ѹ����
    float T0 = 298.15f;  // 25��C in Kelvin
    float B = 3950.0f;   // Bֵ
    float R0 = 10000.0f; // 25��Cʱ�ĵ���ֵ
    
    float tempK = 1.0f / (1.0f/T0 + (1.0f/B) * log(R/R0));
    return tempK - 273.15f;  // ת��Ϊ���϶�
}
