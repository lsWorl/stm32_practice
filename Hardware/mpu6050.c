#include "mpu6050.h"
#include "i2c.h"
#include "delay.h"
#include <math.h>

// 定义MPU6050内部寄存器初始化数组
static const uint8_t MPU_Init_Data[][2] = {
    {MPU_PWR_MGMT1_REG, 0x00},     // 解除休眠状态
    {MPU_SAMPLE_RATE_REG, 0x07},   // 采样率，1000/(1+7)=125Hz
    {MPU_CFG_REG, 0x06},           // 设置数字低通滤波器
    {MPU_GYRO_CFG_REG, 0x18},      // 陀螺仪自检及测量范围，±2000dps
    {MPU_ACCEL_CFG_REG, 0x00}      // 加速计自检、测量范围及高通滤波频率，±2g
};

// 定义MPU6050数据缓存
static short gyro[3], accel[3]; // 陀螺仪和加速度计数据缓存

/**
  * @brief  初始化MPU6050
  * @param  None
  * @retval 0:成功 1:失败
  */
uint8_t MPU6050_Init(void)
{
    uint8_t i;
    
    // 初始化I2C总线
    I2C_Init_Software();
    delay_ms(100);
    
    // 检查MPU6050是否正常
    if(MPU6050_ReadID() != 0x68) // 读取MPU6050的ID，如果ID不正确，返回1
        return 1;
    
    // 配置MPU6050内部寄存器
    for(i = 0; i < sizeof(MPU_Init_Data)/2; i++) //sizeof(MPU_Init_Data)/2的值为5
    {
        I2C_WriteReg(MPU_ADDR, MPU_Init_Data[i][0], MPU_Init_Data[i][1]);
        delay_ms(5);
    }
    
    return 0;
}

/**
  * @brief  读取MPU6050的ID
  * @param  None
  * @retval ID值
  */
uint8_t MPU6050_ReadID(void)
{
    return I2C_ReadReg(MPU_ADDR, MPU_WHO_AM_I_REG);
}

/**
  * @brief  获取MPU6050的原始数据
  * @param  accx,accy,accz:加速度计x,y,z轴的原始读数(带符号)
  * @param  gyrox,gyroy,gyroz:陀螺仪x,y,z轴的原始读数(带符号)
  * @retval None
  */
void MPU6050_GetData(short *accx, short *accy, short *accz,
                     short *gyrox, short *gyroy, short *gyroz)
{
    uint8_t buf[14];
    
    // 读取加速度和陀螺仪的原始数据
    I2C_ReadMultBytes(MPU_ADDR, MPU_ACCEL_XOUTH_REG, 14, buf);
    
    // 合成数据
    *accx = ((uint16_t)buf[0] << 8) | buf[1];
    *accy = ((uint16_t)buf[2] << 8) | buf[3];
    *accz = ((uint16_t)buf[4] << 8) | buf[5];
    *gyrox = ((uint16_t)buf[8] << 8) | buf[9];
    *gyroy = ((uint16_t)buf[10] << 8) | buf[11];
    *gyroz = ((uint16_t)buf[12] << 8) | buf[13];
}

/**
  * @brief  获取MPU6050的姿态角度
  * @param  attitude: 姿态角度结构体指针
  * @retval None
  */
void MPU6050_GetAttitude(MPU6050_Attitude *attitude)
{
    float ax, ay, az;
    
    // 获取原始数据
    MPU6050_GetData(&accel[0], &accel[1], &accel[2],
                    &gyro[0], &gyro[1], &gyro[2]);
    
    // 转换为实际的g值，这里量程是±2g
    ax = accel[0] / 16384.0f;
    ay = accel[1] / 16384.0f;
    az = accel[2] / 16384.0f;
    
    // 根据加速度计算角度
    attitude->pitch = atan2f(ax, sqrtf(ay * ay + az * az)) * 57.3f;
    attitude->roll = atan2f(ay, az) * 57.3f;
    
    // 偏航角需要地磁计配合，这里暂不处理
    attitude->yaw = 0;
}
