#ifndef __I2C_H
#define __I2C_H

#include "stm32f10x.h"

// I2C通信函数
void I2C_Init_Software(void);                                     // 初始化I2C（软件模拟）
void I2C_Start(void);                                            // 发送起始信号
void I2C_Stop(void);                                             // 发送停止信号
void I2C_SendByte(uint8_t byte);                                // 发送一个字节
uint8_t I2C_ReadByte(uint8_t ack);                              // 读取一个字节
uint8_t I2C_WaitAck(void);                                      // 等待应答
void I2C_Ack(void);                                             // 发送应答
void I2C_NAck(void);                                            // 发送非应答

// I2C设备操作函数
uint8_t I2C_WriteReg(uint8_t addr, uint8_t reg, uint8_t data);  // 写寄存器
uint8_t I2C_ReadReg(uint8_t addr, uint8_t reg);                 // 读寄存器
uint8_t I2C_ReadMultBytes(uint8_t addr, uint8_t reg,            // 读取多个字节
                         uint8_t len, uint8_t *buf);

#endif
