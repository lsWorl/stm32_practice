#include "stm32f10x.h"
#include "led.h"
#include "OLED.h"
#include "delay.h"
#include "button.h"

int main(void)
{
    // ��ʼ��ϵͳʱ��
    SystemInit();
    // ��ʼ����ʱ����
    delay_init();
    
    // ��ʼ��LED
    LED_Init();
    // ��ʼ��OLED
    OLED_Init();
    // ��ʼ������
    Button_Init();
    
    // OLED��ʾ����
    OLED_Clear();
    OLED_ShowString(1, 1, "STM32 Test");
    OLED_ShowString(2, 1, "Button Test");
    OLED_ShowString(3, 1, "Press any key");
    
    while(1)
    {
        uint16_t i;
        // ɨ�谴��
        Button_Scan();
        
        // ������״̬
        for(i = 0; i < BTN_COUNT; i++)
        {
            ButtonState state = Button_GetState(i);
            if(state == BTN_SHORT_PRESSED)
            {
                // ��ʾ�����̰���Ϣ
                OLED_ShowString(4, 1, "BTN");
                OLED_ShowNum(4, 4, i+1, 1);
                OLED_ShowString(4, 5, ":Short");
                LED_Toggle(LED1_PIN);  // �л�LED1״̬
                Button_ClearState(i);
            }
            else if(state == BTN_LONG_PRESSED)
            {
                // ��ʾ����������Ϣ
                OLED_ShowString(4, 1, "BTN");
                OLED_ShowNum(4, 4, i+1, 1);
                OLED_ShowString(4, 5, ":Long ");
                LED_Toggle(LED2_PIN);  // �л�LED2״̬
                Button_ClearState(i);
            }
        }
        
        delay_ms(10);  // ������ʱ
    }
}
