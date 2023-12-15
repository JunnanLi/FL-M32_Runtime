

# FengLin M32 (FL-M32) SoC Runtime

FL-M32是一个面向网络处理的小型但高效的32位RISCV片上系统。
FL-M32_runtime是FL-M32 SOC的运行时环境，提供一些基本C开发环境，用于简化用户开发C程序的难度。

## 示例程序

[singleCore_Replace_MAC_Addr](https://github.com/JunnanLi/FL-M32_Runtime/tree/main/Runtime_TLS/demos/singleCore_Replace_MAC_Addr) 为C程序示例代码，实现了源目的MAC地址替换功能。

## 更新日志
* 2023/3/22：在Firmware_SingleCore_Runtime/Makefile.sys中添加RAM_SIZE_KB定义，表示
    指令/数RAM块大小，即RAM_SIZE，并根据该值计算INSTR_LEN, DATA_BASE_ADDR和
    DATA_LEN，以用于更新链接脚本，即link/link.ld文件；
    
    [Makefile.sys](https://github.com/JunnanLi/FL-M32_Runtime/tree/main/Runtime_TLS/FirmwareRuntime/Makefile.sys)更新如下：
           
       RAM_SIZE_KB     = 256 #default                                 
       RAM_SIZE        = $(shell expr $(RAM_SIZE_KB) \* 256)
       INSTR_BASE_ADDR = 0
       INSTR_LEN       = $(shell expr $(RAM_SIZE_KB) \* 128)
       DATA_BASE_ADDR  = $(INSTR_LEN)
       DATA_LEN        = $(INSTR_LEN)

    ORIGIN和LENGTH由Makefile自动更新 ([link.ld](https://github.com/JunnanLi/FL-M32_Runtime/tree/main/Runtime_TLS/FirmwareRuntime/link/link.ld))： 
    
             ram (rwxai) : ORIGIN = 0x00000000, LENGTH = 0x10000
        ram_data (rwxai) : ORIGIN = 0x00010000, LENGTH = 0x10000

    例如, 可以运行"make RAM_SIZE_KB=128"来生成指令/数据块空间为128KB的bin/fex文件;

* 2023/3/22：更新irq定义，固定irq的含义，即修改global_head.v中断相应定义：

       `define TIME_IRQ      7
       `define UART_IRQ      16  
       `define GPIO_IRQ      17  
       `define SPI_IRQ       18  
       `define CSR_IRQ       19  
       `define CSRAM_IRQ     20  
       `define dDMA_IRQ      21  
       `define DMA_IRQ       22  
       `define DRA_IRQ       23 


* 2023/3/25：在[Makefile.sys](https://github.com/JunnanLi/FL-M32_Runtime/tree/main/Runtime_TLS/FirmwareRuntime/Makefile.sys)中增加TIMER_IRQ_PROC_IN_ASM定义, 用于标识是否在中断处理中采用时间中断汇编处理函数；在[Makefile.sys](https://github.com/JunnanLi/FL-M32_Runtime/tree/main/Runtime_TLS/FirmwareRuntime/Makefile.sys)增加DMA_IRQ_PROC_IN_ASM定义, 用于标识是否在DMA中断处理中采用时间中断汇编处理函数；
                                                         
       TIMER_IRQ_PROC_IN_ASM   = 0
       DMA_IRQ_PROC_IN_ASM     = 0
       ifeq ("$(TIMER_IRQ_PROC_IN_ASM)", "1")
           TIMER_IRQ_IN_ASM    = -DTIMER_IRQ_IN_ASM=1
       else
           TIMER_IRQ_IN_ASM    = -DTIMER_IRQ_IN_ASM=0
       endif
       ifeq ("$(DMA_IRQ_PROC_IN_ASM)", "1")
           DMA_IRQ_IN_ASM      = -DDMA_IRQ_IN_ASM=1
       else
           DMA_IRQ_IN_ASM      = -DDMA_IRQ_IN_ASM=0
       endif

    例如，可以使用"make TIMER_IRQ_PROC_IN_ASM=1"来选择使用时间中断汇编处理函数，以及"make TIMER_IRQ_PROC_IN_ASM=0"选择使用基于C程序的时间中断处理函数；同理，使用DMA_IRQ_PROC_IN_ASM=1，选择基于汇编的DMA中断处理函数。

## 其他
如有疑问或者建议，欢迎私信或者发邮件(nudt_ljn@163.com)给我。