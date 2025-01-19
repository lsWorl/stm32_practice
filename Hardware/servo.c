#include "servo.h"

// ��ǰ����Ƕ�
static uint8_t currentAngle = 90;

/**
  * @brief  ��ʼ�����
  * @param  None
  * @retval None
  */
void Servo_Init(void)
{
    GPIO_InitTypeDef GPIO_InitStructure;
    TIM_TimeBaseInitTypeDef TIM_TimeBaseStructure;
    TIM_OCInitTypeDef TIM_OCInitStructure;
    
    // ʹ��ʱ��
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA | RCC_APB2Periph_TIM1, ENABLE);
    
    // ����GPIO
    GPIO_InitStructure.GPIO_Pin = SERVO_PWM_PIN;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;  // �����������
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(SERVO_PORT, &GPIO_InitStructure);
    
    // ����ʱ���ṹ��
    // PWMƵ�� = 72M/(Period+1)/(Prescaler+1)
    // 72000000/20000/72 = 50Hz 1/50 = 20ms
    TIM_TimeBaseStructure.TIM_Period = SERVO_PERIOD - 1;  // 20ms����
    TIM_TimeBaseStructure.TIM_Prescaler = 72 - 1;         // 72��Ƶ
    TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1;
    TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;
    TIM_TimeBaseStructure.TIM_RepetitionCounter = 0;  // ARR�Ĵ���
    TIM_TimeBaseInit(TIM1, &TIM_TimeBaseStructure);
    
    // ����PWMģʽ
    TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1;
    TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;
    TIM_OCInitStructure.TIM_OutputNState = TIM_OutputNState_Disable;  // �������������
    TIM_OCInitStructure.TIM_Pulse = SERVO_MID_PULSE;  // ��ʼ�����м�λ��
    TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High; 
    TIM_OCInitStructure.TIM_OCNPolarity = TIM_OCNPolarity_Low; //���ڻ�����������ã��������ڶ�������в�����Ч
    TIM_OCInitStructure.TIM_OCIdleState = TIM_OCIdleState_Set;  //���������ͨ����TIM1_CH1�� ����״̬ʱ����ߵ�ƽ
    TIM_OCInitStructure.TIM_OCNIdleState = TIM_OCNIdleState_Reset; //���� �������ͨ�� ����״̬
    TIM_OC1Init(TIM1, &TIM_OCInitStructure);
    
    // ʹ��Ԥװ��
    TIM_OC1PreloadConfig(TIM1, TIM_OCPreload_Enable);
    TIM_ARRPreloadConfig(TIM1, ENABLE);
    
    // ʹ��TIM1
    TIM_Cmd(TIM1, ENABLE);
    TIM_CtrlPWMOutputs(TIM1, ENABLE); //���ڿ����Ƿ����ø߼���ʱ����PWM���
    
    // ��λ���м�λ��
    Servo_Reset();
}

/**
  * @brief  ���ö���Ƕ�
  * @param  angle: Ŀ��Ƕȣ�0-180�ȣ�
  * @retval None
  */
void Servo_SetAngle(uint8_t angle)
{
    uint16_t pulse;
    
    // ���ƽǶȷ�Χ
    if(angle > SERVO_MAX_ANGLE)
        angle = SERVO_MAX_ANGLE;
    
    // �Ƕ�ת��Ϊ������
    // pulse = 500 + (angle/180) * 2000
    pulse = SERVO_MIN_PULSE + ((uint32_t)angle * (SERVO_MAX_PULSE - SERVO_MIN_PULSE)) / SERVO_MAX_ANGLE;
    
    // ����PWMռ�ձ�
    TIM_SetCompare1(TIM1, pulse);
    
    // ���µ�ǰ�Ƕ�
    currentAngle = angle;
}

/**
  * @brief  ���ö��������
  * @param  pulse: �����ȣ�500-2500us��
  * @retval None
  */
void Servo_SetPulse(uint16_t pulse)
{
    // ���������ȷ�Χ
    if(pulse < SERVO_MIN_PULSE)
        pulse = SERVO_MIN_PULSE;
    else if(pulse > SERVO_MAX_PULSE)
        pulse = SERVO_MAX_PULSE;
    
    // ����PWMռ�ձ�
    TIM_SetCompare1(TIM1, pulse);
    
    // ���µ�ǰ�Ƕ�
    currentAngle = ((pulse - SERVO_MIN_PULSE) * SERVO_MAX_ANGLE) / (SERVO_MAX_PULSE - SERVO_MIN_PULSE);
}

/**
  * @brief  ��ȡ��ǰ����Ƕ�
  * @param  None
  * @retval uint8_t: ��ǰ�Ƕȣ�0-180�ȣ�
  */
uint8_t Servo_GetAngle(void)
{
    return currentAngle;
}

/**
  * @brief  ��λ������м�λ��
  * @param  None
  * @retval None
  */
void Servo_Reset(void)
{
    Servo_SetAngle(90);  // ��λ��90��λ��
}
