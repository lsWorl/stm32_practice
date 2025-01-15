#include "light_sensor.h"
#include "stm32f10x_adc.h"
#include "stm32f10x_rcc.h"
#include "pin_config.h"

void LightSensor_Init(void)
{
    ADC_InitTypeDef ADC_InitStructure;
    GPIO_InitTypeDef GPIO_InitStructure;
    
    // ʹ��GPIOA��ADC1ʱ��
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA | RCC_APB2Periph_ADC1, ENABLE);
    
    // ����PA0Ϊģ������
    GPIO_InitStructure.GPIO_Pin = LIGHT_SENSOR_PIN;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AIN;
    GPIO_Init(ADC_PORT, &GPIO_InitStructure);
    
    // ADC����
    ADC_InitStructure.ADC_Mode = ADC_Mode_Independent;
    ADC_InitStructure.ADC_ScanConvMode = DISABLE;
    ADC_InitStructure.ADC_ContinuousConvMode = DISABLE;
    ADC_InitStructure.ADC_ExternalTrigConv = ADC_ExternalTrigConv_None;
    ADC_InitStructure.ADC_DataAlign = ADC_DataAlign_Right;
    ADC_InitStructure.ADC_NbrOfChannel = 1;
    ADC_Init(ADC1, &ADC_InitStructure);
    
    // ����ADCͨ��
    ADC_RegularChannelConfig(ADC1, ADC_CHANNEL_LIGHT, 1, ADC_SampleTime_239Cycles5);
    
    // ʹ��ADC
    ADC_Cmd(ADC1, ENABLE);
    
    // ADCУ׼
    ADC_ResetCalibration(ADC1);
    while(ADC_GetResetCalibrationStatus(ADC1));
    ADC_StartCalibration(ADC1);
    while(ADC_GetCalibrationStatus(ADC1));
}

uint16_t LightSensor_GetValue(void)
{
    // ����ת��
    ADC_SoftwareStartConvCmd(ADC1, ENABLE);
    
    // �ȴ�ת�����
    while(ADC_GetFlagStatus(ADC1, ADC_FLAG_EOC) == RESET);
    
    // ����ת�����
    return ADC_GetConversionValue(ADC1);
}
