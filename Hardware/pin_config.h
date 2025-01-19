#ifndef __PIN_CONFIG_H
#define __PIN_CONFIG_H

// OLED ����
#define OLED_SCL_PIN        GPIO_Pin_6  // PB6
#define OLED_SDA_PIN        GPIO_Pin_7  // PB7
#define OLED_PORT           GPIOB  // OLED�������ڵĶ˿�

//MPU6050����
#define MPU6050_SDA_PIN     GPIO_Pin_11  // PB11
#define MPU6050_SCL_PIN     GPIO_Pin_10  // PB10
#define MPU6050_PORT        GPIOB  // MPU6050�������ڵĶ˿�

// ��������������
#define LIGHT_SENSOR_PIN    GPIO_Pin_0  // PA0
#define LIGHT_SENSOR_PORT   GPIOA  // �����������������ڵĶ˿�

// �¶ȴ���������
#define TEMP_SENSOR_PIN     GPIO_Pin_1  // PA1
#define TEMP_SENSOR_PORT    GPIOA   // �¶ȴ������������ڵĶ˿�

// ����������
#define BEEPER_PIN          GPIO_Pin_2  // PA2
#define BEEPER_PORT         GPIOA
// ����������
#define ENCODER_A_PIN       GPIO_Pin_6  // PA6 TIM3_CH1
#define ENCODER_B_PIN       GPIO_Pin_7  // PA7 TIM3_CH2

// �������
#define SERVO_PWM_PIN       GPIO_Pin_8  // PA8
#define SERVO_PORT          GPIOA  // ����������ڵĶ˿�

// LED����
#define LED1_PIN            GPIO_Pin_9  // PA9
#define LED2_PIN            GPIO_Pin_10  // PA10
#define LED3_PIN            GPIO_Pin_11  // PA11
#define LED4_PIN            GPIO_Pin_12  // PA12
#define LED_PORT            GPIOA  // LED�������ڵĶ˿�

// ��ť����
#define BTN1_PIN            GPIO_Pin_5  // PA5
#define BTN2_PIN            GPIO_Pin_0  // PB0
#define BTN3_PIN            GPIO_Pin_1  // PB1
#define BTN4_PIN            GPIO_Pin_12  // PB12


#endif
