#include "i2c.h"
#include "delay.h"
#include "pin_config.h"

// I2C引脚操作宏定义
#define I2C_SCL_HIGH()    GPIO_SetBits(MPU6050_PORT, MPU6050_SCL_PIN) // 设置SCL为高电平
#define I2C_SCL_LOW()     GPIO_ResetBits(MPU6050_PORT, MPU6050_SCL_PIN) // 设置SCL为低电平
#define I2C_SDA_HIGH()    GPIO_SetBits(MPU6050_PORT, MPU6050_SDA_PIN) // 设置SDA为高电平
#define I2C_SDA_LOW()     GPIO_ResetBits(MPU6050_PORT, MPU6050_SDA_PIN) // 设置SDA为低电平
#define I2C_SDA_READ()    GPIO_ReadInputDataBit(MPU6050_PORT, MPU6050_SDA_PIN) // 读取SDA引脚的电平

// 设置SDA为输入
static void SDA_IN(void)
{
    GPIO_InitTypeDef GPIO_InitStructure;
    GPIO_InitStructure.GPIO_Pin = MPU6050_SDA_PIN;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;  // 输入上拉
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(MPU6050_PORT, &GPIO_InitStructure);
}

// 设置SDA为输出
static void SDA_OUT(void)
{
    GPIO_InitTypeDef GPIO_InitStructure;
    GPIO_InitStructure.GPIO_Pin = MPU6050_SDA_PIN;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_OD; // 输出开漏
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(MPU6050_PORT, &GPIO_InitStructure);
}

/**
  * @brief  初始化I2C总线
  * @param  None
  * @retval None
  */
void I2C_Init_Software(void)
{
    GPIO_InitTypeDef GPIO_InitStructure;
    
    // 使能GPIO时钟
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);
    
    // 配置SCL和SDA引脚
    GPIO_InitStructure.GPIO_Pin = MPU6050_SCL_PIN | MPU6050_SDA_PIN;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_OD; // 输出开漏
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(MPU6050_PORT, &GPIO_InitStructure);
    
    // 空闲状态，SCL和SDA都为高电平
    I2C_SCL_HIGH();
    I2C_SDA_HIGH();
}

/**
  * @brief  产生I2C起始信号
  * @param  None
  * @retval None
  */
void I2C_Start(void)
{
    SDA_OUT();
    I2C_SDA_HIGH();
    I2C_SCL_HIGH();
    delay_us(4);
    I2C_SDA_LOW();
    delay_us(4);
    I2C_SCL_LOW();
}

/**
  * @brief  产生I2C停止信号
  * @param  None
  * @retval None
  */
void I2C_Stop(void)
{
    SDA_OUT();
    I2C_SCL_LOW();
    I2C_SDA_LOW();
    delay_us(4);
    I2C_SCL_HIGH();
    I2C_SDA_HIGH();
    delay_us(4);
}

/**
  * @brief  等待应答信号
  * @param  None
  * @retval 0:接收到应答 1:未接收到应答
  */
uint8_t I2C_WaitAck(void)
{
    uint8_t ucErrTime = 0;
    
    SDA_IN();
    I2C_SDA_HIGH();
    delay_us(1);
    I2C_SCL_HIGH();
    delay_us(1);
    
    while(I2C_SDA_READ())
    {
        ucErrTime++;
        if(ucErrTime > 250) //如果等待时间超过250us，则认为I2C总线连接失败
        {
            I2C_Stop();
            return 1;
        }
    }
    I2C_SCL_LOW();
    return 0;
}

/**
  * @brief  产生ACK应答
  * @param  None
  * @retval None
  */
void I2C_Ack(void)
{
    I2C_SCL_LOW();
    SDA_OUT();
    I2C_SDA_LOW();
    delay_us(2);
    I2C_SCL_HIGH();
    delay_us(2);
    I2C_SCL_LOW();
}

/**
  * @brief  不产生ACK应答
  * @param  None
  * @retval None
  */
void I2C_NAck(void)
{
    I2C_SCL_LOW();
    SDA_OUT();
    I2C_SDA_HIGH();
    delay_us(2);
    I2C_SCL_HIGH();
    delay_us(2);
    I2C_SCL_LOW();
}

/**
  * @brief  发送一个字节
  * @param  byte: 要发送的字节
  * @retval None
  */
void I2C_SendByte(uint8_t byte)
{
    uint8_t i;
    SDA_OUT();
    I2C_SCL_LOW();
    
    for(i = 0; i < 8; i++)
    {
        if(byte & 0x80)
            I2C_SDA_HIGH(); //发送的数据是1
        else
            I2C_SDA_LOW(); //发送的数据是0
        byte <<= 1;
        delay_us(2);
        I2C_SCL_HIGH();
        delay_us(2);
        I2C_SCL_LOW();
        delay_us(2);
    }
}

/**
  * @brief  读取一个字节
  * @param  ack: 1:发送ACK 0:发送NACK
  * @retval 读取到的数据
  */
uint8_t I2C_ReadByte(uint8_t ack)
{
    uint8_t i, receive = 0;
    
    SDA_IN();
    for(i = 0; i < 8; i++)
    {
        I2C_SCL_LOW();
        delay_us(2);
        I2C_SCL_HIGH();
        receive <<= 1;
        if(I2C_SDA_READ())
            receive++;
        delay_us(1);
    }
    
    if(!ack)
        I2C_NAck();
    else
        I2C_Ack();
    
    return receive;
}

/**
  * @brief  写入一个寄存器
  * @param  addr: 设备地址
  * @param  reg: 寄存器地址
  * @param  data: 要写入的数据
  * @retval 0:成功 1:失败
  */
uint8_t I2C_WriteReg(uint8_t addr, uint8_t reg, uint8_t data)
{
    I2C_Start(); 
    I2C_SendByte((addr << 1) | 0);  //左移1位，最后一位为0，表示写操作
    if(I2C_WaitAck()) //如果未接收到应答，则停止传输
    {
        I2C_Stop();
        return 1;
    }
    I2C_SendByte(reg);
    I2C_WaitAck();
    I2C_SendByte(data);
    if(I2C_WaitAck()) //如果未接收到应答，则停止传输
    {
        I2C_Stop();
        return 1;
    }
    I2C_Stop();
    return 0;
}

/**
  * @brief  读取一个寄存器
  * @param  addr: 设备地址
  * @param  reg: 寄存器地址
  * @retval 读取到的数据
  */
uint8_t I2C_ReadReg(uint8_t addr, uint8_t reg)
{
    uint8_t res;
    
    I2C_Start();
    I2C_SendByte((addr << 1) | 0);
    I2C_WaitAck();
    I2C_SendByte(reg);
    I2C_WaitAck();
    I2C_Start();
    I2C_SendByte((addr << 1) | 1);
    I2C_WaitAck();
    res = I2C_ReadByte(0);
    I2C_Stop();
    
    return res;
}

/**
  * @brief  连续读取多个字节
  * @param  addr: 设备地址
  * @param  reg: 寄存器地址
  * @param  len: 要读取的长度
  * @param  buf: 读取到的数据缓存
  * @retval 0:成功 1:失败
  */
uint8_t I2C_ReadMultBytes(uint8_t addr, uint8_t reg, uint8_t len, uint8_t *buf)
{
    I2C_Start();
    I2C_SendByte((addr << 1) | 0); //发送设备地址，左移1位，最后一位为0，表示写操作
    if(I2C_WaitAck())
    {
        I2C_Stop();
        return 1;
    }
    I2C_SendByte(reg);
    I2C_WaitAck();
    
    I2C_Start();
    I2C_SendByte((addr << 1) | 1);
    I2C_WaitAck();
    
    while(len)
    {
        if(len == 1)
            *buf = I2C_ReadByte(0);
        else
            *buf = I2C_ReadByte(1);
        len--;
        buf++;
    }
    I2C_Stop();
    return 0;
}
