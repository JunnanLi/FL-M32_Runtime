/*
 *  LwIP for a modified Cv32e40p (RV32IMC) Processor Core.
 *
 *  Copyright (C) 2021-2022 Junnan Li <lijunnan@nudt.edu.cn>. All Rights Reserved.
 *  Copyright and related rights are licensed under the MIT license.
 *
 *  Last updated date: 2022.01.18
 *  Description: basic processing. 
 *  1 tab == 4 spaces!
 */

#ifndef FIRMWARE_H
#define FIRMWARE_H
#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>
// #include <sys/stat.h>
// #include <unistd.h>
// #include <errno.h>
#include <stdarg.h>

#define MSTATUS_MIE_BIT     3           //* for irq;

//>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>//
//* Special address in iCore                                                      //
//<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<//
#define OUTPORT             0x10010004  //* Print address, i.e., UART write;
#define UART_RD_ADDR        0x10010000  //* UART read;
#define UART_WR_ADDR        0x10010004  //* UART write;

//>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>//
//* 0x1002xxxx is left for GPIO                                                   //
//*     0x10020000: '1' to open GPIO;                                             //
//*     0x10020004: 16b bitmap, and '0' to recv/send data, '1' for irq;           //
//*     0x10020008: 16b bitmap, and '0' to recv posedge irq, '1' for negedge;     //
//*     0x1002000c: 16b bitmap, irq result (r);                                   //
//*     0x10020010: 16b bitmap, and '1' to clear irq (w, just maintain one clk);  //
//*     0x10020014: 16b bitmap, and '0' to recv data, '1' for sending;            //
//*     0x10020018: received data;                                                //
//*     0x1002001c: data to send;                                                 //
//* Tips: Low-to-high bits of bitmap correspond to line sequence from 0 to 15.    //
//<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<//
//* Changed by LCL7.                                                              //
//<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<//
#define GPIO_ENABLE                 0x10020000
#define GPIO_DATA_IRQ_SET           0x10020004
#define GPIO_IRQ_SAMPLE_SET         0x10020008
#define GPIO_IRQ_RESULT             0x1002000c
#define GPIO_IRQ_CLEAR              0x10020010
#define GPIO_DATA_DIRECTION_SET     0x10020014
#define GPIO_DATA_RECV              0x10020018
#define GPIO_DATA_SEND              0x1002001c

//>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>//
//* 0x1003xxxx is left for SPI                                                    //
//*     0x10030000: addr to read flash by spi;                                    //
//*     0x10030004: length to read flash by spi;                                  //
//*     0x10030008: request to read flash;                                        //
//*     0x10030010: empty_tag, '0' means received respond_data from flash (r);    //
//*     0x10030014: count of respond_data waiting to read (r);                    //
//*     0x10030018: respond_data (r);                                             //
//--------------------------------------------------------------------------------//
//*     0x10030100: high 32b of request command_data;                             //
//*     0x10030104: low 32b of request command_data;                              //
//*     0x10030108: high 32b of respond command_data (r);                         //
//*     0x1003010c: high 32b of respond command_data (r);                         //
//*     0x10030110: empty_tag, '0' means received respond_data from flash (r);    //
//*     0x10030114: count of respond_data waiting to read (r);                    //
//<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<//
#define FLASH_ADDR          0x10030000
#define FLASH_LENGTH        0x10030004
#define FLASH_REQ           0x10030008
#define FLASH_RESP_TAG      0x10030010
#define FLASH_RESP_USEDW    0x10030014
#define FLASH_RESP_RDATA    0x10030018

#define COMMD_HIGH_32b      0x10030100
#define COMMD_LOW_32b       0x10030104
#define COMMD_RESP_HIGH_32b 0x10030108
#define COMMD_RESP_LOW_32b  0x1003010C
#define COMMD_RESP_TAG      0x10030110
#define COMMD_RESP_USEDW    0x10030114

//>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>//
//* 0x1004xxxx is left for CSR                                                    //
//*     0x10040000: start_en, high 16b is control valid: '1' is valid;            //
//*                           low 16b is control value:                           //
//*                                        '1' to enable the corresponding PE;    //
//*                                        '0' to disable the corresponding PE;   //
//*     0x10040004: guard reg., no-shared (i.e. exclusive for each PE),           //
//*                           0x1234 is valid;                                    //
//*     0x10040008: software version (wr);                                        //
//*     0x1004000c: hardware version (r);                                         //
//*     0x10040010: pe id, 0-2 (r);                                               //
//*     0x10040014: base address (i.e. offset) of instruction RAM for PE_1;       //
//*     0x10040018: base address (i.e. offset) of instruction RAM for PE_2;       //
//*     0x1004001c: base address (i.e. offset) of data RAM for PE_1;              //
//*     0x10040020: base address (i.e. offset) of data RAM for PE_2;              //
//*     0x10040024: to minus system s register (w);                               //
//*     0x10040028: to add system s register (w);                                 //
//*     0x1004002c: to minus system ns register (w);                              //
//*     0x10040030: to add system ns register (w); or to read system ns register; //
//*     0x10040034: to read system s register (r), should read ns register first; //
//*     0x10040038: internal time to gen irq;                                     //
//*     0x1004003c: '1' to reset AiPE;                                            //
//*     0x10040040                                                                //
//*         -0x1004005c: shared registers;                                        //
//*     0x1004007C: x ns/clk, e.g., 20 ns/clk in 50MHz                            //
//<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<//
#define TIMER_NS_ADDR               0x10040030  //* system timer address, ns;
#define TIMER_S_ADDR                0x10040034  //* system timer address, s;
#define SYSTICK_ADDR                0x10040038  //* system timer address, 20ns;

#define CSR_PE_START_EN             0x10040000
#define CSR_GUARD_REG_ADDR          0x10040004
#define CSR_SW_VERSION              0x10040008
#define CSR_CUR_PE_ID               0x10040010  //* current PE (i.e. CPU core) ID
#define CSR_PE1_INSTR_BASE_ADDR     0x10040014
#define CSR_PE2_INSTR_BASE_ADDR     0x10040018
#define CSR_PE1_DATA_BASE_ADDR      0x1004001c
#define CSR_PE2_DATA_BASE_ADDR      0x10040020

//* systime time related registers;
#define CSR_MINUS_SYSTIME_S         0x10040024
#define CSR_ADD_SYSTIME_S           0x10040028
#define CSR_MINUS_SYSTIME_NS        0x1004002c
#define CSR_ADD_SYSTIME_NS          0x10040030
#define CSR_READ_SYSTIME_NS         0x10040030
#define CSR_READ_SYSTIME_S          0x10040034

//* shared registers;
#define CSR_SHARED_REG_TMP1_ADDR    0x10040040
#define CSR_SHARED_REG_TMP2_ADDR    0x10040044

//* cycle
#define CSR_CYCLE_LOW_ADDR          0x10040060
#define CSR_CYCLE_HIGH_ADDR         0x10040064

#define CSR_NS_PER_CLK              0x1004007C

//>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>//
//* 0x1005xxxx is left for CSRAM                                                  //
//*     0x10050000                                                                //
//*         -0x10050100: shared SRAM;                                             //
//<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<//
#define CSRAM_ADDR                  0x10050000

//>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>//
//* 0x1006xxxx is left for dDMA                                                   //
//*     0x10060000: start address for common PEs;                                 //
//*     0x10060004: length to read for common PEs;                                //
//*     0x10060008: start address for AiPE;                                       //
//*     0x1006000c: length to read for AiPE;                                      //
//*     0x10060010: direction, '0' is common PEs -> AiPE, '1' is AiPE -> PEs;     //
//*     0x10060014: write any value to request dma;                               //
//<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<//

//>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>//
//* 0x1007xxxx is left for DMA                                                    //
//*     0x10070000: irq_info (r), '0x80000000' is empty;                          //
//*     '8xxxxxxxx' is irq of finishing recving; low 16b is the first addr of DMA;//
//*     '0xxxxxxxx' is irq of finishing sending; low 16b is the first addr of DMA;//
//*     0x10070004: length info of received pkt (r);                              //
//*     0x10070008: low 16b is lenght to DMA, from NIC to PE (w);                 //
//*     0x1007000c: start addr to buffer pkt, from NIC to PE (w);                 //
//*     0x10070010: low 16b is lenght to DMA, from PE to NIC (w);                 //
//*     0x10070014: start addr to read pkt, from PE to NIC (w);                   //
//*     0x10070018: shared register can be used to buffer count of received pkts; //
//*     0x1007001c: '1' to allow back pressure for receiving pkts;                //
//*     0x10070020: '1' to reset DMA;                                             //
//<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<//
#define DMA_INT_ADDR        0x10070000  //* int. type [31] == '0' is write, [30:0] is start addr;
#define DMA_RECV_LEN_ADDR   0x10070008  //* pBuf address for receiveing pkt;
#define DMA_RECV_ADDR_ADDR  0x1007000c  //* pBuf length for receiveing pkt;
#define DMA_SEND_LEN_ADDR   0x10070010  //* pBuf address for sending pkt;
#define DMA_SEND_ADDR_ADDR  0x10070014  //* pBuf length for sending pkt;
#define DMA_TAG_ADDR        0x10070004  //* pBuf address for recv tag & length;
#define DMA_CNT_RECV_PKT    0x10070018  //* cnt of recved pkt (finished dma);
#define DMA_START_EN        0x1007001C  //* open DMA function;
#define DMA_FILTER_EN       0x10070020  //* to filter pkt, default is broadcast 
                                        //*     pkt (48'hffff_ffff_ffff) to 3 PEs;
#define DMA_FILTER_TYPE_EN  0x10070024  //* {o_filter_dmac_en, o_filter_smac_en, o_filter_type_en}; 
#define DMA_FILTER_DMAC     0x10070028  //* to filter pkt by dmac (last 8b);
#define DMA_FILTER_SMAC     0x1007002c  //* to filter pkt by smac (last 8b);
#define DMA_FILTER_TYPE     0x10070030  //* to filter pkt by type (last 8b);
#define DMA_WAIT_PBUFWR     0x10070034  //* state_dma is at i_wait_free_pBufWR;

//>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>//
//* 0x1008xxxx is left for DRA                                                    //
//*     0x10080000: guart register, 0x1234 is valid;                              //
//*     0x10080004: '1' to start DRA;                                             //
//*     0x10080008: '1' to reset DRA;                                             //
//<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<//
#define DRA_GUARD_ADDR      0x10080000  //* write '0x1234' before configuring other regs;
#define DRA_START_EN_ADDR   0x10080004  //* '1' is enable;

//>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>//
//*     time_spec struct                       //
//<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<//
struct time_spec{
    uint32_t tv_sec;
    uint32_t tv_nsec;
};

volatile unsigned int timer_irq_count;
volatile unsigned int recv_cnt, dma_recv_cnt;
volatile unsigned int send_cnt, dma_send_cnt;

#define CPU_FREQ_HZ   (50000000)  // 50MHz
#define CPU_FREQ_MHZ  (50)        // 50MHz

#endif
