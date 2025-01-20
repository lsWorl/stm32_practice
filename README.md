# STM32F103C8T6 综合学习项目

## 项目概述
这是一个基于STM32F103C8T6单片机的综合学习项目,使用标准库实现，集成了多种常用外设和功能模块，用于学习和实践嵌入式系统开发。

## 硬件资源
- MCU: STM32F103C8T6 (72MHz, 64KB Flash, 20KB RAM)
- 显示: 0.96寸OLED显示屏 (I2C接口)
- 传感器:
  - MPU6050 六轴姿态传感器 (I2C接口)
  - 光敏电阻传感器 (ADC采集)
  - NTC热敏电阻传感器 (ADC采集)
- 输入设备:
  - 旋转编码器 (带按键)
  - 4个独立按键
- 输出设备:
  - 4个LED指示灯
  - 有源蜂鸣器
  - SG90舵机

## 已实现功能

### 1. 硬件驱动层
- [x] LED模块
  - 4个LED指示不同工作状态
  - 支持开关、翻转操作
  - 系统运行、数据采集、设置模式、报警状态指示

- [x] 蜂鸣器模块
  - 支持持续时间可控的蜂鸣
  - 非阻塞工作模式
  - 用于报警提示

- [x] 光敏传感器模块
  - ADC采集光照强度
  - 支持原始值、百分比、等级输出
  - 4级光照强度判断

- [x] 温度传感器模块
  - ADC采集温度
  - 支持温度值计算
  - 4级温度等级判断
  - 温度报警功能

- [x] MPU6050姿态传感器
  - I2C通信
  - 支持读取加速度、角速度
  - 姿态角度计算（俯仰角、横滚角）

- [x] 旋转编码器
  - 正反转检测
  - 用于参数调节

- [x] 舵机控制
  - PWM控制
  - 角度范围0-180度
  - 支持平滑控制

- [x] 按键模块
  - 支持4个独立按键
  - 消抖处理
  - 支持短按、长按检测

### 2. 中间件层
- [x] I2C通信
  - 软件模拟I2C
  - 支持多字节读写
  - 总线状态检测

- [x] ADC采集
  - 支持多通道采样
  - 12位分辨率

- [x] PWM输出
  - 用于舵机控制
  - 精确的脉冲宽度控制

### 3. 应用层
- [x] 系统控制模块
  - 三种工作模式（监测、设置、调试）
  - 传感器数据采集和处理
  - 显示界面管理
  - 用户输入处理
  - 报警功能

## 工作模式说明
1. 监测模式
   - 显示所有传感器数据
   - 实时监测环境参数
   - LED1指示系统运行状态

2. 设置模式
   - 调节温度报警阈值
   - 调节舵机角度
   - LED2指示设置模式

3. 调试模式
   - 测试单个模块功能
   - 显示调试信息
   - LED3指示调试模式

## 操作说明
1. 按键功能
   - MODE键（KEY1）：切换工作模式
   - CONFIRM键（KEY2）：确认设置
   - ALARM键（KEY3）：报警确认/切换设置项
   - RESET键（KEY4）：系统复位

2. 编码器操作
   - 顺时针旋转：增加参数值
   - 逆时针旋转：减小参数值

3. LED指示
   - LED1：系统运行指示
   - LED2：设置模式指示
   - LED3：调试模式指示
   - LED4：报警状态指示

## 项目结构
```
Project/
├── User/
│   ├── main.c
│   └── stm32f10x_it.c
├── Hardware/
│   ├── LED/
│   ├── OLED/
│   ├── MPU6050/
│   ├── Encoder/
│   ├── Servo/
│   ├── Button/
│   ├── Beeper/
│   ├── LightSensor/
│   └── TempSensor/
├── Middlewares/
│   ├── I2C/
│   ├── ADC/
│   └── PWM/
└── Application/
    └── System/
```

## 硬件连接
| 引脚 | 功能         | 连接说明               |
|------|--------------|------------------------|
| PA0  | ADC输入      | 连接热敏电阻分压电路    |
| PA3  | TIM2_CH4输出 | 连接舵机信号线          |
| I2C  | OLED显示屏   | 连接0.96寸OLED显示屏    |

## 实现原理

### 温度测量原理
1. 使用热敏电阻分压电路
2. 通过ADC采集电压值
3. 使用Steinhart-Hart方程计算温度：
   - 1/T = 1/T0 + (1/B)*ln(R/R0)
   - T0: 参考温度（25°C = 298.15K）
   - R0: 参考温度下的电阻值
   - B: 热敏电阻B值
   - R: 当前电阻值

### PWM控制原理
1. 使用TIM2产生50Hz PWM信号
2. 脉宽范围：0.5ms ~ 2.5ms
   - 0.5ms对应0度
   - 1.5ms对应90度
   - 2.5ms对应180度
3. 通过修改TIM2_CCR4寄存器值改变脉宽

### DMA工作原理
1. 使用DMA1通道1
2. 自动将ADC转换结果传输到内存
3. 循环模式，持续更新采样数据
4. 减少CPU开销，提高采样效率

## 代码结构说明

### 主要功能模块
1. **GPIO配置**
   - 配置PA0为模拟输入
   - 配置PA3为PWM输出

2. **DMA配置**
   - 使用DMA1通道1实现ADC数据自动传输
   - 循环模式，持续更新ADC采样数据

3. **ADC配置**
   - 独立模式
   - 连续转换
   - 使用DMA传输
   - 校准ADC

4. **温度计算**
   - 使用Steinhart-Hart方程
   - 10次采样取平均
   - 温度范围：-40°C ~ +125°C

5. **PWM控制**
   - 使用TIM2产生50Hz PWM信号
   - 脉宽范围：0.5ms ~ 2.5ms
   - 对应舵机角度：0° ~ 180°

### 关键函数说明

#### GPIO_Config()
- 功能：配置GPIO引脚
- 参数：无
- 说明：
  - 配置PA0为模拟输入
  - 配置PA3为TIM2_CH4输出

#### TIM2_PWM_Config()
- 功能：配置TIM2 PWM输出
- 参数：无
- 代码详解：
  ```c
  // 配置TIM2时基
  TIM_TimeBaseStructure.TIM_Period = 19999;  // 20ms周期
  TIM_TimeBaseStructure.TIM_Prescaler = 71;  // 72MHz/(71+1) = 1MHz
  TIM_TimeBaseStructure.TIM_ClockDivision = 0;
  TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;
  TIM_TimeBaseInit(TIM2, &TIM_TimeBaseStructure);

  // 配置TIM2通道4 PWM模式
  TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1;
  TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;
  TIM_OCInitStructure.TIM_Pulse = 1500;  // 初始脉宽1.5ms
  TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High;
  TIM_OC4Init(TIM2, &TIM_OCInitStructure);

  // 使能TIM2
  TIM_Cmd(TIM2, ENABLE);
  ```
- 说明：
  - 配置50Hz PWM信号
  - 使用TIM2通道4
  - 初始位置90度（1.5ms脉宽）

#### DMA_Config()
- 功能：配置DMA传输
- 参数：无
- 说明：
  - 使用DMA1通道1
  - 循环模式
  - 自动传输ADC数据

#### ADC_Config()
- 功能：配置ADC
- 参数：无
- 说明：
  - 独立模式
  - 连续转换
  - 使用DMA传输
  - 校准ADC

#### GetTemperature()
- 功能：获取当前温度
- 返回值：float类型温度值
- 代码详解：
  ```c
  uint32_t sum = 0;
  // 计算10次采样平均值
  for(int i = 0; i < ADC_BUFFER_SIZE; i++) {
      sum += ADC_ConvertedValue[i];
  }
  uint16_t adc_value = sum / ADC_BUFFER_SIZE;
  
  // 计算电压值
  float voltage = (float)adc_value * VREF / 4096.0f;
  
  // 计算热敏电阻阻值
  float rt = R1 * voltage / (VREF - voltage);
  
  // 计算温度（Steinhart-Hart方程）
  float temperature = 1.0f / (1.0f/T0 + log(rt/R0)/B) - 273.15f;
  ```
- 说明：
  - 使用Steinhart-Hart方程计算温度
  - 10次采样取平均
  - 温度范围：-40°C ~ +125°C

#### SetServoAngle()
- 功能：设置舵机角度
- 参数：angle - 目标角度（0-180）
- 代码详解：
  ```c
  // 将角度转换为脉宽（单位：us）
  uint16_t pulse_width = 500 + angle * 2000 / 180;
  
  // 将脉宽转换为TIM2计数值
  uint16_t compare_value = pulse_width * 72 / 1000;
  
  // 更新TIM2比较寄存器
  TIM_SetCompare4(TIM2, compare_value);
  ```
- 说明：
  - 将角度转换为PWM脉宽
  - 更新TIM2比较寄存器
  - 50Hz PWM信号，周期20ms

## 使用说明
1. 编译并下载程序到开发板
2. 连接硬件设备
3. 上电后OLED显示"Hello world"
4. 温度实时显示在第二行
5. 舵机自动进行0-180度往复运动

## 注意事项
1. 确保热敏电阻分压电路正确连接
2. 舵机供电电压需匹配
3. OLED显示屏需正确连接I2C接口
4. 温度测量精度受热敏电阻精度影响
