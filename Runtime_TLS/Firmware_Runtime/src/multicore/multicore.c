/*
 *  multicore for TimelyRV (RV32I) Processor Core.
 *
 * 	Anyone is free to copy, modify, publish, use, compile, sell, or
 * 	distribute this software, either in source code form or as a compiled
 * 	binary, for any purpose, commercial or non-commercial, and by any
 * 	means.
 *
 *  Date: 2022.10.13
 *  Description: used to start pe0/pe1/pe2. 
 */

#include "multicore.h"


/****************************************************************************************/
//* Function Description: Disable others PE (except PE-0)                               *
//* Input parameters: NONE                                                              *
//* Return value:                                                                       *
//*     0: success                                                                      *
/****************************************************************************************/
int csr_pe_disable_others(void)
{
    *((volatile uint32_t *) CSR_GUARD_REG_ADDR) = (uint32_t)0x1234;
    *((volatile uint32_t *) CSR_PE_START_EN) = 0x00070001;      // only enable PE-0
    return 0;
}

/****************************************************************************************/
//* Function Description: Enable selected PE (by pe_idx)                                *
//* Input parameters:                                                                   *
//*     pe_isx          : PE index, current is 1 or 2                                   *
//* Return value:                                                                       *
//*     0: success                                                                      *
/****************************************************************************************/
int csr_pe_start_en(uint32_t pe_idx)
{
    pe_idx = pe_idx & 0xf;      // currently only allow PE index from 0~15
    // printf("\r Pre CSR_PE_START_EN: 0x%08x\r\n", *((volatile uint32_t *) CSR_PE_START_EN));
    *((volatile uint32_t *) CSR_GUARD_REG_ADDR) = (uint32_t)0x1234;
    *((volatile uint32_t *) CSR_PE_START_EN) = *((volatile uint32_t *) CSR_PE_START_EN) | \
                                                ((1<<(pe_idx+16)) | (1<<pe_idx));

    return 0;
}

/****************************************************************************************/
//* Function Description: Initial PEs, should only called by one PE (default by PE-0)   *
//* Input parameters: NONE                                                              *
//* Return value:                                                                       *
//*     0: success                                                                      *
/****************************************************************************************/
int multicore_initial(void)
{
    uint32_t cur_pe_id = *((volatile uint32_t *) CSR_CUR_PE_ID);
    // printf("\r Cur PE is PE-%d\r\n", cur_pe_id);
    if (cur_pe_id == 0)
    {
        csr_pe_disable_others();        // only enable PE-0

        *((volatile uint32_t *) CSR_GUARD_REG_ADDR) = (uint32_t)0x1234;
        *((volatile uint32_t *) CSR_PE1_INSTR_BASE_ADDR) = 8192*4;
        *((volatile uint32_t *) CSR_GUARD_REG_ADDR) = (uint32_t)0x1234;
        *((volatile uint32_t *) CSR_PE1_DATA_BASE_ADDR) = 40960*4;
        csr_pe_start_en(1);             // enable PE-1

        *((volatile uint32_t *) CSR_GUARD_REG_ADDR) = (uint32_t)0x1234;
        *((volatile uint32_t *) CSR_PE2_INSTR_BASE_ADDR) = 16384*4;
        *((volatile uint32_t *) CSR_GUARD_REG_ADDR) = (uint32_t)0x1234;
        *((volatile uint32_t *) CSR_PE2_DATA_BASE_ADDR) = 49152*4;
        csr_pe_start_en(2);             // enable PE-2

        // printf("\r CSR_PE_START_EN: 0x%08x\r\n", *((volatile uint32_t *) CSR_PE_START_EN));
        // printf("\r MultiCore initial finished\r\n");

        while (*((volatile uint32_t *) CSR_SHARED_REG_TMP1_ADDR) == 0x0);
        // printf("\r Check PE-%d initial finished\r\n", *((volatile uint32_t *) CSR_SHARED_REG_TMP1_ADDR));
        *((volatile uint32_t *) CSR_SHARED_REG_TMP1_ADDR) = 0x0;
        while (*((volatile uint32_t *) CSR_SHARED_REG_TMP2_ADDR) == 0x0);
        // printf("\r Check PE-%d initial finished\r\n", *((volatile uint32_t *) CSR_SHARED_REG_TMP2_ADDR));
        *((volatile uint32_t *) CSR_SHARED_REG_TMP2_ADDR) = 0x0;
    }
    else
    {
        // printf("\r Error! PE-%d preempt PE-0\r\n", cur_pe_id);
    }

    return 0;
}

/****************************************************************************************/
//* Function Description: Initial PE-1                                                  *
//* Input parameters: NONE                                                              *
//* Return value:                                                                       *
//*     0: success                                                                      *
/****************************************************************************************/
int pe1_initial(void)
{
    uint32_t cur_pe_id = *((volatile uint32_t *) CSR_CUR_PE_ID);
    // printf("\r Cur PE is PE-%d\r\n", cur_pe_id);
    if (cur_pe_id == 1) *((volatile uint32_t *) CSR_SHARED_REG_TMP1_ADDR) = cur_pe_id;

    return 0;
}

/****************************************************************************************/
//* Function Description: Initial PE-2                                                  *
//* Input parameters: NONE                                                              *
//* Return value:                                                                       *
//*     0: success                                                                      *
/****************************************************************************************/
int pe2_initial(void)
{
    uint32_t cur_pe_id = *((volatile uint32_t *) CSR_CUR_PE_ID);
    // printf("\r Cur PE is PE-%d\r\n", cur_pe_id);
    if (cur_pe_id == 2) *((volatile uint32_t *) CSR_SHARED_REG_TMP2_ADDR) = cur_pe_id;

    return 0;
}