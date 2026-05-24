# STM32 标准库 学习项目 (STM32 Standard Peripheral Library Learning Project)

![Language](https://img.shields.io/badge/Language-C-blue.svg)
![Framework](https://img.shields.io/badge/Framework-Standard%20Peripheral%20Library-green.svg)
![IDE](https://img.shields.io/badge/IDE-Keil%20MDK-brightgreen.svg)
![MCU](https://img.shields.io/badge/MCU-STM32F103C8T6-blue.svg)

本项目是基于 STM32 标准外设库 (Standard Peripheral Library) 的学习实践记录，参考了江协科技的经典教程。项目深入探讨了 STM32 的寄存器原理与外设工作机制，是理解底层硬件逻辑的绝佳实践。

---

## 🎯 项目简介
本项目包含了从寄存器级别操作到标准库高级应用的 50 余个实验。与 HAL 库相比，标准库更直接地反映了芯片手册（Datasheet）中的寄存器配置过程，有助于构建扎实的嵌入式底层基础。

---

## 🗺️ 实验分类与学习路径

### 1. 入门与基础操作 (Basics)
- 2-1 寄存器点灯: 纯寄存器操作，理解 GPIO 地址映射
- 2-2 库函数点灯: 开始使用官方标准库封装函数
- 2-3 库函数流水灯: GPIO 多引脚循环控制
- 3-3~3-5: 蜂鸣器驱动、按键扫描、模拟传感器逻辑处理

### 2. 中断与定时器系统 (Interrupts & Timers)
- 5-1~5-2 EXTI: 外部中断原理，处理红外传感器与编码器计次
- 6-1~6-2 Timer: 定时器中断基础，使用外部时钟 (ETR) 计数
- 6-3~6-5 PWM: PWM 输出控制（LED 呼吸灯、舵机、直流电机）
- 6-6~6-7 IC/PWMI: 输入捕获测量频率与占空比，理解频率计原理
- 6-8 Encoder: 硬件编码器接口模式，实现高效测速

### 3. 模拟采集与数据搬运 (ADC & DMA)
- 7-1 ADC: 单通道采集，理解逐次逼近寄存器 (SAR) 原理
- 7-2 ADC: 软件触发多通道轮询采集
- 8-1 DMA: 存储器间传输 (M2M)，理解 DMA 通道与优先级
- 8-2 DMA+ADC: **核心应用**，ADC 扫描模式结合 DMA 自动搬运，实现高采样率多路监控

### 4. 串行通讯总线 (Communications)
- 9-1~9-4 USART: 串口协议核心，实现 HEX/文本数据包解析
- 10-1~10-2 I2C: **深度对比** 软件模拟 I2C 与 硬件 I2C 驱动 MPU6050
- 11-1~11-2 SPI: **深度对比** 软件模拟 SPI 与 硬件 SPI 驱动 W25Q64 Flash

### 5. 系统内核与高级功能 (System & Advanced)
- 12-1 BKP: 备份寄存器与侵入检测
- 12-2~12-4 RTC: 实时时钟配置，学习 LSE 外部低速晶振的使用
- 13-1~13-4 PWR: 电源管理，修改主频以及进入不同等级的低功耗模式
- 14-1~14-2 WDG: 独立看门狗 (IWDG) 与 窗口看门狗 (WWDG) 详解
- 15-1~15-2 Flash: 内部 Flash 擦写，读取芯片 ID 等唯一标识

---

## 📚 重点参考资料
- 📄 [STM32 标准库常用函数笔记](./STM32库函数.docx): 整理了常用外设的 API 说明与配置步骤。

---

## 🛠️ 开发环境与调试
### 1. 软件安装
- **Keil MDK-ARM**: 建议版本 v5.25 或以上。
- **Pack**: 安装 Keil.STM32F1xx_DFP 器件包。

### 2. 工程打开方式
- 每个文件夹下的 Project.uvprojx 即为 Keil 工程文件。
- 如果提示路径丢失，请检查是否移动了文件夹位置。

### 3. 下载器配置
- 推荐使用 **ST-Link V2** 或 **DAP-Link**。
- 在 Keil 中设置：Options for Target -> Debug -> 选择你的调试器 -> Settings -> Flash Download 勾选 Reset and Run。

---
*Created with ❤️ by [Your Name/Github Username]*
