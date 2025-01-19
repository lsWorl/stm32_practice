#include "button.h"
#include "delay.h"

// ������������
#define BUTTON_SCAN_INTERVAL    20      // ɨ������ms��
#define LONG_PRESS_TIME         2000    // ����ʱ�䣨ms��
#define DEBOUNCE_TIME          20       // ����ʱ�䣨ms��



// �������Ŷ���
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

// ����״̬��¼
static struct {
    ButtonState state;      // �����߼�״̬
    uint32_t pressTime;     // ����ʱ��
    uint8_t isPressed;      // �����Ƿ��ڰ���״̬
} keyInfo[KEY_COUNT] = {BTN_IDLE, 0, 0};



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
    uint8_t i;
    
    // ʹ��GPIOʱ��
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA | RCC_APB2Periph_GPIOB | RCC_APB2Periph_AFIO, ENABLE);
    
    // ���ð�������Ϊ��������
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    
    //��һ���ð�������
    for(i = 0; i < KEY_COUNT; i++)
    {
        GPIO_InitStructure.GPIO_Pin = KEY_PINS[i];
        GPIO_Init(KEY_PORTS[i], &GPIO_InitStructure);
        keyInfo[i].state = BTN_IDLE;
        keyInfo[i].isPressed = 0;
    }
    
    // ����EXTI�ж�
    GPIO_EXTILineConfig(GPIO_PortSourceGPIOA, GPIO_PinSource5);  // KEY_MODE
    GPIO_EXTILineConfig(GPIO_PortSourceGPIOB, GPIO_PinSource0);  // KEY_CONFIRM
    GPIO_EXTILineConfig(GPIO_PortSourceGPIOB, GPIO_PinSource1);  // KEY_ALARM
    GPIO_EXTILineConfig(GPIO_PortSourceGPIOB, GPIO_PinSource12); // KEY_RESET
    
    EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;         // �ж�ģʽ
    EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Falling;     // �½��ش���
    EXTI_InitStructure.EXTI_LineCmd = ENABLE;
    
    EXTI_InitStructure.EXTI_Line = EXTI_Line5;  // KEY_MODE
    EXTI_Init(&EXTI_InitStructure);
    EXTI_InitStructure.EXTI_Line = EXTI_Line0;  // KEY_CONFIRM
    EXTI_Init(&EXTI_InitStructure);
    EXTI_InitStructure.EXTI_Line = EXTI_Line1;  // KEY_ALARM
    EXTI_Init(&EXTI_InitStructure);
    EXTI_InitStructure.EXTI_Line = EXTI_Line12; // KEY_RESET
    EXTI_Init(&EXTI_InitStructure);
    
    // ����NVIC
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
    // �����ж�ͨ��
    NVIC_InitStructure.NVIC_IRQChannel = EXTI9_5_IRQn;   // KEY_MODE
    NVIC_Init(&NVIC_InitStructure);
    NVIC_InitStructure.NVIC_IRQChannel = EXTI0_IRQn;     // KEY_CONFIRM
    NVIC_InitStructure.NVIC_IRQChannel = EXTI1_IRQn;     // KEY_ALARM
    NVIC_Init(&NVIC_InitStructure);
    NVIC_InitStructure.NVIC_IRQChannel = EXTI15_10_IRQn; // KEY_RESET
    NVIC_Init(&NVIC_InitStructure);
}

/**
  * @brief  ����ɨ�账�� ��Ҫ���ڰ���״̬�ĸ���
  * @param  None
  * @retval None
  */
void Button_Scan(void)
{
    static uint32_t lastScanTime = 0;
    uint32_t currentTime;
    uint8_t i;
    currentTime = GetTickCount();
    
    // ��ɨ�������д��� С��ɨ�����򲻴���
    if(currentTime - lastScanTime < BUTTON_SCAN_INTERVAL)
        return;

    // ����ɨ��ʱ��
    lastScanTime = currentTime;
    
    for(i = 0; i < KEY_COUNT; i++)
    {
        // ��ȡ����״̬
        uint8_t pinState = GPIO_ReadInputDataBit(KEY_PORTS[i], KEY_PINS[i]);
        
        if(pinState == 0) // ��������
        {
            if(!keyInfo[i].isPressed) //isPressedΪ0��ʾ����δ����
            {
                keyInfo[i].isPressed = 1;
                keyInfo[i].pressTime = currentTime;
                keyInfo[i].state = BTN_DEBOUNCE;
            }
            else if(keyInfo[i].state == BTN_DEBOUNCE) // ������������״̬
            {
                if(currentTime - keyInfo[i].pressTime >= LONG_PRESS_TIME) // ��������ʱ����ڳ���ʱ��
                {
                    keyInfo[i].state = BTN_LONG_PRESS; // ����״̬����Ϊ����״̬
                }
            }
        }
        else // �����ͷ�
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
  * @brief  ��ȡ����״̬
  * @param  key: �������
  * @retval ButtonState: ����״̬
  */
ButtonState Button_GetState(KeyID key)
{
    if(key >= KEY_COUNT)
        return BTN_IDLE;
    return keyInfo[key].state;
}

/**
  * @brief  �������״̬
  * @param  key: �������
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

// �����жϷ�����
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
