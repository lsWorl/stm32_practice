#ifndef __I2C_H
#define __I2C_H

#include "stm32f10x.h"

// I2Cͨ�ź���
void I2C_Init_Software(void);                                     // ��ʼ��I2C�����ģ�⣩
void I2C_Start(void);                                            // ������ʼ�ź�
void I2C_Stop(void);                                             // ����ֹͣ�ź�
void I2C_SendByte(uint8_t byte);                                // ����һ���ֽ�
uint8_t I2C_ReadByte(uint8_t ack);                              // ��ȡһ���ֽ�
uint8_t I2C_WaitAck(void);                                      // �ȴ�Ӧ��
void I2C_Ack(void);                                             // ����Ӧ��
void I2C_NAck(void);                                            // ���ͷ�Ӧ��

// I2C�豸��������
uint8_t I2C_WriteReg(uint8_t addr, uint8_t reg, uint8_t data);  // д�Ĵ���
uint8_t I2C_ReadReg(uint8_t addr, uint8_t reg);                 // ���Ĵ���
uint8_t I2C_ReadMultBytes(uint8_t addr, uint8_t reg,            // ��ȡ����ֽ�
                         uint8_t len, uint8_t *buf);

#endif
