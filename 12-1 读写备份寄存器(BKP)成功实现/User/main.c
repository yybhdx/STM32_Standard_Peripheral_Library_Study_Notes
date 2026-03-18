#include "stm32f10x.h"                  // Device header
#include "Delay.h"
#include "OLED.h"
#include "Key.h"

// 定义存放键值的变量
uint8_t KeyNum;

// 定义用于写入和读出的数组
// 写入的数组
uint16_t ArrayWrite[] = {0x1234, 0x5678};

// 读出的数组
uint16_t ArrayRead[2];

int main(void)
{
	OLED_Init();
	
	Key_Init();
	
	OLED_ShowString(1,1,"W:");
	
	OLED_ShowString(2,1,"R:");
	
	/*初始化BKP*/
	// 1.同时开启PWR和BKP的时钟
	
	// 开启PWR的时钟
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_PWR, ENABLE);
	
	// 开启BPK的时钟
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_BKP, ENABLE);
	
	// 2.使用PWR的一个函数，使能BKP和RTC的访问
	
	// 使能BKP和RTC的访问
	// PWR_BackupAccessCmd():使能或禁止对备份域（Backup Domain） 的访问权限
	PWR_BackupAccessCmd(ENABLE);
	
	
	/* 接下来就能进行读取和写入BKP了*/
	
//	// BKP_WriteBackupRegister()向STM32微控制器的BKP（备份）寄存器写入数据
//	// 向BKP_DR1中写入数据0x1234
//	BKP_WriteBackupRegister(BKP_DR1, 0x1234);
//	
//	// BKP_ReadBackupRegister()从STM32微控制器的指定备份寄存器（BKP）中读取一个16位的数据
//	// 读取我们刚才写的DR1
//	BKP_ReadBackupRegister(BKP_DR1);
	
//	// 用OELD显示它
//	/**
//		* @brief  OLED显示数字（十六进制，正数）
//		* @param  Line 起始行位置，范围：1~4
//		* @param  Column 起始列位置，范围：1~16
//		* @param  Number 要显示的数字，范围：0~0xFFFFFFFF
//		* @param  Length 要显示数字的长度，范围：1~8
//		* @retval 无
//		*/
//	OLED_ShowHexNum(1, 1, BKP_ReadBackupRegister(BKP_DR1), 4); // OLED显示1234
	
	while (1)
	{
		// 读取键码 PB1按下为1
		KeyNum = Key_GetNum();
		
		// 如果PB1按键按下
		if(KeyNum == 1)
		{
			
			// 变换测试数据
			ArrayWrite[0]++;
			ArrayWrite[1]++;
			
			//将数据写入到BKP里面
			// 数据写入BKP_DR1里
			BKP_WriteBackupRegister(BKP_DR1,ArrayWrite[0]);
			
			// 数据写入BKP_DR2里
			BKP_WriteBackupRegister(BKP_DR2,ArrayWrite[1]);
			
			// 刷新显示写入的值
			OLED_ShowHexNum(1, 3, ArrayWrite[0], 4);
			
			OLED_ShowHexNum(1, 8, ArrayWrite[1], 4);
		}
		
		// 读取写入的数字
		
		// DR1的数据放入ArrayRead[0]中
		ArrayRead[0] = BKP_ReadBackupRegister(BKP_DR1);
		
		// DR2的数据放入ArrayRead[1]中
		ArrayRead[1] = BKP_ReadBackupRegister(BKP_DR2);
		
		// 显示读取的值
		
		OLED_ShowHexNum(2, 3, ArrayRead[0], 4);
			
		OLED_ShowHexNum(2, 8, ArrayRead[1], 4);
	}
}

/*为什么代码刚初始化时 OLED的R会显示0000 0000??
你代码中OLED的"R"（读取值）在刚初始化时显示0000 0000，
主要是因为在程序第一次上电运行时，BKP（备份寄存器）之前没有被写入过有效数据，
所以读出的ArrayRead[0]和ArrayRead[1]的数据均为0。*/

/*
第一次按键按下是 1235 5679，因为第一次按下就产生了自增(+1)，所以不是数组默认的1234 5678
如果第一次想要是默认的1234 5678 那可以先写入 再自增*/
