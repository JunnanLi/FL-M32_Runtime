/* Script for -z combreloc: combine and sort reloc sections */
/* Copyright (C) 2014-2018 Free Software Foundation, Inc.
   Copyright (C) 2019 ETH Zürich and University of Bologna
   Copying and distribution of this script, with or without modification,
   are permitted in any medium without royalty provided the copyright
   notice and this notice are preserved.  */

/* This linker script is derived from the default linker script of the RISC-V
   gcc compiler. We have made a few changes to make it suitable for linking bare
   metal programs. These are mostly removing dynamic linking related sections and
   putting sections into our memory regions. */

OUTPUT_FORMAT("elf32-littleriscv", "elf32-littleriscv",
	      "elf32-littleriscv")
OUTPUT_ARCH(riscv)
ENTRY(_start)

MEMORY
{
	/* Our testbench is a bit weird in that we initialize the RAM (thus
	   allowing initialized sections to be placed there). Infact we dump all
	   sections to ram. */

	ram (rwxai) : ORIGIN = 0x00000000, LENGTH = 0x10000
  /*ram (rwxai) : ORIGIN = 0x00000000, LENGTH = 0x4000*/
  /*ram_data (rwxai) : ORIGIN = 0x00020000, LENGTH = 0x18000*/
  /*ram_data (rwxai) : ORIGIN = 0x00010000, LENGTH = 0x10000*/
  ram_data (rwxai) : ORIGIN = 0x00020000, LENGTH = 0x20000
}

SECTIONS
{
  /* we want a fixed entry point */
  PROVIDE(__boot_address = 0x180);

  /* stack and heap related settings */
  __stack_size = DEFINED(__stack_size) ? __stack_size : 0x2000;
  PROVIDE(__stack_size = __stack_size);
  /*__heap_size = DEFINED(__heap_size) ? __heap_size : 0x1000;*/


  /* Read-only sections, merged into text segment: */
  /*PROVIDE (__executable_start = SEGMENT_START("text-segment", 0x10000)); . = SEGMENT_START("text-segment", 0x10000) + SIZEOF_HEADERS;*/


  /* interrupt vectors */
  .vectors (ORIGIN(ram)):
  {
    PROVIDE(__vector_start = .);
    KEEP(*(.vectors));
  } >ram

  /* crt0 init code */
  .init (__boot_address):
  {
    KEEP (*(SORT_NONE(.init)))
    KEEP (*(.text.start))
  } >ram


  /* the bulk of the program: main, libc, functions etc. */
  .text           :
  {
    *(.text.unlikely .text.*_unlikely .text.unlikely.*)
    *(.text.exit .text.exit.*)
    *(.text.startup .text.startup.*)
    *(.text.hot .text.hot.*)
    *(.text .stub .text.* .gnu.linkonce.t.*)
    /* .gnu.warning sections are handled specially by elf32.em.  */
    *(.gnu.warning)
  } >ram


  /* not used by RISC-V*/
  /*.fini           :
  {
    KEEP (*(SORT_NONE(.fini)))
  } >ram*/

  PROVIDE (__etext = .);
  PROVIDE (_etext = .);
  PROVIDE (etext = .);


  /* read-only sections */
  .rodata         :
  {
    *(.rodata .rodata.* .gnu.linkonce.r.*)
  } >ram_data
  .rodata1        :
  {
    *(.rodata1)
  } >ram_data


  /* gcc language agnostic exception related sections (try-catch-finally) */
/*  .eh_frame_hdr :
  {
    *(.eh_frame_hdr) *(.eh_frame_entry .eh_frame_entry.*)
  } >ram
  .eh_frame       : ONLY_IF_RO
  {
    KEEP (*(.eh_frame)) *(.eh_frame.*)
  } >ram
  .gcc_except_table   : ONLY_IF_RO
  {
    *(.gcc_except_table .gcc_except_table.*)
  } >ram
  .gnu_extab   : ONLY_IF_RO
  {
    *(.gnu_extab*)
  } >ram*/
  
  /* Adjust the address for the data segment.  We want to adjust up to
     the same address within the page on the next page up.  */
  . = DATA_SEGMENT_ALIGN (CONSTANT (MAXPAGESIZE), CONSTANT (COMMONPAGESIZE));


  /* Exception handling  */
/*  .eh_frame       : ONLY_IF_RW
  {
    KEEP (*(.eh_frame)) *(.eh_frame.*)
  } >ram
  .gnu_extab      : ONLY_IF_RW
  {
    *(.gnu_extab)
  } >ram
  .gcc_except_table   : ONLY_IF_RW
  {
    *(.gcc_except_table .gcc_except_table.*)
  } >ram
  .exception_ranges   : ONLY_IF_RW
  {
    *(.exception_ranges .exception_ranges*)
  } >ram*/


  /* Thread Local Storage sections  */
/*  .tdata	  :
  {
    PROVIDE_HIDDEN (__tdata_start = .);
    *(.tdata .tdata.* .gnu.linkonce.td.*)
  } >ram_data
  .tbss		  :
  {
    *(.tbss .tbss.* .gnu.linkonce.tb.*) *(.tcommon)
  } >ram_data*/


  /* initialization and termination routines */
  /*.preinit_array     :
  {
    PROVIDE_HIDDEN (__preinit_array_start = .);
    KEEP (*(.preinit_array))
    PROVIDE_HIDDEN (__preinit_array_end = .);
  } >ram
  .init_array     :
  {
    PROVIDE_HIDDEN (__init_array_start = .);
    KEEP (*(SORT_BY_INIT_PRIORITY(.init_array.*) SORT_BY_INIT_PRIORITY(.ctors.*)))
    KEEP (*(.init_array EXCLUDE_FILE (*crtbegin.o *crtbegin?.o *crtend.o *crtend?.o ) .ctors))
    PROVIDE_HIDDEN (__init_array_end = .);
  } >ram
  .fini_array     :
  {
    PROVIDE_HIDDEN (__fini_array_start = .);
    KEEP (*(SORT_BY_INIT_PRIORITY(.fini_array.*) SORT_BY_INIT_PRIORITY(.dtors.*)))
    KEEP (*(.fini_array EXCLUDE_FILE (*crtbegin.o *crtbegin?.o *crtend.o *crtend?.o ) .dtors))
    PROVIDE_HIDDEN (__fini_array_end = .);
  } >ram
  .ctors          :
  {*/
    /* gcc uses crtbegin.o to find the start of
       the constructors, so we make sure it is
       first.  Because this is a wildcard, it
       doesn't matter if the user does not
       actually link against crtbegin.o; the
       linker won't look for a file to match a
       wildcard.  The wildcard also means that it
       doesn't matter which directory crtbegin.o
       is in.  */
    /*KEEP (*crtbegin.o(.ctors))
    KEEP (*crtbegin?.o(.ctors))*/
    /* We don't want to include the .ctor section from
       the crtend.o file until after the sorted ctors.
       The .ctor section from the crtend file contains the
       end of ctors marker and it must be last */
    /*KEEP (*(EXCLUDE_FILE (*crtend.o *crtend?.o ) .ctors))
    KEEP (*(SORT(.ctors.*)))
    KEEP (*(.ctors))
  } >ram
  .dtors          :
  {
    KEEP (*crtbegin.o(.dtors))
    KEEP (*crtbegin?.o(.dtors))
    KEEP (*(EXCLUDE_FILE (*crtend.o *crtend?.o ) .dtors))
    KEEP (*(SORT(.dtors.*)))
    KEEP (*(.dtors))
  } >ram*/

  /* .jcr            : { KEEP (*(.jcr)) } */
  /* .data.rel.ro : { *(.data.rel.ro.local* .gnu.linkonce.d.rel.ro.local.*) *(.data.rel.ro .data.rel.ro.* .gnu.linkonce.d.rel.ro.*) } */
  /* .dynamic        : { *(.dynamic) } */
  /*. = DATA_SEGMENT_RELRO_END (0, .);*/


  /* data sections for initalized data */
  .data           :
  {
    __DATA_BEGIN__ = .;
    *(.data .data.* .gnu.linkonce.d.*)
    SORT(CONSTRUCTORS)
  } >ram_data
  /*.data1          :
  {
    *(.data1)
  } > ram_data*/

  /* no dynamic linking, no object tables required */
  /* .got            : { *(.got.plt) *(.igot.plt) *(.got) *(.igot) } */

  /* We want the small data sections together, so single-instruction offsets
     can access them all, and initialized data all before uninitialized, so
     we can shorten the on-disk segment size.  */
  .sdata          :
  {
    __SDATA_BEGIN__ = .;
    *(.srodata.cst16) *(.srodata.cst8) *(.srodata.cst4) *(.srodata.cst2) *(.srodata .srodata.*)
    *(.sdata .sdata.* .gnu.linkonce.s.*)
  } >ram_data
  _edata = .; PROVIDE (edata = .);
  . = .;


  /* zero initialized sections */
  __bss_start = .;
  .sbss           :
  {
    *(.dynsbss)
    *(.sbss .sbss.* .gnu.linkonce.sb.*)
    *(.scommon)
  } >ram_data
  .bss            :
  {
   *(.dynbss)
   *(.bss .bss.* .gnu.linkonce.b.*)
   *(COMMON)
   /* Align here to ensure that the .bss section occupies space up to
      _end.  Align after .bss to ensure correct alignment even if the
      .bss section disappears because there are no input sections.
      FIXME: Why do we need it? When there is no .bss section, we don't
      pad the .data section.  */
   . = ALIGN(. != 0 ? 32 / 8 : 1);
  } >ram_data
  . = ALIGN(32 / 8);
  . = SEGMENT_START("ldata-segment", .);
  . = ALIGN(32 / 8);
  __BSS_END__ = .;
  __bss_end = .;

  /* The compiler uses this to access data in the .sdata, .data, .sbss and .bss
     sections with fewer instructions (relaxation). This reduces code size. */
  __global_pointer$ = MIN(__SDATA_BEGIN__ + 0x800,
		    MAX(__DATA_BEGIN__ + 0x800, __BSS_END__ - 0x800));
  _end = .; PROVIDE (end = .);
  . = DATA_SEGMENT_END (.);


  /* heap: we should consider putting this to the bottom of the address space */
  /*.heap          :
  {
   PROVIDE(__heap_start = .);
   . = __heap_size;
   PROVIDE(__heap_end = .);
  } >ram_data*/


  /* stack: we should consider putting this further to the top of the address
    space */
  .stack         : ALIGN(16) /* this is a requirement of the ABI(?) */
  {
   PROVIDE(__stack_start = .);
   . = __stack_size;
   PROVIDE(_sp = .);
   PROVIDE(__stack_end = .);
  } >ram_data


  /** Stabs debugging sections.  */
  /*.stab          0 : { *(.stab) }
  .stabstr       0 : { *(.stabstr) }
  .stab.excl     0 : { *(.stab.excl) }
  .stab.exclstr  0 : { *(.stab.exclstr) }
  .stab.index    0 : { *(.stab.index) }
  .stab.indexstr 0 : { *(.stab.indexstr) }
  .comment       0 : { *(.comment) }*/
  /** DWARF debug sections.
     Symbols in the DWARF debugging sections are relative to the beginning
     of the section so we begin them at 0.  */
  /** DWARF 1 */
  /*.debug          0 : { *(.debug) }
  .line           0 : { *(.line) }*/
  /** GNU DWARF 1 extensions */
  /*.debug_srcinfo  0 : { *(.debug_srcinfo) }
  .debug_sfnames  0 : { *(.debug_sfnames) }*/
  /** DWARF 1.1 and DWARF 2 */
  /*.debug_aranges  0 : { *(.debug_aranges) }
  .debug_pubnames 0 : { *(.debug_pubnames) }*/
  /** DWARF 2 */
  /*.debug_info     0 : { *(.debug_info .gnu.linkonce.wi.*) }
  .debug_abbrev   0 : { *(.debug_abbrev) }
  .debug_line     0 : { *(.debug_line .debug_line.* .debug_line_end ) }
  .debug_frame    0 : { *(.debug_frame) }
  .debug_str      0 : { *(.debug_str) }
  .debug_loc      0 : { *(.debug_loc) }
  .debug_macinfo  0 : { *(.debug_macinfo) }*/
  /** SGI/MIPS DWARF 2 extensions */
  /*.debug_weaknames 0 : { *(.debug_weaknames) }
  .debug_funcnames 0 : { *(.debug_funcnames) }
  .debug_typenames 0 : { *(.debug_typenames) }
  .debug_varnames  0 : { *(.debug_varnames) }*/
  /** DWARF 3 */
  /*.debug_pubtypes 0 : { *(.debug_pubtypes) }
  .debug_ranges   0 : { *(.debug_ranges) }*/
  /** DWARF Extension.  */
  /*.debug_macro    0 : { *(.debug_macro) }
  .debug_addr     0 : { *(.debug_addr) }
  .gnu.attributes 0 : { KEEP (*(.gnu.attributes)) }
  /DISCARD/ : { *(.note.GNU-stack) *(.gnu_debuglink) *(.gnu.lto_*) }*/
}
