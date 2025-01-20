#include "led.h"

/**
  * @brief  ��ʼ��LED��GPIO
  * @param  None
  * @retval None
  */
void LED_Init(void)
{
    GPIO_InitTypeDef GPIO_InitStructure;
    
    // ʹ��LED���ڵ�GPIO�˿�ʱ��
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);
    
    // ����LED����Ϊ�������
    GPIO_InitStructure.GPIO_Pin = LED1_PIN | LED2_PIN | LED3_PIN | LED4_PIN;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP; // �������
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(LED_PORT, &GPIO_InitStructure);
    
    // ��ʼ״̬������LED�ر�
    LED_AllOff();
}

/**
  * @brief  ����ָ����LED
  * @param  LED_PIN: Ҫ���Ƶ�LED����
  * @retval None
  */
void LED_On(uint16_t LED_PIN)
{
    GPIO_ResetBits(LED_PORT, LED_PIN);
}

/**
  * @brief  �ر�ָ����LED
  * @param  LED_PIN: Ҫ���Ƶ�LED����
  * @retval None
  */
void LED_Off(uint16_t LED_PIN)
{
    GPIO_SetBits(LED_PORT, LED_PIN);
}

/**
  * @brief  ��תָ����LED״̬
  * @param  LED_PIN: Ҫ���Ƶ�LED����
  * @retval None
  */
void LED_Toggle(uint16_t LED_PIN)
{
    LED_PORT->ODR ^= LED_PIN;
}

/**
  * @brief  ��������LED
  * @param  None
  * @retval None
  */
void LED_AllOn(void)
{
    LED_On(LED1_PIN);
    LED_On(LED2_PIN);
    LED_On(LED3_PIN);
    LED_On(LED4_PIN);
}

/**
  * @brief  �ر�����LED
  * @param  None
  * @retval None
  */
void LED_AllOff(void)
{
    LED_Off(LED1_PIN);
    LED_Off(LED2_PIN);
    LED_Off(LED3_PIN);
    LED_Off(LED4_PIN);
}

/**
  * @brief  ϵͳ����ָʾ��LED1��
  * @param  None
  * @retval None
  */
void LED_SystemRunning(void)
{
    LED_Toggle(LED1_PIN);
}
/**
  * @brief  ���ݲɼ�ָʾ��LED2��
  * @param  None
  * @retval None
  */
void LED_DataCollecting(void)
{
    LED_On(LED2_PIN);
}

/**
  * @brief  ����ģʽָʾ��LED3��
  * @param  None
  * @retval None
  */
void LED_SettingMode(void)
{
    LED_On(LED3_PIN);
}

/**
  * @brief  ����״ָ̬ʾ��LED4��
  * @param  None
  * @retval None
  */
void LED_AlarmStatus(void)
{
    LED_Toggle(LED4_PIN);
}
