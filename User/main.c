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
    
    // 初始化系统时钟
    SystemInit();
    // 初始化延时函数
    delay_init();
    
    // 初始化外设
    LED_Init();
    OLED_Init();
    Button_Init();
    Beeper_Init();
    LightSensor_Init();
    
    // OLED显示测试提示
    OLED_Clear();
    OLED_ShowString(1, 0, "ADC Value:");
    OLED_ShowString(2, 0, "Light   :");
    OLED_ShowString(3, 0, "Level   :");
    
    while(1)
    {
        // 获取光照信息
        adcValue = LightSensor_GetValue();
        lightPercent = LightSensor_GetPercent();
        lightLevel = LightSensor_GetLevel();
        OLED_ShowString(3, 0, "Level:");
        OLED_ShowNum(3, 7, lightLevel, 2);
        // 更新显示
        OLED_ShowNum(1, 10, adcValue, 4);      // 显示ADC值
        OLED_ShowNum(2, 10, lightPercent, 3);   // 显示光照百分比
        
        delay_ms(100);  // 100ms更新一次
    }
}
