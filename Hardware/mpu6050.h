#ifndef __MPU6050_H
#define __MPU6050_H

#include "stm32f10x.h"

typedef struct {
    int16_t accel_x;
    int16_t accel_y;
    int16_t accel_z;
    int16_t gyro_x;
    int16_t gyro_y;
    int16_t gyro_z;
} MPU6050_Data;

void MPU6050_Init(void);
void MPU6050_ReadData(MPU6050_Data *data);

#endif
