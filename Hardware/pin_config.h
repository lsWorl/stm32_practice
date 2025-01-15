#ifndef __PIN_CONFIG_H
#define __PIN_CONFIG_H

// I2C1 ??
#define I2C1_SCL_PIN        GPIO_Pin_6  // PB6
#define I2C1_SDA_PIN        GPIO_Pin_7  // PB7
#define I2C1_PORT           GPIOB
#define I2C1_CLK           RCC_APB1Periph_I2C1
#define I2C1_SPEED         400000  // 400kHz

// ADC ??
#define LIGHT_SENSOR_PIN    GPIO_Pin_0  // PA0
#define TEMP_SENSOR_PIN     GPIO_Pin_1  // PA1
#define ADC_CHANNEL_LIGHT   ADC_Channel_0
#define ADC_PORT            GPIOA

// ?????
#define BEEPER_PIN          GPIO_Pin_2  // PA2
#define BEEPER_PORT         GPIOA

// ?????
#define ENCODER_A_PIN       GPIO_Pin_3  // PA3
#define ENCODER_B_PIN       GPIO_Pin_4  // PA4
#define ENCODER_PORT        GPIOA

// ??PWM??
#define SERVO_PWM_PIN       GPIO_Pin_8  // PA8
#define SERVO_PWM_PORT      GPIOA

// LED?????
#define LED1_PIN            GPIO_Pin_0  // PB0
#define LED2_PIN            GPIO_Pin_1  // PB1
#define LED3_PIN            GPIO_Pin_10  // PB10
#define LED4_PIN            GPIO_Pin_3  // PB3
#define LED_PORT            GPIOB

// ????
#define BTN1_PIN            GPIO_Pin_4  // PB4
#define BTN2_PIN            GPIO_Pin_5  // PB5
#define BTN3_PIN            GPIO_Pin_8  // PB8
#define BTN4_PIN            GPIO_Pin_9  // PB9
#define BTN_PORT            GPIOB

#endif
