#include "stm32f10x.h"

#include <stdlib.h>
#include "system.h"

int main(void)
{

    // ϵͳ��ʼ��
    System_Init();

    // ��ѭ��
    while (1)
    {
        // ϵͳ������
        System_Process();

        // ��ʱ
        delay_ms(20); // ����ɨ���ٶ�
    }
}
