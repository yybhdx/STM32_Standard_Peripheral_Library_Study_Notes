#include "stm32f10x.h"                  // Device header
#include "Delay.h"
#include "LED.h"
#include "Key.h"
#include "OLED.h"
#include "Serial.h"
#include <String.h>

/*文本数据包是可变包长，含包头包尾
以'@'符号为包头，换行的两个符号为包尾
中间的载荷字符数量不固定
我们这个程序只写接收的部分，
因为发送的部分不方便像HEX数组那样一个一个更改的。
所以发送就直接在主函数里SendString,或者printf就行了。非常简单*/

uint8_t KeyNum;

int main(void)
{
//	// LCD模块初始化
	OLED_Init(); // 初始化OLED显示屏
	
	Serial_Init(); //初始化USART1
	
	LED_Init();
	
	
	OLED_ShowString(1, 1, "TxPacket");
	OLED_ShowString(3, 1, "RxPacket");
	
	
	
	while (1)
	{
		// 判断是否接收到数据包
 		if(Serial_RxFlag == 1) // 接收数据包的标志位为1，代表接收到数据包
		{
			
			// 清除第四行
			OLED_ShowString(4,1,"                   ");
			
			// OLED显示收到的字符串
			/**
				* @brief  OLED显示字符串
				* @param  Line 起始行位置，范围：1~4
				* @param  Column 起始列位置，范围：1~16
				* @param  String 要显示的字符串，范围：ASCII可见字符
				* @retval 无
				*/
			OLED_ShowString(4,1,Serial_RxPacket);
			
			//用C语言的官方库<String.h>里面的函数判断两个字符是否相等
			// 如果接收的字符 == "LED_ON"
			if(strcmp(Serial_RxPacket, "LED_ON")==0)
			{
				// 那么点亮LED
				LED1_ON(); //设置PA1引脚为低电平
				
				// 向串口助手回传字符串，表示LED以打开
				Serial_SendString("LED_ON_OK\r\n");
				
				// OLED显示数据
				// 清除第2行
				OLED_ShowString(2,1,"                   ");
				OLED_ShowString(2,1,"LED_ON_OK");
			}
			//用C语言的官方库<String.h>里面的函数判断两个字符是否相等
			// 如果接收的字符 == "LED_OFF"
			else if(strcmp(Serial_RxPacket, "LED_OFF")==0)
			{
				// 那么点亮LED
				LED1_OFF(); //设置PA1引脚为高电平
				
				// 向串口助手回传字符串，表示LED以打开
				Serial_SendString("LED_OFF_OK\r\n");
				
				// OLED显示数据
				// 清除第2行
				OLED_ShowString(2,1,"                   ");
				OLED_ShowString(2,1,"LED_OFF_OK");
			}
			
			// 上面的指令都不匹配 ,则不执行操作
			else{
				
				// 向串口助手回传字符串，表示错误
				Serial_SendString("ERROR_COMMAND\r\n");
				
				// OLED显示数据
				// 清除第2行
				OLED_ShowString(2,1,"                   ");
				OLED_ShowString(2,1,"ERROR_COMMAND");				
				
			}
			
			// 操作完成之后，清除标志位
			Serial_RxFlag = 0;
		}
	}
}

/*程序的问题:
如果连续发送数据包，程序处理不及时，可能导致数据包错位
在这里，文本数据包，每个数据包是独立的，不存在连续
这如果错位了，问题就比较大
所以我们在程序中修改一下，等每次处理完成之后，在开始接收下一个数据包*/


