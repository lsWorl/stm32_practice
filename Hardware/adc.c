#include "adc.h"
#include "stm32f10x.h"

/**
  * @brief  ADC��ʼ��
  * @param  ADCx: ADC�˿�
  * @param  ADC_Channel: ADCͨ��
  * @retval None
  */
void ADCinit(ADC_TypeDef* ADCx, uint8_t ADC_Channel)
{
    GPIO_InitTypeDef GPIO_InitStruct;
    ADC_InitTypeDef ADC_InitStruct;
    
    /* ʹ��GPIO��ADCʱ�� */
    if(ADCx == ADC1) {
        RCC_APB2PeriphClockCmd(RCC_APB2Periph_ADC1, ENABLE);
        
        /* ����ADC����Ϊģ������ */
        if(ADC_Channel == ADC_Channel_0) {
            RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);
            GPIO_InitStruct.GPIO_Pin = GPIO_Pin_0;
        }
        else if(ADC_Channel == ADC_Channel_1) {
            RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);
            GPIO_InitStruct.GPIO_Pin = GPIO_Pin_1;
        }
        else if(ADC_Channel == ADC_Channel_2) {
            RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);
            GPIO_InitStruct.GPIO_Pin = GPIO_Pin_2;
        }
        else if(ADC_Channel == ADC_Channel_3) {
            RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);
            GPIO_InitStruct.GPIO_Pin = GPIO_Pin_3;
        }
        
        GPIO_InitStruct.GPIO_Mode = GPIO_Mode_AIN;
        GPIO_Init(GPIOA, &GPIO_InitStruct);
    }
    
    /* ADC���� */
    ADC_InitStruct.ADC_Mode = ADC_Mode_Independent;
    ADC_InitStruct.ADC_ScanConvMode = DISABLE;
    ADC_InitStruct.ADC_ContinuousConvMode = DISABLE;
    ADC_InitStruct.ADC_ExternalTrigConv = ADC_ExternalTrigConv_None;
    ADC_InitStruct.ADC_DataAlign = ADC_DataAlign_Right;
    ADC_InitStruct.ADC_NbrOfChannel = 1;
    ADC_Init(ADCx, &ADC_InitStruct);
    
    /* ����ADCͨ�� */
    ADC_RegularChannelConfig(ADCx, ADC_Channel, 1, ADC_SampleTime_55Cycles5);
    
    /* ʹ��ADC */
    ADC_Cmd(ADCx, ENABLE);
    
    /* ADCУ׼ */
    ADC_ResetCalibration(ADCx);
    while(ADC_GetResetCalibrationStatus(ADCx));
    ADC_StartCalibration(ADCx);
    while(ADC_GetCalibrationStatus(ADCx));
}

/**
  * @brief  ��ȡADCֵ
  * @param  ADCx: ADC�˿�
  * @retval ADCת�����
  */
uint16_t ADC_Read(ADC_TypeDef* ADCx)
{
    /* ����ת�� */
    ADC_SoftwareStartConvCmd(ADCx, ENABLE);
    
    /* �ȴ�ת����� */
    while(ADC_GetFlagStatus(ADCx, ADC_FLAG_EOC) == RESET);
    
    /* ����ת����� */
    return ADC_GetConversionValue(ADCx);
}
