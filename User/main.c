#include "stm32f10x.h"
#include "led.h"
#include "OLED.h"
#include "delay.h"
#include "button.h"
#include "beeper.h"

// �������Ժ���
void Button_Test(void)
{
    KeyID key;
    ButtonState state;
    static const char* KEY_NAMES[] = {
        "MODE ",   // 6�ַ�������ʾ
        "CONF ",
        "ALARM",
        "RESET"
    };
    
    // ɨ�谴��
    Button_Scan();
    
    // ������״̬
    for(key = KEY_MODE; key < KEY_COUNT; key++)
    {
        state = Button_GetState(key);
        
        // ���ݰ���״ִ̬����Ӧ����
        switch(state)
        {
            case BTN_SHORT_PRESS:
                OLED_ShowString((uint8_t)(key + 1), 1, (char *)KEY_NAMES[key]); // ��key+1����ʾ������
                OLED_ShowString((uint8_t)(key + 1), 7, ":SHORT");       // ��ʾ�̰�״̬
                LED_Toggle(LED1_PIN);                        // �л�LED1״̬
                Beeper_Beep(2000);                           // ����2s
                Button_ClearState(key);                      // �������״̬
                break;
                
            case BTN_LONG_PRESS:
                OLED_ShowString((uint8_t)(key + 1), 1, (char *)KEY_NAMES[key]); // ��key+1����ʾ������
                OLED_ShowString((uint8_t)(key + 1), 7, ":LONG ");       // ��ʾ����״̬
                LED_Toggle(LED2_PIN);                        // �л�LED2״̬
                Beeper_Beep(3000);                           // ����3s
                Button_ClearState(key);                      // �������״̬
                break;
                
            default:
                break;
        }
    }
}

int main(void)
{
    // ��ʼ��ϵͳʱ��
    SystemInit();
    // ��ʼ����ʱ����
    delay_init();
    
    // ��ʼ������
    LED_Init();
    OLED_Init();
    Button_Init();
    Beeper_Init();
    
    // OLED��ʾ������ʾ
    OLED_Clear();
    OLED_ShowString(1, 0, "Button Test");
    OLED_ShowString(2, 0, "Short:Beep 1000ms");
    OLED_ShowString(3, 0, "Long:Beep 3000ms");
    
    while(1)
    {
        Button_Test();  // ִ�а�������
        delay_ms(10);   // ������ʱ
    }
}
