#include "system.h"
#include "delay.h"
#include "pin_config.h"
// ϵͳ״̬����
static SystemState systemState = {
    SYS_MODE_MONITOR,   // ���ģʽ
    0,                  // �Ƿ��ڱ���״̬
    0,                  // �Ƿ��ѳ�ʼ��
    0.0f,               // �¶�ֵ
    0,                  // ���յȼ�
    {0.0f, 0.0f, 0.0f}, // ��̬����
    30.0f,              // �¶ȱ�����ֵ
    90};                // ����Ƕ�
//�鿴MPU6050��ʼ��״̬
uint8_t init_status;
/**
 * @brief  ϵͳ��ʼ��
 * @param  None
 * @retval None
 */
void System_Init(void)
{
  // ��ʼ��ϵͳʱ��
  SystemInit();
  // ��ʼ����ʱ����
  delay_init();

  // ��ʼ����������
  LED_Init();
  OLED_Init();
  Beeper_Init();
  LightSensor_Init();
  TempSensor_Init();
  
  Encoder_Init();
  Servo_Init();
  Button_Init();
  init_status = MPU6050_Init();
  if (init_status != 0)
    {

        OLED_ShowString(1, 1, "MPU6050 Error!");
        // ��ʾMPU6050��ID ���ID��ȡ������FF�����ʾI2C��������ʧ��
        OLED_ShowHexNum(2, 1, MPU6050_ReadID(), 2);
        while (1)
            ;
    }
  // LED�Լ�
  LED_AllOn();
  delay_ms(500);
  LED_AllOff();

  // OLED��ʾ��ӭ��Ϣ
  OLED_Clear();
  OLED_ShowString(1, 1, "System Init...");
  OLED_ShowString(2, 1, "Welcome!");
  delay_ms(1000);
  
  // ����ϵͳ״̬
  systemState.isInitialized = 1;
  System_SetMode(SYS_MODE_MONITOR);
}

/**
 * @brief  ϵͳ��������
 * @param  None
 * @retval None
 */
void System_Process(void)
{
  // ���´���������
  systemState.temperature = TempSensor_GetTemp();
  systemState.lightLevel = LightSensor_GetPercent();
  MPU6050_GetAttitude(&systemState.attitude);

  // �����û�����
  System_HandleInput();

  // ��鱨������
  System_CheckAlarm();

  // ������ʾ
  System_UpdateDisplay();

  // ϵͳ����ָʾ
  LED_SystemRunning();
}

/**
 * @brief  ����ϵͳģʽ
 * @param  mode: Ŀ��ģʽ
 * @retval None
 */
void System_SetMode(SystemMode mode)
{
  systemState.mode = mode;

  // ��������ʾģʽ����
  OLED_Clear();
  switch (mode)
  {
  case SYS_MODE_MONITOR:
    OLED_ShowString(1, 1, "Monitor Mode");
    LED_Off(LED2_PIN | LED3_PIN);
    OLED_Clear();
    break;

  case SYS_MODE_SETTING:
    OLED_ShowString(1, 1, "Setting Mode");
    LED_On(LED2_PIN);
    LED_Off(LED3_PIN);
    OLED_Clear();
    break;

  case SYS_MODE_DEBUG:
    OLED_ShowString(1, 1, "Debug Mode");
    LED_On(LED3_PIN);
    LED_Off(LED2_PIN);
    OLED_Clear();
    break;
  }
  delay_ms(500);
}

/**
 * @brief  ������ʾ
 * @param  None
 * @retval None
 */
void System_UpdateDisplay(void)
{
  switch (systemState.mode)
  {
  case SYS_MODE_MONITOR:
    // ��ʾ���д���������
    OLED_ShowString(1, 1, "Temp:");
    OLED_ShowNum(1, 6, (uint16_t)systemState.temperature, 2);
    OLED_ShowString(1, 8, ".");
    OLED_ShowNum(1, 9, (uint16_t)(systemState.temperature * 10) % 10, 1);

    OLED_ShowString(2, 1, "Light:");
    OLED_ShowNum(2, 7, systemState.lightLevel, 3);
    OLED_ShowString(2, 10, "%");

    OLED_ShowString(3, 1, "Pitch:");
    OLED_ShowSignedNum(3, 7, (int16_t)systemState.attitude.pitch, 3);

    OLED_ShowString(4, 1, "Roll:");
    OLED_ShowSignedNum(4, 6, (int16_t)systemState.attitude.roll, 3);
    break;

  case SYS_MODE_SETTING:
    // ��ʾ���ò���
    OLED_ShowString(2, 1, "Temp Threshold:");
    OLED_ShowNum(2, 15, (uint16_t)systemState.tempThreshold, 2);

    OLED_ShowString(3, 1, "Servo Angle:");
    OLED_ShowNum(3, 13, systemState.servoAngle, 3);
    break;

  case SYS_MODE_DEBUG:
    // ��ʾ������Ϣ
    OLED_ShowString(1, 1, "Debug Mode");
    OLED_ShowNum(2, 1, (uint16_t)systemState.tempThreshold, 2);
    OLED_ShowNum(3, 1, (uint16_t)Encoder_GetCount(), 3);
    break;
  }
}

/**
 * @brief  ��鱨������
 * @param  None
 * @retval None
 */
void System_CheckAlarm(void)
{
  // ����¶��Ƿ񳬹���ֵ
  if (systemState.temperature > systemState.tempThreshold)
  {
    if (!systemState.isAlarming)
    {
      systemState.isAlarming = 1;
      Beeper_Beep(500); // ����������
    }
    LED_AlarmStatus(); // LED��˸
  }
  else
  {
    systemState.isAlarming = 0;
    LED_Off(LED4_PIN);
  }
}

/**
 * @brief  �����û�����
 * @param  None
 * @retval None
 */
void System_HandleInput(void)
{
  static ButtonState lastState = BTN_IDLE;
  ButtonState btnState;

  // ɨ�谴��
  Button_Scan();

  // ��ȡģʽ����״̬
  btnState = Button_GetState(KEY_MODE);
  //�����һ��������״̬Ϊ�̰���������һ��״̬���Ƕ̰������л�ģʽ
  if (btnState == BTN_SHORT_PRESS && lastState != BTN_SHORT_PRESS)
  {
    // �л�ģʽ
    switch (systemState.mode)
    {
    case SYS_MODE_MONITOR:
      System_SetMode(SYS_MODE_SETTING);
      break;
    case SYS_MODE_SETTING:
      System_SetMode(SYS_MODE_DEBUG);
      break;
    case SYS_MODE_DEBUG:
      System_SetMode(SYS_MODE_MONITOR);
      break;
    }
  }
  lastState = btnState;

  // ������ģʽ�£�ʹ�ñ��������ڲ���
  if (systemState.mode == SYS_MODE_SETTING)
  {
    EncoderState encState = Encoder_GetState();
    if (encState == ENCODER_CW)
    {
      systemState.tempThreshold += 0.5f;
      if (systemState.tempThreshold > 50.0f)
        systemState.tempThreshold = 50.0f;
    }
    else if (encState == ENCODER_CCW)
    {
      systemState.tempThreshold -= 0.5f;
      if (systemState.tempThreshold < 0.0f)
        systemState.tempThreshold = 0.0f;
    }
  }
}

/**
 * @brief  ��ȡϵͳ״̬
 * @param  None
 * @retval SystemState*: ϵͳ״ָ̬��
 */
SystemState *System_GetState(void)
{
  return &systemState;
}
