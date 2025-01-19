#include "light_sensor.h"

// 光照等级阈值定义（ADC值）
#define LIGHT_THRESHOLD_DARK 3000   // 黑暗阈值
#define LIGHT_THRESHOLD_DIM 1500    // 昏暗阈值
#define LIGHT_THRESHOLD_NORMAL 1000 // 正常阈值

/**
 * @brief  初始化光敏传感器的ADC
 * @param  None
 * @retval None
 */
void LightSensor_Init(void)
{
  GPIO_InitTypeDef GPIO_InitStructure;
  ADC_InitTypeDef ADC_InitStructure;

  // 使能ADC1和GPIOA的时钟
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_ADC1 | RCC_APB2Periph_GPIOA, ENABLE);

  // 配置PA0为模拟输入
  GPIO_InitStructure.GPIO_Pin = LIGHT_SENSOR_PIN;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AIN;
  GPIO_Init(LIGHT_SENSOR_PORT, &GPIO_InitStructure);

  // ADC1配置
  ADC_DeInit(ADC1);
  ADC_InitStructure.ADC_Mode = ADC_Mode_Independent;    // 独立模式
  ADC_InitStructure.ADC_ScanConvMode = DISABLE;         // 单通道模式
  ADC_InitStructure.ADC_ContinuousConvMode = DISABLE;  // 单次转换模式
  ADC_InitStructure.ADC_ExternalTrigConv = ADC_ExternalTrigConv_None; // 外部触发模式
  ADC_InitStructure.ADC_DataAlign = ADC_DataAlign_Right; // 数据对齐
  ADC_InitStructure.ADC_NbrOfChannel = 1;
  ADC_Init(ADC1, &ADC_InitStructure);

  // 配置ADC通道和采样时间
  ADC_RegularChannelConfig(ADC1, ADC_Channel_0, 1, ADC_SampleTime_239Cycles5); // 配置ADC通道和采样时间

  // 使能ADC1
  ADC_Cmd(ADC1, ENABLE);

  // ADC校准
  ADC_ResetCalibration(ADC1); // 复位校准
  while (ADC_GetResetCalibrationStatus(ADC1))
    ;
  ADC_StartCalibration(ADC1); // 开始校准
  while (ADC_GetCalibrationStatus(ADC1))
    ;
}

/**
 * @brief  获取ADC转换值
 * @param  None
 * @retval uint16_t: ADC转换结果(0-4095)
 */
uint16_t LightSensor_GetValue(void)
{
  //配置ADC通道为PA0
  ADC_RegularChannelConfig(ADC1, ADC_Channel_0, 1, ADC_SampleTime_239Cycles5);
  
  // 启动ADC转换
  ADC_SoftwareStartConvCmd(ADC1, ENABLE);
  // 等待转换完成
  while (!ADC_GetFlagStatus(ADC1, ADC_FLAG_EOC))
    ;
  // 返回转换结果
  return ADC_GetConversionValue(ADC1);
}

/**
 * @brief  获取光照强度百分比
 * @param  None
 * @retval uint8_t: 光照强度百分比(0-100)
 */
uint8_t LightSensor_GetPercent(void)
{
  uint32_t value = LightSensor_GetValue();
  // 将ADC值（0-4095）转换为百分比（0-100）
  return (uint8_t)(100 - ((value * 100) / 4095));
}

/**
 * @brief  获取光照等级
 * @param  None
 * @retval LightLevel: 光照等级
 */
LightLevel LightSensor_GetLevel(void)
{
  uint16_t value = LightSensor_GetValue();

  if (value > LIGHT_THRESHOLD_NORMAL && value < LIGHT_THRESHOLD_DIM) // 模拟量在1000-1500之间为正常光照
    return LIGHT_LEVEL_NORMAL;
  else if (value > LIGHT_THRESHOLD_DIM && value < LIGHT_THRESHOLD_DARK) // 模拟量在1500-3000之间为昏暗光照
    return LIGHT_LEVEL_DIM;
  else if (value > LIGHT_THRESHOLD_DARK) // 模拟量在3000以上为黑暗光照  
    return LIGHT_LEVEL_DARK;
  else
    return LIGHT_LEVEL_BRIGHT;
}
