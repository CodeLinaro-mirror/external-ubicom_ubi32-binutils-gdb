/* Target-dependent code for Qualcomm Ubi32.

   Copyright (C) 2016 Free Software Foundation, Inc.

   This file is part of GDB.

   This program is free software; you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the Free Software Foundation; either version 3 of the License, or
   (at your option) any later version.

   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.

   You should have received a copy of the GNU General Public License
   along with this program.  If not, see <http://www.gnu.org/licenses/>.  */


/* Target-dependent structure in gdbarch.  */
struct gdbarch_tdep
{
  /* No target-dependent data currently defined.  */
};


/* Internal GDB register numbering.
   This is different from the register numbering used in the ISA.  */
enum ubicom32v6_gdb_regnum {
  UBI32_D0_REGNUM = 0,				/* general data */
  UBI32_D1_REGNUM = 1,				/* general data */
  UBI32_D2_REGNUM = 2,				/* general data */
  UBI32_D3_REGNUM = 3,				/* general data */
  UBI32_D4_REGNUM = 4,				/* general data */
  UBI32_D5_REGNUM = 5,				/* general data */
  UBI32_D6_REGNUM = 6,				/* general data */
  UBI32_D7_REGNUM = 7,				/* general data */
  UBI32_D8_REGNUM = 8,				/* general data */
  UBI32_D9_REGNUM = 9,				/* general data */
  UBI32_D10_REGNUM = 10,			/* general data */
  UBI32_D11_REGNUM = 11,			/* general data */
  UBI32_D12_REGNUM = 12,			/* general data */
  UBI32_D13_REGNUM = 13,			/* general data */
  UBI32_D14_REGNUM = 14,			/* general data */
  UBI32_D15_REGNUM = 15,			/* general data */
  UBI32_A0_REGNUM = 16,				/* general address */
  UBI32_A1_REGNUM = 17,				/* general address */
  UBI32_A2_REGNUM = 18,				/* general address */
  UBI32_A3_REGNUM = 19,				/* general address */
  UBI32_A4_REGNUM = 20,				/* general address */
  UBI32_A5_REGNUM = 21,				/* general address / link register */
  UBI32_A6_REGNUM = 22,				/* general address / frame pointer  */
  UBI32_FP_REGNUM = UBI32_A6_REGNUM,		/* frame pointer */
  UBI32_A7_REGNUM = 23,				/* gerenal address / stack pointer */
  UBI32_SP_REGNUM = UBI32_A7_REGNUM,		/* stack pointer */
  UBI32_LR_REGNUM  = UBI32_A5_REGNUM,		/* link register (return address) */
  UBI32_PC_REGNUM = 36,				/* program counter */
  UBI32_RET_REGNUM = UBI32_D0_REGNUM,		/* return value */
  UBI32_RET2_REGNUM = UBI32_D1_REGNUM,		/* 2nd word of 8-byte return value */
  UBI32_SRET_REGNUM = UBI32_D0_REGNUM,		/* returned struct address */
  UBI32_ROSR_REGNUM = 30,			/* read-only status */
  UBI32_MT_ACTIVE_REGNUM = 52,			/* threads active status */
  UBI32_MT_EN_REGNUM = 57,			/* threads enabled */
  UBI32_MT_PRI_REGNUM = 58,			/* thread priorities */
  UBI32_MT_SCHED_REGNUM = 59,			/* thread scheduling policies */
  UBI32_NUM_REGS = 134,				/* Total number of registers */
};

/* FIXME:  Are all of these values needed?  */
struct ubi32_frame_cache
{
  /* Base address.  */
  CORE_ADDR base;
  CORE_ADDR pc;

  /* Do we have a frame?  */
  int frameless_p;

  /* Frame size.  */
  int framesize;

  /* Frame register.  */
  int fp_regnum;

  /* Offsets to saved registers.  */
  int register_offsets[UBI32_NUM_REGS];

  /* Table of saved registers.  */
  struct trad_frame_saved_reg *saved_regs;
};


/* Ubi32 instruction format (little-endian host).  */
union ubi32_instruction
{
  unsigned int word;

  struct format1
  {
    unsigned int source1:11;
    unsigned int opcode_ex:5;
    unsigned int dest:11;
    unsigned int opcode:5;
  } format1;

  struct format2
  {
    unsigned int source1:11;
    unsigned int bitn:5;
    unsigned int dest:11;
    unsigned int opcode:5;
  } format2;

  struct format3
  {
    unsigned int source1:11;
    unsigned int source2:4;
    unsigned int x:1;
    unsigned int dest:11;
    unsigned int opcode:5;
  } format3;

  struct format4
  {
    unsigned int source1:11;
    union
    {
      unsigned int bitn:5;     /* Fmt 4a.  */
      unsigned int source2:5;  /* Fmt 4b: high bit == 0. */
    };
    unsigned int Dn:4;
    unsigned int x:1;
    unsigned int opcode_ex:5;
    unsigned int imm:1;
    unsigned int opcode:5;
  } format4;

  struct format5
  {
    unsigned int source1:11;
    unsigned int imm:16;
    unsigned int opcode:5;
  } format5;

  struct format6
  {
    unsigned int imm:16;
    unsigned int dest:11;
    unsigned int opcode:5;
  } format6;

  struct format7
  {
    unsigned int offset:21;
    unsigned int c:1;
    unsigned int p:1;
    unsigned int cond:4;
    unsigned int opcode:5;
  } format7;

  struct format8
  {
    unsigned int offset:21;
    unsigned int an:3;
    unsigned int off:3;
    unsigned int opcode:5;
  } format8;

  struct format9
  {
    unsigned int offset_low:5;
    unsigned int am:3;
    unsigned int offset_mid:5;
    unsigned int opcode_ex:5;
    unsigned int offset_hi:5;
    unsigned int an:3;
    unsigned int off:3;
    unsigned int opcode:5;
  } format9;

  struct format10
  {
    unsigned int source1:11;
    union
    {
      unsigned int imm:5;      /* Fmt 10a.  */
      unsigned int source2:5;  /* Fmt 10b: high bit == 0. */
    };
    unsigned int dsp:5;
    unsigned int opcode_ex:5;
    unsigned int imms:1;
    unsigned int opcode:5;
  } format10;

  struct format11_12
  {
    unsigned int remainder:21;
    unsigned int opcode_ex:5;
    unsigned int x:1;
    unsigned int opcode:5;
  } format11_12;
};

/* Ubi32 operand masks.  */
#define UBI32_OPND_MODE_MASK	0x700
#define UBI32_OPND_INDIRECT	0x400
#define UBI32_OPND_INCR_DECR	0x200
#define UBI32_OPND_DIRECT	0x100
#define UBI32_OPND_SIGNED_IMM	0x000
#define UBI32_OPND_PDIR_MASK	0x010
#define UBI32_OPND_PRE_INCR	0x010
#define UBI32_OPND_POST_INCR	0x000
#define UBI32_OPND_ADDR_REG	0x0e0
#define UBI32_OPND_OFFSET_REG	0x00f
#define UBI32_OPND_HI_IMM	0x300
#define UBI32_OPND_LO_IMM	0x01f
#define UBI32_OPND_REGNO	0x0ff
#define UBI32_OPND_SIGNED_VAL	0x0ff

#define UBI32_OPND_ADDR_MODE(opnd) (opnd & UBI32_OPND_MODE_MASK)
#define UBI32_OPND_INCR_MODE(opnd) (opnd & UBI32_OPND_PDIR_MASK)


/* Opcodes:  primary opcode in high-order byte, extended opcode in
   low-order byte.  */
#define UBI32_INST_SUSPEND	0x0001
#define UBI32_INST_RET		0x0004
#define UBI32_INST_BKPT		0x0007
#define UBI32_INST_MOVE4	0x000c
#define UBI32_INST_LEA4		0x001c
#define UBI32_INST_PDEC		0x001e
#define UBI32_INST_LEA1		0x001f
#define UBI32_INST_MOVEI	0x1900
#define UBI32_INST_CALL		0x1b00
#define UBI32_INST_CALLI	0x1e00
#define UBI32_INST_JMP		0x1a00
