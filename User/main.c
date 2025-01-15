#include "stm32f10x.h"
#include "OLED.h"
#include <math.h>
#include <stdio.h>

// Temperature sensor parameters
#define VREF 3.3f  // ADC reference voltage
#define R1 10000.0f  // Voltage divider resistor
#define B 3950.0f  // Thermistor B constant
#define T0 298.15f  // Reference temperature (25¡ãC in Kelvin)
#define R0 10000.0f  // Thermistor resistance at T0

// DMA configuration
#define ADC_BUFFER_SIZE 10
uint16_t ADC_ConvertedValue[ADC_BUFFER_SIZE];

void GPIO_Config(void)
{
    GPIO_InitTypeDef GPIO_InitStructure;

    // Enable GPIOA clock
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);
    
    // Configure PA3 as TIM2_CH4 alternate function push-pull output
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_3;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(GPIOA, &GPIO_InitStructure);

    // Configure PA0 as analog input for temperature sensor
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AIN;
    GPIO_Init(GPIOA, &GPIO_InitStructure);
}

void DMA_Config(void)
{
    DMA_InitTypeDef DMA_InitStructure;
    
    // Enable DMA1 clock
    RCC_AHBPeriphClockCmd(RCC_AHBPeriph_DMA1, ENABLE);
    
    // Configure DMA1 Channel1
    DMA_InitStructure.DMA_PeripheralBaseAddr = (uint32_t)&ADC1->DR;
    DMA_InitStructure.DMA_MemoryBaseAddr = (uint32_t)ADC_ConvertedValue;
    DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralSRC;
    DMA_InitStructure.DMA_BufferSize = ADC_BUFFER_SIZE;
    DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable;
    DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable;
    DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_HalfWord;
    DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_HalfWord;
    DMA_InitStructure.DMA_Mode = DMA_Mode_Circular;
    DMA_InitStructure.DMA_Priority = DMA_Priority_High;
    DMA_InitStructure.DMA_M2M = DMA_M2M_Disable;
    DMA_Init(DMA1_Channel1, &DMA_InitStructure);
    
    // Enable DMA1 Channel1
    DMA_Cmd(DMA1_Channel1, ENABLE);
}

void TIM2_PWM_Config(void)
{
    TIM_TimeBaseInitTypeDef TIM_TimeBaseStructure;
    TIM_OCInitTypeDef TIM_OCInitStructure;

    // Enable TIM2 clock
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2, ENABLE);

    // Configure TIM2 base
    TIM_TimeBaseStructure.TIM_Period = 19999;  // 20ms period
    TIM_TimeBaseStructure.TIM_Prescaler = 71;  // 72MHz/(71+1) = 1MHz
    TIM_TimeBaseStructure.TIM_ClockDivision = 0;
    TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;
    TIM_TimeBaseInit(TIM2, &TIM_TimeBaseStructure);

    // Configure TIM2 Channel4 PWM mode
    TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1;
    TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;
    TIM_OCInitStructure.TIM_Pulse = 1500;  // Initial pulse width 1.5ms
    TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High;
    TIM_OC4Init(TIM2, &TIM_OCInitStructure);

    // Enable TIM2
    TIM_Cmd(TIM2, ENABLE);
}

void ADC_Config(void)
{
    ADC_InitTypeDef ADC_InitStructure;

    // Enable ADC1 clock
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_ADC1, ENABLE);
    
    // ADC configuration
    ADC_InitStructure.ADC_Mode = ADC_Mode_Independent;
    ADC_InitStructure.ADC_ScanConvMode = ENABLE;
    ADC_InitStructure.ADC_ContinuousConvMode = ENABLE;
    ADC_InitStructure.ADC_ExternalTrigConv = ADC_ExternalTrigConv_None;
    ADC_InitStructure.ADC_DataAlign = ADC_DataAlign_Right;
    ADC_InitStructure.ADC_NbrOfChannel = 1;
    ADC_Init(ADC1, &ADC_InitStructure);

    // Configure ADC Channel 0
    ADC_RegularChannelConfig(ADC1, ADC_Channel_0, 1, ADC_SampleTime_239Cycles5);

    // Enable ADC DMA
    ADC_DMACmd(ADC1, ENABLE);

    // Enable ADC
    ADC_Cmd(ADC1, ENABLE);

    // Calibrate ADC
    ADC_ResetCalibration(ADC1);
    while(ADC_GetResetCalibrationStatus(ADC1));
    ADC_StartCalibration(ADC1);
    while(ADC_GetCalibrationStatus(ADC1));

    // Start ADC conversion
    ADC_SoftwareStartConvCmd(ADC1, ENABLE);
}

float GetTemperature(void)
{
    uint32_t sum = 0;
    uint8_t i;
    float voltage, rt, temperature;
    uint16_t adc_value; 
    // Calculate average of 10 samples
    for(i = 0; i < ADC_BUFFER_SIZE; i++) {
        sum += ADC_ConvertedValue[i];
    }
    adc_value = sum / ADC_BUFFER_SIZE;
    
    // Convert ADC value to voltage 4096 is total number of bits
    voltage = (float)adc_value * VREF / 4096.0f;

    // Calculate thermistor resistance
    rt = R1 * voltage / (VREF - voltage);

    // Calculate temperature using Steinhart-Hart equation
    temperature = 1.0f / (1.0f/T0 + log(rt/R0)/B) - 273.15f;

    return temperature;
}

int main(void)
{
    // uint8_t turn = 0;
    // Initialize peripherals
    GPIO_Config();
    DMA_Config();
    ADC_Config();
    TIM2_PWM_Config();
    
    // Initialize OLED
    OLED_Init();
    OLED_Clear();
    
    // Display initial message
    OLED_ShowString(1, 1, "Hello world");
    
    while(1)
    {
        uint32_t i;
        // Get and display temperature
        float temperature = GetTemperature();
        char temp_str[16];
        sprintf(temp_str, "Temp:%.1fC", temperature);
        for(i = 0; i < 100000; i++);
        OLED_ShowString(2, 1, temp_str);
        if(temperature > 20.0f)
        {
            TIM_SetCompare4(TIM2, 2000);
        }
        else
        {
            TIM_SetCompare4(TIM2, 1000);
        }
    }
}
