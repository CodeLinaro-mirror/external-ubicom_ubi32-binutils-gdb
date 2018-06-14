/* tc-ubi32.h -- Header file for tc-ubi32.c.
   Copyright (C) 2017-2018 Eager Consulting.

   This file is part of GAS, the GNU Assembler.

   GAS is free software; you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the Free Software Foundation; either version 2, or (at your option)
   any later version.

   GAS is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.

   You should have received a copy of the GNU General Public License
   along with GAS; see the file COPYING.  If not, write to
   the Free Software Foundation, 59 Temple Place - Suite 330,
   Boston, MA 02111-1307, USA. */

#ifndef TC_UBICOM32
#define TC_UBICOM32

/* The target BFD architecture.  */
#define TARGET_ARCH bfd_arch_ubi32

/* Default to little endian.  */
#define TARGET_BYTES_BIG_ENDIAN	0

/* Define BFD_UBI32_OLD_NAME to use old "ubicom32" names.  */
#define BFD_UBI32_OLD_NAME
#ifdef BFD_UBI32_OLD_NAME
#define TARGET_FORMAT (target_big_endian ? "elf32-bigubicom32" : "elf32-littleubicom32")
#else
#define TARGET_FORMAT (target_big_endian ? "elf32-bigubi32" : "elf32-littleubi32")
#endif

/* Permit temporary numeric labels. */
#define LOCAL_LABELS_FB 1

/* .-foo gets turned into PC relative relocs. */
#define DIFF_EXPR_OK

/* UBICOM32 uses '(' and ')' as punctuation in addressing mode syntax. */
#define RELAX_PAREN_GROUPING

/* We don't need to handle .word strangely. */
#define WORKING_DOT_WORD

/* special characters for hex and bin literals */
#define LITERAL_PREFIXDOLLAR_HEX
#define LITERAL_PREFIXPERCENT_BIN
#define DOUBLESLASH_LINE_COMMENTS

#define obj_fix_adjustable(fixP) ubi32_fix_adjustable (fixP)
extern bfd_boolean ubi32_fix_adjustable (struct fix *);

/* Permit temporary numeric labels.  */
#define LOCAL_LABELS_FB 1

extern int ubi32_force_relocation (struct fix *);
#define TC_FORCE_RELOCATION(fixp) ubi32_force_relocation(fixp)

#define md_convert_frag(abfd,sec,fragP)

/* Values passed to md_apply_fix don't include the symbol value.  */
#define MD_APPLY_SYM_VALUE(FIX) 0

#define MD_PCREL_FROM_SECTION(FIXP, SEC) 0

#define md_number_to_chars(ptr,use,nbytes) \
  (target_big_endian ? number_to_chars_bigendian (ptr, use, nbytes) \
   		     : number_to_chars_littleendian (ptr, use, nbytes))

#define md_undefined_symbol(name)	(0)
#define md_operand(x)

extern int tc_ubi32_regname_to_dw2regnum (char *regname);
extern void tc_ubi32_frame_initial_instructions (void);

/* The link register is a5.  */
#define DWARF2_DEFAULT_RETURN_COLUMN	0x15

/* Registers are generally saved at negative offsets to the CFA.  */
#define DWARF2_CIE_DATA_ALIGNMENT	(-4)

/* We want .cfi_* pseudo-ops for generating unwind info.  */
#define TARGET_USE_CFIPOP   (ubi32_mach == bfd_mach_ubi32v6 || ubi32_mach == bfd_mach_ubi32v61)

/* CFI hooks.  */
#define tc_regname_to_dw2regnum            tc_ubi32_regname_to_dw2regnum
#define tc_cfi_frame_initial_instructions  tc_ubi32_frame_initial_instructions

/* GDB Stack Register number.  */
#define SP_REGNUM 	23

extern const char *ubi32_gdb_register_names[];

#endif
