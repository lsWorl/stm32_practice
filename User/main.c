#include "stm32f10x.h"
#include "led.h"
#include "OLED.h"
#include "delay.h"
#include "button.h"
#include "beeper.h"

// 按键测试函数
void Button_Test(void)
{
    KeyID key;
    ButtonState state;
    static const char* KEY_NAMES[] = {
        "MODE ",   // 6字符对齐显示
        "CONF ",
        "ALARM",
        "RESET"
    };
    
    // 扫描按键
    Button_Scan();
    
    // 处理按键状态
    for(key = KEY_MODE; key < KEY_COUNT; key++)
    {
        state = Button_GetState(key);
        
        // 根据按键状态执行相应操作
        switch(state)
        {
            case BTN_SHORT_PRESS:
                OLED_ShowString((uint8_t)(key + 1), 1, (char *)KEY_NAMES[key]); // 第key+1行显示按键名
                OLED_ShowString((uint8_t)(key + 1), 7, ":SHORT");       // 显示短按状态
                LED_Toggle(LED1_PIN);                        // 切换LED1状态
                Beeper_Beep(2000);                           // 短鸣2s
                Button_ClearState(key);                      // 清除按键状态
                break;
                
            case BTN_LONG_PRESS:
                OLED_ShowString((uint8_t)(key + 1), 1, (char *)KEY_NAMES[key]); // 第key+1行显示按键名
                OLED_ShowString((uint8_t)(key + 1), 7, ":LONG ");       // 显示长按状态
                LED_Toggle(LED2_PIN);                        // 切换LED2状态
                Beeper_Beep(3000);                           // 长鸣3s
                Button_ClearState(key);                      // 清除按键状态
                break;
                
            default:
                break;
        }
    }
}

int main(void)
{
    // 初始化系统时钟
    SystemInit();
    // 初始化延时函数
    delay_init();
    
    // 初始化外设
    LED_Init();
    OLED_Init();
    Button_Init();
    Beeper_Init();
    
    // OLED显示测试提示
    OLED_Clear();
    OLED_ShowString(1, 0, "Button Test");
    OLED_ShowString(2, 0, "Short:Beep 1000ms");
    OLED_ShowString(3, 0, "Long:Beep 3000ms");
    
    while(1)
    {
        Button_Test();  // 执行按键测试
        delay_ms(10);   // 短暂延时
    }
}
