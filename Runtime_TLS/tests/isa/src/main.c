/*
 *  ISA_Test for a modified Cv32e40p (RV32IMC) Processor Core.
 *
 *  Copyright (C) 2021-2023 Junnan Li <lijunnan@nudt.edu.cn>.
 *  Copyright and related rights are licensed under the MIT license.
 *
 *  Date: 2023.04.23
 *  Description: Used to test RISC-V ISA. 
 */

#include "firmware.h"

int main(){
    //* system inilization, open all interrupt (32_bitmap);
    irq_init();

    //* for timer_irq;
    timer_irq_count = 0;
    
    printf("\rsystem boot finished\r\n");
    
    //* set timer_irq_value, i.e., SYSTICK_ADDR
    // *((volatile uint32_t *) SYSTICK_ADDR) = 50000000;

    //* test isa;
    __TEST_RV32UI_ISA();
    __TEST_RV32UM_ISA();
    printf("ISA test is pased\r\n");
    while(1);

    return 0;
}

