#include "stm32f10x.h"
#include "led.h"
#include "OLED.h"
#include "delay.h"
#include "button.h"
#include "beeper.h"
#include "light_sensor.h"
#include "temp_sensor.h"

// 温度等级显示字符串
static const char *TEMP_LEVEL_STR[] = {
    "LOW   ",
    "NORMAL",
    "HIGH  ",
    "ALERT "};

int main(void)
{
    uint16_t adcValue;
    float temperature;
    TempLevel tempLevel;

    // 初始化系统时钟
    SystemInit();
    // 初始化延时函数
    delay_init();

    // 初始化外设
    LED_Init();
    OLED_Init();
    Button_Init();
    Beeper_Init();
    TempSensor_Init();

    // OLED显示测试提示
    OLED_Clear();
    OLED_ShowString(1, 0, "ADC Value:");
    OLED_ShowString(2, 0, "Temp(C):");
    OLED_ShowString(3, 0, "Level:");

    while (1)
    {
        // 获取温度信息
        adcValue = TempSensor_GetADC();
        temperature = TempSensor_GetTemp();
        tempLevel = TempSensor_GetLevel();

        // 更新显示
        OLED_ShowNum(1, 10, adcValue, 4); // 显示ADC值

        // 显示温度值（保留1位小数）
        OLED_ShowNum(2, 8, (uint16_t)temperature, 2);
        OLED_ShowString(2, 10, ".");
        OLED_ShowNum(2, 11, (uint16_t)((temperature - (int)temperature) * 10), 1);

        // 显示温度等级
        OLED_ShowString(3, 7, (char *)TEMP_LEVEL_STR[tempLevel]);

        // 根据温度等级控制LED和蜂鸣器
        switch (tempLevel)
        {
        case TEMP_LOW: // 温度过低
            LED_On(LED1_PIN);
            LED_Off(LED2_PIN | LED3_PIN | LED4_PIN);
            break;

        case TEMP_NORMAL: // 温度正常
            LED_On(LED2_PIN);
            LED_Off(LED1_PIN | LED3_PIN | LED4_PIN);
            break;

        case TEMP_HIGH: // 温度偏高
            LED_On(LED3_PIN);
            LED_Off(LED1_PIN | LED2_PIN | LED4_PIN);
            break;

        case TEMP_ALERT: // 温度警告
            LED_Toggle(LED4_PIN);
            LED_Off(LED1_PIN | LED2_PIN | LED3_PIN);
            Beeper_Beep(500); // 蜂鸣器报警
            break;
        }
        delay_ms(100); // 100ms更新一次
    }
}
