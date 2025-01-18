#ifndef __TEMP_SENSOR_H
#define __TEMP_SENSOR_H

#include "stm32f10x.h"
#include "pin_config.h"

// �¶ȵȼ�����
typedef enum {
    TEMP_LOW = 0,     // �¶ȹ���
    TEMP_NORMAL,      // �¶�����
    TEMP_HIGH,        // �¶�ƫ��
    TEMP_ALERT        // �¶Ⱦ���
} TempLevel;

// �¶ȴ���������
void TempSensor_Init(void);           // ��ʼ���¶ȴ�����
uint16_t TempSensor_GetADC(void);     // ��ȡADCԭʼֵ
float TempSensor_GetTemp(void);       // ��ȡʵ���¶�ֵ�����϶ȣ�
TempLevel TempSensor_GetLevel(void);  // ��ȡ�¶ȵȼ�

#endif
