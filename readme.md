

# FengLin M32 (FL-M32) SoC Runtime

FL-M32 is a small and efficient, 32-bit, RISCV-based SoC for Network processing.
FL-M32_Runtime is the runtime environment for FL-M32 SoC, and has implemented some basic 
functions to simplify C-based development.

## modification list：
* 2023/3/22：add RAM_SIZE_KB at Firmware_SingleCore_Runtime/Makefile.sys,
    which used to calculate RAM_SIZE, INSTR_LEN, DATA_BASE_ADDR and 
    DATA_LEN; RAM_SIZE is lines of firmware.hex, while {INSTR_LEN,...} are
    used to reconfigure memory size in link/link.ld;
        Modified in [Makefile.sys](https://github.com/JunnanLi/FL-M32_Runtime/Firmware_SingleCore_Runtime/Makefile.sys).
           
           RAM_SIZE_KB     = 256 #default                                 
           RAM_SIZE        = $(shell expr $(RAM_SIZE_KB) \* 256)
           INSTR_BASE_ADDR = 0
           INSTR_LEN       = $(shell expr $(RAM_SIZE_KB) \* 128)
           DATA_BASE_ADDR  = $(INSTR_LEN)
           DATA_LEN        = $(INSTR_LEN)


    Modified in [link.ld](https://github.com/JunnanLi/FL-M32_Runtime/Firmware_SingleCore_Runtime/link/link.ld)
           ram (rwxai) : ORIGIN = 0x00000000, LENGTH = 0x10000
       ram_data (rwxai) : ORIGIN = 0x00010000, LENGTH = 0x10000

    So, you can use makefile with "make RAM_SIZE_KB=128" for 128KB Memory;

* 2023/3/22：fix the difination of 32b irq_bitmap;

       i.e.,                                                           
           `define TIME_IRQ      7
           `define UART_IRQ      16  
           `define GPIO_IRQ      17  
           `define SPI_IRQ       18  
           `define CSR_IRQ       19  
           `define CSRAM_IRQ     20  
           `define dDMA_IRQ      21  
           `define DMA_IRQ       22  
           `define DRA_IRQ       23 


* 2023/3/25：add TIMER_IRQ_PROC_IN_ASM at Firmware_SingleCore_Runtime/     
    Makefile.sys, which used to compile timer irq processing writen in asm;
    add DRA_IRQ_PROC_IN_ASM at Firmware_SingleCore_Runtime/Makefile.sys,      
    which used to compile dma irq processing writen in asm;

       i.e.,                                                           
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

    So, you can use makefile with "make TIMER_IRQ_PROC_IN_ASM=1" to choose 
     processing time irq in asm or "make TIMER_IRQ_PROC_IN_ASM=0" to
     choose processing time irq in c; it is same to DMA_IRQ_PROC_IN_ASM=1;
