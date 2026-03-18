#include "stm32f10x.h"                  // Device header

/*引脚配置层*/

/**
  * 函    数：SPI写SS引脚电平
  * 参    数：BitValue 协议层传入的当前需要写入SS的电平，范围0~1
  * 返 回 值：无
  * 注意事项：此函数需要用户实现内容，当BitValue为0时，需要置SS为低电平，当BitValue为1时，需要置SS为高电平
  */
void MySPI_W_SS(uint8_t BitValue)
{
	GPIO_WriteBit(GPIOA, GPIO_Pin_4, (BitAction)BitValue);		//根据BitValue，设置SS引脚的电平
}

/**
  * 函    数：SPI初始化
  * 参    数：无
  * 返 回 值：无
  * 注意事项：此函数需要用户实现内容，实现SS、SCK、MOSI和MISO引脚的初始化
  */
void MySPI_Init(void)
{
	/*开启时钟*/
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);	//开启GPIOA的时钟
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_SPI1, ENABLE);	//开启SPI1的时钟
	
	
	/*GPIO初始化*/
	// CS引脚 (PA4)
	GPIO_InitTypeDef GPIO_InitStructure;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_4 ;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA, &GPIO_InitStructure);					//将PA4初始化为推挽输出
	
	// 初始化SCK引脚 (PA5) 和 MOSI引脚 (PA7)
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP; // 改为复用推挽输出
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_5 | GPIO_Pin_7 ;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA, &GPIO_InitStructure);					//将PA5和PA7引脚初始化为复用推挽输出
	
	// MISO引脚 (PA6)
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;  // 上拉输入
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6 ;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA, &GPIO_InitStructure);					//将PA4、PA5和PA7引脚初始化为推挽输出
	
	/*初始化SPI外设*/
	SPI_InitTypeDef SPI_InitStruct;
	SPI_InitStruct.SPI_Mode =  SPI_Mode_Master; // 设置主/从模式为SPI_Mode_Master(主机模式)
	SPI_InitStruct.SPI_Direction = SPI_Direction_2Lines_FullDuplex; // 设置通信方向为SPI_Direction_2Lines_FullDuplex(双线全双工)
	SPI_InitStruct.SPI_DataSize = SPI_DataSize_8b; // 设置数据帧长度为SPI_DataSize_8b(8位数据)
	SPI_InitStruct.SPI_FirstBit = SPI_FirstBit_MSB;
	SPI_InitStruct.SPI_BaudRatePrescaler = SPI_BaudRatePrescaler_128;
	SPI_InitStruct.SPI_CPOL = SPI_CPOL_Low; // 设置时钟极性（空闲状态）为SPI_CPOL_Low(低电平) 因为我们选择的SPI模式为模式0,空闲默认是低电平，所以选择Low这个参数
	SPI_InitStruct.SPI_CPHA = SPI_CPHA_1Edge; // 设置时钟相位（采样边沿）为SPI_CPHA_1Edge(第1个边沿采样(即移入数据)) 这里，我们选择SPI模式0，CPHA=0，所以选择1Edge这个参数
	SPI_InitStruct.SPI_NSS = SPI_NSS_Soft; // 设置片选（NSS）管理方式为SPI_NSS_Soft(软件控制)
	SPI_InitStruct.SPI_CRCPolynomial = 7; // 设置CRC校验多项式为默认值7 这个我们不必过多了解
	SPI_Init(SPI1, &SPI_InitStruct);
	
	/*使能SPI外设*/
	SPI_Cmd(SPI1, ENABLE);
	MySPI_W_SS(1);
}

/*协议层*/

/**
  * 函    数：SPI起始
  * 参    数：无
  * 返 回 值：无
  */
void MySPI_Start(void)
{
	MySPI_W_SS(0);				//拉低SS，开始时序
}

/**
  * 函    数：SPI终止
  * 参    数：无
  * 返 回 值：无
  */
void MySPI_Stop(void)
{
	MySPI_W_SS(1);				//拉高SS，终止时序
}

/**
  * 函    数：SPI交换传输一个字节，使用SPI模式0
  * 参    数：ByteSend 要发送的一个字节
  * 返 回 值：接收的一个字节
  */
uint8_t MySPI_SwapByte(uint8_t ByteSend)
{
	// 1.调用函数 检查TXE置位
  // SPI_I2S_GetFlagStatus()
  // 检查STM32中SPI或I2S外设的特定状态标志
	// 指定要检查的SPI或I2S外设为SPI1
	// 指定要检查的状态标志为,SPI_I2S_FLAG_TXE：发送缓冲区为空标志。表示数据寄存器（DR）已空，可以写入新的待发送数据。
	// SET：指定的标志位状态为1（置位）。RESET：指定的标志位状态为0（复位）。
	while(SPI_I2S_GetFlagStatus(SPI1, SPI_I2S_FLAG_TXE) != SET);

	// 2.发送数据
  //SPI_I2S_SendData()通过STM32的SPI或I2S外设发送一个数据
	// SPIx：指定要操作的SPI或I2S外设为SPI1
	// Data：要发送的数据为ByteSend
	SPI_I2S_SendData(SPI1, ByteSend);
	
	// 3.等待RXNE标志位
	while(SPI_I2S_GetFlagStatus(SPI1,SPI_I2S_FLAG_RXNE) != SET);
	
	// 4.读取DR，从RDR里，把交换接收的数据读出来
	//SPI_I2S_ReceiveData() 从指定的 SPI 或 I2S 外设接收一个数据
	// SPIx：指定要读取数据的 SPI/I2S 外设为SPI1
	return SPI_I2S_ReceiveData(SPI1);

}
