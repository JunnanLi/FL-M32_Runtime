#include "firmware.h"
#include "../system/system.h"

#ifndef _MULTICORE_H_
#define _MULTICORE_H_

int csr_pe_start_en(uint32_t pe_idx);
int csr_pe_disable_others(void);

int multicore_initial(void);
int pe1_initial(void);
int pe2_initial(void);

#endif
