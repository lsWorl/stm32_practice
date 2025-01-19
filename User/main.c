#include "stm32f10x.h"

#include <stdlib.h>
#include "system.h"

int main(void)
{

    // 系统初始化
    System_Init();

    // 主循环
    while (1)
    {
        // 系统主处理
        System_Process();

        // 延时
        delay_ms(20); // 控制扫描速度
    }
}
