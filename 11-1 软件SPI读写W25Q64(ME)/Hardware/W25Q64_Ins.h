#ifndef __W25Q64_INS_H
#define __W25Q64_INS_H

/*写保护控制指令*/
#define W25Q64_WRITE_ENABLE 0x06  /*写使能，执行写入/擦除操作前必须发送*/
#define W25Q64_WRITE_DISABLE 0x04 /*写禁止，取消写使能状态*/

/*状态寄存器操作指令*/
#define W25Q64_READ_STATUS_REGISTER_1 0x05 /*读取状态寄存器1?*/
#define W25Q64_READ_STATUS_REGISTER_2 0x35 /*读取状态寄存器2*/
#define W25Q64_WRITE_STATUS_REGISTER 0x01  /*写入状态寄存器*/

/*存储操作指令*/
#define W25Q64_PAGE_PROGRAM 0x02      /*页编程，可写入1-256字节数据*/
#define W25Q64_QUAD_PAGE_PROGRAM 0x32 /*四线页编程，提高写入速度*/

/*擦除操作*/
#define W25Q64_BLOCK_ERASE_64KB 0xD8 /* 擦除64KB块*/
#define W25Q64_BLOCK_ERASE_32KB 0x52 /*擦除32KB扇区*/
#define W25Q64_SECTOR_ERASE_4KB 0x20 /*擦除4KB扇区*/
#define W25Q64_CHIP_ERASE 0xC7       /*整片擦除*/
#define W25Q64_ERASE_SUSPEND 0x75    /* 暂停擦除操作*/
#define W25Q64_ERASE_RESUME 0x7A     /*恢复擦除操作*/

/*设备配置与ID读取指令*/
#define W25Q64_POWER_DOWN 0xB9                       /*掉电模式*/
#define W25Q64_HIGH_PERFORMANCE_MODE 0xA3            /*高性能模式*/
#define W25Q64_CONTINUOUS_READ_MODE_RESET 0xFF       /*连续读取模式复位*/
#define W25Q64_RELEASE_POWER_DOWN_HPM_DEVICE_ID 0xAB /*退出掉电并读取设备ID*/
#define W25Q64_MANUFACTURER_DEVICE_ID 0x90           /* 读取制造商和设备ID*/
#define W25Q64_READ_UNIQUE_ID 0x4B                   /*读取唯一ID*/
#define W25Q64_JEDEC_ID 0x9F                         /*读取JEDEC ID*/

/*读取操作指令*/
#define W25Q64_READ_DATA 0x03               /*标准读取数据*/
#define W25Q64_FAST_READ 0x0B               /* 快速读取（需要1个虚拟时钟周期*/
#define W25Q64_FAST_READ_DUAL_OUTPUT 0x3B   /*双输出快速读取*/
#define W25Q64_FAST_READ_DUAL_IO 0xBB       /*双IO快速读取*/
#define W25Q64_FAST_READ_QUAD_OUTPUT 0x6B   /*四输出快速读取*/
#define W25Q64_FAST_READ_QUAD_IO 0xEB       /*四IO快速读取*/
#define W25Q64_OCTAL_WORD_READ_QUAD_IO 0xE3 /*八字四IO读取*/

/*辅助定义*/
#define W25Q64_DUMMY_BYTE 0xFF /*用于数据交换*/

#endif
