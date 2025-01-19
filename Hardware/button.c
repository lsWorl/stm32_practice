#include "button.h"
#include "delay.h"

// 按键参数定义
#define BUTTON_SCAN_INTERVAL    20      // 扫描间隔（ms）
#define LONG_PRESS_TIME         2000    // 长按时间（ms）
#define DEBOUNCE_TIME          20       // 消抖时间（ms）



// 按键引脚定义
static const uint16_t KEY_PINS[KEY_COUNT] = {
    BTN1_PIN,   // KEY_MODE  (BTN1)
    BTN2_PIN,   // KEY_CONFIRM (BTN2)
    BTN3_PIN,   // KEY_ALARM (BTN3)
    BTN4_PIN    // KEY_RESET (BTN4)
};

static GPIO_TypeDef* const KEY_PORTS[KEY_COUNT] = {
    GPIOA,      // KEY_MODE  (PA5)
    GPIOB,      // KEY_CONFIRM (PB0)
    GPIOB,      // KEY_ALARM (PB1)
    GPIOB       // KEY_RESET (PB12)
};

// 按键状态记录
static struct {
    ButtonState state;      // 按键逻辑状态
    uint32_t pressTime;     // 按下时刻
    uint8_t isPressed;      // 按键是否处于按下状态
} keyInfo[KEY_COUNT] = {BTN_IDLE, 0, 0};



/**
  * @brief  初始化按键GPIO
  * @param  None
  * @retval None
  */
void Button_Init(void)
{
    GPIO_InitTypeDef GPIO_InitStructure;
    EXTI_InitTypeDef EXTI_InitStructure;
    NVIC_InitTypeDef NVIC_InitStructure;
    uint8_t i;
    
    // 使能GPIO时钟
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA | RCC_APB2Periph_GPIOB | RCC_APB2Periph_AFIO, ENABLE);
    
    // 配置按键引脚为输入上拉
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    
    //逐一配置按键引脚
    for(i = 0; i < KEY_COUNT; i++)
    {
        GPIO_InitStructure.GPIO_Pin = KEY_PINS[i];
        GPIO_Init(KEY_PORTS[i], &GPIO_InitStructure);
        keyInfo[i].state = BTN_IDLE;
        keyInfo[i].isPressed = 0;
    }
    
    // 配置EXTI中断
    GPIO_EXTILineConfig(GPIO_PortSourceGPIOA, GPIO_PinSource5);  // KEY_MODE
    GPIO_EXTILineConfig(GPIO_PortSourceGPIOB, GPIO_PinSource0);  // KEY_CONFIRM
    GPIO_EXTILineConfig(GPIO_PortSourceGPIOB, GPIO_PinSource1);  // KEY_ALARM
    GPIO_EXTILineConfig(GPIO_PortSourceGPIOB, GPIO_PinSource12); // KEY_RESET
    
    EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;         // 中断模式
    EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Falling;     // 下降沿触发
    EXTI_InitStructure.EXTI_LineCmd = ENABLE;
    
    EXTI_InitStructure.EXTI_Line = EXTI_Line5;  // KEY_MODE
    EXTI_Init(&EXTI_InitStructure);
    EXTI_InitStructure.EXTI_Line = EXTI_Line0;  // KEY_CONFIRM
    EXTI_Init(&EXTI_InitStructure);
    EXTI_InitStructure.EXTI_Line = EXTI_Line1;  // KEY_ALARM
    EXTI_Init(&EXTI_InitStructure);
    EXTI_InitStructure.EXTI_Line = EXTI_Line12; // KEY_RESET
    EXTI_Init(&EXTI_InitStructure);
    
    // 配置NVIC
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
    // 配置中断通道
    NVIC_InitStructure.NVIC_IRQChannel = EXTI9_5_IRQn;   // KEY_MODE
    NVIC_Init(&NVIC_InitStructure);
    NVIC_InitStructure.NVIC_IRQChannel = EXTI0_IRQn;     // KEY_CONFIRM
    NVIC_InitStructure.NVIC_IRQChannel = EXTI1_IRQn;     // KEY_ALARM
    NVIC_Init(&NVIC_InitStructure);
    NVIC_InitStructure.NVIC_IRQChannel = EXTI15_10_IRQn; // KEY_RESET
    NVIC_Init(&NVIC_InitStructure);
}

/**
  * @brief  按键扫描处理 主要用于按键状态的更新
  * @param  None
  * @retval None
  */
void Button_Scan(void)
{
    static uint32_t lastScanTime = 0;
    uint32_t currentTime;
    uint8_t i;
    currentTime = GetTickCount();
    
    // 按扫描间隔进行处理 小于扫描间隔则不处理
    if(currentTime - lastScanTime < BUTTON_SCAN_INTERVAL)
        return;

    // 更新扫描时间
    lastScanTime = currentTime;
    
    for(i = 0; i < KEY_COUNT; i++)
    {
        // 读取按键状态
        uint8_t pinState = GPIO_ReadInputDataBit(KEY_PORTS[i], KEY_PINS[i]);
        
        if(pinState == 0) // 按键按下
        {
            if(!keyInfo[i].isPressed) //isPressed为0表示按键未按下
            {
                keyInfo[i].isPressed = 1;
                keyInfo[i].pressTime = currentTime;
                keyInfo[i].state = BTN_DEBOUNCE;
            }
            else if(keyInfo[i].state == BTN_DEBOUNCE) // 按键处于消抖状态
            {
                if(currentTime - keyInfo[i].pressTime >= LONG_PRESS_TIME) // 按键按下时间大于长按时间
                {
                    keyInfo[i].state = BTN_LONG_PRESS; // 按键状态设置为长按状态
                }
            }
        }
        else // 按键释放
        {
            if(keyInfo[i].isPressed)
            {
                if(currentTime - keyInfo[i].pressTime >= DEBOUNCE_TIME)
                {
                    if(keyInfo[i].state != BTN_LONG_PRESS)
                    {
                        keyInfo[i].state = BTN_SHORT_PRESS;
                    }
                }
                keyInfo[i].isPressed = 0;
            }
        }
    }
}

/**
  * @brief  获取按键状态
  * @param  key: 按键编号
  * @retval ButtonState: 按键状态
  */
ButtonState Button_GetState(KeyID key)
{
    if(key >= KEY_COUNT)
        return BTN_IDLE;
    return keyInfo[key].state;
}

/**
  * @brief  清除按键状态
  * @param  key: 按键编号
  * @retval None
  */
void Button_ClearState(KeyID key)
{
    if(key < KEY_COUNT)
    {
        keyInfo[key].state = BTN_IDLE;
        keyInfo[key].isPressed = 0;
    }
}

// 按键中断服务函数
void EXTI9_5_IRQHandler(void)
{
    if(EXTI_GetITStatus(EXTI_Line5) != RESET)  // KEY_MODE
    {
        keyInfo[KEY_MODE].pressTime = GetTickCount();
        keyInfo[KEY_MODE].state = BTN_DEBOUNCE;
        keyInfo[KEY_MODE].isPressed = 1;
        EXTI_ClearITPendingBit(EXTI_Line5);
    }
    if(EXTI_GetITStatus(EXTI_Line0) != RESET)  // KEY_CONFIRM
    {
        keyInfo[KEY_CONFIRM].pressTime = GetTickCount();
        keyInfo[KEY_CONFIRM].state = BTN_DEBOUNCE;
        keyInfo[KEY_CONFIRM].isPressed = 1;
        EXTI_ClearITPendingBit(EXTI_Line0);
    }
}

void EXTI1_IRQHandler(void)
{
    if(EXTI_GetITStatus(EXTI_Line1) != RESET)  // KEY_ALARM
    {
        keyInfo[KEY_ALARM].pressTime = GetTickCount();
        keyInfo[KEY_ALARM].state = BTN_DEBOUNCE;
        keyInfo[KEY_ALARM].isPressed = 1;
        EXTI_ClearITPendingBit(EXTI_Line1);
    }
}

void EXTI15_10_IRQHandler(void)
{
    if(EXTI_GetITStatus(EXTI_Line12) != RESET)  // KEY_RESET
    {
        keyInfo[KEY_RESET].pressTime = GetTickCount();
        keyInfo[KEY_RESET].state = BTN_DEBOUNCE;
        keyInfo[KEY_RESET].isPressed = 1;
        EXTI_ClearITPendingBit(EXTI_Line12);
    }
}
