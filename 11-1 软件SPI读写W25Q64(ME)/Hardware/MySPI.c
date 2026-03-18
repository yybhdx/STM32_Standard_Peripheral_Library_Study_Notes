#include "stm32f10x.h" // Device header

/*写SS的引脚*/
/*由于SPI的速度非常快，所以我们操作完引脚就不加延时了*/
void MySPI_W_SS(uint8_t BitValue)
{
  GPIO_WriteBit(GPIOA, GPIO_Pin_4, (BitAction)BitValue);
}

/*写SCK的引脚*/
void MySPI_W_SCK(uint8_t BitValue)
{
  GPIO_WriteBit(GPIOA, GPIO_Pin_5, (BitAction)BitValue);
}

/*写MOSI的引脚*/
void MySPI_W_MOSI(uint8_t BitValue)
{
  GPIO_WriteBit(GPIOA, GPIO_Pin_7, (BitAction)BitValue);
}

/*读取MISO的引脚状态*/
uint8_t MySPI_W_MISO(void)
{
  return GPIO_ReadInputDataBit(GPIOA, GPIO_Pin_6);
}

/*SPI初始化*/
void MySPI_Init(void)
{
  /*开启时钟*/
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE); // 开启GPIOA的时钟

  /*GPIO初始化*/
  /*输出引脚配置成推挽输出，输入引脚配置成浮空或上拉输入*/
  /*时钟，主机输出和片选，都是输入引脚，所以这三个角是推挽输出模式(PA4 PA5 PA7)*/
  /*从机输入是输入引脚，所以这角是浮空或上拉输入，我们选择上拉输入即可(PA6)*/
  GPIO_InitTypeDef GPIO_InitStructure;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_4 | GPIO_Pin_5 | GPIO_Pin_7;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_Init(GPIOA, &GPIO_InitStructure); // 将PA4 PA5 PA7引脚初始化为推挽输出

  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_Init(GPIOA, &GPIO_InitStructure); // 将PA6引脚初始化为上拉输入

  /*设置GPIO初始化后的默认电平*/

  /*片选引脚置为高电平(设备没被选择)*/
  MySPI_W_SS(1);

  /*SCK置为低电平(默认选择模式0)*/
  MySPI_W_SCK(0);

  /*MOSI无明确规定，可以不管*/

  /*MISO是输入引脚，不用输出电平*/
}

/*SPI的三个时序基本单元*/

/*起始信号*/
void MYSPI_Start(void)
{
  /*片选线置低电平*/
  MySPI_W_SS(0);
}

/*结束信号*/
void MYSPI_Stop(void)
{
  /*片选线置高电平*/
  MySPI_W_SS(1);
}

/*交换一个字节(也叫读写一个字节)*/
/*使用掩码的方法进行操作*/
/*选择模式0*/
uint8_t MySPI_SwapByte(uint8_t ByteSend)
{
  uint8_t a = 0; // 用于接收并返回数据
  uint8_t i;

  /*根据SPI模式0的时序*/

  /*在SS下降沿的时候开始交换字节*/
  /*SS下降沿是触发数据移出的条件，先有了下降沿，之后才会有数据移出这个动作*/
  /*对于硬件SPI来说，由于使用了硬件移位寄存器的电路，所以这两个动作几乎是同时发生的*/
  /*对于软件SPI来说，由于程序是一条条执行的，我们不可能同时完成两个动作，所以软件SPI，我们就直接躺平，直接把他看成是先后执行的逻辑*/

  // 1.SS下降沿
  MySPI_W_SS(0);

  for (i = 0; i < 8; i++)
  {
    // 2.移出数据
    // 主机移出数据的最高位放到MOSI上 从机移出数据的最高位放到MISO上
    MySPI_W_MOSI(ByteSend & (0X80 >> i)); // 用右移的方式挑出数据的位数(也可以说是用来屏蔽其他的无关位)

    // 3.SCK上升沿(模式0的SCK空闲状态是低电平，第一个边沿是上升沿(从0到1))
    // 从机会自动把数据读走，不归我们管，我们主机只需要读取MISO(从机的数据)的数据就行了
    MySPI_W_SCK(1);

    // 4.移入数据
    if (MySPI_W_MISO() == 1)
    {
      a |= 0X80 >> i; // 将最高位存在a中
    }

    // 5.SCK下降沿
    MySPI_W_SCK(0);
  }

  // // 6.移出数据
  // MySPI_W_MOSI(ByteSend & 0X40); // 写MOSI的次高位

  // // 7.进入循环

  return a;
}

// /*代码优化:为了契合SPI移位示意图的模型,使用移位数据的本身进行操作*/
// /*交换一个字节(也叫读写一个字节)*/
// /*选择模式0*/
// uint8_t MySPI_SwapByte(uint8_t ByteSend)
// {
//   for (int i = 0; i < 8; i++)
//   {
//     MySPI_W_MOSI(ByteSend & 0X80 >> i); // 发送ByteSend的最高位
//     ByteSend <<= 1; // 每次左移一位 最低位会自动补0
//     MySPI_W_SCK(1);
//     if (MySPI_W_MISO() == 1)
//     {
//       ByteSend |= 0X01; // 收到的数据放在ByteSend当前的最低位
//     }
//   }

//   return ByteSend;
// }
