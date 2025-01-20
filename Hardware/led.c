#include "led.h"

/**
  * @brief  初始化LED的GPIO
  * @param  None
  * @retval None
  */
void LED_Init(void)
{
    GPIO_InitTypeDef GPIO_InitStructure;
    
    // 使能LED所在的GPIO端口时钟
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);
    
    // 配置LED引脚为推挽输出
    GPIO_InitStructure.GPIO_Pin = LED1_PIN | LED2_PIN | LED3_PIN | LED4_PIN;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP; // 推挽输出
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(LED_PORT, &GPIO_InitStructure);
    
    // 初始状态，所有LED关闭
    LED_AllOff();
}

/**
  * @brief  点亮指定的LED
  * @param  LED_PIN: 要控制的LED引脚
  * @retval None
  */
void LED_On(uint16_t LED_PIN)
{
    GPIO_ResetBits(LED_PORT, LED_PIN);
}

/**
  * @brief  关闭指定的LED
  * @param  LED_PIN: 要控制的LED引脚
  * @retval None
  */
void LED_Off(uint16_t LED_PIN)
{
    GPIO_SetBits(LED_PORT, LED_PIN);
}

/**
  * @brief  翻转指定的LED状态
  * @param  LED_PIN: 要控制的LED引脚
  * @retval None
  */
void LED_Toggle(uint16_t LED_PIN)
{
    LED_PORT->ODR ^= LED_PIN;
}

/**
  * @brief  点亮所有LED
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
  * @brief  关闭所有LED
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
  * @brief  系统运行指示（LED1）
  * @param  None
  * @retval None
  */
void LED_SystemRunning(void)
{
    LED_Toggle(LED1_PIN);
}
/**
  * @brief  数据采集指示（LED2）
  * @param  None
  * @retval None
  */
void LED_DataCollecting(void)
{
    LED_On(LED2_PIN);
}

/**
  * @brief  设置模式指示（LED3）
  * @param  None
  * @retval None
  */
void LED_SettingMode(void)
{
    LED_On(LED3_PIN);
}

/**
  * @brief  报警状态指示（LED4）
  * @param  None
  * @retval None
  */
void LED_AlarmStatus(void)
{
    LED_Toggle(LED4_PIN);
}
