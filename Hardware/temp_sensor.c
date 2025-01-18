#include "temp_sensor.h"
#include <math.h>

// �¶���ֵ���壨���϶ȣ�
#define TEMP_THRESHOLD_LOW    10.0f  // ������ֵ
#define TEMP_THRESHOLD_HIGH   20.0f  // ������ֵ
#define TEMP_THRESHOLD_ALERT  30.0f  // ������ֵ

// NTC�����������
#define TEMP_NOMINAL         25.0f   // ����¶ȣ����϶ȣ�
#define THERMISTOR_NOMINAL   10000.0f // ��Ƶ��裨ŷķ��
#define BCOEFFICIENT        3950.0f  // Betaϵ��
#define SERIES_RESISTOR     10000.0f // ��ѹ���裨ŷķ��

/**
  * @brief  ��ʼ���¶ȴ�������ADC
  * @param  None
  * @retval None
  */
void TempSensor_Init(void)
{
    GPIO_InitTypeDef GPIO_InitStructure;
    ADC_InitTypeDef ADC_InitStructure;
    
    // ʹ��ADC1��GPIOA��ʱ��
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_ADC1 | RCC_APB2Periph_GPIOA, ENABLE);
    
    // ����PA1Ϊģ������
    GPIO_InitStructure.GPIO_Pin = TEMP_SENSOR_PIN;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AIN;
    GPIO_Init(TEMP_SENSOR_PORT, &GPIO_InitStructure);
    
    // ADC1����
    ADC_DeInit(ADC1);
    ADC_InitStructure.ADC_Mode = ADC_Mode_Independent;
    ADC_InitStructure.ADC_ScanConvMode = DISABLE;
    ADC_InitStructure.ADC_ContinuousConvMode = DISABLE;
    ADC_InitStructure.ADC_ExternalTrigConv = ADC_ExternalTrigConv_None;
    ADC_InitStructure.ADC_DataAlign = ADC_DataAlign_Right;
    ADC_InitStructure.ADC_NbrOfChannel = 1;
    ADC_Init(ADC1, &ADC_InitStructure);
    
    // ����ADCͨ���Ͳ���ʱ��
    ADC_RegularChannelConfig(ADC1, ADC_Channel_1, 1, ADC_SampleTime_239Cycles5);
    
    // ʹ��ADC1
    ADC_Cmd(ADC1, ENABLE);
    
    // ADCУ׼
    ADC_ResetCalibration(ADC1);
    while(ADC_GetResetCalibrationStatus(ADC1));
    ADC_StartCalibration(ADC1);
    while(ADC_GetCalibrationStatus(ADC1));
}

/**
  * @brief  ��ȡADCת��ֵ
  * @param  None
  * @retval uint16_t: ADCת�����(0-4095)
  */
uint16_t TempSensor_GetADC(void)
{
    // ����ADCת��
    ADC_SoftwareStartConvCmd(ADC1, ENABLE);
    // �ȴ�ת�����
    while(!ADC_GetFlagStatus(ADC1, ADC_FLAG_EOC));
    // ����ת�����
    return ADC_GetConversionValue(ADC1);
}

/**
  * @brief  ��ȡʵ���¶�ֵ
  * @param  None
  * @retval float: �¶�ֵ�����϶ȣ�
  */
float TempSensor_GetTemp(void)
{
    float adc_value, resistance, steinhart;
    
    // ��ȡADCֵ���������ֵ
    adc_value = (float)TempSensor_GetADC();
    resistance = SERIES_RESISTOR / ((4095.0f / adc_value) - 1.0f);
    
    // ʹ��Steinhart-Hart���̼����¶�
    steinhart = resistance / THERMISTOR_NOMINAL;     // (R/Ro)
    steinhart = log(steinhart);                     // ln(R/Ro)
    steinhart /= BCOEFFICIENT;                      // 1/B * ln(R/Ro)
    steinhart += 1.0f / (TEMP_NOMINAL + 273.15f);   // + (1/To)
    steinhart = 1.0f / steinhart;                   // ����
    steinhart -= 273.15f;                           // ת��Ϊ���϶�
    
    return steinhart;
}

/**
  * @brief  ��ȡ�¶ȵȼ�
  * @param  None
  * @retval TempLevel: �¶ȵȼ�
  */
TempLevel TempSensor_GetLevel(void)
{
    float temp = TempSensor_GetTemp();
    
    if(temp < TEMP_THRESHOLD_LOW)
        return TEMP_LOW;
    else if(temp < TEMP_THRESHOLD_HIGH)
        return TEMP_NORMAL;
    else if(temp < TEMP_THRESHOLD_ALERT)
        return TEMP_HIGH;
    else
        return TEMP_ALERT;
}
