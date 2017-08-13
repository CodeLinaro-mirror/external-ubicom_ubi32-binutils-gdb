/* Print Qualcomm Ubi32 instructions. 

   Copyright (C) 2017 Free Software Foundation, Inc.
   Contributed by Michael J. Eager (eager@eagercon.com)

   This file is part of the GNU opcodes library.

   This library is free software; you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the Free Software Foundation; either version 3, or (at your option)
   any later version.

   It is distributed in the hope that it will be useful, but WITHOUT
   ANY WARRANTY; without even the implied warranty of MERCHANTABILITY
   or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public
   License for more details.

   You should have received a copy of the GNU General Public License
   along with this program; if not, write to the Free Software
   Foundation, Inc., 51 Franklin Street - Fifth Floor, Boston,
   MA 02110-1301, USA.  */


/* 
   Print all Ubi32 instructions without verifying that they are valid 
   for the current processor variant. 

   Format 11 & 12 (Floating Point) instructions are not printed. 
*/


// #include <stdio.h>
#include "config.h"
#include "bfd.h"
#include "dis-asm.h"
#include "opintl.h"

/* Opcode description.  */
static struct
{
   int format;		/* Instruction format.  */
   int offset;		/* Op extension offset. */
   int size;		/* Op extension size.   */
} translate_opcode_to_format[32] =
{
 /* fm of sz  fm  of sz   fm  of sz   fm  of sz   */
  {1, 11, 5}, {1, 11, 5}, {4, 21, 5},{11, 21, 5}, /* op 00 - 03 */
  {2,  0, 0}, {2,  0, 0},{10, 21, 5}, {2,  0, 0}, /* op 04 - 07 */
  {3, 15, 1}, {3, 15, 1}, {3, 15, 1}, {3, 15, 1}, /* op 08 - 0b */
  {3, 15, 1}, {3, 15, 1}, {3, 15, 1}, {3, 15, 1}, /* op 0c - 0f */
  {3, 15, 1}, {3, 15, 1}, {3, 15, 1}, {3, 15, 1}, /* op 10 - 13 */
  {3, 15, 1}, {3, 15, 1}, {3, 15, 1}, {3, 15, 1}, /* op 14 - 17 */
  {5,  0, 0}, {6,  0, 0}, {7,  0, 0}, {8,  0, 0}, /* op 18 - 1b */
  {8,  0, 0}, {8,  0, 0}, {9, 11, 5}, {9, 11, 5}, /* op 1c - 1f */
};

static char *op_fmt1[2][32] =
  {{ 0,           "suspend",   "flush",     "sync",
     "ret",       "prefetch",  "tbtst",     "bkpt",
     "sysret",    "syscall",   "not.4",     "not.2",
     "move.4",    "move.2",    "movea",     "move.1",
     "tbrest",    "flushni",   "setcsr",    "tbset",
     "tbclr",     "ext.2",     "inval",     "ext.1",
     "swapb.2",   "swapb.4",   "fxcnv",     "fxcnv.t",
     "lea.4",     "lea.2",     "pdec",      "lea.1" },
   { "moveio.4",  "moveio.2",  "moveio.1",  "moveiop.4",
     "moveiop.2", "moveiop.1" } };

static char *op_fmt2[32] =
  { 0, 0, 0, 0, "bset", "bclr", 0, 0 };

static char *op_fmt3[2][32] =
  {{ 0,           0,           0,           0,
     0,           0,           0,           0,
     "and.2",     "and.4",     "or.2",      "or.4",
     "xor.2",     "xor.4",     "add.2",     "add.4",
     "addc",      "sub.2",     "sub.4",     "subc",
     "pxblend",   "pxvi",      0,           0,
     0,           0,           0,           0,
     0,           0,           0,           0	},
   { 0,           0,           0,           0,
     0,           0,           0,           0,
     "and.1",     "and.4",     "or.1",      "or.4",
     "xor.1",     "xor.4",     "add.1",     "add.4",
     "addc",      "sub.1",     "sub.4",     "subc",
     "pxblend.t", "pxvi",      0,           0,
     0,           0,           0,           0,
     0,           0,           0,           0	} };

static char *op_fmt4[32] =
  {  "fxhi",      "muls",      "fxhi.s",    "mulu",
     0,           "mulf",      "btst",      0,
     "crcgen",    "mac",       "lsl.1",     "lsr.1",
     "asr.1",     0,           0,           0,
     "lsl.4",     "lsl.2",     "lsr.4",     "lsr.2",
     "asr.4",     "asr.2",     "bfextu",    0,
     "bfrvrs",    0,           "shftd",     0,
     "merge",     0,           "shmrg.2",   "shmrg.1" };

#if 0
static char *op_fmt5_8[32] =
  {  0,           0,           0,           0,
     0,           0,           0,	    0,
     0,           0,           0,           0,
     0,           0,           0,	    0,
     0,           0,           0,           0,
     0,           0,           0,	    0,
     "cmpi",      "movei",     "jmp",       "call",
     "movei",     "moveaih",   0,	    0 };
#endif

static char *op_fmt10[32] =
  {  "muls",      "macs",      "mulu",      "macu",
     "mulf",      "macf",      "macs.4",    "macus",
     "muls.4",    "msuf",      "mulu.4",    "macu.4",
     0,           0,           0,           0,
     "madd.4",    "madd.2",    "msub.4",    "msub.2",
     0,		  0,           0,           0,
     0,           0,           0,           0,
     0,           0,           0,           0 };

#if 0
static char *op_fmt11_12[32] =
  {  "fadds",     "fsubs",     "fmuls",     "fdivs",
     "fi2d",      "fs2d",      "fs2l",      "fsqrts",
     "fnegs",     "fabss",     "fi2s",      "fs2i",
     "fcmps",     0,           0,           0,
    "fadd",      "fsubd",      "fmuld",     "fdivd",
    "fl2s",      "fd2s",       "fd2i",      "fsqrtd"
    "fnegd",     "fabsd",      "fl2d",      "fd2l",
    "fcmpd",     0,            0,           0, };
#endif

static char *areg_names[8] =
  { "a0", "a1", "a2", "a3", "a4", "a5", "a6", "sp" };

static char *reg_names[256] =					/*  Reg No */
  { "d0", "d1", "d2", "d3", "d4", "d5", "d6", "d7",		/* 00 - 07 */
    "d8", "d9", "d10", "d11", "d12", "d13", "d14", "d15",	/* 08 - 0f */
    "res", "res", "res", "res", "res", "res", "res", "res",	/* 10 - 17 */
    "res", "res", "res", "res", "res", "res", "res", "res",	/* 18 - 1f */
    "a0", "a1", "a2", "a3", "a4", "a5", "a6", "sp",		/* 20 - 27 */
    "mac_hi", "mac_lo", "mac_rc15", "source3",			/* 28 - 2b */
    "inst_cnt", "csr", "rosr", "iread_data",			/* 2c - 2f */
    "int_mask0", "int_mask1", "int_mask2", "int_mask3",		/* 30 - 33 */
    "pc", "trap_cause", "acc1_hi", "acc1_lo",			/* 34 - 37 */
    "previous_pc", "ucsr", "spsr0", "res",			/* 38 - 3b */
    "res", "res", "res", "res",					/* 3c - 3f */
    "chip_id", "int_stat0", "int_stat1", "int_stat2",		/* 40 - 43 */
    "int_stat3", "int_set0", "int_set1", "int_set2",		/* 44 - 47 */
    "int_set3", "int_clr0", "int_clr1", "int_clr2",		/* 48 - 4b */
    "int_clr3", "global_ctrl", "mt_active", "mt_active_set",	/* 4c - 4f */
    "mt_active_clr", "mt_dbg_active",				/* 50 - 51 */
    "mt_dbg_active_set", "mt_en",				/* 52 - 53 */
    "mt_hpri", "mt_hrt", "mt_break", "mt_break_clr",		/* 54 - 57 */
    "mt_single_step", "mt_min_delay_en",			/* 58 - 59 */
    "mt_break_set", "perr_addr",				/* 5a - 5b */
    "dcapt", "dcapt_pc", "dcapt_tnum", "mt_dgb_active_clr",	/* 5c - 5f */
    "scratchpad0", "scratchpad1",				/* 60 - 61 */
    "scratchpad2", "scratchpad3",				/* 62 - 63 */
    "scratchpad4", "scratchpad5",				/* 64 - 65 */
    "scratchpad6", "scratchpad7",				/* 66 - 67 */
    "cfg", "mt_i_blocked",					/* 68 - 69 */
    "mt_d_blocked", "mt_i_blocked_set",				/* 6a - 6b */
    "mt_d_blocked_set", "mt_blocked_clr",			/* 6c - 6d */
    "mt_trap_en", "mt_trap",					/* 6e - 6f */
    "mt_trap_set", "mt_trap_clr", "sep", "mt_btb_en",		/* 70 - 73 */
    "btb_ctrl", "tnum", "res", "res",				/* 74 - 77 */
    "res", "res", "res", "res",					/* 78 - 7b */
    "res", "res", "res", "res",					/* 7c - 7f */
    "i_range0_hi", "i_range1_hi", "i_range2_hi", "i_range3_hi",	/* 80 - 83 */
    "i_range4_hi", "i_range5_hi", "i_range6_hi", "i_range7_hi",	/* 84 - 87 */
    "i_range0_lo", "i_range1_lo", "i_range2_lo", "i_range3_lo",	/* 88 - 8b */
    "i_range4_lo", "i_range5_lo", "i_range6_lo", "i_range7_lo",	/* 8c - 8f */
    "i_range0_en", "i_range1_en", "i_range2_en", "i_range3_en",	/* 90 - 93 */
    "i_range4_en", "i_range5_en", "i_range6_en", "i_range7_en",	/* 94 - 97 */
    "d_range0_hi", "d_range1_hi", "d_range2_hi", "d_range3_hi",	/* 98 - 9b */
    "d_range4_hi", "d_range5_hi", "d_range6_hi", "d_range7_hi",	/* 9c - 9f */
    "d_range0_lo", "d_range1_lo", "d_range2_lo", "d_range3_lo",	/* a0 - a3 */
    "d_range4_lo", "d_range5_lo", "d_range6_lo", "d_range7_lo",	/* a4 - a7 */
    "d_range0_en", "d_range1_en", "d_range2_en", "d_range3_en",	/* a8 - ab */
    "d_range4_en", "d_range5_en", "d_range6_en", "d_range7_en",	/* ac - af */
    "i_range0_user_en", "i_range1_user_en",			/* b0 - b1 */
    "i_range2_user_en", "i_range3_user_en",			/* b2 - b3 */
    "i_range4_user_en", "i_range5_user_en",			/* b4 - b5 */
    "i_range6_user_en", "i_range7_user_en",			/* b6 - b7 */
    "d_range0_user_en", "d_range1_user_en",			/* b8 - b9 */
    "d_range2_user_en", "d_range3_user_en",			/* ba - bb */
    "d_range4_user_en", "d_range5_user_en",			/* bc - bd */
    "d_range6_user_en", "d_range7_user_en",			/* be - bf */
  };


static void
operand_internal (unsigned int opnd, int opnd_sz, char *buf,
		  int pdec_flag)
{
  int code = (opnd & 0x700) >> 8;

  switch (code)
    {
      case 0:
	{
	  int simm = opnd & 0x0ff;
	  if (simm & 0x80)
	    simm |= -0x100;

	  sprintf (buf, "#%d", simm);
	  break;
	}

      case 1:
	{
	  unsigned int reg  = opnd & 0x0ff;

	  if (reg_names[reg] == 0)
	    sprintf (buf, "#0x%x", reg);
	  else
	    sprintf (buf, "%s", reg_names[reg]);
	  break;
	}

      case 2:
	{
	  unsigned int areg = (opnd & 0x0e0) >> 5;
	  unsigned int m    = (opnd >> 4) & 0x1;
	  int imm  = opnd & 0x00f;

	  if (imm & 0x8)
	    imm |= -0x10;
	  imm *= opnd_sz;

	  if (m)
	    sprintf (buf, "%d(%s)++", imm, areg_names[areg]);
	  else
	    sprintf (buf, "(%s)%d++", areg_names[areg], imm);
	  break;
	}

      case 3:
	{
	  unsigned int areg = (opnd & 0x0e0) >> 5;
	  unsigned int dreg = opnd & 0x00f;

	  sprintf (buf, "(%s,d%d)", areg_names[areg], dreg);
	  break;
	}

      default:
	{
	  unsigned int areg = (opnd & 0x0e0) >> 5;
	  unsigned int limm = ((opnd & 0x300) >> 3) + (opnd & 0x1f);

	  limm *= opnd_sz;

	  /* Convert immediate to inverse for PDEC. */
	  if (pdec_flag)
	    {
	      if (limm != 0)
		{
		  int value = limm;
		  value = ~value;
		  value++;
		  limm = value & 0x1fc;
		}
	      else
		limm = 512;
	    }

	  if (limm == 0)
	    sprintf (buf, "(%s)", areg_names[areg]);
	  else
	    sprintf (buf, "%d(%s)", limm, areg_names[areg]);
	  break;
	}
    }
}

static void
operand (unsigned int opnd, int opnd_sz, char *buf)
{
  operand_internal (opnd, opnd_sz, buf, 0);
}

static void
pdec_operand (unsigned int opnd, int opnd_sz, char *buf)
{
  operand_internal (opnd, opnd_sz, buf, 1);
}


static void
print_insn_fmt1 (bfd_vma pc ATTRIBUTE_UNUSED, unsigned int insn,
		 int major, int ext, disassemble_info *info)
{
  enum sub_fmt_t { I, Z, S, D, B, P };  /* Inv, Zero, Src, Dest, Both S&D, PDEC */
  static int sub_fmt[32] =
    { I, Z, D, D, S, D, S, S,
      B, D, B, B, B, B, B, B,
      B, D, B, B, B, B, D, B,
      B, B, B, B, B, B, P, B };
  static int sopnd_sz_tab[2][32] =
    {{ 0, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 2, 4, 2, 4, 1,
       4, 4, 4, 4, 4, 2, 4, 1, 2, 4, 4, 4, 4, 2, 4, 1 },
     { 4, 2, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
       0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 }};
  static int dopnd_sz_tab[2][32] =
    {{ 0, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 2, 4, 2, 4, 1,
       4, 4, 4, 4, 4, 2, 4, 1, 2, 4, 4, 4, 4, 4, 4, 4 },
     { 4, 2, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
       0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 }};
  unsigned int source = insn & 0x000007ff;
  unsigned int dest = (insn >> 16) & 0x7ff;
  char sbuf[30], dbuf[30];
  int sopnd_sz = sopnd_sz_tab[major][ext];
  int dopnd_sz = dopnd_sz_tab[major][ext];

  switch (sub_fmt[ext])
    {
       case Z:		/* Zero operands */
	 (*info->fprintf_func) (info->stream, "%s",
				op_fmt1[major][ext]);
	 break;

       case S:		/* Source operand */
	 operand (source, sopnd_sz, sbuf);
	(*info->fprintf_func) (info->stream, "%s %s",
				op_fmt1[major][ext], sbuf);
	 break;

       case D:		/* Destination operand */
	 operand (dest, dopnd_sz, dbuf);
	 (*info->fprintf_func) (info->stream, "%s %s",
				op_fmt1[major][ext], dbuf);
	 break;

       case B:		/* Both source and destination operands */
	 operand (source, sopnd_sz, sbuf);
	 operand (dest, dopnd_sz, dbuf);
	 (*info->fprintf_func) (info->stream, "%s %s,%s",
				op_fmt1[major][ext], dbuf, sbuf);
	 break;

       case P:		/* PDEC operand */
	 pdec_operand (source, sopnd_sz, sbuf);
	 operand (dest, dopnd_sz, dbuf);
	 (*info->fprintf_func) (info->stream, "%s %s,%s",
				op_fmt1[major][ext], dbuf, sbuf);
	 break;

       default:
	 (*info->fprintf_func) (info->stream, "*unknown*");
	 break;
    }
}

static void
print_insn_fmt2 (bfd_vma pc ATTRIBUTE_UNUSED, unsigned int insn,
		 int major, int ext ATTRIBUTE_UNUSED,
		 disassemble_info *info)
{
  unsigned int source = insn & 0x000007ff;
  unsigned int dest = (insn >> 16) & 0x7ff;
  unsigned int bitn = (insn >> 11) & 0x1f;
  char sbuf[30], dbuf[30];
  char *op;

  operand (source, 4, sbuf);
  operand (dest, 4, dbuf);
  op = op_fmt2[major];
  if (op == NULL)
    op = "*unknown*";

  (*info->fprintf_func) (info->stream, "%s %s,%s,#0x%x", op, dbuf,
			 sbuf, bitn);
}

static void
print_insn_fmt3 (bfd_vma pc ATTRIBUTE_UNUSED, unsigned int insn,
		 int major, int ext, disassemble_info *info)
{
  static int opnd_sz_tab[2][32] =
    {{ 0, 0, 0, 0, 0, 0, 0, 0,
       2, 4, 2, 4, 2, 4, 2, 4,
       4, 2, 4, 4, 4, 4, 4, 0,
       0, 0, 0, 0, 0, 0, 0, 0 },
     { 0, 0, 0, 0, 0, 0, 0, 0,
       1, 4, 1, 4, 1, 4, 1, 4,
       4, 1, 4, 4, 4, 4, 4, 0,
       0, 0, 0, 0, 0, 0, 0, 0 }};
  unsigned int source = insn & 0x000007ff;
  unsigned int dest = (insn >> 16) & 0x7ff;
  unsigned int src2 = (insn >> 11) & 0x0f;
  char sbuf[30], dbuf[30];
  char *op;
  int opnd_sz = opnd_sz_tab[ext][major];

  operand (source, opnd_sz, sbuf);
  operand (dest, opnd_sz, dbuf);
  op = op_fmt3[ext][major];
  if (op == NULL)
    op = "*unknown*";

  (*info->fprintf_func) (info->stream, "%s %s,%s,d%d", op, dbuf,
			 sbuf, src2);
}

static void
print_insn_fmt4 (bfd_vma pc ATTRIBUTE_UNUSED, unsigned int insn,
		 int major ATTRIBUTE_UNUSED, int ext,
		 disassemble_info *info)
{
  static int opnd_sz_tab[32] =
    { 4, 4, 4, 4, 0, 4, 4, 0,
      4, 4, 1, 1, 1, 0, 0, 0,
      4, 2, 4, 2, 4, 2, 4, 0,
      4, 0, 4, 0, 4, 0, 2, 1 };
  unsigned int source = insn & 0x000007ff;
  unsigned int dreg = (insn >> 16) & 0x0f;
  unsigned int cnt  = (insn >> 11) & 0x1f;
  unsigned int flag = (insn >> 26) & 0x01;
  char sbuf[30];
  char *op;
  int opnd_sz = opnd_sz_tab[ext];

  operand (source, opnd_sz, sbuf);
  op = op_fmt4[ext];
  if (op == NULL)
    op = "*unknown*";

  if (ext == 0x06)	/* btst */
    {
      if (flag)
	(*info->fprintf_func) (info->stream, "%s %s,d%d", op,
			       sbuf, cnt);
      else
	(*info->fprintf_func) (info->stream, "%s %s,#0x%x", op,
			       sbuf, cnt);
    }
  else
    {
      if (flag)
	(*info->fprintf_func) (info->stream, "%s d%d,%s,d%d", op,
			       dreg, sbuf, cnt);
      else
	(*info->fprintf_func) (info->stream, "%s d%d,%s,#0x%x", op,
			       dreg, sbuf, cnt);
    }
}

static void
print_insn_fmt5 (bfd_vma pc ATTRIBUTE_UNUSED, unsigned int insn,
		 int major ATTRIBUTE_UNUSED, int ext ATTRIBUTE_UNUSED,
		 disassemble_info *info)
{
  unsigned int source = insn & 0x000007ff;
  int imm = (insn >> 11) & 0xffff;
  char sbuf[30];

  if (imm & 0x8000)
    imm |= -0x10000;

  operand (source, 2, sbuf);
  (*info->fprintf_func) (info->stream, "cmpi %s,#%d", sbuf, imm);
}

static void
print_insn_fmt6 (bfd_vma pc ATTRIBUTE_UNUSED, unsigned int insn,
		 int major ATTRIBUTE_UNUSED, int ext ATTRIBUTE_UNUSED,
		 disassemble_info *info)
{
  unsigned int dest = (insn >> 16) & 0x7ff;
  int imm = insn & 0x0000ffff;
  char dbuf[30];

  if (imm & 0x8000)
    imm |= -0x10000;

  operand (dest, 2, dbuf);
  if (dest == 0 && imm == 0)
    (*info->fprintf_func) (info->stream, "nop");
  else
    (*info->fprintf_func) (info->stream, "movei %s,#%d", dbuf, imm);
}

static void
print_insn_fmt7 (bfd_vma pc, unsigned int insn,
		 int major ATTRIBUTE_UNUSED, int ext ATTRIBUTE_UNUSED,
		 disassemble_info *info)
{
  static char *cond[16] =
    { "f",  "lo", "hs", "eq", "ge", "gt", "hi", "le",
      "ls", "lt", "mi", "ne", "pl", "t",  "vc", "vs" };

  int offset = insn & 0x001fffff;
  unsigned int c = (insn >> 21) & 0x01;
  unsigned int p = (insn >> 22) & 0x01;
  unsigned int cc = (insn >> 23) & 0x0f;

  (*info->fprintf_func) (info->stream, "jmp%s%s%s ", cond[cc],
			 (c ? "" : ".s"),
			 (p ? ".t" : ".f"));
  offset = pc + (offset << 2);
  (*info->print_address_func) (offset, info);
}

static void
print_insn_fmt8 (bfd_vma pc, unsigned int insn, int major,
		 int ext ATTRIBUTE_UNUSED, disassemble_info *info)
{
  unsigned int ofs21 = insn & 0x001fffff;
  unsigned int areg = (insn >> 21) & 0x07;
  unsigned int ofs3 = (insn >> 24) & 0x07;
  int offset = (ofs3 << 21) | ofs21;

  if (major == 0x1b)		/* call*/
    {
      (*info->fprintf_func) (info->stream, "call %s,0x", areg_names[areg]);
      offset = pc + (offset << 2);
    }
  else if (major == 0x1c)	/* moveai */
    {
      (*info->fprintf_func) (info->stream, "moveai %s,0x", areg_names[areg]);
      offset = offset << 7;
    }
  else if (major == 0x1d)	/* moveaih */
    {
      (*info->fprintf_func) (info->stream, "moveai %s,0x", areg_names[areg]);
      offset =  (offset << 7 | 0x1000000);
    }
  else
    (*info->fprintf_func) (info->stream, "*unknown* ");

  (*info->print_address_func) (offset, info);

}

static void
print_insn_fmt9 (bfd_vma pc ATTRIBUTE_UNUSED,
		 unsigned int insn ATTRIBUTE_UNUSED,
		 int major ATTRIBUTE_UNUSED, int ext,
		 disassemble_info *info)
{
  char *op = "*unknown*";
  int ofs1 = (insn & 0x07000000) >> 24;
  unsigned int an = (insn & 0x00e00000) >> 21;
  int ofs2 = (insn & 0x001f0000) >> 16;
  int ofs3 = (insn & 0x00000700) >> 8;
  unsigned int am = (insn & 0x000000e0) >> 5;
  int ofs4 = (insn & 0x0000001f);
  int offset = ((ofs1 << 13) | (ofs2 << 8) | (ofs3 << 5) | ofs4) << 2;

  if (offset & 0x20000)
    offset |= -0x40000;

  if (ext == 0x00)
    op = "calli";
  else if (ext == 0x01)
    op = "leai";
  else if (ext == 0x02)
    op = "calli.b";

  (*info->fprintf_func) (info->stream, "%s %s,%d(%s)", op,
			 areg_names[an], offset, areg_names[am]);
}

static void
print_insn_fmt10 (bfd_vma pc ATTRIBUTE_UNUSED,
		  unsigned int insn ATTRIBUTE_UNUSED,
		  int major ATTRIBUTE_UNUSED, int ext ATTRIBUTE_UNUSED,
		  disassemble_info *info)
{
  unsigned int imm_flag = (insn & 0x04000000) >> 26;
  unsigned int dsp_ctrl = (insn & 0x001f0000) >> 16;  /* CTS0A flags */
  unsigned int imm = (insn & 0x0000f800) >> 11;
  unsigned int sreg = (insn & 0x00007800) >> 11;
  unsigned int source = insn & 0x7ff;
  char *op = op_fmt10[ext];
  char *dest = (dsp_ctrl & 0x01) ? "acc1" : "acc0";
  char *src2 = (dsp_ctrl & 0x04) ? "acc" : "d";
  char *accx = (dsp_ctrl & 0x08) ? "_lo" : "";
  char *clip = (dsp_ctrl & 0x08) ? ".c" : "";
  char sbuf[30];

  if (op == 0)
    op = "*unknown*";

  operand (source, 4, sbuf);

  if (imm_flag == 0)
    (*info->fprintf_func) (info->stream, "%s%s %s,%s%s,#%d", op, clip,
			   dest, accx, sbuf, imm);
  else
    (*info->fprintf_func) (info->stream, "%s%s %s%s,%s,%s%d", op, clip,
			   dest, accx, sbuf, src2, sreg);
}

static void
print_insn_fmt11 (bfd_vma pc ATTRIBUTE_UNUSED,
		  unsigned int insn ATTRIBUTE_UNUSED,
		  int major ATTRIBUTE_UNUSED, int ext ATTRIBUTE_UNUSED,
		  disassemble_info *info)
{
  (*info->fprintf_func) (info->stream, "format11");
}

static void
print_insn_fmt12 (bfd_vma pc ATTRIBUTE_UNUSED,
		  unsigned int insn ATTRIBUTE_UNUSED,
		  int major ATTRIBUTE_UNUSED, int ext ATTRIBUTE_UNUSED,
		  disassemble_info *info)
{
  (*info->fprintf_func) (info->stream, "format12");
}

typedef void (*print_format_t)(bfd_vma pc, unsigned int insn,
			       int major, int ext,
			       disassemble_info *info);
static print_format_t print_format[13] =
  { NULL, print_insn_fmt1, print_insn_fmt2, print_insn_fmt3,
    print_insn_fmt4, print_insn_fmt5, print_insn_fmt6,
    print_insn_fmt7, print_insn_fmt8, print_insn_fmt9,
    print_insn_fmt10, print_insn_fmt11, print_insn_fmt12 };

int
print_insn_ubi32 (bfd_vma pc, disassemble_info *info)
{
  bfd_byte buf[4];   /* All Ubi32 instructions are 4 bytes. */
  int status;
  unsigned int insn;
  int major_opcode, opcode_extension = 0;
  int sz, ofs, format;
  char *op ATTRIBUTE_UNUSED;
	
  info->bytes_per_chunk = 4;

  status = (*info->read_memory_func) (pc, buf, 4, info);

  if (status != 0)
    {
      (*info->memory_error_func) (status, pc, info);
      return -1;
    }

  insn = bfd_get_bits (buf, 32, info->endian == BFD_ENDIAN_BIG);

  major_opcode = insn >> 27 & 0x1f;

  /* Extract opcode extension or alternate format flags. */
  format = translate_opcode_to_format[major_opcode].format;
  ofs = translate_opcode_to_format[major_opcode].offset;
  sz = translate_opcode_to_format[major_opcode].size;
  if (sz != 0)
    opcode_extension = insn >> ofs & ((1 << sz) - 1);

  if (format > 0 && format <= 12)
    print_format[format](pc, insn, major_opcode, opcode_extension, info);
  else
    fprintf (stderr, _("Internal error decoding instruction 0x%8.8x\n"), insn);

  return 4;	
}
