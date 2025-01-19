#include "encoder.h"
#include "delay.h"
#include "pin_config.h"
// ����������ṹ�����
static EncoderTypeDef encoder;


/**
 * @brief  ��ʼ��������
 * @param  None
 * @retval None
 */
void Encoder_Init(void)
{
  GPIO_InitTypeDef GPIO_InitStructure;
  TIM_TimeBaseInitTypeDef TIM_TimeBaseStructure;
  TIM_ICInitTypeDef TIM_ICInitStructure;

  // ʹ��ʱ��
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);
  RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3, ENABLE);

  // ���ñ�����AB������
  GPIO_InitStructure.GPIO_Pin = ENCODER_A_PIN|ENCODER_B_PIN;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU; // ��������
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_Init(GPIOA, &GPIO_InitStructure);

  // ���ö�ʱ��
  TIM_TimeBaseStructure.TIM_Period = 65536 - 1; // ���������ֵ
  TIM_TimeBaseStructure.TIM_Prescaler = 0;      // ����Ƶ
  TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1;
  TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;
  TIM_TimeBaseInit(TIM3, &TIM_TimeBaseStructure);

  // ���ñ������ӿ� TIM_EncoderMode_TI12 ��ʾʹ��TIM3��CH1��CH3��Ϊ����������
  TIM_EncoderInterfaceConfig(TIM3, TIM_EncoderMode_TI12,
                             TIM_ICPolarity_Rising, TIM_ICPolarity_Rising);

  // �������벶�� TIM_Channel_1 ��ʾʹ��TIM3��CH1��Ϊ����������
  TIM_ICInitStructure.TIM_Channel = TIM_Channel_1;
  TIM_ICInitStructure.TIM_ICPolarity = TIM_ICPolarity_Rising;
  TIM_ICInitStructure.TIM_ICSelection = TIM_ICSelection_DirectTI;
  TIM_ICInitStructure.TIM_ICPrescaler = TIM_ICPSC_DIV1;
  TIM_ICInitStructure.TIM_ICFilter = 0xF; // �˲�������
  TIM_ICInit(TIM3, &TIM_ICInitStructure);

  TIM_ICInitStructure.TIM_Channel = TIM_Channel_2;
  TIM_ICInit(TIM3, &TIM_ICInitStructure);

  // ʹ�ܶ�ʱ��
  TIM_Cmd(TIM3, ENABLE);

  // ��ʼ������������
  encoder.count = 0;
  encoder.lastCount = 0;
  encoder.buttonState = 0;
}

/**
 * @brief  ��ȡ������״̬
 * @param  None
 * @retval EncoderState: ������״̬
 */
EncoderState Encoder_GetState(void)
{
  // diffΪ������������ֵ
  int16_t diff = encoder.count - encoder.lastCount;
  // �����ť״̬Ϊ1����Ϊ���°�ť
  if (encoder.buttonState)
    return ENCODER_PUSH;
  // ���������ֵ����0����Ϊ˳ʱ����ת
  else if (diff > 0)
    return ENCODER_CW;
  // ���������ֵС��0����Ϊ��ʱ����ת
  else if (diff < 0)
    return ENCODER_CCW;
  // ����Ϊ����״̬
  else
    return ENCODER_IDLE;
}

/**
 * @brief  ��ȡ����������ֵ
 * @param  None
 * @retval int16_t: ����ֵ
 */
int16_t Encoder_GetCount(void)
{
  // ����ǰ����ֵ��ֵ��lastCount
  encoder.lastCount = encoder.count;
  // ����ǰ����ֵ��ֵ��count
  encoder.count = (int16_t)TIM_GetCounter(TIM3);
  // ���ص�ǰ����ֵ
  return encoder.count;
}

/**
 * @brief  ��ȡ��ť״̬
 * @param  None
 * @retval uint8_t: ��ť״̬��1Ϊ���£�0Ϊ�ͷ�
 */
uint8_t Encoder_GetButtonState(void)
{
  return encoder.buttonState;
}

/**
 * @brief  ���ñ���������ֵ
 * @param  count: ����ֵ
 * @retval None
 */
void Encoder_SetCount(int16_t count)
{
  encoder.count = count;
  encoder.lastCount = count;
  TIM_SetCounter(TIM3, (uint16_t)count);
}

/**
 * @brief  �������������ֵ
 * @param  None
 * @retval None
 */
void Encoder_ClearCount(void)
{
  Encoder_SetCount(0);
}
