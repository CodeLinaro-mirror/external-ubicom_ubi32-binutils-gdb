/* tc-ubi32.h -- Header file for tc-ubi32.c.
   Copyright (C) 2017 Free Software Foundation, Inc.

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

#define TC_UBICOM32

#if 0
#ifndef BFD_ASSEMBLER
/* leading space so will compile with cc */
 #error UBICOM32 support requires BFD_ASSEMBLER
#endif
#endif

#define LISTING_HEADER "IP3xxx GAS "

/* The target BFD architecture.  */
#define TARGET_ARCH bfd_arch_ubi32

/* Default to big endian.  */
#ifndef TARGET_BYTES_BIG_ENDIAN
#define TARGET_BYTES_BIG_ENDIAN	1
#endif

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

#define MD_APPLY_FIX3
#define md_apply_fix gas_cgen_md_apply_fix

/* special characters for hex and bin literals */
#define LITERAL_PREFIXDOLLAR_HEX
#define LITERAL_PREFIXPERCENT_BIN
#define DOUBLESLASH_LINE_COMMENTS

/* call md_pcrel_from_section, not md_pcrel_from */
long md_pcrel_from_section (struct fix *, segT);
#define MD_PCREL_FROM_SECTION(FIXP, SEC) md_pcrel_from_section (FIXP, SEC)

#define obj_fix_adjustable(fixP) ubi32_fix_adjustable (fixP)
extern bfd_boolean ubi32_fix_adjustable (struct fix *);

/* Permit temporary numeric labels.  */
#define LOCAL_LABELS_FB 1

#define TC_HANDLES_FX_DONE

#define tc_gen_reloc gas_cgen_tc_gen_reloc

#define TC_FORCE_RELOCATION(fixp) ubi32_force_relocation(fixp)
extern int ubi32_force_relocation (struct fix *);

/* Values passed to md_apply_fix don't include the symbol value.  */
#define MD_APPLY_SYM_VALUE(FIX) 0
