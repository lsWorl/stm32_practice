#include "stm32f10x.h"
#include "led.h"
#include "OLED.h"
#include "delay.h"
#include "button.h"
#include "beeper.h"
#include "mpu6050.h"
#include "encoder.h"
#include "servo.h"
#include <stdlib.h>

int main(void)
{
    MPU6050_Attitude attitude;
    short accx, accy, accz;
    short gyrox, gyroy, gyroz;
    uint8_t init_status;
    int16_t encoderCount;
    EncoderState encoderState;
    uint8_t angle = 90;
    int8_t direction = 1;  // 1��ʾ���ӽǶȣ�-1��ʾ���ٽǶ�
    
    // ��ʼ��ϵͳʱ��
    SystemInit();
    // ��ʼ����ʱ����
    delay_init();
    
    // ��ʼ������
    LED_Init();
    OLED_Init();
    Encoder_Init();
    Servo_Init();
    
    // OLED��ʾ������ʾ
    OLED_Clear();
    OLED_ShowString(0, 0, "Servo Test");
    OLED_ShowString(1, 0, "Angle:");
    OLED_ShowString(2, 0, "Mode: Sweep");
    
    // ��ʼ��MPU6050�����
    init_status = MPU6050_Init();
    if(init_status != 0)
    {
        
        OLED_ShowString(1, 1, "MPU6050 Error!");
        // ��ʾMPU6050��ID ���ID��ȡ������FF�����ʾI2C��������ʧ��
        OLED_ShowHexNum(2, 1, MPU6050_ReadID(), 2);
        while(1);
    }
    
    while(1)
    {
        // ��ȡMPU6050����
        MPU6050_GetData(&accx, &accy, &accz, &gyrox, &gyroy, &gyroz);
        MPU6050_GetAttitude(&attitude);
        
        // ��ȡ����������
        encoderCount = Encoder_GetCount();
        encoderState = Encoder_GetState();
        
        // ��ʾ����ֵ
        if(encoderCount < 0)
        {
            OLED_ShowChar(1, 6, '-');
            OLED_ShowNum(1, 7, -encoderCount, 5);
        }
        else
        {
            OLED_ShowChar(1, 6, '+');
            OLED_ShowNum(1, 7, encoderCount, 5);
        }
        
        // ��ʾ״̬
        switch(encoderState)
        {
            case ENCODER_IDLE:
                OLED_ShowString(2, 6, "IDLE  ");
                LED_Off(LED2_PIN | LED3_PIN);
                break;
            
            case ENCODER_CW:
                OLED_ShowString(2, 6, "CW    ");
                LED_On(LED2_PIN);
                LED_Off(LED3_PIN);
                break;
            
            case ENCODER_CCW:
                OLED_ShowString(2, 6, "CCW   ");
                LED_On(LED3_PIN);
                LED_Off(LED2_PIN);
                break;
            
            case ENCODER_PUSH:
                OLED_ShowString(2, 6, "PUSH  ");
                LED_Toggle(LED4_PIN);
                Encoder_ClearCount();  // ���°�ťʱ�������
                break;
        }
        
        // ��ʾ��ǰ�Ƕ�
        OLED_ShowNum(1, 7, angle, 3);
        
        // ���ö���Ƕ�
        Servo_SetAngle(angle);
        
        // �Ƕ��Զ�ɨ�裨0-180��������
        angle += direction;
        
        // ����߽�ʱ�ı䷽��
        if(angle >= 180)
        {
            direction = -1;
            LED_On(LED2_PIN);
            LED_Off(LED3_PIN);
        }
        else if(angle <= 0)
        {
            direction = 1;
            LED_On(LED3_PIN);
            LED_Off(LED2_PIN);
        }
        
        // ϵͳ����ָʾ
        LED_Toggle(LED1_PIN);
        
        delay_ms(20);  // ����ɨ���ٶ�
    }
}
