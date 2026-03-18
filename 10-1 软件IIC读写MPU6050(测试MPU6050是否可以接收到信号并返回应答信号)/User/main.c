#include "stm32f10x.h"                  // Device header
#include "Delay.h"
#include "LED.h"
#include "Key.h"
#include "OLED.h"


uint8_t KeyNum;		//定义用于接收按键键码的变量


int main(void)
{
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);		//开启GPIOB的时钟
	
	/*GPIO初始化*/
	GPIO_InitTypeDef GPIOB_InitStructure;
	GPIOB_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIOB_InitStructure.GPIO_Pin = GPIO_Pin_6 | GPIO_Pin_7;
	GPIOB_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOB, &GPIOB_InitStructure);		
	// PB6默认低电平 PB7默认高电平给OLDE供电
	GPIO_ResetBits(GPIOB, GPIO_Pin_6);

	GPIO_SetBits(GPIOB, GPIO_Pin_7);;

//	// LCD模块初始化
	OLED_Init(); // 初始化OLED显示屏
	
	/*软件IIC初始化(释放总线 使IIC处于空闲状态) */
	MyI2C_Init();
	
	/*指定地址写*/
	/* 发送起始信号 */
	MyI2C_Start();
	
	/* 发送从机地址加读写位 */
	// 0XD0是0X68(1101 000,MPU6050的从机地址) 左移一位加上写位(0)
	// 总线上有MPU6050才能显示0 没有就显示1
	MyI2C_SendByte(0XD0);  // AD0 = 0的地址
//	MyI2C_SendByte(0XD2);  // AD0 = 1的地址
	/* 主机接收从机的应答信号 */
	uint8_t Ack =  MyI2C_ReceiveAck();
	
	/* 主机发送从机寄存器地址 */
//	MyI2C_SendByte(0XD0);
	
	/* 主机接收从机的应答信号 */
	
	
	/* 主机写入数据 */
	
	/* 主机接收从机的应答信号 */
	
	/* 发送停止信号 */ 
	MyI2C_Stop();
	
	// 利用OLED在OLED的第一行第一列显示Ack 看看是0还是1
	// 如果是0 则说明没问题，如果显示1，则是有问题
	OLED_ShowNum(1, 1, Ack, 3);
	
	while (1)
	{
		// 利用OLED在OLED的第一行第一列显示Ack 看看是0还是1
		// 如果是0 则说明没问题，如果显示1，则是有问题
		OLED_ShowNum(1, 1, Ack, 3);
	}
}
