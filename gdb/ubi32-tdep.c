/* Target-dependent code for Qualcomm UBI32.

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


#include "defs.h"
#include "arch-utils.h"
#include "target-descriptions.h"
#include "ubi32-tdep.h"
#include "dis-asm.h"
#include "opcodes/ubi32-dis.h"
#include "frame-base.h"
#include "frame-unwind.h"
#include "trad-frame.h"
#include "dwarf2-frame.h"
#include "gdbcore.h"

/* Register names of the Qualcomm UBI32 V6/V6.1 processor.
   The register numbers are GDB internal, not ISA register numbers.  */
static const char *ubi32_register_names[] =
{
  /* General data registers.  */
  "d0", "d1", "d2",  "d3",  "d4",  "d5",  "d6",  "d7",	/*  0- 7 */
  "d8", "d9", "d10", "d11", "d12", "d13", "d14", "d15",	/*  8-15 */
  /* Address registers.  */
  "a0", "a1", "a2",  "a3",  "a4",  "a5",  "a6",  "sp",	/* 16-23 */
  /* Accumulator 0.  */
  "acc0_hi", "acc0_lo", "mac_rc16", "source_3", 	/* 24-27 */
  /* Context & Control.  */
  "context_cnt", "csr", "rosr", "iread_data",		/* 28-31 */
  /* Interrupt mask registers.  */
  "int_mask0", "int_mask1", "int_mask2", "int_mask3", 	/* 32-35 */
  /* Program counter & trap.  */
  "pc", "rap_cause",					/* 36-37 */
  /* Accumulator 1.  */
  "acc1_hi", "acc1_lo", 				/* 38-39 */
  /* Previous PC & Control register.  */
  "previous_pc", "ucsr",				/* 40-41 */
  /* Chip ID.  */
  "chip_id",						/* 42    */
  /* Interrupt status, set, and clear.  */
  "int_stat0", "int_stat1", "int_stat2", "int_stat3",	/* 43-46 */
  "int_set0", "int_set1", "int_set2", "int_set3",	/* 47-50 */
  "int_clr0", "int_clr1", "int_clr2", "int_clr3",	/* 51-54 */
  /* Global control.  */
  "global_ctrl",					/* 55	 */
  /* Threads active status, set, and clear.  */
  "mt_active", "mt_active_set", "mt_active_clr",	/* 56-58 */
  /* Debugging treads active status and set.  */
  "mt_dbg_active", "mt_dbg_active_set",			/* 59-60 */
  /* Thread control enable, priority, scheduling.  */
  "mt_en", "mt_pri", "mt_sched", 			/* 61-63 */
  /* Thread stopped in break and clear.  */
  "mt_break", "mt_break_clr",				/* 64-65 */
  /* Single-step threads.  */
  "mt_single_step",					/* 66	 */
  /* Threads delay scheduling.  */
  "mt_min_del_en",					/* 67	 */
  /* Thread break set.  */
  "mt_break_set",					/* 68	 */
  /* Data capture address.  */
  "dcapt",						/* 69	 */
  /* Debugging threads active status clear.  */
  "mt_dbg_active_clr",					/* 70	 */
  /* Scratchpad registers.  */
  "scratchpad0", "scratchpad1", "scratchpad2",		/* 71-73 */
  "scratchpad3", "scratchpad4", "scratchpad5",		/* 74-76 */
  "scratchpad6",					/* 77	 */
  /* Chip configuration.  */
  "chip_cfg",						/* 78	 */
  /* Thread status and trap control.  */
  "mt_i_blocked", "mt_d_blocked", 			/* 79-80 */
  "mt_i_blocked_set", "mt_d_blocked_set",		/* 81-82 */
  "mt_blocked_clr", "mt_trap_en",			/* 83-84 */
  "mt_trap", "mt_trap_set", "mt_trap_clr",		/* 85-87 */
  /* Supervisor Call Entry Point.  */
  "sep",						/* 88	 */
  /* Valid instruction address ranges high, low, enable.  */
  "i_range0_hi", "i_range1_hi", "i_range2_hi", "i_range3_hi", /* 89-92 */
  "i_range0_lo", "i_range1_lo", "i_range2_lo", "i_range3_lo", /* 93-96 */
  "i_range0_en", "i_range1_en", "i_range2_en", "i_range3_en", /* 97-100 */
  /* Valid data address ranges high, low, enable.  */
  "d_range0_hi", "d_range1_hi", "d_range2_hi", "d_range3_hi",
  "d_range4_hi", "d_range5_hi",				/* 101-106 */
  "d_range0_lo", "d_range1_lo", "d_range2_lo", "d_range3_lo",
  "d_range4_lo", "d_range5_lo",				/* 107-112 */
  "d_range0_en", "d_range1_en", "d_range2_en", "d_range3_en",
  "d_range4_en", "d_range5_en",				/* 113-118 */
  /* Valid user instruction address ranges enable.  */
  "i_range0_user_en", "i_range1_user_en",
  "i_range2_user_en", "i_range3_user_en",		/* 119-122 */
  /* Valid user data address ranges enable.  */
  "d_range0_user_en", "d_range1_user_en",
  "d_range2_user_en", "d_range3_user_en",
  "d_range4_user_en", "d_range5_user_en",		/* 123-128 */
  /* FP control registers.  */
  "mt_fp_flush", "mt_fp_blocked", "mt_fp_blocked_set",	/* 129-131 */
  /* BTB control registers.  */
  "mt_btb_en", "btb_ctrl",				/* 132-133 */
};
#define UBI32_NUM_REGS ARRAY_SIZE (ubi32_register_names)


/* Return the name of register regnum.  */
static const char *
ubi32_register_name (struct gdbarch *gdbarch, int regnum ATTRIBUTE_UNUSED)
{
  if (regnum >= 0 && regnum < UBI32_NUM_REGS)
    return ubi32_register_names[regnum];
  return NULL;
}

static int
map_isa_reg_to_gdb (int isa_reg)
{
  /* FIXME:  Verify that this simplification works.  */
  if (isa_reg > 16)
    isa_reg -= 16;
  return isa_reg;
}

static struct type *
ubi32_register_type (struct gdbarch *gdbarch, int num)
{
  return builtin_type (gdbarch)->builtin_int;
}

/* Extract opcode for instruction.  */
static int
ubi32_extract_opcode (union ubi32_instruction instruction)
{
  /* Major opcode is in same location for each instruction format.  */
  unsigned opcode = instruction.format1.opcode;

  switch (opcode)
  {
    case 0x00:
    case 0x01:
      opcode = instruction.format1.opcode << 8
	       | instruction.format1.opcode_ex;
      break;

    case 0x02:
      opcode = instruction.format4.opcode << 8
	       | instruction.format4.opcode_ex;
      break;

    case 0x1e:
    case 0x1f:
      opcode = instruction.format9.opcode << 8
	       | instruction.format9.opcode_ex;
      break;

    case 0x06:
      opcode = instruction.format10.opcode << 8
	       | instruction.format10.opcode_ex;
      break;

    case 0x03:
      opcode = instruction.format11_12.opcode << 8
	       | instruction.format11_12.opcode_ex;
      break;

    default:
      opcode = instruction.format1.opcode << 8;
      break;
  }

  return opcode;
}

#if 0
/* Test if address field mode is immediate.  */
static int
is_operand_immediate (unsigned int field)
{
  return (0x400 & field);
}
#endif

/* Test if address field mode is direct.  */
static int
is_operand_direct (unsigned int opnd)
{
  return (UBI32_OPND_ADDR_MODE (opnd) == UBI32_OPND_DIRECT);
}

/* Test if address field mode is indirect.  */
static int
is_operand_indirect (unsigned int opnd)
{
  return (opnd && UBI32_OPND_INDIRECT);
}

/* Test if address field mode is preincrement.  */
static int
is_operand_preincrement (unsigned int opnd)
{
  return (UBI32_OPND_ADDR_MODE (opnd) == UBI32_OPND_INCR_DECR
	  && UBI32_OPND_INCR_MODE(opnd) == UBI32_OPND_PRE_INCR);
}

/* Extract offset value from operand.
   Note that the offset is not scaled by operand size.  */
static int
extract_operand_offset (unsigned int opnd)
{
  return ((opnd & UBI32_OPND_HI_IMM) >> 3) | (opnd & UBI32_OPND_LO_IMM);
}

/* Extract general register number from address field.
   Map ISA register number to GDB register number.  */
static int
extract_general_register (unsigned int field)
{
  return map_isa_reg_to_gdb (field & UBI32_OPND_REGNO);
}

/* Extract address register number from address field.  */
static int
extract_addr_register (unsigned int opnd)
{
  return (((opnd & UBI32_OPND_ADDR_REG) >> 5) + UBI32_A0_REGNUM);
}

/* Extract offset register number from address field.  */
static int
extract_offset_register (unsigned int opnd)
{
  return (opnd & UBI32_OPND_OFFSET_REG);
}

/* Is instruction a branch (jump, call, ret, bkpt, suspend)?  */
static int
is_branch_insn (union ubi32_instruction insn)
{
  int opcode = ubi32_extract_opcode (insn);

  switch (opcode)
  {
    case UBI32_INST_RET:
    case UBI32_INST_BKPT:
    case UBI32_INST_SUSPEND:
    case UBI32_INST_CALL:
    case UBI32_INST_CALLI:
    case UBI32_INST_JMP:
      return 1;
  }

  return 0;
}


/* Analyze prologue, return PC of first instruction after prologue.  */
static CORE_ADDR
ubi32_analyze_prologue (struct gdbarch *gdbarch, const CORE_ADDR start_pc,
			const CORE_ADDR current_pc,
			struct ubi32_frame_cache *cache,
			struct frame_info *this_frame)
{
  int num_insn = 0;
  int max_insns = 100;
  union ubi32_instruction insn;
  CORE_ADDR pc = start_pc;
  enum bfd_endian byte_order = gdbarch_byte_order (gdbarch);
  int opcode;
  CORE_ADDR end_prologue = start_pc;
  int reg;

/* printf ("ubi32_analyze_prologue (start_pc = 0x%8.8x, current_pc = 0x%8.8x)\n",
(int)start_pc, (int)current_pc); */

  if (current_pc < start_pc)
    return current_pc;


  insn.word = read_memory_unsigned_integer (pc, 4, byte_order);
  num_insn++;
  opcode = ubi32_extract_opcode (insn);
/* printf ("insn %d: 0x%8.8x: %8.8x --  opcode = %04x\n", num_insn, (int)pc,
		insn.word, opcode); */

  /* Handle varargs only at start of prologue.  */
  /* move.4 -4(sp)++,d9  Copy 1st varargs argument to stack.  */
  if (opcode == UBI32_INST_MOVE4
      && is_operand_preincrement (insn.format1.dest)
      && extract_addr_register (insn.format1.dest) == UBI32_SP_REGNUM
      && is_operand_direct (insn.format1.source1)
      && extract_general_register (insn.format1.source1) == UBI32_D9_REGNUM)
    {
      pc += 4;
      insn.word = read_memory_unsigned_integer (pc, 4, byte_order);
      num_insn++;
      opcode = ubi32_extract_opcode (insn);
      /* printf ("insn %d: 0x%8.8x: %8.8x --  opcode = %04x\n", num_insn, (int)pc,
	       insn.word, opcode); */

      for (reg = 8; reg >= 0; reg--)
        {
	  /* move.4 -4(sp)++,d8  Copy 2nd - 13th varargs arguments to stack.  */
	  if (num_insn > max_insns || pc > current_pc)
	    break;

	  if (!(opcode == UBI32_INST_MOVE4
	      && is_operand_preincrement (insn.format1.dest)
	      && extract_addr_register (insn.format1.dest) == UBI32_SP_REGNUM
	      && is_operand_direct (insn.format1.source1)
	      && extract_general_register (insn.format1.source1) == UBI32_D0_REGNUM + reg))
	    {
	      break;
	    }

	  pc += 4;
	  insn.word = read_memory_unsigned_integer (pc, 4, byte_order);
	  num_insn++;
	  opcode = ubi32_extract_opcode (insn);
	  /* printf ("insn %d: 0x%8.8x: %8.8x --  opcode = %04x\n", num_insn, (int)pc,
		  insn.word, opcode); */
        }
    }

/* FIXME:  Check for leai sp,-num(sp) to allocate array in stack.  */

  while (pc < current_pc && num_insn < max_insns)
    {
      /* movei d15,#-96  Copy negative frame size to a register.  */
      if (opcode == UBI32_INST_MOVEI
	  && is_operand_direct (insn.format6.dest)
	  && insn.format6.imm < 0)
	{
	  reg = extract_general_register (insn.format6.dest);

	  /* Check that the code sequence really did compute ``SP = SP -
	     SP_OFFSET''.  If it didn't bail out returning a PC pointing to
	     the first instruction before this entire mess started.  */
	  insn.word = read_memory_unsigned_integer (pc, 4, byte_order);
	  pc += 4;
	  opcode = ubi32_extract_opcode (insn);
/* printf ("insn %d: 0x%8.8x: %8.8x --  opcode = %04x\n", num_insn, (int)pc,
		insn.word, opcode); */

	  /* lea.1 sp,(sp,d15)  Subtract frame size from sp.  */
	  if ((opcode == UBI32_INST_LEA1
	       && is_operand_direct (insn.format1.dest)
	       && is_operand_indirect (insn.format1.source1)
	       && extract_general_register (insn.format1.dest) == UBI32_SP_REGNUM
	       && extract_addr_register (insn.format1.source1) == UBI32_SP_REGNUM
	       && extract_offset_register (insn.format1.source1) == reg)
              ||
	      /* add.4 sp, sp, d15  Alternative insn.  */
	      (opcode == UBI32_INST_ADD4
	       && is_operand_direct (insn.format3.dest)
	       && is_operand_direct (insn.format3.source1)
	       && extract_general_register (insn.format3.dest) == UBI32_SP_REGNUM
	       && extract_general_register (insn.format3.source2) == reg))
	    {
	      end_prologue = pc + 4;
	    }
	}
      /* move.4	-32(sp)++,a5 */
      else if (opcode == UBI32_INST_MOVE4
	       && is_operand_preincrement (insn.format1.dest)
	       && extract_addr_register (insn.format1.dest) == UBI32_SP_REGNUM
	       && is_operand_direct (insn.format1.source1)
	       && extract_general_register (insn.format1.source1) > UBI32_D9_REGNUM)
	{
	  end_prologue = pc + 4;
	}
      /* pdec sp, 48(sp) */
      else if (opcode == UBI32_INST_PDEC
	       && is_operand_direct (insn.format1.dest)
	       && extract_general_register (insn.format1.dest) == UBI32_SP_REGNUM
	       && is_operand_indirect (insn.format1.source1)
	       && extract_addr_register (insn.format1.source1) == UBI32_SP_REGNUM)
	{
	  end_prologue = pc + 4;
	}
      /* move.4 +4(sp),a6  Save call-preserved registers.  */
      else if (opcode == UBI32_INST_MOVE4
	       && is_operand_indirect (insn.format1.dest)
	       && extract_addr_register (insn.format1.dest) == UBI32_SP_REGNUM
	       && extract_operand_offset (insn.format1.dest) > 0
	       && is_operand_direct (insn.format1.source1)
	       && extract_general_register (insn.format1.source1) > UBI32_D9_REGNUM)
	{
	  end_prologue = pc + 4;
	}
      /* move.4 (sp),a6  Likewise, in leaf frame.  */
      else if (opcode == UBI32_INST_MOVE4
	       && is_operand_indirect (insn.format1.dest)
	       && extract_addr_register (insn.format1.dest) == UBI32_SP_REGNUM
	       && is_operand_direct (insn.format1.source1)
	       && extract_general_register (insn.format1.source1) > UBI32_D9_REGNUM)
	{
	  end_prologue = pc + 4;
	}
      /* lea.4 a6, offset(sp)  Setup frame pointer.  */
      else if (opcode == UBI32_INST_LEA4
	       && is_operand_direct (insn.format1.dest)
	       && extract_addr_register (insn.format1.dest) == UBI32_FP_REGNUM
	       && is_operand_indirect (insn.format1.source1)
	       && extract_addr_register (insn.format1.source1) == UBI32_SP_REGNUM)
	{
	  end_prologue = pc + 4;
	}
      else if (is_branch_insn (insn))
	    break;

      /* Read next instruction. */
      pc += 4;
      insn.word = read_memory_unsigned_integer (pc, 4, byte_order);
      num_insn++;
      opcode = ubi32_extract_opcode (insn);
/* printf ("insn %d: 0x%8.8x: %8.8x --  opcode = %04x\n", num_insn, (int)pc,
		insn.word, opcode); */
    }

  return end_prologue;
}

static CORE_ADDR
ubi32_skip_prologue (struct gdbarch *gdbarch, CORE_ADDR start_pc)
{
  struct ubi32_frame_cache cache;
  CORE_ADDR limit_pc, func_addr, func_end_addr = 0;

  /* See if we can determine the end of the prologue via the symbol table.
     If so, then return either PC, or the PC after the prologue, whichever
     is greater.  */
  if (find_pc_partial_function (start_pc, NULL, &func_addr, &func_end_addr))
    {
      CORE_ADDR post_prologue_pc
	= skip_prologue_using_sal (gdbarch, func_addr);
      if (post_prologue_pc != 0)
	return max (start_pc, post_prologue_pc);
    }

#if 0
  /* Can't determine prologue from the symbol table, need to examine
     instructions.  */

  /* Find an upper limit on the function prologue using the debug
     information.  If the debug information could not be used to provide
     that bound, then use an arbitrary large number as the upper bound.  */
  limit_pc = skip_prologue_using_sal (gdbarch, start_pc);
  if (limit_pc == 0)
    limit_pc = start_pc + 100;          /* Magic.  */
#else
  /* If we found the end of the function, use that as the limit, 
     otherwise pick an arbitrary limit.  */
  if (func_end_addr)
    limit_pc = func_end_addr;
  else
    limit_pc = start_pc + 100;          /* Magic.  */
#endif

  return ubi32_analyze_prologue (gdbarch, start_pc, limit_pc, &cache, NULL);
}

static struct ubi32_frame_cache *
ubi32_frame_cache (struct frame_info *next_frame, void **this_cache)
{
  struct ubi32_frame_cache *cache;
  struct gdbarch *gdbarch = get_frame_arch (next_frame);
  CORE_ADDR func;
  int rn;

  if (*this_cache)
    return (struct ubi32_frame_cache *) *this_cache;

  cache = FRAME_OBSTACK_ZALLOC (struct ubi32_frame_cache);
  *this_cache = cache;

  cache->base = 0;
  cache->pc = 0;
  cache->frameless_p = 0;

  cache->saved_regs = trad_frame_alloc_saved_regs (next_frame);

  /* Clear offsets to saved regs in frame.  */
  for (rn = 0; rn < gdbarch_num_regs (gdbarch); rn++)
    cache->register_offsets[rn] = -1;

  func = get_frame_func (next_frame);

  cache->pc = get_frame_address_in_block (next_frame);

  return cache;
}

/* Return an instruction to set a breakpoint at PCPTR, adjusting PCPTR if
   necessary, and store in LENPTR the size of the returned instruction.  */

static const unsigned char *
ubi32_breakpoint_from_pc (struct gdbarch *gdbarch, CORE_ADDR *pc,
			  int *len)
{
  /* BKPT instruction.  */
  static unsigned char break_insn[] = { 0x00, 0x00, 0x38, 0x00 };

  *len = sizeof (break_insn);
  return break_insn;
}

/* Given THIS_FRAME, find the previous frame's resume PC (which will
   be used to construct the previous frame's ID, after looking up the
   containing function).  */

static CORE_ADDR
ubi32_unwind_pc (struct gdbarch *gdbarch, struct frame_info *this_frame)
{
  CORE_ADDR pc;
  pc = frame_unwind_register_unsigned (this_frame, UBI32_PC_REGNUM);
  return pc;
}

/* Given a GDB frame, determine the address of the calling function's
   frame.  This will be used to create a new GDB frame struct.  */
static void
ubi32_frame_this_id (struct frame_info *this_frame, void **this_cache,
		     struct frame_id *this_id)
{
  struct ubi32_frame_cache *cache =
    ubi32_frame_cache (this_frame, this_cache);

  /* This marks the outermost frame.  */
  if (cache->base == 0)
    return;

  (*this_id) = frame_id_build (cache->base, cache->pc);
}

static struct value *
ubi32_frame_prev_register (struct frame_info *this_frame,
				 void **this_cache, int regnum)
{
  struct ubi32_frame_cache *cache =
    ubi32_frame_cache (this_frame, this_cache);

  if (cache->frameless_p)
    {
/* FIXME: Do we need to do something different for frameless?  */
      return trad_frame_get_prev_register (this_frame,
					   cache->saved_regs, regnum);
    }
  else
    return trad_frame_get_prev_register (this_frame, cache->saved_regs,
					 regnum);

}


static const struct frame_unwind ubi32_frame_unwind = {
  NORMAL_FRAME,
  default_frame_unwind_stop_reason,
  ubi32_frame_this_id,
  ubi32_frame_prev_register,
  NULL,
  default_frame_sniffer
};


static struct gdbarch *
ubi32_gdbarch_init (struct gdbarch_info info, struct gdbarch_list *arches)
{
  struct gdbarch *gdbarch;
  struct tdesc_arch_data *tdesc_data = NULL;
  const struct target_desc *tdesc = info.target_desc;
  struct gdbarch_tdep *tdep;
  int i;

  /* Find a candidate among extant architectures.  Only one architecture so
     far, so use simple search. */
  arches = gdbarch_list_lookup_by_info (arches, &info);
  if (arches != NULL)
    {
      return arches->gdbarch;
    }

  /* Check any target description for validity.  */
  if (tdesc_has_registers (tdesc))
    {
      const struct tdesc_feature *feature;
      int valid_p;

      feature = tdesc_find_feature (tdesc, "org.gnu.gdb.ubi32.core");
      if (feature == NULL)
        return NULL;
      tdesc_data = tdesc_data_alloc ();

      valid_p = 1;
      for (i = 0; i < UBI32_NUM_REGS; i++)
        valid_p &= tdesc_numbered_register (feature, tdesc_data, i,
                                            ubi32_register_names[i]);
      if (!valid_p)
        {
          tdesc_data_cleanup (tdesc_data);
          return NULL;
        }
    }

  /* Allocate space for the new architecture.  */
  tdep = XNEW (struct gdbarch_tdep);
  gdbarch = gdbarch_alloc (&info, tdep);

  set_gdbarch_num_regs (gdbarch, UBI32_NUM_REGS);
  set_gdbarch_register_name (gdbarch, ubi32_register_name);
  set_gdbarch_register_type (gdbarch, ubi32_register_type);

  /* Register numbers of stack pointer and program counter.  */
  set_gdbarch_sp_regnum (gdbarch, UBI32_SP_REGNUM);
  set_gdbarch_pc_regnum (gdbarch, UBI32_PC_REGNUM);

  /* Set data sizes.  */
  set_gdbarch_num_pseudo_regs (gdbarch, 0);
  set_gdbarch_ptr_bit (gdbarch, 32);
  set_gdbarch_short_bit (gdbarch, 16);
  set_gdbarch_int_bit (gdbarch, 32);
  set_gdbarch_long_bit (gdbarch, 32);
  set_gdbarch_long_long_bit (gdbarch, 64);
  set_gdbarch_float_bit (gdbarch, 32);
  set_gdbarch_double_bit (gdbarch, 64);
  set_gdbarch_long_double_bit (gdbarch, 64);
  set_gdbarch_float_format (gdbarch, floatformats_ieee_single);
  set_gdbarch_double_format (gdbarch, floatformats_ieee_double);
  set_gdbarch_long_double_format (gdbarch, floatformats_ieee_double);

  set_gdbarch_skip_prologue (gdbarch, ubi32_skip_prologue);
  set_gdbarch_inner_than (gdbarch, core_addr_lessthan);
  set_gdbarch_breakpoint_from_pc (gdbarch, ubi32_breakpoint_from_pc);
  set_gdbarch_unwind_pc(gdbarch, ubi32_unwind_pc);

  /* Unwind the frame.  */
  dwarf2_append_unwinders (gdbarch);
  frame_unwind_append_unwinder (gdbarch, &ubi32_frame_unwind);
  frame_base_append_sniffer (gdbarch, dwarf2_frame_base_sniffer);

  set_gdbarch_print_insn (gdbarch, print_insn_ubi32);

  return gdbarch;
}

/* Silence -Wmissing-prototypes */
initialize_file_ftype _initialize_ubi32_tdep;

void
_initialize_ubi32_tdep (void)
{
  register_gdbarch_init (bfd_arch_ubi32, ubi32_gdbarch_init);
}
