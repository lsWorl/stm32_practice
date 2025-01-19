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
    int8_t direction = 1;  // 1表示增加角度，-1表示减少角度
    
    // 初始化系统时钟
    SystemInit();
    // 初始化延时函数
    delay_init();
    
    // 初始化外设
    LED_Init();
    OLED_Init();
    Encoder_Init();
    Servo_Init();
    
    // OLED显示测试提示
    OLED_Clear();
    OLED_ShowString(0, 0, "Servo Test");
    OLED_ShowString(1, 0, "Angle:");
    OLED_ShowString(2, 0, "Mode: Sweep");
    
    // 初始化MPU6050并检查
    init_status = MPU6050_Init();
    if(init_status != 0)
    {
        
        OLED_ShowString(1, 1, "MPU6050 Error!");
        // 显示MPU6050的ID 如果ID读取到的是FF，则表示I2C总线连接失败
        OLED_ShowHexNum(2, 1, MPU6050_ReadID(), 2);
        while(1);
    }
    
    while(1)
    {
        // 获取MPU6050数据
        MPU6050_GetData(&accx, &accy, &accz, &gyrox, &gyroy, &gyroz);
        MPU6050_GetAttitude(&attitude);
        
        // 获取编码器数据
        encoderCount = Encoder_GetCount();
        encoderState = Encoder_GetState();
        
        // 显示计数值
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
        
        // 显示状态
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
                Encoder_ClearCount();  // 按下按钮时清零计数
                break;
        }
        
        // 显示当前角度
        OLED_ShowNum(1, 7, angle, 3);
        
        // 设置舵机角度
        Servo_SetAngle(angle);
        
        // 角度自动扫描（0-180度往返）
        angle += direction;
        
        // 到达边界时改变方向
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
        
        // 系统运行指示
        LED_Toggle(LED1_PIN);
        
        delay_ms(20);  // 控制扫描速度
    }
}
