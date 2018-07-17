/* ubi32.h.  Qualcomm Ubi32 opcode list for GDB, the GNU debugger.
   Copyright (C) Eager Consulting.
   Written by Michael J. Eager <eager@eagercon.com>.

   This file is part of GDB, GAS, and the GNU binutils.

   GDB, GAS, and the GNU binutils are free software; you can redistribute
   them and/or modify them under the terms of the GNU General Public
   License as published by the Free Software Foundation; either version
   3, or (at your option) any later version.

   GDB, GAS, and the GNU binutils are distributed in the hope that they
   will be useful, but WITHOUT ANY WARRANTY; without even the implied
   warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See
   the GNU General Public License for more details.

   You should have received a copy of the GNU General Public License
   along with this program; see the file COPYING3. If not,
   see <http://www.gnu.org/licenses/>.  */

#ifndef _UBI32_H_
#define _UBI32_H_

#include <stdint.h>
#include <stdlib.h>

/* Describe each instruction.  */
struct op_table_t
{
  const char *mnemonic;
  enum op_fmt_t
    { FMT_1A,  FMT_1B,  FMT_1C,  FMT_1D,  FMT_2,   FMT_3,   FMT_4A,
      FMT_4B,
      FMT_5,   FMT_6,   FMT_7,   FMT_8,   FMT_9,   FMT_10,  FMT_11A,
      FMT_11B, FMT_11C, FMT_11D, FMT_12A, FMT_12B, FMT_12C,
      FMT_12D, FMT_MAC  /* Macros */
    } format;
  enum op_scale_t
    { SZ_0 = -1, SZ_1 = 0, SZ_2 = 1, SZ_4 = 2
    } scale;
  uint32_t instruction;
  uint32_t mask;
  uint32_t flags;
  uint32_t version;
};

extern struct op_table_t op_table[];

/* op_table_t.version flags.  */
#define UBI32_V1  0x01
#define UBI32_V2  0x02
#define UBI32_V3  0x04
#define UBI32_V4  0x08
#define UBI32_V5  0x10
#define UBI32_V6  0x20
#define UBI32_V61 0x40
#define UBI32_ALL 0xFF

/* op_table_t.flags special instruction handling flags */
#define FLAG_NONE 0x00
#define FLAG_EXT  0x01	  /* Opcode has extensions, eg. jmp<cc>.	*/
#define FLAG_MAC  0x02	  /* Opcode is macro NOP.			*/
#define FLAG_PDEC 0x04	  /* Opcode is PDEX, special offset calc. 	*/
#define FLAG_LEA  0x08	  /* Opcode is LEA, special dest offset calc. 	*/
#define FLAG_MOVEAI 0x10  /* Opcode is either MOVEAI or MOVEAIH. 	*/
#define FLAG_SCSR 0x20	  /* Opcode is SETCSR, force dest to CSR.	*/

/* Describe registers.  */
struct reg_table_t
{
  const char *name;
  uint32_t num;
  uint32_t rw;
  enum reg_class {
      NONE  = 0x00,
      DREG  = 0x01,
      AREG  = 0x02,
      PRIV  = 0x04,
      ACC32 = 0x08,
      ACC64 = 0x10
    } class;
  uint32_t version;
};

extern struct reg_table_t reg_table[];

/* Register number definitions.  */
#define REGNO_A0	0x20
#define REGNO_ACC0	0x28
#define REGNO_ACC0_HI	0x28
#define REGNO_ACC0_LO	0x29
#define REGNO_CSR	0x2d
#define REGNO_ACC1	0x36
#define REGNO_ACC1_HI	0x36
#define REGNO_ACC1_LO	0x37

/* reg_table_t.rw flags.  */
#define REG_R 0x01	/* Readable register.    */
#define REG_W 0x02	/* Writeable register.   */
#define REG_PRIV 0x04	/* Privileged register.  */

/* Describe JMP condition codes.  */
struct cc_t {
  const char *cond;
  int code;
};

extern struct cc_t cc_tab[];

/* DSP instruction Control bits.  */
#define DSP_CTRL_A 0x01 /* Destination register - 0 = ACC0, 1 = ACC1.  */
#define DSP_CTRL_0 0x02	/* Reserved - always zero.  */
#define DSP_CTRL_S 0x04 /* Source 2: 0 = Dn register, 1 = ACC register.  */
#define DSP_CTRL_T 0x08	/* Source 2: 0 = use bits 0-15, 1 = use bits 16-31.  */
#define DSP_CTRL_C 0x10 /* Result: 0 = 48 bit result, 1 = clip to 32 bits.  */

#endif
