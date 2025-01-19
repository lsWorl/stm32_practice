#ifndef __MPU6050_H
#define __MPU6050_H

#include "stm32f10x.h"
#include "pin_config.h"

// MPU6050寄存器地址定义
#define MPU_ADDR            0x68    // MPU6050器件地址
#define MPU_SELF_TESTX_REG 0x0D    // 自检寄存器X
#define MPU_SELF_TESTY_REG 0x0E    // 自检寄存器Y
#define MPU_SELF_TESTZ_REG 0x0F    // 自检寄存器Z
#define MPU_SELF_TESTA_REG 0x10    // 自检寄存器A
#define MPU_SAMPLE_RATE_REG 0x19    // 采样频率分频器
#define MPU_CFG_REG        0x1A    // 配置寄存器
#define MPU_GYRO_CFG_REG   0x1B    // 陀螺仪配置寄存器
#define MPU_ACCEL_CFG_REG  0x1C    // 加速度计配置寄存器
#define MPU_MOTION_DET_REG 0x1F    // 运动检测阀值设置寄存器
#define MPU_FIFO_EN_REG    0x23    // FIFO使能寄存器
#define MPU_I2CMST_CTRL_REG 0x24   // IIC主机控制寄存器
#define MPU_I2CSLV0_ADDR_REG 0x25  // IIC从机0器件地址寄存器
#define MPU_I2CSLV0_REG    0x26    // IIC从机0数据地址寄存器
#define MPU_I2CSLV0_CTRL_REG 0x27  // IIC从机0控制寄存器
#define MPU_I2CSLV1_ADDR_REG 0x28  // IIC从机1器件地址寄存器
#define MPU_I2CSLV1_REG    0x29    // IIC从机1数据地址寄存器
#define MPU_I2CSLV1_CTRL_REG 0x2A  // IIC从机1控制寄存器
#define MPU_I2CSLV2_ADDR_REG 0x2B  // IIC从机2器件地址寄存器
#define MPU_I2CSLV2_REG    0x2C    // IIC从机2数据地址寄存器
#define MPU_I2CSLV2_CTRL_REG 0x2D  // IIC从机2控制寄存器
#define MPU_I2CSLV3_ADDR_REG 0x2E  // IIC从机3器件地址寄存器
#define MPU_I2CSLV3_REG    0x2F    // IIC从机3数据地址寄存器
#define MPU_I2CSLV3_CTRL_REG 0x30  // IIC从机3控制寄存器
#define MPU_I2CSLV4_ADDR_REG 0x31  // IIC从机4器件地址寄存器
#define MPU_I2CSLV4_REG    0x32    // IIC从机4数据地址寄存器
#define MPU_I2CSLV4_DO_REG 0x33    // IIC从机4写数据寄存器
#define MPU_I2CSLV4_CTRL_REG 0x34  // IIC从机4控制寄存器
#define MPU_I2CSLV4_DI_REG 0x35    // IIC从机4读数据寄存器
#define MPU_I2CMST_STA_REG 0x36    // IIC主机状态寄存器
#define MPU_INTBP_CFG_REG  0x37    // 中断/旁路设置寄存器
#define MPU_INT_EN_REG     0x38    // 中断使能寄存器
#define MPU_INT_STA_REG    0x3A    // 中断状态寄存器
#define MPU_ACCEL_XOUTH_REG 0x3B   // 加速度值,X轴高8位寄存器
#define MPU_ACCEL_XOUTL_REG 0x3C   // 加速度值,X轴低8位寄存器
#define MPU_ACCEL_YOUTH_REG 0x3D   // 加速度值,Y轴高8位寄存器
#define MPU_ACCEL_YOUTL_REG 0x3E   // 加速度值,Y轴低8位寄存器
#define MPU_ACCEL_ZOUTH_REG 0x3F   // 加速度值,Z轴高8位寄存器
#define MPU_ACCEL_ZOUTL_REG 0x40   // 加速度值,Z轴低8位寄存器
#define MPU_TEMP_OUTH_REG  0x41    // 温度值高8位寄存器
#define MPU_TEMP_OUTL_REG  0x42    // 温度值低8位寄存器
#define MPU_GYRO_XOUTH_REG 0x43    // 陀螺仪值,X轴高8位寄存器
#define MPU_GYRO_XOUTL_REG 0x44    // 陀螺仪值,X轴低8位寄存器
#define MPU_GYRO_YOUTH_REG 0x45    // 陀螺仪值,Y轴高8位寄存器
#define MPU_GYRO_YOUTL_REG 0x46    // 陀螺仪值,Y轴低8位寄存器
#define MPU_GYRO_ZOUTH_REG 0x47    // 陀螺仪值,Z轴高8位寄存器
#define MPU_GYRO_ZOUTL_REG 0x48    // 陀螺仪值,Z轴低8位寄存器
#define MPU_PWR_MGMT1_REG  0x6B    // 电源管理寄存器1
#define MPU_PWR_MGMT2_REG  0x6C    // 电源管理寄存器2
#define MPU_WHO_AM_I_REG   0x75    // 器件ID寄存器

// 姿态数据结构体
typedef struct {
    float pitch;    // 俯仰角
    float roll;     // 横滚角
    float yaw;      // 偏航角
} MPU6050_Attitude;

// MPU6050函数
uint8_t MPU6050_Init(void);                    // 初始化MPU6050
uint8_t MPU6050_ReadID(void);                  // 读取MPU6050的ID
void MPU6050_GetData(short *accx, short *accy, short *accz,
                     short *gyrox, short *gyroy, short *gyroz); // 获取原始数据
void MPU6050_GetAttitude(MPU6050_Attitude *attitude); // 获取姿态角度

#endif
