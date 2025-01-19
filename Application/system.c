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
    90};                // 舵机角度
//查看MPU6050初始化状态
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
  static ButtonState lastState = BTN_IDLE;
  ButtonState btnState;

  // 扫描按键
  Button_Scan();

  // 获取模式按键状态
  btnState = Button_GetState(KEY_MODE);
  //如果第一个按键的状态为短按，并且上一次状态不是短按，则切换模式
  if (btnState == BTN_SHORT_PRESS && lastState != BTN_SHORT_PRESS)
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
  lastState = btnState;

  // 在设置模式下，使用编码器调节参数
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
 * @brief  获取系统状态
 * @param  None
 * @retval SystemState*: 系统状态指针
 */
SystemState *System_GetState(void)
{
  return &systemState;
}
