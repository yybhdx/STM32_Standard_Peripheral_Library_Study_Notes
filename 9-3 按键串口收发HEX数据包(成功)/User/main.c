#include "stm32f10x.h"                  // Device header
#include "Delay.h"
#include "LED.h"
#include "Key.h"
#include "OLED.h"
#include "Serial.h"

uint8_t KeyNum;

int main(void)
{
//	// LCD模块初始化
	OLED_Init(); // 初始化OLED显示屏
	
	Serial_Init(); //初始化USART1
	
	Key_Init();
	
	OLED_ShowString(1, 1, "TxPacket");
	OLED_ShowString(3, 1, "RxPacket");
	
	// 填充缓冲区数组
	Serial_TxPacket[0] = 0x01;
	Serial_TxPacket[1] = 0x02;
	Serial_TxPacket[2] = 0x03;
	Serial_TxPacket[3] = 0x04;
	
	
	while (1)
	{
		// 按一下按键，变化一下数据，发送到串口助手上
		KeyNum = Key_GetNum();
		
		if(KeyNum == 1) // PB1按下，执行发送
		{
			Serial_TxPacket[0]++;
			Serial_TxPacket[1]++;
			Serial_TxPacket[2]++;
			Serial_TxPacket[3]++;
			
			// 为发送的数组上的四个数据，添加包头包尾
			Serial_SendPacket();
			
			OLED_ShowHexNum(2, 1, Serial_RxPacket[0], 2);
			OLED_ShowHexNum(2, 4, Serial_RxPacket[1], 2);
			OLED_ShowHexNum(2, 7, Serial_RxPacket[2], 2);
			OLED_ShowHexNum(2, 10, Serial_RxPacket[3], 2);
		}
		
		// 主循环中通过不断读取RxFlag
		if(Serial_GetRxFlag() == 1) // 收到了数据包
		{
			/**
			* @brief  OLED显示数字（十六进制，正数）
			* @param  Line 起始行位置，范围：1~4
			* @param  Column 起始列位置，范围：1~16
			* @param  Number 要显示的数字，范围：0~0xFFFFFFFF
			* @param  Length 要显示数字的长度，范围：1~8
			* @retval 无
			*/
			OLED_ShowHexNum(4, 1, Serial_RxPacket[0], 2);
			OLED_ShowHexNum(4, 4, Serial_RxPacket[1], 2);
			OLED_ShowHexNum(4, 7, Serial_RxPacket[2], 2);
			OLED_ShowHexNum(4, 10, Serial_RxPacket[3], 2);
		}
	}
}

/*这个程序还隐藏了一个问题需要注意一下
就是这个RxPacket数组，它是一个同时被写入又同时被读出的数组，
在中断函数里，我们会依次写入它，在主函数里，我们又会依次读出它。
这会造成什么问题呢??
就是数据包之间可能会混在一起
比如你读出的过程太慢
前面两个数据刚读出来，等了一会，才继续往后读取，
那这时后面的数据就可能会刷新为下一个数据包的数据
也就是你读出来的数据可能一部分属于上一个数据包，
另一部分数据下一个数据包，这个问题需要注意一下。
那解决方法呢，可以是在接收部分加入判断:就是在每一个数据包读取处理完毕只会，
再接收下一个数据包。
当然很多情况下其实还可以不进行处理
像是这种HEX数据包，多是用于传输各种传感器的每个独立数据
比如陀螺仪的X Y Z轴数据，温湿度数据等等
它们相邻数据包之间的数据，具有连续性。
这样即使相邻数据包混在一起了，也没关系
所以这种情况下，就不需要关心这个问题。
具体到底怎么处理，还需要大家结合实际情况来操作了。
这里就提一下这个可能存在的问题，大家了解一下就行了*/


