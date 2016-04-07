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
