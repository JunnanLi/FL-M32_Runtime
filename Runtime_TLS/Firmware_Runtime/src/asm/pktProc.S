/* Copyright (c) 2022-2023  National University of Defense and Technology.
 * This copyrighted material is made available to anyone wishing to use,
 * modify, copy, or redistribute it subject to the terms and conditions
 * of the FreeBSD License.   
 */

#include "custom_ops.S" //* for DRA;

    .global __test_custom_instr
    .type   __test_custom_instr, @function
__test_custom_instr:
    rf2_addi_instr(10, x1, x1);
    rf2_slti_instr(10, x1, x2);
    rf2_sltiu_instr(11, x1, x3);
    rf2_xori_instr(11, x1, x4);
    rf2_ori_instr(11, x4, x5);
    rf2_andi_instr(10, x5, x6);
    rf2_slli_instr(8, x1, x7);
    rf2_srli_instr(4, x1, x8);
    rf2_srai_instr(1, x1, x9);

    rf2_exch_instr(1, x1, x31);
    rf2_exch_instr(0, x1, x10);

    rf2_lui_instr(0x10000, x11);

    rf2_lb_instr(0, x0, x12);
    rf2_lh_instr(0, x0, x13);
    rf2_lw_instr(0, x0, x14);
    rf2_lbu_instr(0, x0, x15);
    rf2_lhu_instr(0, x0, x16);

    rf2_sb_instr(0, x11, x12);
    rf2_sh_instr(0, x11, x13);
    rf2_sw_instr(0, x11, x14);

    rf2_lb_instr(0, x11, x14);
    rf2_lh_instr(0, x11, x13);
    rf2_lw_instr(0, x11, x12);

    rf2_add_instr(x1, x7, x17);
    rf2_sub_instr(x1, x8, x18);
    rf2_sll_instr(x1, x9, x19);
    rf2_slt_instr(x1, x0, x20);
    rf2_sltu_instr(x1, x1, x21);
    rf2_xor_instr(x1, x2, x22);
    rf2_srl_instr(x1, x3, x23);
    rf2_sra_instr(x1, x4, x24);
    rf2_or_instr(x1, x5, x25);
    rf2_and_instr(x1, x6, x26);
    ret
    .size   __test_custom_instr, . - __test_custom_instr
    

    .global __to_recv_pkt
    .type   __to_recv_pkt, @function
__to_recv_pkt:
    rf2_lui_instr(0x20000000, x30); //* to recv pkt;
    addi x0, x1, 0
    addi x0, x2, 0
    rf2_exch_instr(1, x30, x10);    //* exchange to common reg (a0);
    ret
    .size   __to_recv_pkt, . - __to_recv_pkt

    .global __test_proc_pkt
    .type   __test_proc_pkt, @function
__test_proc_pkt:
    rf2_slli_instr(16, x16, x20);
    rf2_srli_instr(16, x20, x20);
    rf2_exch_instr(1, x20, x10);   //* get length;
    ret
    .size   __test_proc_pkt, . - __test_proc_pkt

    .global __test_replace_mac
    .type   __test_replace_mac, @function
__test_replace_mac:
    rf2_srli_instr(16, x0, x20);    //* get dmac[0:1] in low-16b
    rf2_slli_instr(16, x0, x21);    //* get dmac[2:3] in high-16b
    rf2_srli_instr(16, x1, x22);    //* get dmac[3:4] in low-16b
    rf2_slli_instr(16, x1, x24);    //* get smac[0:1] in high-16b
    rf2_srli_instr(16, x2, x25);    //* get smac[2:3] in low-16b
    rf2_slli_instr(16, x2, x26);    //* get smac[3:4] in high-16b
    
    rf2_add_instr(x24, x25,x0);
    rf2_add_instr(x20, x26,x1);
    rf2_add_instr(x21, x22,x2);

    rf2_slli_instr(16, x16, x27);
    rf2_srli_instr(16, x27, x27);
    rf2_exch_instr(1,  x27, x10);   //* get length;
    ret
    .size   __test_replace_mac, . - __test_replace_mac

    .global __add_next_512b_first32b_with_1
    .type   __add_next_512b_first32b_with_1, @function
__add_next_512b_first32b_with_1:
    rf2_lui_instr(0x40000000, x30); //* to write data;
    rf2_lui_instr(0x80000000, x30); //* to read data;
    rf2_slli_instr(0,  x26, x27);
    rf2_slli_instr(0,  x26, x27);
    rf2_slli_instr(0,  x26, x27);
    rf2_slli_instr(0,  x26, x27);
    rf2_slli_instr(0,  x26, x27);
    rf2_slli_instr(0,  x26, x27);
    rf2_lui_instr(0x8000000, x30);  //* to replace data;
    rf2_slli_instr(0,  x26, x27);
    rf2_slli_instr(0,  x26, x27);

    rf2_addi_instr(1, x15,  x15);   //* add 1;

    // rf2_slli_instr(16, x16, x27);
    // rf2_srli_instr(16, x27, x27);
    // rf2_exch_instr(1,  x27, x10);   //* get length;
    ret
    .size   __add_next_512b_first32b_with_1, . - __add_next_512b_first32b_with_1

    .global __to_send_pkt
    .type   __to_send_pkt, @function
__to_send_pkt:
    rf2_slli_instr(4, x16, x16);
    rf2_srli_instr(4, x16, x16);
    rf2_exch_instr(0,  x10, x27);   //* get next PE;
    rf2_slli_instr(28, x27, x27);
    rf2_or_instr(x27,  x16, x16);   //* write next PE to metadata;
    rf2_lui_instr(0x10000000, x30); //* to send pkt;
    ret
    .size   __to_send_pkt, . - __to_send_pkt

    .global __test_send_pkt
    .type   __test_send_pkt, @function
__test_send_pkt:
    rf2_slli_instr(4, x16, x16);
    rf2_srli_instr(4, x16, x16);
    rf2_lui_instr(0x10000000, x30); //* to send pkt;
    ret
    .size   __test_send_pkt, . - __test_send_pkt

    .global __read_type
    .type   __read_type, @function
__read_type:
    rf2_srli_instr(16, x3, x20);
    rf2_exch_instr(1, x20, x10);   //* get length;
    ret
    .size   __read_type, . - __read_type

    .global __read_next_512b_first32b
    .type   __read_next_512b_first32b, @function
__read_next_512b_first32b:
    rf2_lui_instr(0x80000000, x30); //* to read data;
    rf2_slli_instr(0,  x26, x27);
    rf2_slli_instr(0,  x26, x27);
    rf2_slli_instr(0,  x26, x27);
    rf2_slli_instr(0,  x26, x27);
    rf2_slli_instr(0,  x26, x27);
    rf2_slli_instr(0,  x26, x27);

    rf2_lui_instr(0x8000000, x30);  //* to replace data;
    rf2_slli_instr(0,  x26, x27);
    rf2_slli_instr(0,  x26, x27);

    rf2_exch_instr(1,  x0,  x10);    //* return las 32b;
    ret
    .size   __read_next_512b_first32b, . - __read_next_512b_first32b