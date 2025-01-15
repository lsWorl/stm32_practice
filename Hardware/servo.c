#include "servo.h"
#include "pwm.h"
#include "stm32f10x.h"

#define SERVO_MIN_PULSE 500   // 0.5ms
#define SERVO_MAX_PULSE 2500  // 2.5ms
#define SERVO_PERIOD 20000    // 20ms

/**
  * @brief  �����ʼ��
  * @param  TIMx: ��ʱ���˿�
  * @param  Channel: PWMͨ��
  * @retval None
  */
void Servo_Init(TIM_TypeDef* TIMx, uint16_t Channel)
{
    /* ��ʼ��PWM����Ϊ20ms */
    PWM_Init(TIMx, Channel, SERVO_PERIOD, SERVO_MIN_PULSE);
}

/**
  * @brief  ���ö���Ƕ�
  * @param  TIMx: ��ʱ���˿�
  * @param  Channel: PWMͨ��
  * @param  angle: �Ƕ�ֵ (0-180)
  * @retval None
  */
void Servo_SetAngle(TIM_TypeDef* TIMx, uint16_t Channel, uint8_t angle)
{
    uint16_t pulse;
    
    /* ���ƽǶȷ�Χ */
    if(angle > 180) angle = 180;
    
    /* �������� */
    pulse = SERVO_MIN_PULSE + (angle * (SERVO_MAX_PULSE - SERVO_MIN_PULSE)) / 180;
    
    /* ����PWM���� */
    PWM_SetDuty(TIMx, Channel, pulse);
}
