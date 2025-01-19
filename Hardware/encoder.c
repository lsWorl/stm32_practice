#include "encoder.h"
#include "delay.h"
#include "pin_config.h"
// 定义编码器结构体变量
static EncoderTypeDef encoder;


/**
 * @brief  初始化编码器
 * @param  None
 * @retval None
 */
void Encoder_Init(void)
{
  GPIO_InitTypeDef GPIO_InitStructure;
  TIM_TimeBaseInitTypeDef TIM_TimeBaseStructure;
  TIM_ICInitTypeDef TIM_ICInitStructure;

  // 使能时钟
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);
  RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3, ENABLE);

  // 配置编码器AB相引脚
  GPIO_InitStructure.GPIO_Pin = ENCODER_A_PIN|ENCODER_B_PIN;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU; // 上拉输入
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_Init(GPIOA, &GPIO_InitStructure);

  // 配置定时器
  TIM_TimeBaseStructure.TIM_Period = 65536 - 1; // 计数器最大值
  TIM_TimeBaseStructure.TIM_Prescaler = 0;      // 不分频
  TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1;
  TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;
  TIM_TimeBaseInit(TIM3, &TIM_TimeBaseStructure);

  // 配置编码器接口 TIM_EncoderMode_TI12 表示使用TIM3的CH1和CH3作为编码器输入
  TIM_EncoderInterfaceConfig(TIM3, TIM_EncoderMode_TI12,
                             TIM_ICPolarity_Rising, TIM_ICPolarity_Rising);

  // 配置输入捕获 TIM_Channel_1 表示使用TIM3的CH1作为编码器输入
  TIM_ICInitStructure.TIM_Channel = TIM_Channel_1;
  TIM_ICInitStructure.TIM_ICPolarity = TIM_ICPolarity_Rising;
  TIM_ICInitStructure.TIM_ICSelection = TIM_ICSelection_DirectTI;
  TIM_ICInitStructure.TIM_ICPrescaler = TIM_ICPSC_DIV1;
  TIM_ICInitStructure.TIM_ICFilter = 0xF; // 滤波器设置
  TIM_ICInit(TIM3, &TIM_ICInitStructure);

  TIM_ICInitStructure.TIM_Channel = TIM_Channel_2;
  TIM_ICInit(TIM3, &TIM_ICInitStructure);

  // 使能定时器
  TIM_Cmd(TIM3, ENABLE);

  // 初始化编码器变量
  encoder.count = 0;
  encoder.lastCount = 0;
  encoder.buttonState = 0;
}

/**
 * @brief  获取编码器状态
 * @param  None
 * @retval EncoderState: 编码器状态
 */
EncoderState Encoder_GetState(void)
{
  // diff为编码器计数差值
  int16_t diff = encoder.count - encoder.lastCount;
  // 如果按钮状态为1，则为按下按钮
  if (encoder.buttonState)
    return ENCODER_PUSH;
  // 如果计数差值大于0，则为顺时针旋转
  else if (diff > 0)
    return ENCODER_CW;
  // 如果计数差值小于0，则为逆时针旋转
  else if (diff < 0)
    return ENCODER_CCW;
  // 否则为空闲状态
  else
    return ENCODER_IDLE;
}

/**
 * @brief  获取编码器计数值
 * @param  None
 * @retval int16_t: 计数值
 */
int16_t Encoder_GetCount(void)
{
  // 将当前计数值赋值给lastCount
  encoder.lastCount = encoder.count;
  // 将当前计数值赋值给count
  encoder.count = (int16_t)TIM_GetCounter(TIM3);
  // 返回当前计数值
  return encoder.count;
}

/**
 * @brief  获取按钮状态
 * @param  None
 * @retval uint8_t: 按钮状态，1为按下，0为释放
 */
uint8_t Encoder_GetButtonState(void)
{
  return encoder.buttonState;
}

/**
 * @brief  设置编码器计数值
 * @param  count: 计数值
 * @retval None
 */
void Encoder_SetCount(int16_t count)
{
  encoder.count = count;
  encoder.lastCount = count;
  TIM_SetCounter(TIM3, (uint16_t)count);
}

/**
 * @brief  清除编码器计数值
 * @param  None
 * @retval None
 */
void Encoder_ClearCount(void)
{
  Encoder_SetCount(0);
}
