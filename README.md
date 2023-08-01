# 多功能运维ROV嵌入式开发框架
___
## 1.前言
本项目采用cmake + make + arm-none-eabi-gcc 交叉编译工具链开发，下载程序到单片机使用openocd
`请事先下载好对应环境`</br>
_Linux环境安装_</br>
```bash
sudo apt-get install arm-none-eabi-gcc
sudo apt-get install openocd
```
_Win环境安装_</br>
交叉编译器：进入ARM官网下载[下载链接](https://developer.arm.com/downloads/-/gnu-rm)</br>
openocd：[下载链接](https://github.com/xpack-dev-tools/openocd-xpack/releases/)</br>
整个windows环境下工具链开发可以参考文章：[参考文章](https://zhuanlan.zhihu.com/p/576972892)</br>
`项目使用make，所以需要安装mingw，无需安装Ninja作为构建工具`

## 2.食用
_win环境下需要安装Git_
```bash
git clone git@github.com:Hao-Lion-ZJU/ROV_SHKJ.git
mkdir build -p && cd build
#windows环境下需要指定构建工具，否则win下cmake会自动帮你选择nmake
#然而这不是我们期望的，所以需要 -G "Unix Makefiles"指定，Linux环境下不需要
cmake -G "Unix Makefiles" ..
#根据你CPU的最大核心数决定
make -j8
#连接仿真器下载,项目使用STLINK，如果更换，请修改openocd.cfg中的配置
openocd -f ./openocd.cfg -c download
```
## 3.简介
### 设备
|硬件|型号|参数|
|---|---|---|
|主控芯片|STM32H743IIT6|480MHZ,2MB Flash,1MB RAM|
### 软件环境

 - Toolchain/IDE : cmake + make + arm-none-eabi-gcc / VSCode
 - STM32CubeMx:6.8.0
 - package version: STM32Cube FW_H7 V1.11.0
 - FreeRTOS version: 10.3.1
 - LWIP version: 2.1.2
 - CMSIS-RTOS version: 1.02
### 编程规范
- 变量和函数命名方式遵循 Unix/Linux 风格
- 不需要精确计时的任务，采用自行实现的软件定时器实现，定时精度受任务调度影响

## 4.整体框架
### 文件树  
```
ROV/
├── application
│   └── protocol（通信协议定义头文件）
│   ├── (others).c/h（各个任务的处理函数源文件）
├── bsp					(板级支持包)
├── components
│   ├──algorithm （存放常用数学算法）
│   ├── controller（存放控制器实现）
│   └── devices（存放ROV上设备的驱动文件）
│   ├── support（存放一些数据结构和校验算法）
├── Drivers	(CUBEMX生成HAL库驱动)
├── Inc		（CUBEMX生成外设头文件）
├── Middlewares（CUBEMX生成LWIP、FREERTOS等三方库）
├── Src		（CUBEMX生成外设源文件）
└── Startup	(启动文件)
├── openocd.cfg	(openocd配置文件，指定仿真器类型，端口等等)
├── ROV.ioc（CUBEMX工程）
├── STM32H743IITX_FLASH.ld（链接脚本）
└── STM32H743x.svd（CPU对应的外设地址映射表，供DEBUG查看寄存器状态时使用）
#其余文件与项目无关

```


