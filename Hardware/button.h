#ifndef __BUTTON_H
#define __BUTTON_H

#include "stm32f10x.h"
#include "pin_config.h"

// �����߼�״̬����
typedef enum {
    BTN_IDLE = 0,       // ����״̬
    BTN_DEBOUNCE,       // ����״̬
    BTN_SHORT_PRESS,    // �̰�״̬
    BTN_LONG_PRESS      // ����״̬
} ButtonState;

// ������Ŷ���
typedef enum {
    KEY_MODE = 0,   // ģʽ�л����� (BTN1)
    KEY_CONFIRM,    // ȷ�ϰ��� (BTN2)
    KEY_ALARM,      // ����ȷ�ϰ��� (BTN3)
    KEY_RESET,      // ��λ���� (BTN4)
    KEY_COUNT       // ��������
} KeyID;

// ������ʼ��
void Button_Init(void);

// ����ɨ�躯��������ѭ���е��ã�
void Button_Scan(void);

// ��ȡ����״̬
ButtonState Button_GetState(KeyID key);

// �������״̬
void Button_ClearState(KeyID key);

#endif
