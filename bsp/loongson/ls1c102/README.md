龙芯1C板级支持包


---

## 1. 简介

龙芯1C102

此BSP基于龙芯1C102工程：

| 硬件 | 描述 |
| -- | -- |
|芯片型号| 1C102 |
|CPU| LA132 |
|主频| 最高10MHz |
|IRAM| 4KB |
|DRAM| 4KB |
|片内Flash| 128KB |

## 2. 编译说明

如果编译正确无误，会产生rtthread.elf、rtthread.bin文件。

## 3. 烧写及执行

使用龙芯官方EJTAG仿真器下载

### 3.1 运行结果

如果编译 & 烧写无误，会在串口上看到RT-Thread的启动logo信息：

```

 \ | /
- RT -     Thread Operating System
 / | \     4.0.0 build Jan  5 2019
 2006 - 2018 Copyright by rt-thread team
msh >

```

## 4. 驱动支持情况及计划

| 驱动 | 支持情况  |  备注  |
| ------ | ----  | :------:  |
| UART | 支持 | 仅注册UART0 |
| GPIO | 支持 |  |

### 4.1 IO在板级支持包中的映射情况

| IO号 | 板级包中的定义 |
| -- | -- |
| 调试串口 | GPIO6/GPIO7 |

## 5. 联系人信息

维护人：

## 6. 参考

* LS1C101芯片[用户手册][3]
* LS1C101芯片[数据手册][4]
* 龙芯1C101 RT-thread 4.0移植[5]

  [1]: https://www.rt-thread.org/page/download.html
  [2]: https://sourcery.mentor.com/GNUToolchain/package13851/public/mips-sde-elf/mips-2016.05-7-mips-sde-elf.exe
  [3]: http://www.loongson.cn/uploadfile/cpu/1C101/Loongson_1C101_user.pdf
  [4]: http://www.loongson.cn/uploadfile/cpu/1C101/Loongson_1C101_data.pdf
  [5]: https://gitee.com/zhuangwei123/ls1c101
