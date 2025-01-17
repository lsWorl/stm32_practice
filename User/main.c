#include "stm32f10x.h"
#include "led.h"
#include "OLED.h"
#include "delay.h"
#include "button.h"

int main(void)
{
    // 初始化系统时钟
    SystemInit();
    // 初始化延时函数
    delay_init();
    
    // 初始化LED
    LED_Init();
    // 初始化OLED
    OLED_Init();
    // 初始化按键
    Button_Init();
    
    // OLED显示测试
    OLED_Clear();
    OLED_ShowString(1, 1, "STM32 Test");
    OLED_ShowString(2, 1, "Button Test");
    OLED_ShowString(3, 1, "Press any key");
    
    while(1)
    {
        uint16_t i;
        // 扫描按键
        Button_Scan();
        
        // 处理按键状态
        for(i = 0; i < BTN_COUNT; i++)
        {
            ButtonState state = Button_GetState(i);
            if(state == BTN_SHORT_PRESSED)
            {
                // 显示按键短按信息
                OLED_ShowString(4, 1, "BTN");
                OLED_ShowNum(4, 4, i+1, 1);
                OLED_ShowString(4, 5, ":Short");
                LED_Toggle(LED1_PIN);  // 切换LED1状态
                Button_ClearState(i);
            }
            else if(state == BTN_LONG_PRESSED)
            {
                // 显示按键长按信息
                OLED_ShowString(4, 1, "BTN");
                OLED_ShowNum(4, 4, i+1, 1);
                OLED_ShowString(4, 5, ":Long ");
                LED_Toggle(LED2_PIN);  // 切换LED2状态
                Button_ClearState(i);
            }
        }
        
        delay_ms(10);  // 短暂延时
    }
}
