#include "delay.h"

// ����ȫ�ֱ���
volatile uint32_t sysTickCount;

// ��ʼ���ӳٺ���
void delay_init(void)
{
    // ���� SysTick ʱ��ԴΪϵͳʱ�ӣ�HCLK��
    SysTick_CLKSourceConfig(SysTick_CLKSource_HCLK);

    // ������װ��ֵ������ϵͳʱ��Ϊ 72 MHz����ʱ����Ϊ 1 ms��
    SysTick->LOAD = (SystemCoreClock / 1000) - 1;

    // ���õ�ǰֵΪ 0
    SysTick->VAL = 0;

    // ���� SysTick ��ʱ��
    // �����жϣ�TICKINT = 1����������ʱ����ENABLE = 1��
    SysTick->CTRL = SysTick_CTRL_TICKINT_Msk | SysTick_CTRL_ENABLE_Msk;
}

// ��ʱnus
// nusΪҪ��ʱ��us��.
void delay_us(uint32_t nus) {
    uint32_t start_tick = SysTick->VAL;  // ��ȡ��ǰ����ֵ
    uint32_t ticks = nus * (SystemCoreClock / 1000000);  // ������Ҫ�ļ������� 1us = 1000000 / SystemCoreClock  

    while (1) {
        uint32_t current_tick = SysTick->VAL; // ��ȡ��ǰ����ֵ VAL�д�ŵ��ǵ�ǰ����ֵ
        if (current_tick > start_tick) { 
            // ���������������� 0 ���¿�ʼ��
            if ((current_tick - start_tick) >= ticks) { // �����ǰ����ֵ��ȥ��ʼ����ֵ���ڵ�����Ҫ�ļ�������������ʱ���
                break; 
            }
        } else { // ���������δ���
            if ((start_tick - current_tick) >= ticks) {
                break;
            }
        }
    }
}

// ��ʱnms
void delay_ms(uint32_t nms)
{
    uint32_t startTick = sysTickCount; // ��¼��ʼʱ�̵� sysTickCount
    while ((sysTickCount - startTick) < nms)
    {
        // �ȴ���ʱ���
    }
}

// ��ȡϵͳʱ��
uint32_t GetTickCount(void)
{
    return sysTickCount;
}

// SysTick�жϷ������
void SysTick_Handler(void)
{
    sysTickCount++;
}
