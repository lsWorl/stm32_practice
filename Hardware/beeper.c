#include "beeper.h"
#include "delay.h"

/**
  * @brief  ��ʼ��������GPIO
  * @param  None
  * @retval None
  */
void Beeper_Init(void)
{
    GPIO_InitTypeDef GPIO_InitStructure;
    
    // ʹ��GPIOAʱ��
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);
    
    // ���÷���������Ϊ�������
    GPIO_InitStructure.GPIO_Pin = BEEPER_PIN;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(BEEPER_PORT, &GPIO_InitStructure);
    
    // ��ʼ״̬Ϊ�ر�
    Beeper_Off();
}

/**
  * @brief  �򿪷�����
  * @param  None
  * @retval None
  */
void Beeper_On(void)
{
    GPIO_ResetBits(BEEPER_PORT, BEEPER_PIN);
}

/**
  * @brief  �رշ�����
  * @param  None
  * @retval None
  */
void Beeper_Off(void)
{
    GPIO_SetBits(BEEPER_PORT, BEEPER_PIN);
}

/**
  * @brief  �л�������״̬
  * @param  None
  * @retval None
  */
void Beeper_Toggle(void)
{
    BEEPER_PORT->ODR ^= BEEPER_PIN;
}

/**
  * @brief  ����������ָ��ʱ��
  * @param  ms: ����ʱ��(����)
  * @retval None
  */
void Beeper_Beep(uint16_t ms)
{
    Beeper_On();
    delay_ms(ms);
    Beeper_Off();
}
