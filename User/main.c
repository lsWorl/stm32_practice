#include "stm32f10x.h"
#include "led.h"
#include "OLED.h"
#include "delay.h"
#include "button.h"
#include "beeper.h"
#include "light_sensor.h"


int main(void)
{
    uint16_t adcValue;
    uint8_t lightPercent;
    LightLevel lightLevel;
    
    // ��ʼ��ϵͳʱ��
    SystemInit();
    // ��ʼ����ʱ����
    delay_init();
    
    // ��ʼ������
    LED_Init();
    OLED_Init();
    Button_Init();
    Beeper_Init();
    LightSensor_Init();
    
    // OLED��ʾ������ʾ
    OLED_Clear();
    OLED_ShowString(1, 0, "ADC Value:");
    OLED_ShowString(2, 0, "Light   :");
    OLED_ShowString(3, 0, "Level   :");
    
    while(1)
    {
        // ��ȡ������Ϣ
        adcValue = LightSensor_GetValue();
        lightPercent = LightSensor_GetPercent();
        lightLevel = LightSensor_GetLevel();
        OLED_ShowString(3, 0, "Level:");
        OLED_ShowNum(3, 7, lightLevel, 2);
        // ������ʾ
        OLED_ShowNum(1, 10, adcValue, 4);      // ��ʾADCֵ
        OLED_ShowNum(2, 10, lightPercent, 3);   // ��ʾ���հٷֱ�
        
        delay_ms(100);  // 100ms����һ��
    }
}
