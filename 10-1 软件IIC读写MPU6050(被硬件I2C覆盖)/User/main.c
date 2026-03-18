#include "stm32f10x.h"                  // Device header
#include "Delay.h"
#include "LED.h"
#include "Key.h"
#include "OLED.h"
//#include "MPU6050.h"
/**/

uint8_t KeyNum;		//定义用于接收按键键码的变量

// 定义六个变量用于接取MPU6050的加速度器和角速度器(陀螺仪)的值
// 可以分别接收XYZ轴的加速度值和角速度(陀螺仪)值
int16_t AX, AY, AZ, GX, GY, GZ;

uint8_t ID;
int main(void)
{
//	// LCD模块初始化
	OLED_Init(); // 初始化OLED显示屏
	
	/*初始化MPU6050 */
	// 在这个函数里面就已经调用了 I2C的初始化
	MPU6050_Init();
	
//	/* 读寄存器 */
//	// 读取MPU6050的ID号存储在变量ID中 芯片手册中ID号的地址是0X75
//	uint8_t ID =  MPU6050_ReadReg(0X75);
//	
//	OLED_ShowHexNum(1, 1, ID, 2); // 输出68
	
	
	
	/* 写寄存器 */
	// 1.首先要解除芯片的睡眠模式(由电源管理寄存器1的SLEEP位来控制)
	// 我们直接把这个寄存器写入0X00 就可以解除睡眠模式了
	// 寄存器手册中 电源管理寄存器地址是0X6B
	
//	// 给电源管理寄存器1(地址位0X6B 写入0X00 从而解除睡眠模式)
//	MPU6050_WirteReg(0X6B, 0X00);
//	
//	// 解除睡眠后，给采样频率分频器(地址为0X19) 写入0XAA值
//	MPU6050_WirteReg(0X19, 0XAA);
//	
//	// 读出0x19下的寄存器看看有没有写入成功
//	uint8_t a =  MPU6050_ReadReg(0X19);
//	
//	OLED_ShowHexNum(1, 1, a, 2); // 输出AA

// 显示设备ID
// MPU6050_GetID(void)
	OLED_ShowString(1, 1, "ID");
	ID = MPU6050_GetID();
	OLED_ShowHexNum(1, 4, ID, 2);
	while (1)
	{
		// 主循环中不断读取数据
		// 传入6个参数的地址(用&取地址符合取地址)
		MPU6050_GetData(&AX, &AY, &AZ, &GX, &GY, &GZ);
		
		
		// OLED显示6个轴的数据
		
		// 234行的第一列 分别显示AX AY AZ 即X Y Z 的加速度
		OLED_ShowSignedNum(2, 1, AX, 5);
		OLED_ShowSignedNum(3, 1, AY, 5);
		OLED_ShowSignedNum(4, 1, AZ, 5);
		
		// 234行的第8列 分别显示GX GY GZ 即X Y Z 的角速度
		OLED_ShowSignedNum(2, 8, GX, 5);
		OLED_ShowSignedNum(3, 8, GY, 5);
		OLED_ShowSignedNum(4, 8, GZ, 5);
	}
}
