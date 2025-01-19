#include "light_sensor.h"

// ���յȼ���ֵ���壨ADCֵ��
#define LIGHT_THRESHOLD_DARK 3000   // �ڰ���ֵ
#define LIGHT_THRESHOLD_DIM 1500    // �谵��ֵ
#define LIGHT_THRESHOLD_NORMAL 1000 // ������ֵ

/**
 * @brief  ��ʼ��������������ADC
 * @param  None
 * @retval None
 */
void LightSensor_Init(void)
{
  GPIO_InitTypeDef GPIO_InitStructure;
  ADC_InitTypeDef ADC_InitStructure;

  // ʹ��ADC1��GPIOA��ʱ��
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_ADC1 | RCC_APB2Periph_GPIOA, ENABLE);

  // ����PA0Ϊģ������
  GPIO_InitStructure.GPIO_Pin = LIGHT_SENSOR_PIN;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AIN;
  GPIO_Init(LIGHT_SENSOR_PORT, &GPIO_InitStructure);

  // ADC1����
  ADC_DeInit(ADC1);
  ADC_InitStructure.ADC_Mode = ADC_Mode_Independent;    // ����ģʽ
  ADC_InitStructure.ADC_ScanConvMode = DISABLE;         // ��ͨ��ģʽ
  ADC_InitStructure.ADC_ContinuousConvMode = DISABLE;  // ����ת��ģʽ
  ADC_InitStructure.ADC_ExternalTrigConv = ADC_ExternalTrigConv_None; // �ⲿ����ģʽ
  ADC_InitStructure.ADC_DataAlign = ADC_DataAlign_Right; // ���ݶ���
  ADC_InitStructure.ADC_NbrOfChannel = 1;
  ADC_Init(ADC1, &ADC_InitStructure);

  // ����ADCͨ���Ͳ���ʱ��
  ADC_RegularChannelConfig(ADC1, ADC_Channel_0, 1, ADC_SampleTime_239Cycles5); // ����ADCͨ���Ͳ���ʱ��

  // ʹ��ADC1
  ADC_Cmd(ADC1, ENABLE);

  // ADCУ׼
  ADC_ResetCalibration(ADC1); // ��λУ׼
  while (ADC_GetResetCalibrationStatus(ADC1))
    ;
  ADC_StartCalibration(ADC1); // ��ʼУ׼
  while (ADC_GetCalibrationStatus(ADC1))
    ;
}

/**
 * @brief  ��ȡADCת��ֵ
 * @param  None
 * @retval uint16_t: ADCת�����(0-4095)
 */
uint16_t LightSensor_GetValue(void)
{
  //����ADCͨ��ΪPA0
  ADC_RegularChannelConfig(ADC1, ADC_Channel_0, 1, ADC_SampleTime_239Cycles5);
  
  // ����ADCת��
  ADC_SoftwareStartConvCmd(ADC1, ENABLE);
  // �ȴ�ת�����
  while (!ADC_GetFlagStatus(ADC1, ADC_FLAG_EOC))
    ;
  // ����ת�����
  return ADC_GetConversionValue(ADC1);
}

/**
 * @brief  ��ȡ����ǿ�Ȱٷֱ�
 * @param  None
 * @retval uint8_t: ����ǿ�Ȱٷֱ�(0-100)
 */
uint8_t LightSensor_GetPercent(void)
{
  uint32_t value = LightSensor_GetValue();
  // ��ADCֵ��0-4095��ת��Ϊ�ٷֱȣ�0-100��
  return (uint8_t)(100 - ((value * 100) / 4095));
}

/**
 * @brief  ��ȡ���յȼ�
 * @param  None
 * @retval LightLevel: ���յȼ�
 */
LightLevel LightSensor_GetLevel(void)
{
  uint16_t value = LightSensor_GetValue();

  if (value > LIGHT_THRESHOLD_NORMAL && value < LIGHT_THRESHOLD_DIM) // ģ������1000-1500֮��Ϊ��������
    return LIGHT_LEVEL_NORMAL;
  else if (value > LIGHT_THRESHOLD_DIM && value < LIGHT_THRESHOLD_DARK) // ģ������1500-3000֮��Ϊ�谵����
    return LIGHT_LEVEL_DIM;
  else if (value > LIGHT_THRESHOLD_DARK) // ģ������3000����Ϊ�ڰ�����  
    return LIGHT_LEVEL_DARK;
  else
    return LIGHT_LEVEL_BRIGHT;
}
