#include "system.h"
#include "delay.h"
#include "pin_config.h"
// 系统状态变量
static SystemState systemState = {
    SYS_MODE_MONITOR,   // 监测模式
    0,                  // 是否处于报警状态
    0,                  // 是否已初始化
    0.0f,               // 温度值
    0,                  // 光照等级
    {0.0f, 0.0f, 0.0f}, // 姿态数据
    30.0f,              // 温度报警阈值
    90};                // 舵机初始角度
// 查看MPU6050初始化状态
uint8_t init_status;
/**
 * @brief  系统初始化
 * @param  None
 * @retval None
 */
void System_Init(void)
{
  // 初始化系统时钟
  SystemInit();
  // 初始化延时函数
  delay_init();

  // 初始化所有外设
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
    // 显示MPU6050的ID 如果ID读取到的是FF，则表示I2C总线连接失败
    OLED_ShowHexNum(2, 1, MPU6050_ReadID(), 2);
    while (1)
      ;
  }
  // LED自检
  LED_AllOn();
  delay_ms(500);
  LED_AllOff();

  // OLED显示欢迎信息
  OLED_Clear();
  OLED_ShowString(1, 1, "System Init...");
  OLED_ShowString(2, 1, "Welcome!");
  delay_ms(1000);

  // 设置系统状态
  systemState.isInitialized = 1;
  System_SetMode(SYS_MODE_MONITOR);
}

/**
 * @brief  系统主处理函数
 * @param  None
 * @retval None
 */
void System_Process(void)
{
  // 更新传感器数据
  systemState.temperature = TempSensor_GetTemp();
  systemState.lightLevel = LightSensor_GetPercent();
  MPU6050_GetAttitude(&systemState.attitude);

  // 处理用户输入
  System_HandleInput();

  // 检查报警条件
  System_CheckAlarm();

  // 更新显示
  System_UpdateDisplay();

  // 舵机转动
  Servo_Running();

  // 系统运行指示
  LED_SystemRunning();
}

/**
 * @brief  设置系统模式
 * @param  mode: 目标模式
 * @retval None
 */
void System_SetMode(SystemMode mode)
{
  systemState.mode = mode;

  // 清屏并显示模式标题
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
 * @brief  更新显示
 * @param  None
 * @retval None
 */
void System_UpdateDisplay(void)
{
  switch (systemState.mode)
  {
  case SYS_MODE_MONITOR:
    // 显示所有传感器数据
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
    // 显示设置参数
    OLED_ShowString(2, 1, "Temp Threshold:");
    OLED_ShowNum(2, 15, (uint16_t)systemState.tempThreshold, 2);

    OLED_ShowString(3, 1, "Servo Angle:");
    OLED_ShowNum(3, 13, systemState.servoAngle, 3);
    break;

  case SYS_MODE_DEBUG:
    // 显示调试信息
    OLED_ShowString(1, 1, "Debug Mode");
    OLED_ShowNum(2, 1, (uint16_t)systemState.tempThreshold, 2);
    OLED_ShowNum(3, 1, (uint16_t)Encoder_GetCount(), 3);
    break;
  }
}

/**
 * @brief  检查报警条件
 * @param  None
 * @retval None
 */
void System_CheckAlarm(void)
{
  // 检查温度是否超过阈值
  if (systemState.temperature > systemState.tempThreshold)
  {
    if (!systemState.isAlarming)
    {
      systemState.isAlarming = 1;
      Beeper_Beep(500); // 蜂鸣器报警
    }
    LED_AlarmStatus(); // LED闪烁
  }
  else
  {
    systemState.isAlarming = 0;
    LED_Off(LED4_PIN);
  }
}

/**
 * @brief  处理用户输入
 * @param  None
 * @retval None
 */
void System_HandleInput(void)
{
  static ButtonState ModeButton_lastState = BTN_IDLE,
                     ConfirmButton_lastState = BTN_IDLE,
                     AlarmButton_lastState = BTN_IDLE,
                     ResetButton_lastState = BTN_IDLE;
  ButtonState ModeButton_btnState, ConfirmButton_btnState,
      AlarmButton_btnState,
      ResetButton_btnState;
  EncoderState encState;
  static uint8_t alarm_flag = 0;

  // 扫描按键
  Button_Scan();
  // 获取所有按键状态
  ModeButton_btnState = Button_GetState(KEY_MODE);
  ConfirmButton_btnState = Button_GetState(KEY_CONFIRM);
  AlarmButton_btnState = Button_GetState(KEY_ALARM);
  ResetButton_btnState = Button_GetState(KEY_RESET);
  // 如果第一个按键的状态为短按，并且上一次状态不是短按，则切换模式
  if (ModeButton_btnState == BTN_SHORT_PRESS && ModeButton_lastState != BTN_SHORT_PRESS)
  {
    // 切换模式
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

  // 如果第二个按键的状态为短按，并且上一次状态不是短按，则确认设置
  if (ConfirmButton_btnState == BTN_SHORT_PRESS && ConfirmButton_lastState != BTN_SHORT_PRESS)
  {
    // 若在设置模式下，则确认设置值
    if (systemState.mode == SYS_MODE_SETTING)
    {
      // 直接转回监测模式
      System_SetMode(SYS_MODE_MONITOR);
    }
  }
  // 如果第三个按键的状态为短按，并且上一次状态不是短按，则切换模式
  if (AlarmButton_btnState == BTN_SHORT_PRESS && AlarmButton_lastState != BTN_SHORT_PRESS){
    if(alarm_flag)
      alarm_flag = 0;
    else
      alarm_flag = 1;
  }

  // 按下第四个按钮将所有值复位
  if (ResetButton_btnState == BTN_SHORT_PRESS && ResetButton_lastState != BTN_SHORT_PRESS)
  {
    MPU6050_Attitude defaultAttitude = {0.0f, 0.0f, 0.0f};
    systemState.attitude = defaultAttitude;
    systemState.isAlarming = 0;
    systemState.isInitialized = 1;
    systemState.lightLevel = 0;
    systemState.servoAngle = 90;
    systemState.temperature = 0.0f;
    systemState.tempThreshold = 30.0f;
    System_SetMode(SYS_MODE_MONITOR);
    OLED_Clear();
  }

  // 在设置模式下，使用编码器调节参数
  if (systemState.mode == SYS_MODE_SETTING)
  {
    encState = Encoder_GetState();
    // 如果报警标志为1，则调节舵机角度
    if (alarm_flag)
    {
      // 调节舵机角度
      if (encState == ENCODER_CW)
      {
        systemState.servoAngle += 1;
        if (systemState.servoAngle > 180)
          systemState.servoAngle = 180;
      }
      else if (encState == ENCODER_CCW)
      {
        systemState.servoAngle -= 1;
        if (systemState.servoAngle == 0)
          systemState.servoAngle = 0;
      }
    }
    else
    {
      // 调节温度门槛
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

  // 更新按键状态
  ModeButton_lastState = ModeButton_btnState;
  ConfirmButton_lastState = ConfirmButton_btnState;
  AlarmButton_lastState = AlarmButton_btnState;
  ResetButton_lastState = ResetButton_btnState;
}

/**
 * @brief  获取系统状态
 * @param  None
 * @retval SystemState*: 系统状态指针
 */
SystemState *System_GetState(void)
{
  return &systemState;
}

/**
  * @brief  超过阈值，则转动舵机
  * @param  None
  * @retval None
  */
void Servo_Running(void){
  if(systemState.temperature > systemState.tempThreshold)
    Servo_SetAngle(systemState.servoAngle);
  else
    Servo_SetAngle(90);
}
