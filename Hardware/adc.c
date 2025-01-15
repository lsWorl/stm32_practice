#include "adc.h"
#include "stm32f10x.h"

/**
  * @brief  ADC初始化
  * @param  ADCx: ADC端口
  * @param  ADC_Channel: ADC通道
  * @retval None
  */
void ADCinit(ADC_TypeDef* ADCx, uint8_t ADC_Channel)
{
    GPIO_InitTypeDef GPIO_InitStruct;
    ADC_InitTypeDef ADC_InitStruct;
    
    /* 使能GPIO和ADC时钟 */
    if(ADCx == ADC1) {
        RCC_APB2PeriphClockCmd(RCC_APB2Periph_ADC1, ENABLE);
        
        /* 配置ADC引脚为模拟输入 */
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
    
    /* ADC配置 */
    ADC_InitStruct.ADC_Mode = ADC_Mode_Independent;
    ADC_InitStruct.ADC_ScanConvMode = DISABLE;
    ADC_InitStruct.ADC_ContinuousConvMode = DISABLE;
    ADC_InitStruct.ADC_ExternalTrigConv = ADC_ExternalTrigConv_None;
    ADC_InitStruct.ADC_DataAlign = ADC_DataAlign_Right;
    ADC_InitStruct.ADC_NbrOfChannel = 1;
    ADC_Init(ADCx, &ADC_InitStruct);
    
    /* 配置ADC通道 */
    ADC_RegularChannelConfig(ADCx, ADC_Channel, 1, ADC_SampleTime_55Cycles5);
    
    /* 使能ADC */
    ADC_Cmd(ADCx, ENABLE);
    
    /* ADC校准 */
    ADC_ResetCalibration(ADCx);
    while(ADC_GetResetCalibrationStatus(ADCx));
    ADC_StartCalibration(ADCx);
    while(ADC_GetCalibrationStatus(ADCx));
}

/**
  * @brief  读取ADC值
  * @param  ADCx: ADC端口
  * @retval ADC转换结果
  */
uint16_t ADC_Read(ADC_TypeDef* ADCx)
{
    /* 启动转换 */
    ADC_SoftwareStartConvCmd(ADCx, ENABLE);
    
    /* 等待转换完成 */
    while(ADC_GetFlagStatus(ADCx, ADC_FLAG_EOC) == RESET);
    
    /* 返回转换结果 */
    return ADC_GetConversionValue(ADCx);
}
