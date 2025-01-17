#ifndef __BEEPER_H
#define __BEEPER_H

#include "stm32f10x.h"
#include "pin_config.h"

// ���������ƺ���
void Beeper_Init(void);        // ��ʼ��������
void Beeper_On(void);          // �򿪷�����
void Beeper_Off(void);         // �رշ�����
void Beeper_Toggle(void);      // �л�������״̬
void Beeper_Beep(uint16_t ms); // ����ָ��ʱ�������룩

#endif
