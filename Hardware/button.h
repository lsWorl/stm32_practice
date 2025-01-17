#ifndef __BUTTON_H
#define __BUTTON_H

#include "stm32f10x.h"
#include "pin_config.h"

// ����״̬����
typedef enum {
    BTN_RELEASED = 0,    // �����ͷ�
    BTN_PRESSED,         // ��������
    BTN_SHORT_PRESSED,   // �̰�
    BTN_LONG_PRESSED     // ����
} ButtonState;

// ����ID����
typedef enum {
    BTN1 = 0,   // ģʽ�л�����
    BTN2,       // ����ȷ�ϰ���
    BTN3,       // ����ȷ�ϰ���
    BTN4,       // ϵͳ��λ����
    BTN_COUNT   // ��������
} ButtonID;

// ������ʼ��
void Button_Init(void);

// ����ɨ�躯��������ѭ���е��ã�
void Button_Scan(void);

// ��ȡ����״̬
ButtonState Button_GetState(ButtonID btnId);

// �������״̬
void Button_ClearState(ButtonID btnId);

// �����жϴ����������жϷ�������е��ã�
void Button_EXTI_IRQHandler(ButtonID btnId);

#endif
