#include "mpu6050.h"
#include "stm32f10x_i2c.h"
#include "stm32f10x_rcc.h"
#include "pin_config.h"
#include "delay.h"

#define MPU6050_ADDR 0x68 << 1  // I2C???

// MPU6050????????
#define MPU6050_RA_PWR_MGMT_1   0x6B
#define MPU6050_RA_ACCEL_XOUT_H 0x3B
#define MPU6050_RA_GYRO_XOUT_H  0x43
#define MPU6050_RA_CONFIG       0x1A
#define MPU6050_RA_GYRO_CONFIG  0x1B
#define MPU6050_RA_ACCEL_CONFIG 0x1C

// §µ?????
static int16_t accel_offset[3] = {0};
static int16_t gyro_offset[3] = {0};

void MPU6050_Init(void)
{
    I2C_InitTypeDef I2C_InitStructure;
    GPIO_InitTypeDef GPIO_InitStructure;
    
    // ???GPIOB??I2C1???
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_I2C1, ENABLE);
    
    // ????I2C????
    GPIO_InitStructure.GPIO_Pin = I2C1_SCL_PIN | I2C1_SDA_PIN;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_OD;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(I2C1_PORT, &GPIO_InitStructure);
    
    // I2C????
    I2C_InitStructure.I2C_Mode = I2C_Mode_I2C;
    I2C_InitStructure.I2C_DutyCycle = I2C_DutyCycle_2;
    I2C_InitStructure.I2C_OwnAddress1 = 0x00;
    I2C_InitStructure.I2C_Ack = I2C_Ack_Enable;
    I2C_InitStructure.I2C_AcknowledgedAddress = I2C_AcknowledgedAddress_7bit;
    I2C_InitStructure.I2C_ClockSpeed = I2C1_SPEED;
    I2C_Init(I2C1, &I2C_InitStructure);
    
    // ???I2C
    I2C_Cmd(I2C1, ENABLE);
    
    // ????MPU6050
    MPU6050_WriteByte(MPU6050_RA_PWR_MGMT_1, 0x00);  // ?????υτ
    MPU6050_WriteByte(MPU6050_RA_CONFIG, 0x03);      // ????DLPF?42Hz
    MPU6050_WriteByte(MPU6050_RA_GYRO_CONFIG, 0x18); // ??2000dps
    MPU6050_WriteByte(MPU6050_RA_ACCEL_CONFIG, 0x18);// ??16g
    
    // §µ???????
    MPU6050_Calibrate();
}

void MPU6050_Calibrate(void)
{
    int32_t accel_sum[3] = {0};
    int32_t gyro_sum[3] = {0};
    int16_t ax, ay, az, gx, gy, gz;
    
    // ???100???????????
    for(uint8_t i = 0; i < 100; i++) {
        MPU6050_GetMotion6(&ax, &ay, &az, &gx, &gy, &gz);
        accel_sum[0] += ax;
        accel_sum[1] += ay;
        accel_sum[2] += az;
        gyro_sum[0] += gx;
        gyro_sum[1] += gy;
        gyro_sum[2] += gz;
        delay_ms(10);
    }
    
    // ?????????
    accel_offset[0] = (int16_t)(accel_sum[0] / 100);
    accel_offset[1] = (int16_t)(accel_sum[1] / 100);
    accel_offset[2] = (int16_t)(accel_sum[2] / 100 - 16384); // ???1g
    gyro_offset[0] = (int16_t)(gyro_sum[0] / 100);
    gyro_offset[1] = (int16_t)(gyro_sum[1] / 100);
    gyro_offset[2] = (int16_t)(gyro_sum[2] / 100);
}

void MPU6050_GetMotion6(int16_t* ax, int16_t* ay, int16_t* az,
                        int16_t* gx, int16_t* gy, int16_t* gz)
{
    uint8_t buf[14];
    MPU6050_ReadBytes(MPU6050_RA_ACCEL_XOUT_H, 14, buf);
    
    *ax = (int16_t)((buf[0] << 8) | buf[1]) - accel_offset[0];
    *ay = (int16_t)((buf[2] << 8) | buf[3]) - accel_offset[1];
    *az = (int16_t)((buf[4] << 8) | buf[5]) - accel_offset[2];
    *gx = (int16_t)((buf[8] << 8) | buf[9]) - gyro_offset[0];
    *gy = (int16_t)((buf[10] << 8) | buf[11]) - gyro_offset[1];
    *gz = (int16_t)((buf[12] << 8) | buf[13]) - gyro_offset[2];
}

// ????I2C??§Υ???????????...
