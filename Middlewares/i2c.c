#include "i2c.h"
#include "delay.h"
#include "pin_config.h"

// I2C���Ų����궨��
#define I2C_SCL_HIGH()    GPIO_SetBits(MPU6050_PORT, MPU6050_SCL_PIN) // ����SCLΪ�ߵ�ƽ
#define I2C_SCL_LOW()     GPIO_ResetBits(MPU6050_PORT, MPU6050_SCL_PIN) // ����SCLΪ�͵�ƽ
#define I2C_SDA_HIGH()    GPIO_SetBits(MPU6050_PORT, MPU6050_SDA_PIN) // ����SDAΪ�ߵ�ƽ
#define I2C_SDA_LOW()     GPIO_ResetBits(MPU6050_PORT, MPU6050_SDA_PIN) // ����SDAΪ�͵�ƽ
#define I2C_SDA_READ()    GPIO_ReadInputDataBit(MPU6050_PORT, MPU6050_SDA_PIN) // ��ȡSDA���ŵĵ�ƽ

// ����SDAΪ����
static void SDA_IN(void)
{
    GPIO_InitTypeDef GPIO_InitStructure;
    GPIO_InitStructure.GPIO_Pin = MPU6050_SDA_PIN;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;  // ��������
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(MPU6050_PORT, &GPIO_InitStructure);
}

// ����SDAΪ���
static void SDA_OUT(void)
{
    GPIO_InitTypeDef GPIO_InitStructure;
    GPIO_InitStructure.GPIO_Pin = MPU6050_SDA_PIN;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_OD; // �����©
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(MPU6050_PORT, &GPIO_InitStructure);
}

/**
  * @brief  ��ʼ��I2C����
  * @param  None
  * @retval None
  */
void I2C_Init_Software(void)
{
    GPIO_InitTypeDef GPIO_InitStructure;
    
    // ʹ��GPIOʱ��
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);
    
    // ����SCL��SDA����
    GPIO_InitStructure.GPIO_Pin = MPU6050_SCL_PIN | MPU6050_SDA_PIN;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_OD; // �����©
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(MPU6050_PORT, &GPIO_InitStructure);
    
    // ����״̬��SCL��SDA��Ϊ�ߵ�ƽ
    I2C_SCL_HIGH();
    I2C_SDA_HIGH();
}

/**
  * @brief  ����I2C��ʼ�ź�
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
  * @brief  ����I2Cֹͣ�ź�
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
  * @brief  �ȴ�Ӧ���ź�
  * @param  None
  * @retval 0:���յ�Ӧ�� 1:δ���յ�Ӧ��
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
        if(ucErrTime > 250) //����ȴ�ʱ�䳬��250us������ΪI2C��������ʧ��
        {
            I2C_Stop();
            return 1;
        }
    }
    I2C_SCL_LOW();
    return 0;
}

/**
  * @brief  ����ACKӦ��
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
  * @brief  ������ACKӦ��
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
  * @brief  ����һ���ֽ�
  * @param  byte: Ҫ���͵��ֽ�
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
            I2C_SDA_HIGH(); //���͵�������1
        else
            I2C_SDA_LOW(); //���͵�������0
        byte <<= 1;
        delay_us(2);
        I2C_SCL_HIGH();
        delay_us(2);
        I2C_SCL_LOW();
        delay_us(2);
    }
}

/**
  * @brief  ��ȡһ���ֽ�
  * @param  ack: 1:����ACK 0:����NACK
  * @retval ��ȡ��������
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
  * @brief  д��һ���Ĵ���
  * @param  addr: �豸��ַ
  * @param  reg: �Ĵ�����ַ
  * @param  data: Ҫд�������
  * @retval 0:�ɹ� 1:ʧ��
  */
uint8_t I2C_WriteReg(uint8_t addr, uint8_t reg, uint8_t data)
{
    I2C_Start(); 
    I2C_SendByte((addr << 1) | 0);  //����1λ�����һλΪ0����ʾд����
    if(I2C_WaitAck()) //���δ���յ�Ӧ����ֹͣ����
    {
        I2C_Stop();
        return 1;
    }
    I2C_SendByte(reg);
    I2C_WaitAck();
    I2C_SendByte(data);
    if(I2C_WaitAck()) //���δ���յ�Ӧ����ֹͣ����
    {
        I2C_Stop();
        return 1;
    }
    I2C_Stop();
    return 0;
}

/**
  * @brief  ��ȡһ���Ĵ���
  * @param  addr: �豸��ַ
  * @param  reg: �Ĵ�����ַ
  * @retval ��ȡ��������
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
  * @brief  ������ȡ����ֽ�
  * @param  addr: �豸��ַ
  * @param  reg: �Ĵ�����ַ
  * @param  len: Ҫ��ȡ�ĳ���
  * @param  buf: ��ȡ�������ݻ���
  * @retval 0:�ɹ� 1:ʧ��
  */
uint8_t I2C_ReadMultBytes(uint8_t addr, uint8_t reg, uint8_t len, uint8_t *buf)
{
    I2C_Start();
    I2C_SendByte((addr << 1) | 0); //�����豸��ַ������1λ�����һλΪ0����ʾд����
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
