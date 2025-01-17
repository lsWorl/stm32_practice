#include "button.h"
#include "delay.h"

// ������������
#define BUTTON_SCAN_INTERVAL    20      // ɨ������ms��
#define LONG_PRESS_TIME         3000    // ����ʱ�䣨ms��
#define DEBOUNCE_TIME          20       // ����ʱ�䣨ms��

// �������Ŷ���
static const uint16_t BTN_PINS[BTN_COUNT] = {
    BTN1_PIN,   // BTN1
    BTN2_PIN,   // BTN2
    BTN3_PIN,   // BTN3
    BTN4_PIN    // BTN4
};

static GPIO_TypeDef* const BTN_PORTS[BTN_COUNT] = {
    GPIOA,      // BTN1 - PA5
    GPIOA,      // BTN2 - PA6
    GPIOB,      // BTN3 - PB1
    GPIOB       // BTN4 - PB12
};

// ����״̬��¼
static ButtonState buttonStates[BTN_COUNT] = {0};
static uint32_t buttonPressTime[BTN_COUNT] = {0};
static uint32_t sysTickCount = 0;  // ϵͳ��ʱ������

// ��ȡϵͳ����ʱ�䣨ms��
static uint32_t GetSystemTime(void)
{
    return sysTickCount;
}

// SysTick�жϴ�����
void SysTick_Handler(void)
{
    sysTickCount++;
}

/**
  * @brief  ��ʼ������GPIO
  * @param  None
  * @retval None
  */
void Button_Init(void)
{
    GPIO_InitTypeDef GPIO_InitStructure;
    EXTI_InitTypeDef EXTI_InitStructure;
    NVIC_InitTypeDef NVIC_InitStructure;
    
    // ʹ��GPIOʱ��
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA | RCC_APB2Periph_GPIOB | RCC_APB2Periph_AFIO, ENABLE);
    
    // ���ð�������Ϊ��������
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    
    for(int i = 0; i < BTN_COUNT; i++)
    {
        GPIO_InitStructure.GPIO_Pin = BTN_PINS[i];
        GPIO_Init(BTN_PORTS[i], &GPIO_InitStructure);
    }
    
    // ����EXTI�ж�
    // BTN1 - PA5
    GPIO_EXTILineConfig(GPIO_PortSourceGPIOA, GPIO_PinSource5);
    // BTN2 - PA6
    GPIO_EXTILineConfig(GPIO_PortSourceGPIOA, GPIO_PinSource6);
    // BTN3 - PB1
    GPIO_EXTILineConfig(GPIO_PortSourceGPIOB, GPIO_PinSource1);
    // BTN4 - PB12
    GPIO_EXTILineConfig(GPIO_PortSourceGPIOB, GPIO_PinSource12);
    
    EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;
    EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Falling;
    EXTI_InitStructure.EXTI_LineCmd = ENABLE;
    
    EXTI_InitStructure.EXTI_Line = EXTI_Line5;  // BTN1
    EXTI_Init(&EXTI_InitStructure);
    EXTI_InitStructure.EXTI_Line = EXTI_Line6;  // BTN2
    EXTI_Init(&EXTI_InitStructure);
    EXTI_InitStructure.EXTI_Line = EXTI_Line1;  // BTN3
    EXTI_Init(&EXTI_InitStructure);
    EXTI_InitStructure.EXTI_Line = EXTI_Line12; // BTN4
    EXTI_Init(&EXTI_InitStructure);
    
    // ����NVIC
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
    
    NVIC_InitStructure.NVIC_IRQChannel = EXTI9_5_IRQn;  // BTN1, BTN2
    NVIC_Init(&NVIC_InitStructure);
    NVIC_InitStructure.NVIC_IRQChannel = EXTI1_IRQn;    // BTN3
    NVIC_Init(&NVIC_InitStructure);
    NVIC_InitStructure.NVIC_IRQChannel = EXTI15_10_IRQn;// BTN4
    NVIC_Init(&NVIC_InitStructure);
    
    // ����SysTickΪ1ms�ж�
    SysTick_Config(SystemCoreClock / 1000);
}

/**
  * @brief  ��ȡ��������״̬
  * @param  btnId: ����ID
  * @retval 0: ����, 1: �ͷ�
  */
static uint8_t Button_ReadPin(ButtonID btnId)
{
    return GPIO_ReadInputDataBit(BTN_PORTS[btnId], BTN_PINS[btnId]);
}

/**
  * @brief  ����ɨ�账��
  * @param  None
  * @retval None
  */
void Button_Scan(void)
{
    static uint32_t lastScanTime = 0;
    uint32_t currentTime = GetSystemTime();
    
    // ��ɨ�������д���
    if(currentTime - lastScanTime < BUTTON_SCAN_INTERVAL)
        return;
    
    lastScanTime = currentTime;
    
    for(int i = 0; i < BTN_COUNT; i++)
    {
        if(Button_ReadPin(i) == 0)  // ��������
        {
            if(buttonStates[i] == BTN_RELEASED)
            {
                buttonStates[i] = BTN_PRESSED;
                buttonPressTime[i] = currentTime;
            }
            else if(buttonStates[i] == BTN_PRESSED && 
                    (currentTime - buttonPressTime[i] >= LONG_PRESS_TIME))
            {
                buttonStates[i] = BTN_LONG_PRESSED;
            }
        }
        else  // �����ͷ�
        {
            if(buttonStates[i] == BTN_PRESSED)
            {
                if(currentTime - buttonPressTime[i] >= DEBOUNCE_TIME)
                {
                    buttonStates[i] = BTN_SHORT_PRESSED;
                }
            }
            else if(buttonStates[i] != BTN_RELEASED)
            {
                buttonStates[i] = BTN_RELEASED;
            }
        }
    }
}

/**
  * @brief  ��ȡ����״̬
  * @param  btnId: ����ID
  * @retval ButtonState: ����״̬
  */
ButtonState Button_GetState(ButtonID btnId)
{
    if(btnId >= BTN_COUNT)
        return BTN_RELEASED;
    return buttonStates[btnId];
}

/**
  * @brief  �������״̬
  * @param  btnId: ����ID
  * @retval None
  */
void Button_ClearState(ButtonID btnId)
{
    if(btnId < BTN_COUNT)
        buttonStates[btnId] = BTN_RELEASED;
}

// BTN1��BTN2���жϷ�����
void EXTI9_5_IRQHandler(void)
{
    if(EXTI_GetITStatus(EXTI_Line5) != RESET)  // BTN1
    {
        buttonPressTime[BTN1] = GetSystemTime();
        buttonStates[BTN1] = BTN_PRESSED;
        EXTI_ClearITPendingBit(EXTI_Line5);
    }
    if(EXTI_GetITStatus(EXTI_Line6) != RESET)  // BTN2
    {
        buttonPressTime[BTN2] = GetSystemTime();
        buttonStates[BTN2] = BTN_PRESSED;
        EXTI_ClearITPendingBit(EXTI_Line6);
    }
}

// BTN3���жϷ�����
void EXTI1_IRQHandler(void)
{
    if(EXTI_GetITStatus(EXTI_Line1) != RESET)
    {
        buttonPressTime[BTN3] = GetSystemTime();
        buttonStates[BTN3] = BTN_PRESSED;
        EXTI_ClearITPendingBit(EXTI_Line1);
    }
}

// BTN4���жϷ�����
void EXTI15_10_IRQHandler(void)
{
    if(EXTI_GetITStatus(EXTI_Line12) != RESET)
    {
        buttonPressTime[BTN4] = GetSystemTime();
        buttonStates[BTN4] = BTN_PRESSED;
        EXTI_ClearITPendingBit(EXTI_Line12);
    }
}
