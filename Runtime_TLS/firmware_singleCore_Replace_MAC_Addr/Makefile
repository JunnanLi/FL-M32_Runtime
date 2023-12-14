# SHELL := /bin/bash
RUNTIME_PATH 	= ../Firmware_SingleCore_Runtime
include $(RUNTIME_PATH)/Makefile.sys

##############################################################################
## user-related param, you can add SRC,OBJS,DIR for new flods;
### SRC
MAIN_SRC 		= ${wildcard src/*.c}
### OBJ
MAIN_OBJS 		= $(patsubst %.c,%.o,$(notdir $(MAIN_SRC)))	
### DIR
MAIN_DIR		= src
##############################################################################

FIRMWARE_OBJS 	= $(addprefix obj/, ${MAIN_OBJS} ${SYSTEM_OBJS} $(IRQ_OBJS) \
					$(NETIF_OBJS) ${ASM_OBJS})
VPATH           = ${MAIN_DIR} ${SYSTEM_DIR} $(IRQ_DIR) \
					$(NETIF_DIR) ${ASM_DIR}
INCLUDES		= -I$(RUNTIME_PATH)/src 

all: hex_obj_dir conf_link_ld firmware
	@echo "---------------------------------------------------------"
	@echo "+  Current RAM size is $(RAM_SIZE_KB) KB"             
	@echo "+  Last modified date is 2023/03/23"                  
	@echo "---------------------------------------------------------"

##############################################################################
## gen firmware.hex
firmware: obj/firmware.bin
	python3 $(RUNTIME_PATH)/python_script/makehex.py $< $(RAM_SIZE) \
		> hex/$(addsuffix .hex,$@)
	python3 $(RUNTIME_PATH)/python_script/makehex_8files.py $< $(RAM_SIZE) 0\
		> hex/$(addsuffix _instr_0.hex,$@)
	python3 $(RUNTIME_PATH)/python_script/makehex_8files.py $< $(RAM_SIZE) 1\
		> hex/$(addsuffix _instr_1.hex,$@)
	python3 $(RUNTIME_PATH)/python_script/makehex_8files.py $< $(RAM_SIZE) 2\
		> hex/$(addsuffix _instr_2.hex,$@)
	python3 $(RUNTIME_PATH)/python_script/makehex_8files.py $< $(RAM_SIZE) 3\
		> hex/$(addsuffix _instr_3.hex,$@)
	python3 $(RUNTIME_PATH)/python_script/makehex_8files.py $< $(RAM_SIZE) 4\
		> hex/$(addsuffix _data_0.hex,$@)
	python3 $(RUNTIME_PATH)/python_script/makehex_8files.py $< $(RAM_SIZE) 5\
		> hex/$(addsuffix _data_1.hex,$@)
	python3 $(RUNTIME_PATH)/python_script/makehex_8files.py $< $(RAM_SIZE) 6\
		> hex/$(addsuffix _data_2.hex,$@)
	python3 $(RUNTIME_PATH)/python_script/makehex_8files.py $< $(RAM_SIZE) 7\
		> hex/$(addsuffix _data_3.hex,$@)
	python3 $(RUNTIME_PATH)/python_script/makebin.py $< $(RAM_SIZE) \
		hex/$(addsuffix .bin,$@)
#	cp hex/firmware.hex /home/lijunnan/share/firmware.hex
#	cp hex/firmware.bin /home/lijunnan/share/firmware.bin
	cp hex/firmware.hex ../iCore_Controller/firmware.hex
##############################################################################

##############################################################################
## gen firmware.bin
obj/firmware.bin: obj/firmware.elf obj/firmware.elf.S
	$(TOOLCHAIN_PREFIX)objcopy -O binary $< $@;
	wc -c obj/firmware.bin
	size obj/firmware.elf
	chmod -x $@
##############################################################################


##############################################################################
## gen firmware.elf.S
obj/firmware.elf.S: obj/firmware.elf
	$(TOOLCHAIN_PREFIX)objdump $< -s -d > $@
## gen firmware.elf
obj/firmware.elf: ${FIRMWARE_OBJS} $(RUNTIME_PATH)/link/link.ld
	$(TOOLCHAIN_PREFIX)gcc $(CCFLAGS) -Os -ffreestanding -nostdlib -o $@ \
		-Wl,-Bstatic,-T,$(RUNTIME_PATH)/link/link.ld,-Map,hex/firmware.map,--strip-debug \
		${FIRMWARE_OBJS} -lgcc
	chmod -x $@
##############################################################################

##############################################################################
## compile .S
obj/%.o: $(RUNTIME_PATH)/src/asm/%.S
	$(TOOLCHAIN_PREFIX)gcc -c $(CCFLAGS) -o $@ $< $(TIMER_IRQ_IN_ASM) \
		$(DMA_IRQ_IN_ASM)
## compile .c
obj/%.o: %.c
	$(TOOLCHAIN_PREFIX)gcc -c $(CCFLAGS) $(INCLUDES) -Os --std=c99 \
		$(GCC_WARNS) -o $@ $<
## mkdir
hex_obj_dir:
	mkdir hex obj
## configure link.ld
conf_link_ld:
	python3 $(RUNTIME_PATH)/python_script/pemembaseconfig.py $(RUNTIME_PATH)/link/link.ld \
				-instrbase $(INSTR_BASE_ADDR) -instrlen $(INSTR_LEN) \
				-database $(DATA_BASE_ADDR) -datalen $(DATA_LEN)
##############################################################################

##############################################################################
.PHONY: clean help
clean:
	rm -r  hex obj
help:
	@echo "----------------------------------------------------------"
	@echo "+  You can use 'make RAM_SIZE_KB=128' to specify RAM size."
	@echo "+  You can use 'make TIMER_IRQ_PROC_IN_ASM=1' to choose"
	@echo "+   processing time irq in asm."
	@echo "+  You can use 'make DMA_IRQ_PROC_IN_ASM=1' to choose"
	@echo "+   processing dma irq in asm."
	@echo "----------------------------------------------------------"
##############################################################################
