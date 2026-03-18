#include "stm32f10x.h"                  // Device header
#include "MPU6050_Reg.h" // 引用寄存器宏定义的头文件


// 写入MPU6050
#define MPU6050_ADDRESS 0XD0

// 将I2C_CheckEvent函数封装成一个带有超时退出机制的WaitEvent函数
void MPU6050_WaitEvent(I2C_TypeDef* I2Cx, uint32_t I2C_EVENT)
{
	 uint32_t Timeout;
	 Timeout = 10000;
	 
	 // 起始条件发送后会发出EV5事件，所以我们要等待EV5事件的到来(使用while循环)
	 // 指定要操作的I2C外设为I2C2，指定要检查的I2C事件为I2C_EVENT_MASTER_MODE_SELECT,EV5事件
	 while(I2C_CheckEvent(I2Cx, I2C_EVENT) != SUCCESS)
	 {
		 Timeout--;
		 // 循环超时之后跳出循环，防止阻塞，继续执行后面的代码
		 if(Timeout == 0)
		 {
			 break;
			 // return; // 当然也可以用return return 是函数返回值的关键字，同时它也能结束这个函数，就是while我不循环了，后面的代码我也不执行了，直接跳出这个函数 
		 }
	 }
 
}

// 从MPU6050读

/* 指定地址写寄存器 */
// 参数是八位的寄存器地址 和八位的数据
void MPU6050_WirteReg(uint8_t RegAddress, uint8_t Data)
{
	
 // 1.生成起始条件
 // I2C_GenerateSTART()函数，替换掉这个MyI2C_Start();函数
 // 另外，软件I2C的这些函数，内部都有Delay操作，是一种阻塞式的流程
 // 也就是函数运行完之后，对应的波形也肯定发送完毕了
 // 所以上一个函数运行完之后，对应的波形也肯定发送完毕了，上一个函数运行完之后，就可以紧跟下一个函数
 /* 但是下面这个函数(I2C_GenerateSTART)，包括之后的硬件I2C函数，都不是阻塞式的，这些硬件I2C函数只管给寄存器的位置置1
 或者只在DR写入数据，就结束，退出函数，至于波形是否发送完毕，它是不管的，所以对于这种非阻塞式的程序
 在函数结束之后，我们都要等待相应的标志位，来确保这个函数的操作执行到位了*/
 I2C_GenerateSTART(I2C2, ENABLE); //硬件I2C生成起始条件
 // 起始条件发送后会发出EV5事件，所以我们要等待EV5事件的到来(使用while循环)
 // 指定要操作的I2C外设为I2C2，指定要检查的I2C事件为I2C_EVENT_MASTER_MODE_SELECT,EV5事件
 MPU6050_WaitEvent(I2C2, I2C_EVENT_MASTER_MODE_SELECT); //等待EV5
 
 // 这个while循环加多了，一旦总线出问题了，就很容易造成整个程序卡死，所以我们需要设计一个超时退出的机制，但是目前程序先这样
 
 // 起始条件发出后，我们就要发送从机地址，接收应答了
 // 2.发送从机地址(就是发送一个字节，直接向DR寄存器写入一个字节就行了)
 // I2C_Send7bitAddress():在主模式下，向总线发送一个7位从设备地址，并指明接下来的数据传输方向（读或写）
 // 指定要操作的I2C外设为I2C2
 // 指定从设备的7位地址为MPU6050_ADDRESS
 // 本次通信的数据传输方向(也就是从机地址的最低位(读写位))为I2C_Direction_Transmitter，即读写位置0
 I2C_Send7bitAddress(I2C2, MPU6050_ADDRESS, I2C_Direction_Transmitter); //硬件I2C发送从机地址，方向为发送
 
 // 3.接收应答(这里不需要再用库函数来操作，因为在这个库函数中，发送数据都自带了接收应答的过程，同样，接收数据也自带了发送应答的过程)
 // 如果应答错误，硬件会通过标志位和中断来提示我们，所以发送地址之后，应答位就不需要处理了
 // 我们直接等待事件
 
 // 4.当地址发出，接收应答位之后，就会产生EV6事件
 // I2C_EVENT_MASTER_TRANSMITTER_MODE_SELECTED：主机发送器模式已选择（从机地址已应答）
 MPU6050_WaitEvent(I2C2, I2C_EVENT_MASTER_TRANSMITTER_MODE_SELECTED); //等待EV6
 
 // 5.EV6事件之后有个EV8_1事件，这个EV8_1事件就是想告诉你，你该写入DR发送数据了，我们并不需要等待这个EV8_1事件的
 // 而且这个I2C_CheckEvent()函数的第三个参数的列表里，也没有EV8_1事件的参数
 // 所以这时，我们就直接写入DR,发送数据
 
 // I2C_SendData():将一字节数据写入指定的I2C外设的数据寄存器（DR），从而启动一次数据发送过程
 // 指定要操作的I2C外设为I2C2
 // 指定要发送的单字节数据。这是一个8位无符号整数（uint8_t），即你要发送的实际数据内容。为RegAddress
 I2C_SendData(I2C2, RegAddress); //硬件I2C发送寄存器地址
 
 // 这个操作之后，还是等待事件，我们写入DR后，需要等待的是EV8事件
 // 检查EV8事件
 // 指定要操作的I2C外设为I2C2
 // 指定要检查的I2C事件为EV8(I2C_EVENT_MASTER_BYTE_TRANSMITTING)事件:发送一个数据字节后等待
 MPU6050_WaitEvent(I2C2, I2C_EVENT_MASTER_BYTE_TRANSMITTING); //等待EV8
 
 // 继续，当等到了EV8事件之后我们就可以继续写入下一个数据，我们继续调用SendData
 // I2C_SendData():将一字节数据写入指定的I2C外设的数据寄存器（DR），从而启动一次数据发送过程
 // 指定要操作的I2C外设为I2C2
 // 指定要发送的单字节数据。这是一个8位无符号整数（uint8_t），即你要发送的实际数据内容。为参数Data
 I2C_SendData(I2C2, Data); //硬件I2C发送数据
 
 // 等待事件
 // 由于我们的Data是最后一个字节，发送完Data之后，就需要终止了，所以最后等待的这个事件有所不同
 // 当我们有连续的数据需要发送时，在发送过程中，我们需要等待EV8事件，而当我们发送完最后有个字节时，需要等待的，就是EV8_2事件了_
 // 什么时候会产生EV8_2呢??，主机发送的图片中的解释是BTF标志位为1，也就是移位完成了，并且没有新的数据可以发的时候，置BTF，也就是EV8_2
 // 检查EV8_2事件
 // 指定要操作的I2C外设为I2C2
 // 指定要检查的I2C事件为EV8_2(I2C_EVENT_MASTER_BYTE_TRANSMITTED)事件:主机字节已传输完成
 // 总结一下，当我们需要发送多个字节的数据流时，中间的字节，写入DR之后，需要等待EV8事件，也就是
 // I2C_EVENT_MASTER_BYTE_TRANSMITTING,
 // 最后一个字节，写入DR之后，需要等待EV8_2事件，也就是BYTE_TEANSMITTED，这就是流程
 MPU6050_WaitEvent(I2C2, I2C_EVENT_MASTER_BYTE_TRANSMITTED); //等待EV8_2
 
 // 那在TEANSMITTED之后，我们就可以终止了
 // I2C_GenerateSTOP():在I2C总线上生成停止条件
 // 指定要操作的I2C外设为I2C2
 // 指定停止条件生成的新状态为ENABLE
 I2C_GenerateSTOP(I2C2, ENABLE); //硬件I2C生成终止条件
 

}


/* 指定地址读寄存器 */
// 前面和指定地址写是一样的，只是指定地址还没有开始写
// 参数是指定读的寄存器地址
uint8_t MPU6050_ReadReg(uint8_t RegAddress)
{
	// 定义一个变量用于存储接收到的数据
	uint8_t a;
	
	// 复合格式指定地址的一部分
	I2C_GenerateSTART(I2C2, ENABLE);										//硬件I2C生成起始条件
	MPU6050_WaitEvent(I2C2, I2C_EVENT_MASTER_MODE_SELECT);					//等待EV5
	
	I2C_Send7bitAddress(I2C2, MPU6050_ADDRESS, I2C_Direction_Transmitter);	//硬件I2C发送从机地址，方向为发送
	MPU6050_WaitEvent(I2C2, I2C_EVENT_MASTER_TRANSMITTER_MODE_SELECTED);	//等待EV6
	
	I2C_SendData(I2C2, RegAddress);											//硬件I2C发送寄存器地址
	
	// 当使用I2C_EVENT_MASTER_BYTE_TRANSMITTING时，那实际这个实际发生时，RegAddress的波形起始还没有完全发送完毕
	// 这时，再直接产生重复起始条件，会不会把这个数据截断呢??那经过实测，其实并不会截断
	// 当我们调用起始条件I2C_GenerateSTART之后，如果当前还有字节正在移位，那这个起始条件将会延迟，等待当前字节发送完毕之后才能产生
	// 所以这里使用I2C_EVENT_MASTER_BYTE_TRANSMITTING还是I2C_EVENT_MASTER_BYTE_TRANSMITTED，都没问题
	// 如果用I2C_EVENT_MASTER_BYTE_TRANSMITTING，那下面重复起始条件之后将会等待,如果用I2C_EVENT_MASTER_BYTE_TRANSMITTED
	// 那会等波形全部发完了，再产生重复起始条件，那么这里我们保守起见，这里还是用I2C_EVENT_MASTER_BYTE_TRANSMITTED
	// 也就是按照设计要求来，在数据流的最后一个字节，还是等待TRANSMITTED比较好
	MPU6050_WaitEvent(I2C2, I2C_EVENT_MASTER_BYTE_TRANSMITTED);	//等待EV8_2
	
	// 在指定地址之后，生成重复起始条件
	I2C_GenerateSTART(I2C2, ENABLE);										//硬件I2C生成起始条件
	
	// 生成重复起始条件后，参考主机接收的序列图，此时需要等待EV5事件
	MPU6050_WaitEvent(I2C2, I2C_EVENT_MASTER_MODE_SELECT);					//等待EV5
	
	// 接着继续，下一步，还是发送从机地址，不过这里的读写位就是要读的方向了
	// 参数 I2Cx：指定要操作的I2C外设为I2C2
	// 参数 Address：指定从设备的7位地址为MPU6050_ADDRESS
	// 参数 I2C_Direction:I2C_Direction_Receiver：表示STM32作为主接收器（Master Receiver），即从从设备读取数据。
	I2C_Send7bitAddress(I2C2, MPU6050_ADDRESS, I2C_Direction_Receiver);
	
	// 寻址之后，等待EV6事件
	// 但是主机接收的EV6并不是主机发送的EV6
	// I2Cx：指定要操作的I2C外设为I2C2
	// 2C_EVENT：指定要检查的I2C事件为EV6 事件 (I2C_EVENT_MASTER_RECEIVER_MODE_SELECTED) ，即主机接收器模式已选择
	MPU6050_WaitEvent(I2C2, I2C_EVENT_MASTER_RECEIVER_MODE_SELECTED);	//等待EV6
	
	// 接着进入到主机接收的模式之后，就开始接收从机发送的数据波形了
	
	/*ACK置0*/
	// 在接收最后一个字节之前提前将应答失能
	// I2C_AcknowledgeConfig()控制STM32作为I2C通信主机时，在接收数据后是否向从机发送应答信号（ACK）
	// 参数 I2Cx：指定要配置的I2C外设为I2C2
	// 参数 NewState：指定应答功能的新状态 :DISABLE：失能应答。STM32在接收一个字节后，将不发送ACK信号（即发送NACK）。
	I2C_AcknowledgeConfig(I2C2, DISABLE);
	
	/*STOP位置1*/
	// I2C_GenerateSTOP()在I2C总线上生成停止条件
	// 参数 I2Cx：指定要操作的I2C外设为I2C2
	// 参数 NewState：指定停止条件生成的新状态。为ENABLE：使能停止条件生成
	I2C_GenerateSTOP(I2C2, ENABLE);
	
	// 等这些做完之后，我们再等待EV7事件(主机已接收到一个字节)，等EV7事件产生后，一个字节的数据就已经在DR里面了，我们读取DR即可拿出这一个字节
	// I2C_CheckEvent()检查I2C总线通信过程中的特定事件状态
	// I2Cx：指定要操作的I2C外设为I2C2
	// I2C_EVENT：指定要检查的I2C事件为I2C_EVENT_MASTER_BYTE_RECEIVED:主机已接收到一个字节(接收数据时等待)
	MPU6050_WaitEvent(I2C2, I2C_EVENT_MASTER_BYTE_RECEIVED);	//等待EV7
	
	// I2C_ReceiveData()从STM32的I2C数据寄存器中读取接收到的数据，并存到a变量里
	// 参数 I2Cx：指定要读取数据的I2C外设为I2C2
	a = I2C_ReceiveData(I2C2);
	
	// 最后别忘了把ACK再置回1
	// 为什么要这样做呢?我们的想法是，默认状态下ACK就是1，给从机应答，在收最后一个字节之前，临时把ACK置0，给非应答
	// 所以在接收函数的最后，要恢复默认的ACK=1，这个流程是为了方便指定地址收多个字节，虽然我们现在程序中至始至终只有收一个字节的
	// 参数 I2Cx：指定要配置的I2C外设为I2C2
	// 参数 NewState：指定应答功能的新状态:ENABLE：使能应答。STM32在成功接收一个字节后，会自动发送ACK信号给从机
	I2C_AcknowledgeConfig(I2C2, ENABLE);
	
	// 返回读到的数据
	return a;
}
// MPU6050初始化
void MPU6050_Init(void)
{
	// 使用硬件I2C 替换掉这个MyI2C_Init()软件I2C函数
//	MyI2C_Init();
	
	// 1.第一步，开启12C外设和对应GPIO口的时钟
	
	// 开启RCC的I2C2的时钟
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_I2C2, ENABLE);
	
	// 开启GPIO的时钟(我们需要使用PB10和PB11)
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);
	
	// 2.第二步，把12C外设对应的GPIO口初始化为复用开漏模式
	/*GPIO初始化*/
	GPIO_InitTypeDef GPIO_InitStructure; 
	// 为什么对应的引脚要配置成复用开漏模式?? 
	// 开漏，这是I2C协议的设计要求，复用，就是GPIO的控制权要交给硬件外设，我们这是硬件I2C，那控制引脚的任务肯定得交给外设来做了，
	// 如果是之前的软件I2C的话，我们通过程序来控制引脚，那就是通用开漏模式
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_OD;
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10 | GPIO_Pin_11;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOB, &GPIO_InitStructure);						//将PB10和PB11引脚初始化为复用开漏输出
	
	
	// 第三步，初始化I2C外设，使用结构体，对整个I2C进行配置
	I2C_InitTypeDef I2C_InitStruct;
	I2C_InitStruct.I2C_Mode = I2C_Mode_I2C; // 设置I2C的工作模式为标准I2C模式
	I2C_InitStruct.I2C_ClockSpeed = 50000; // 频率设置为50KHZ
	I2C_InitStruct.I2C_DutyCycle = I2C_DutyCycle_2; // 表示占空比为2:1，低电平时间和高电平时间是2:1的比例关系 // 因为占空比仅在快速模式（时钟频率高于100kHz）下需要配置。 所以这里配置了也没用
	I2C_InitStruct.I2C_Ack = I2C_Ack_Enable; // I2C_Ack_Enable（使能应答）
	I2C_InitStruct.I2C_AcknowledgedAddress = I2C_AcknowledgedAddress_7bit; // 定义I2C接口将应答7位地址还是10位地址。I2C_AcknowledgedAddress_7bit（7位地址）
	I2C_InitStruct.I2C_OwnAddress1 =  0X00; // 我们STM32在那时不需要做从机呗别人使唤，所以这个地址可以随便给一个，只要不和总线上其他设备的地址重复就行
	I2C_Init(I2C2, &I2C_InitStruct);
	
	// 第四步，I2C_Cmd();，使能I2C2
	I2C_Cmd(I2C2, ENABLE);
	
	
	// 1.配置电源管理寄存器1
	// 给电源管理寄存器1写入0X01
	MPU6050_WirteReg(MPU6050_PWR_MGMT_1, 0X01); // 解除睡眠模式 ， 选择陀螺仪时钟
	
	// 2.配置电源管理寄存器2
	// 给电源管理寄存器2写入0X00
	MPU6050_WirteReg(MPU6050_PWR_MGMT_2, 0X00); // 6个轴均不待机
	
	// 3.配置采样率分频寄存器
	// 给采样率分频寄存器写入0X09(10分频)
	MPU6050_WirteReg(MPU6050_SMPLRT_DIV, 0X09);	// 采样分频为10
	
	// 4.配置配置寄存器
	// 给配置寄存器写入0X06(平滑滤波)
	MPU6050_WirteReg(MPU6050_CONFIG, 0X06); // 滤波参数最大
	
	// 5.配置陀螺仪配置寄存器
	// 给陀螺仪配置寄存器写入0X18(设置最大量程)
	MPU6050_WirteReg(MPU6050_GYRO_CONFIG, 0X18);	// 陀螺仪选择最大量程
	
	// 6.配置加速度计配置寄存器
	// 给加速度计配置寄存器写入0X18(设置最大量程，且不用高通滤波器)
	MPU6050_WirteReg(MPU6050_ACCEL_CONFIG, 0X18);	// 加速度计选择最大量程
}

/* 获取芯片ID号的函数 */
uint8_t MPU6050_GetID(void)
{
	
	return MPU6050_ReadReg(MPU6050_WHO_AM_I);
}

/* 获取数据寄存器的函数 */
// 根据任务需求，这个函数需要返回6个int16_t的数据，分别表示XYZ的加速度值和陀螺仪值
// 但是C语言中，函数的返回值只能有一个，所以，这里就需要特殊操作来实现返回六个值的任务

// 多返回值函数的设计方法有很多
// 1.在函数外面定义6个全局变量，子函数读到的数据直接写在全局变量里面，然后6个全局变量在主函数里进行共享。这样就相当于返回了6个值
// 2.用指针，进行变量的地址传递，来实现多返回值
// 3.更高阶的方法，就是用结构体，对多个变量进行打包，然后再统一进行传递(这就是STM32的库函数里面用到的，比如:GPIO_Init())

// 这里我们使用第二种方法:用指针的地址传递。所以在函数参数这里，写上六个输出参数
// 这六个参数，均是int16_t的指针类型。之后我们会在主函数里进行定义变量，通过指针，把主函数变量的地址传递到子函数里面,子函数中，通过传递过来的地址，操作主函数的变量
// 这样，子函数结束后，主函数变量的值，就是子函数想要返回的值
void MPU6050_GetData(int16_t *AccX,int16_t *AccY,int16_t *AccZ,
										int16_t *GyroX,int16_t *GyroY,int16_t *GyroZ)
{
	// 获取寄存器数据
	uint8_t DataH = 0; // 高8位数据
	uint8_t DataL = 0; // 低8位数据
	
	// 1.读取加速度寄存器X轴的高8位
	DataH = MPU6050_ReadReg(MPU6050_ACCEL_XOUT_H);
	
	// 2.读取加速度寄存器X轴的低8位
	DataL = MPU6050_ReadReg(MPU6050_ACCEL_XOUT_L);
	
	// 高八位左移8位 再|上低八位 得到16位数据 
	// 得到16位数据之后 用指针引用传递进来的地址，将X轴的值传给AccX
	// *AccX 解引用，获取指针 AccX所指向的内存地址中存储的值
	*AccX = (DataH << 8) | DataL;
	
	// 1.读取加速度寄存器Y轴的高8位
	DataH = MPU6050_ReadReg(MPU6050_ACCEL_YOUT_H);
	
	// 2.读取加速度寄存器Y轴的低8位
	DataL = MPU6050_ReadReg(MPU6050_ACCEL_YOUT_L);
	
	// 高八位左移8位 再|上低八位 得到16位数据 
	// 得到16位数据之后 用指针引用传递进来的地址，将X轴的值传给AccX
	// *AccX 解引用，获取指针 AccX所指向的内存地址中存储的值
	*AccY = (DataH << 8) | DataL;
	
	// 1.读取加速度寄存器Z轴的高8位
	DataH = MPU6050_ReadReg(MPU6050_ACCEL_ZOUT_H);
	
	// 2.读取加速度寄存器Z轴的低8位
	DataL = MPU6050_ReadReg(MPU6050_ACCEL_ZOUT_L);
	
	// 高八位左移8位 再|上低八位 得到16位数据 
	// 得到16位数据之后 用指针引用传递进来的地址，将X轴的值传给AccX
	// *AccX 解引用，获取指针 AccX所指向的内存地址中存储的值
	*AccZ = (DataH << 8) | DataL;
	
	
	// 1.读取陀螺仪寄存器X轴的高8位
	DataH = MPU6050_ReadReg(MPU6050_GYRO_XOUT_H);
	
	// 2.读取陀螺仪寄存器X轴的低8位
	DataL = MPU6050_ReadReg(MPU6050_GYRO_XOUT_L);
	
	// 高八位左移8位 再|上低八位 得到16位数据 
	// 得到16位数据之后 用指针引用传递进来的地址，将X轴的值传给AccX
	// *AccX 解引用，获取指针 AccX所指向的内存地址中存储的值
	*GyroX = (DataH << 8) | DataL;
	
	// 1.读取陀螺仪寄存器Y轴的高8位
	DataH = MPU6050_ReadReg(MPU6050_GYRO_YOUT_H);
	
	// 2.读取陀螺仪寄存器Y轴的低8位
	DataL = MPU6050_ReadReg(MPU6050_GYRO_YOUT_L);
	
	// 高八位左移8位 再|上低八位 得到16位数据 
	// 得到16位数据之后 用指针引用传递进来的地址，将X轴的值传给AccX
	// *AccX 解引用，获取指针 AccX所指向的内存地址中存储的值
	*GyroY = (DataH << 8) | DataL;
	
	// 1.读取陀螺仪寄存器Z轴的高8位
	DataH = MPU6050_ReadReg(MPU6050_GYRO_ZOUT_H);
	
	// 2.读取陀螺仪寄存器Z轴的低8位
	DataL = MPU6050_ReadReg(MPU6050_GYRO_ZOUT_L);
	
	// 高八位左移8位 再|上低八位 得到16位数据 
	// 得到16位数据之后 用指针引用传递进来的地址，将X轴的值传给AccX
	// *AccX 解引用，获取指针 AccX所指向的内存地址中存储的值
	*GyroZ = (DataH << 8) | DataL;
}
