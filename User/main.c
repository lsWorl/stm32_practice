#include "stm32f10x.h"
#include "led.h"
#include "OLED.h"
#include "delay.h"
#include "button.h"
#include "beeper.h"
#include "light_sensor.h"
#include "temp_sensor.h"

// �¶ȵȼ���ʾ�ַ���
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

    // ��ʼ��ϵͳʱ��
    SystemInit();
    // ��ʼ����ʱ����
    delay_init();

    // ��ʼ������
    LED_Init();
    OLED_Init();
    Button_Init();
    Beeper_Init();
    TempSensor_Init();

    // OLED��ʾ������ʾ
    OLED_Clear();
    OLED_ShowString(1, 0, "ADC Value:");
    OLED_ShowString(2, 0, "Temp(C):");
    OLED_ShowString(3, 0, "Level:");

    while (1)
    {
        // ��ȡ�¶���Ϣ
        adcValue = TempSensor_GetADC();
        temperature = TempSensor_GetTemp();
        tempLevel = TempSensor_GetLevel();

        // ������ʾ
        OLED_ShowNum(1, 10, adcValue, 4); // ��ʾADCֵ

        // ��ʾ�¶�ֵ������1λС����
        OLED_ShowNum(2, 8, (uint16_t)temperature, 2);
        OLED_ShowString(2, 10, ".");
        OLED_ShowNum(2, 11, (uint16_t)((temperature - (int)temperature) * 10), 1);

        // ��ʾ�¶ȵȼ�
        OLED_ShowString(3, 7, (char *)TEMP_LEVEL_STR[tempLevel]);

        // �����¶ȵȼ�����LED�ͷ�����
        switch (tempLevel)
        {
        case TEMP_LOW: // �¶ȹ���
            LED_On(LED1_PIN);
            LED_Off(LED2_PIN | LED3_PIN | LED4_PIN);
            break;

        case TEMP_NORMAL: // �¶�����
            LED_On(LED2_PIN);
            LED_Off(LED1_PIN | LED3_PIN | LED4_PIN);
            break;

        case TEMP_HIGH: // �¶�ƫ��
            LED_On(LED3_PIN);
            LED_Off(LED1_PIN | LED2_PIN | LED4_PIN);
            break;

        case TEMP_ALERT: // �¶Ⱦ���
            LED_Toggle(LED4_PIN);
            LED_Off(LED1_PIN | LED2_PIN | LED3_PIN);
            Beeper_Beep(500); // ����������
            break;
        }
        delay_ms(100); // 100ms����һ��
    }
}
