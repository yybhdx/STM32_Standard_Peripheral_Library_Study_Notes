#ifndef __MPU6050_H
#define __MPU6050_H

/* 指定地址写寄存器 */
void MPU6050_WirteReg(uint8_t RegAddress, uint8_t Data);

/* 指定地址读寄存器 */
uint8_t MPU6050_ReadReg(uint8_t RegAddress);

// MPU6050初始化
void MPU6050_Init(void);

// 获取芯片ID号的函数
uint8_t MPU6050_GetID(void);

// 获取数据寄存器
void MPU6050_GetData(int16_t *AccX,int16_t *AccY,int16_t *AccZ,
										int16_t *GyroX,int16_t *GyroY,int16_t *GyroZ);
#endif
