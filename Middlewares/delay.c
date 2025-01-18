#include "delay.h"

// 定义全局变量
volatile uint32_t sysTickCount;

// 初始化延迟函数
void delay_init(void)
{
    // 配置 SysTick 时钟源为系统时钟（HCLK）
    SysTick_CLKSourceConfig(SysTick_CLKSource_HCLK);

    // 设置重装载值（假设系统时钟为 72 MHz，定时周期为 1 ms）
    SysTick->LOAD = (SystemCoreClock / 1000) - 1;

    // 设置当前值为 0
    SysTick->VAL = 0;

    // 配置 SysTick 定时器
    // 启用中断（TICKINT = 1），启动定时器（ENABLE = 1）
    SysTick->CTRL = SysTick_CTRL_TICKINT_Msk | SysTick_CTRL_ENABLE_Msk;
}

// 延时nus
// nus为要延时的us数.
void delay_us(uint32_t nus) {
    uint32_t start_tick = SysTick->VAL;  // 获取当前计数值
    uint32_t ticks = nus * (SystemCoreClock / 1000000);  // 计算需要的计数次数 1us = 1000000 / SystemCoreClock  

    while (1) {
        uint32_t current_tick = SysTick->VAL; // 获取当前计数值 VAL中存放的是当前计数值
        if (current_tick > start_tick) { 
            // 如果计数器溢出（从 0 重新开始）
            if ((current_tick - start_tick) >= ticks) { // 如果当前计数值减去起始计数值大于等于需要的计数次数，则延时完成
                break; 
            }
        } else { // 如果计数器未溢出
            if ((start_tick - current_tick) >= ticks) {
                break;
            }
        }
    }
}

// 延时nms
void delay_ms(uint32_t nms)
{
    uint32_t startTick = sysTickCount; // 记录起始时刻的 sysTickCount
    while ((sysTickCount - startTick) < nms)
    {
        // 等待延时完成
    }
}

// 获取系统时间
uint32_t GetTickCount(void)
{
    return sysTickCount;
}

// SysTick中断服务程序
void SysTick_Handler(void)
{
    sysTickCount++;
}
