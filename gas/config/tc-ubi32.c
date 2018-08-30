/* tc-ubi32.c -- Assembler for the Qualcomm Ubi32
   Copyright (C) 2017-2018 Eager Consulting.

   This file is part of GAS, the GNU Assembler.
   Written by Michael Eager <eager@eagercon.com>.

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
   Boston, MA 02111-1307, USA.  */

#include "as.h"
#include "config.h"
#include "subsegs.h"
#include "dw2gencfi.h"
#include "safe-ctype.h"
#include "dwarf2dbg.h"


#include "opcode/ubi32.h"

#include <stdint.h>

enum options
{
  OPTION_UBI32V2 = OPTION_MD_BASE,
  OPTION_UBI32V3,
  OPTION_UBI32V4,
  OPTION_UBI32V5,
  OPTION_UBI32V6,
  OPTION_UBI32V61,
  OPTION_UBI32_FDPIC,
  OPTION_EB,
  OPTION_EL,
  OPTION_CPU
};

struct option md_longopts[] =
{
/* Define BFD_UBI32_OLD_NAME to use old ubicom32 names.  */
  { "mubicom32v1", no_argument, NULL, OPTION_UBI32V2 },
  { "mubicom32v2", no_argument, NULL, OPTION_UBI32V2 },
  { "mubicom32v3", no_argument, NULL, OPTION_UBI32V3 },
  { "mubicom32v4", no_argument, NULL, OPTION_UBI32V4 },
  { "mubicom32v5", no_argument, NULL, OPTION_UBI32V5 },
  { "mubicom32v6", no_argument, NULL, OPTION_UBI32V6 },
  { "mubicom32v61", no_argument, NULL, OPTION_UBI32V61 },
  { "mubi32v1", no_argument, NULL, OPTION_UBI32V2 },
  { "mubi32v2", no_argument, NULL, OPTION_UBI32V2 },
  { "mubi32v3", no_argument, NULL, OPTION_UBI32V3 },
  { "mubi32v4", no_argument, NULL, OPTION_UBI32V4 },
  { "mubi32v5", no_argument, NULL, OPTION_UBI32V5 },
  { "mubi32v6", no_argument, NULL, OPTION_UBI32V6 },
  { "mubi32v61", no_argument, NULL, OPTION_UBI32V61 },
  { "mfdpic", no_argument, NULL, OPTION_UBI32_FDPIC },
  { "EB", no_argument, NULL, OPTION_EB},
  { "EL", no_argument, NULL, OPTION_EL},
  { "mcpu", required_argument, NULL, OPTION_CPU },
  { NULL, no_argument, NULL, 0 },
};
size_t md_longopts_size = sizeof (md_longopts);

const char * md_shortopts = "";

const char comment_chars[]        = ";";
const char line_comment_chars[]   = "#";
const char line_separator_chars[] = "";
const char EXP_CHARS[]            = "eE";
const char FLT_CHARS[]            = "dD";

/* Mach selected from command line.  */
int ubi32_mach = bfd_mach_ubi32v61;


void
md_show_usage (stream)
    FILE * stream;
{
  fprintf (stream, _("UBI32 specific command line options:\n"));
#ifdef BFD_UBI32_OLD_NAME
  fprintf (stream, _("  -mubicom32v1  ISA v1 (IP3035)\n"));
  fprintf (stream, _("  -mubicom32v2  ISA v2 (IP3k)\n"));
  fprintf (stream, _("  -mubicom32v3  ISA v3 (IP5k)\n"));
  fprintf (stream, _("  -mubicom32v4  ISA v4 (IP7k)\n"));
  fprintf (stream, _("  -mubicom32v5  ISA v5 (IP8k)\n"));
  fprintf (stream, _("  -mubicom32v6  ISA v6 (Akronite)\n"));
  fprintf (stream, _("  -mubicom32v61 ISA v6.1 (Hawkeye)\n"));
#else
  fprintf (stream, _("  -mubi32v1  ISA v1 (IP3035)\n"));
  fprintf (stream, _("  -mubi32v2  ISA v2 (IP3k)\n"));
  fprintf (stream, _("  -mubi32v3  ISA v3 (IP5k)\n"));
  fprintf (stream, _("  -mubi32v4  ISA v4 (IP7k)\n"));
  fprintf (stream, _("  -mubi32v5  ISA v5 (IP8k)\n"));
  fprintf (stream, _("  -mubi32v6  ISA v6 (Akronite)\n"));
  fprintf (stream, _("  -mubi32v61 ISA v6.1 (Hawkeye)\n"));
#endif
  fprintf (stream, _("  -mfdpic       This in addition to the v3, v4 or v5 flags will produce an FD-PIC .o.\n"));
  fprintf (stream, _("  -EB           Assemble code for a big-endian cpu\n"));
  fprintf (stream, _("  -EL           Assemble code for a little-endian cpu\n"));
  fprintf (stream, _("  -mcpu=        Assemble code for specified cpu\n"));
}

/* The target specific pseudo-ops which we support.  */
const pseudo_typeS md_pseudo_table[] =
{
    { "file",	(void (*)(int))dwarf2_directive_file,	0 },
    { "loc",	dwarf2_directive_loc,	0 },
#ifdef FIXME
    { "picptr", ubi32_pic_ptr,	4 },
#endif
    { "word",	cons,			4 },
    { NULL,	NULL,			0 }
};

const char *
md_atof (int type, char *litP, int *sizeP)
{
  return ieee_md_atof (type, litP, sizeP, target_big_endian);
}

/* Insert value into buffer at offset bits for len bits.  */
/* FIXME -- check for overflow? */
static void
insert_bits (int *buf, int value, int offset, int len)
{
  int mask = (1 << len) - 1;

  *buf |= (value & mask) << offset;
}

/* Apply a fixup to the object file.  */
void
md_apply_fix (fixS *fixP, valueT *valP, segT seg ATTRIBUTE_UNUSED)
{
  char *where = fixP->fx_frag->fr_literal + fixP->fx_where;
  valueT value = *valP;

  if (fixP->fx_addsy == 0 && !fixP->fx_pcrel)
    fixP->fx_done = 1;

  /* On a 64-bit host, silently truncate 'value' to 32 bits for
     consistency with the behaviour on 32-bit hosts.  Remember value
     for emit_reloc.  */
  value &= 0xffffffff;
  value ^= 0x80000000;
  value -= 0x80000000;

  *valP = value;
  fixP->fx_addnumber = value;

  /* Same treatment for fixP->fx_offset.  */
  fixP->fx_offset &= 0xffffffff;
  fixP->fx_offset ^= 0x80000000;
  fixP->fx_offset -= 0x80000000;

  switch (fixP->fx_r_type)
    {
    case BFD_RELOC_UBI32_21_PCREL:
      /* where should always be word offset. */
      gas_assert (((long)where & 0x3) == 0);
      value >>= 2;
      insert_bits ((int *)where, value, 0, 21);
      fixP->fx_offset = *valP;
      break;

    case BFD_RELOC_UBI32_24_PCREL:
      /* where should always be word offset. */
      gas_assert (((long)where & 0x3) == 0);
      value >>= 2;
      insert_bits ((int *)where, value, 0, 21);
      insert_bits ((int *)where, value >> 21, 24, 3);
      fixP->fx_offset = *valP;
      break;

    case BFD_RELOC_UBI32_HI24:
    case BFD_RELOC_UBI32_LO7_S:
    case BFD_RELOC_UBI32_LO7_2_S:
    case BFD_RELOC_UBI32_LO7_4_S:
    case BFD_RELOC_UBI32_LO7_D:
    case BFD_RELOC_UBI32_LO7_2_D:
    case BFD_RELOC_UBI32_LO7_4_D:
      fixP->fx_offset = *valP;
      break;

    case BFD_RELOC_8:
      if (fixP->fx_done || !seg->use_rela_p)
	*where = value;
      break;

    case BFD_RELOC_16:
      if (fixP->fx_done || !seg->use_rela_p)
	md_number_to_chars (where, value, 2);
      break;

    case BFD_RELOC_32:
      if (fixP->fx_done || !seg->use_rela_p)
	md_number_to_chars (where, value, 4);
      break;

    case BFD_RELOC_24:
    case BFD_RELOC_64:
    case BFD_RELOC_UNUSED:
    default:
      as_bad_where (fixP->fx_file, fixP->fx_line,
		    _("bad relocation fixup type (%d)"), fixP->fx_r_type);

      break;
    }
}

/* Force relocations to be done in linker, even if they can
   be resolved at assembly time.  This is to provide output
   identical to the old assembler.

   Comments in old (CGEN-based) assembler say that this is
   to support relaxation in linker.   This should be removed.
*/
int
ubi32_force_relocation (fix)
     fixS * fix;
{
  if ((fix->fx_r_type == BFD_RELOC_UBI32_21_PCREL)
      || (fix->fx_r_type == BFD_RELOC_UBI32_24_PCREL))
    return 1;

  return 0;
}

/* Return an initial guess of the length by which a fragment must grow to
   hold a branch to reach its destination.
   Also updates fr_type/fr_subtype as necessary.

   Called just before doing relaxation.
   Any symbol that is now undefined will not become defined.
   The guess for fr_var is ACTUALLY the growth beyond fr_fix.
   Whatever we do to grow fr_fix or fr_var contributes to our returned value.
   Although it may not be explicit in the frag, pretend fr_var starts with a
   0 value.  */

int
md_estimate_size_before_relax (fragP, segment)
     fragS * fragP;
     segT    segment ATTRIBUTE_UNUSED;
{
  int    old_fr_fix = fragP->fr_fix;

  /* The only thing we have to handle here are symbols outside of the
     current segment.  They may be undefined or in a different segment in
     which case linker scripts may place them anywhere.
     However, we can't finish the fragment here and emit the reloc as insn
     alignment requirements may move the insn about.  */

  return (fragP->fr_var + fragP->fr_fix - old_fr_fix);
}

/* Generate a reloc for fixup.  */
arelent *
tc_gen_reloc (asection *seg ATTRIBUTE_UNUSED, fixS *fixp)
{
  arelent *reloc;

  reloc = XNEW (arelent);

  reloc->sym_ptr_ptr = XNEW (asymbol *);
  *reloc->sym_ptr_ptr = symbol_get_bfdsym (fixp->fx_addsy);
  reloc->address = fixp->fx_frag->fr_address + fixp->fx_where;
  reloc->howto = bfd_reloc_type_lookup (stdoutput, fixp->fx_r_type);
  if (reloc->howto == NULL)
    {
      as_bad_where (fixp->fx_file, fixp->fx_line,
		    _("cannot represent %s relocation in object file"),
		    bfd_get_reloc_code_name (fixp->fx_r_type));
      return NULL;
    }
  reloc->addend = fixp->fx_offset;

  return reloc;
}

/* Collect info about operand.  */
struct operand_t
{
  int value;
  bfd_reloc_code_real_type reloc;
  expressionS exp;
};

/* Ubi32 architectural register numbers.  */
static struct hash_control *reg_hash;

/* Ubi32 GDB register numbers.  */
static struct hash_control *gdb_reg_hash;

void
md_begin (void)
{
  struct reg_table_t *reg = reg_table;
  const char **gdb_reg = ubi32_gdb_register_names;

  /* Record the specific machine in the elf header flags area */
  bfd_set_private_flags (stdoutput, ubi32_mach);

  /* Set the machine type */
  bfd_default_set_arch_mach (stdoutput, bfd_arch_ubi32, ubi32_mach & 0xffff);

  /* Initialize arch register table.  */
  reg_hash = hash_new_sized (511);   /* Pick arbitrary size for hash table.  */
  while (reg->name)
  {
    hash_insert (reg_hash, reg->name, (void *) reg);
    reg++;
  }

  /* Initialize GDB register table.  */
  gdb_reg_hash = hash_new_sized (511);   /* Pick arbitrary size for hash table.  */
  while (*gdb_reg)
  {
    hash_insert (gdb_reg_hash, *gdb_reg, (void *) gdb_reg);
    gdb_reg++;
  }
}

static int ubi32_version = UBI32_V61;
int
md_parse_option (int c ATTRIBUTE_UNUSED, const char * arg ATTRIBUTE_UNUSED)
{
  /* FIXME */
  // int pic_state = ubi32_mach & 0xffff0000;

  switch (c)
    {
    case OPTION_UBI32V2:
      ubi32_mach = bfd_mach_ubi32;
      ubi32_version = UBI32_V2;
      break;

   case OPTION_UBI32V3:
      ubi32_mach = bfd_mach_ubi32v3;
      ubi32_version = UBI32_V3;
      break;

    case OPTION_UBI32V4:
      ubi32_mach = bfd_mach_ubi32v4;
      ubi32_version = UBI32_V4;
      break;

    case OPTION_UBI32V5:
      ubi32_mach = bfd_mach_ubi32v5;
      ubi32_version = UBI32_V5;
      break;

    case OPTION_UBI32V6:
      ubi32_mach = bfd_mach_ubi32v6;
      ubi32_version = UBI32_V6;
      break;

    case OPTION_UBI32V61:
      ubi32_mach = bfd_mach_ubi32v61;
      ubi32_version = UBI32_V61;
      break;

#if 0
    case OPTION_CPU_UBI32_FDPIC:
      /* FIXME */
      ubi32_mach |= EF_UBI32_FDPIC;
      as_warn ("-fdpic ignored");
      break;
#endif

    case OPTION_EB:
      target_big_endian = 1;
      break;

    case OPTION_EL:
      target_big_endian = 0;
      break;

    default:
      return 0;
    }

  /* FIXME */
  // ubi32_mach |= pic_state;

  return 1;
}

valueT
md_section_align (segment, size)
     segT   segment;
     valueT size;
{
  int align = bfd_get_section_alignment (stdoutput, segment);
  return ((size + (1 << align) - 1) & (0xFFFFFFFF << align));
}


static struct op_table_t *
ubi32_lookup_insn (char *str, int len, int version)
{
  /* FIXME -- convert to binary search with duplicates. */
  struct op_table_t *insn;

  for (insn = op_table; insn->mnemonic; insn++)
    {
      if (insn->version & version)
	{
	   if ((strncasecmp (insn->mnemonic, str, len) == 0)
	       || ((insn->flags & FLAG_EXT)
	           && (strncasecmp (insn->mnemonic, str, strlen (insn->mnemonic)) == 0)))
	     return insn;
	}
    }
  return NULL;
}



static const char *
parse_literal (char **strp, char lit)
{
  if (**strp == lit)
    {
      (*strp)++;
      return NULL;
     }
  return _("invalid syntax");
}

/* Parse register by class.  */
/* FIXME:  Allow register address. */
static const char *
parse_register_1 (char **strp, int *regno, enum reg_class class)
{
  unsigned len = 0;
  struct reg_table_t *reg;
  char regname[20];

  while (len < sizeof (regname)
         && (ISALNUM((*strp)[len]) || (*strp)[len] == '_'))
    {
      regname[len] = TOLOWER ((*strp)[len]);
      len++;
    }
  regname[len] = '\0';

  reg = hash_find_n (reg_hash, regname, len);
  if (reg && (class == NONE || class == reg->class))
    {
      *regno = reg->num;
      *strp += len;
      return NULL;
    }

  return _("invalid register name");
}

/* Parse any register.  */
static const char *
parse_register (char **strp, int *regno)
{
  return parse_register_1 (strp, regno, NONE);
}

/* Parse A register.  */
static const char *
parse_areg (char **strp, int *regno)
{
  const char *err;
  int reg;

  if ((err = parse_register_1 (strp, &reg, AREG)))
    return err;
  *regno = reg - REGNO_A0;
  return NULL;
}

/* Parse D register.  */
static const char *
parse_dreg (char **strp, int *regno)
{
  return parse_register_1 (strp, regno, DREG);
}

/* Parse ACC register.  Return FPS32 encoding.  */
static const char *
parse_accreg (char **strp, int *regno, enum reg_class class)
{
  const char *err;
  int reg;

  if ((err = parse_register_1 (strp, &reg, class)))
    return _("invalid acc reg");

  if (class == ACC64)
    {
      if (reg == REGNO_ACC0)
	*regno = 0x10;
      if (reg == REGNO_ACC1)
	*regno = 0x12;
    }
  else
    {
      if (reg == REGNO_ACC0_LO)
	*regno = 0x10;
      else if (reg == REGNO_ACC0_HI)
	*regno = 0x11;
      else if (reg == REGNO_ACC1_LO)
	*regno = 0x12;
      else if (reg == REGNO_ACC1_HI)
	*regno = 0x13;
    }

  return NULL;
}

/* Parse D or ACC register.  Return FPS32 encoding. */
static const char *
parse_accdreg (char **strp, int *regno, enum reg_class class)
{
  const char *err;

  if (!parse_accreg (strp, regno, class))
    return NULL;

  if ((err = parse_dreg (strp, regno)))
    return err;
  if ((class == ACC64) && (*regno & 1))
    return _("Even D register required");

  return NULL;
}

/* FIXME -- move to ubi32-asm.c?
 * FIXME -- pick better name -- parse_addr vs parse_address.
 * FIXME -- check required operand type (constant, symbol).
 */
static const char *
parse_address (char **strp, int *addr)
{
  expressionS exp;
  char *errmsg;
  char *hold = input_line_pointer;

/* FIXME:  Do we need a setjmp here?  */
  input_line_pointer = *strp;
  expression (&exp);

  *strp = input_line_pointer;
  input_line_pointer = hold;

  *addr = 0;
  errmsg = NULL;
  switch (exp.X_op)
    {
      case O_illegal:
	errmsg = _("illegal operand");
	break;
      case O_absent:
	errmsg = _("illegal operand");
	break;
      case O_constant:
      case O_register:
	*addr = exp.X_add_number;
	break;
      default:
	// FIXME:	queue_fixup (exp);
	// FIXME: Are expressions allowed?
        gas_assert (0);
	break;
    }

  return errmsg;
}

/* FIXME -- move to ubi32-asm.c?
 * FIXME -- pick better name -- parse_addr vs parse_address.
 * FIXME -- check required operand type (constant, symbol)
 */
static const char *
parse_address_operand (char **strp, struct operand_t *operand, bfd_reloc_code_real_type reloc)
{
  char *errmsg;
  char *hold = input_line_pointer;

  input_line_pointer = *strp;
  expression (&operand->exp);
  *strp = input_line_pointer;
  input_line_pointer = hold;

  operand->value = 0;
  operand->reloc = 0;
  errmsg = NULL;
  switch (operand->exp.X_op)
    {
      case O_illegal:
	errmsg = _("illegal operand");
	break;
      case O_absent:
	errmsg = _("illegal operand");
	break;
      case O_constant:
      case O_register:
	operand->value = operand->exp.X_add_number;
	break;
      case O_symbol:
	// operand->value = operand->exp.X_add_number;
	operand->reloc = reloc;
	break;
      default:
	errmsg = _("unknown expression type");
	break;
    }

  return errmsg;
}

/* FIXME -- better name? */
struct op_offset_tab_t
{
  const char *operator;
  enum bfd_reloc_code_real reloc;
  enum op_scale_t scale;
  unsigned int shift;
  unsigned int mask;
};

static struct op_offset_tab_t
op_offset_tab[] =
{
  { "%lo(",			BFD_RELOC_LO16, SZ_4, 0, 0xffff },
  { "%hi(",			BFD_RELOC_HI16, SZ_4, 16, 0xffff },
  { "%got_lo(", 		BFD_RELOC_UBI32_GOT_LO, SZ_4, 0, 0xffff },
  { "%got_hi(", 		BFD_RELOC_UBI32_GOT_HI , SZ_4, 16, 0xffff},
  { "%funcdesc_got_lo(",	BFD_RELOC_UBI32_FUNCDESC_GOT_LO , SZ_4, 0, 0xffff},
  { "%funcdesc_got_hi(",	BFD_RELOC_UBI32_FUNCDESC_GOT_HI , SZ_4, 16, 0xffff},
  { "%got_funcdesc_lo(",	BFD_RELOC_UBI32_FUNCDESC_GOT_LO , SZ_4, 0, 0xffff},
  { "%got_funcdesc_hi(",	BFD_RELOC_UBI32_FUNCDESC_GOT_HI , SZ_4, 16, 0xffff},
  { NULL, 0, 0, 0, 0 }
};

static struct op_offset_tab_t
op_offset_imm16[] =
{
  { "%lo(",			BFD_RELOC_LO16, SZ_0, 0, 0xffff },
  { "%hi(",			BFD_RELOC_HI16, SZ_0, 16, 0xffff },
  { "%got_lo(", 		BFD_RELOC_UBI32_GOT_LO, SZ_0, 0, 0xffff },
  { "%got_hi(", 		BFD_RELOC_UBI32_GOT_HI , SZ_0, 16, 0xffff},
  { "%funcdesc_got_lo(",	BFD_RELOC_UBI32_FUNCDESC_GOT_LO , SZ_0, 0, 0xffff},
  { "%funcdesc_got_hi(",	BFD_RELOC_UBI32_FUNCDESC_GOT_HI , SZ_0, 16, 0xffff},
  { "%got_funcdesc_lo(",	BFD_RELOC_UBI32_FUNCDESC_GOT_LO , SZ_0, 0, 0xffff},
  { "%got_funcdesc_hi(",	BFD_RELOC_UBI32_FUNCDESC_GOT_HI , SZ_0, 16, 0xffff},
  { NULL, 0, 0, 0, 0 }
};

static struct op_offset_tab_t
op_offset_imm25[] =
{
  { "%hi(",			BFD_RELOC_UBI32_HI24, SZ_0, 7, 0x1ffffff },
  { "%got_hi(", 		BFD_RELOC_UBI32_GOT_HI24, SZ_0, 7, 0x1ffffff},
  { "%funcdesc_got_hi(",	BFD_RELOC_UBI32_FUNCDESC_GOT_HI24, SZ_0, 7, 0x1ffffff},
  { "%got_funcdesc_hi(",	BFD_RELOC_UBI32_FUNCDESC_GOT_HI24, SZ_0, 7, 0x1ffffff},
  { NULL, 0, 0, 0, 0 }
};

static struct op_offset_tab_t
op_offset_imm7_s[] =
{
  { "%lo(",			BFD_RELOC_UBI32_LO7_S, SZ_1, 0, 0x7f },
  { "%lo(",			BFD_RELOC_UBI32_LO7_2_S, SZ_2, 0, 0x7f },
  { "%lo(",			BFD_RELOC_UBI32_LO7_4_S, SZ_4, 0, 0x7f },
  { "%got_lo(", 		BFD_RELOC_UBI32_GOT_LO7_S, SZ_1, 0, 0x7f},
  { "%got_lo(", 		BFD_RELOC_UBI32_GOT_LO7_2_S, SZ_2, 0, 0x7f},
  { "%got_lo(", 		BFD_RELOC_UBI32_GOT_LO7_4_S, SZ_4, 0, 0x7f},
  { "%funcdesc_got_lo(",	BFD_RELOC_UBI32_FUNCDESC_GOT_LO7_S, SZ_1, 0, 0x7f},
  { "%funcdesc_got_lo(",	BFD_RELOC_UBI32_FUNCDESC_GOT_LO7_2_S, SZ_2, 0, 0x7f},
  { "%funcdesc_got_lo(",	BFD_RELOC_UBI32_FUNCDESC_GOT_LO7_4_S, SZ_4, 0, 0x7f},
  { "%got_funcdesc_lo(",	BFD_RELOC_UBI32_FUNCDESC_GOT_LO7_S, SZ_1, 0, 0x7f},
  { "%got_funcdesc_lo(",	BFD_RELOC_UBI32_FUNCDESC_GOT_LO7_2_S, SZ_2, 0, 0x7f},
  { "%got_funcdesc_lo(",	BFD_RELOC_UBI32_FUNCDESC_GOT_LO7_4_S, SZ_4, 0, 0x7f},
  { NULL, 0, 0, 0, 0 }
};

static struct op_offset_tab_t
op_offset_imm7_d[] =
{
  { "%lo(",			BFD_RELOC_UBI32_LO7_D, SZ_1, 0, 0x7f },
  { "%lo(",			BFD_RELOC_UBI32_LO7_2_D, SZ_2, 0, 0x7f },
  { "%lo(",			BFD_RELOC_UBI32_LO7_4_D, SZ_4, 0, 0x7f },
  { "%got_lo(", 		BFD_RELOC_UBI32_GOT_LO7_D, SZ_1, 0, 0x7f},
  { "%got_lo(", 		BFD_RELOC_UBI32_GOT_LO7_2_D, SZ_2, 0, 0x7f},
  { "%got_lo(", 		BFD_RELOC_UBI32_GOT_LO7_4_D, SZ_4, 0, 0x7f},
  { "%funcdesc_got_lo(",	BFD_RELOC_UBI32_FUNCDESC_GOT_LO7_D, SZ_1, 0, 0x7f},
  { "%funcdesc_got_lo(",	BFD_RELOC_UBI32_FUNCDESC_GOT_LO7_2_D, SZ_2, 0, 0x7f},
  { "%funcdesc_got_lo(",	BFD_RELOC_UBI32_FUNCDESC_GOT_LO7_4_D, SZ_4, 0, 0x7f},
  { "%got_funcdesc_lo(",	BFD_RELOC_UBI32_FUNCDESC_GOT_LO7_D, SZ_1, 0, 0x7f},
  { "%got_funcdesc_lo(",	BFD_RELOC_UBI32_FUNCDESC_GOT_LO7_2_D, SZ_2, 0, 0x7f},
  { "%got_funcdesc_lo(",	BFD_RELOC_UBI32_FUNCDESC_GOT_LO7_4_D, SZ_4, 0, 0x7f},
  { NULL, 0, 0, 0, 0 }
};

/* FIXME -- add size limit.  */
static const char *
parse_immed (char **strp, int *immed, int size ATTRIBUTE_UNUSED)
{
  char *endptr;

  *immed = strtol (*strp, &endptr, 0);
  if (*strp != endptr)
    {
      *strp = endptr;
      return NULL;
    }
  return _("invalid number");
}

/* FIXME -- offset signed?  unsigned?  */
static const char *
parse_offset (char **strp, int *offset, enum op_scale_t scale, int size ATTRIBUTE_UNUSED)
{
  const char *errmsg = NULL;
  int value;
  struct op_offset_tab_t *op = op_offset_tab;

  if (**strp == '%')
    {
      while (op->operator)
        {
	  if (strncasecmp (*strp, op->operator, strlen (op->operator)) == 0
	      && op->scale == scale)
	    {
	      *strp += strlen (op->operator);
	      if (!(errmsg = parse_address (strp, &value))
		 && !(errmsg = parse_literal (strp, ')')))
		{
		  *offset = (value >> op->shift) & op->mask;
		  return NULL;
		}
	      else
		return errmsg;
	    }
	  op++;
        }
      return _("unrecognized %% operator");
    }

  return parse_immed (strp, offset, size);
}

/* FIXME -- offset signed?  unsigned?  */
static const char *
parse_offset_operand (char **strp, struct operand_t *offset, int size ATTRIBUTE_UNUSED,
		      enum op_scale_t scale, struct op_offset_tab_t *op)
{
  const char *errmsg = NULL;

  if (**strp == '%')
    {
      while (op->operator)
        {
	  if (strncasecmp (*strp, op->operator, strlen (op->operator)) == 0
	      && op->scale == scale)
	    {
	      *strp += strlen (op->operator);
	      if (!(errmsg = parse_address_operand (strp, offset, op->reloc))
		 && !(errmsg = parse_literal (strp, ')')))
		{
		  offset->value >>= op->shift;
		  offset->value &= op->mask;
		  return NULL;
		}
	      else
		return errmsg;
	    }
	  op++;
        }
      return _("unrecognized %% operator");
    }

  offset->reloc = 0;
  /* FIXME -- scale value?  */
  return parse_address (strp, &offset->value);
}

struct bitops_t
{
  const char *operator;
  enum bt_mode_t { BT_NONE, BT_BIT, BT_LSB, BT_MSB } mode;
};

static struct bitops_t bitops[] =
{
  { "%bit(",    BT_BIT  },	/* Convert bit patern to bit number.  */
  { "%lsbbit(", BT_LSB  },	/* Find Least Significant Bit set.    */
  { "%msbbit(", BT_MSB  },	/* Find Most Significant Bit set.    */
  { NULL,       BT_NONE }
};

static const char *
parse_bitcnt (char **strp, int *immed)
{
  char *endptr;
  struct bitops_t *op;
  enum bt_mode_t mode = BT_NONE;
  int value;
  int retval = 0;

  for (op = bitops; op->operator; op++)
    {
      if (strncasecmp (*strp, op->operator, strlen (op->operator)) == 0)
	{
	  *strp += strlen (op->operator);
	  mode = op->mode;
	  break;
	}
    }

  value = strtol (*strp, &endptr, 0);
  if (*strp == endptr)
    return _("invalid bit number");

  if (mode != BT_NONE && *endptr++ != ')')
    return _("missing ')'");

  switch (mode)
    {
      case BT_NONE:		/* Bit position */
	retval = value;
	if (value < 0 || value > 32)
	  return _("number not in range 0-31");
	break;

      case BT_BIT:		/* Convert bitmask to bit position */
	if (value == 0)
	  return _("attempt to find bit index of 0");
	/* FALLTHROUGH */
      case BT_MSB:		/* Find Most Significant Bit in pattern. */
	retval = 31;
	while ((value & 0x80000000) == 0)
	  {
	    retval--;
	    value <<= 1;
	  }
	if (mode == BT_BIT && (value & 0x7FFFFFFF) != 0)
	  return _("more than one bit set in bitmask");
	break;

      case BT_LSB:		/* Find Least Significant Bit in pattern. */
	retval = 0;
	while ((value & 0x00000001) == 0)
	  {
	    retval++;
	    value >>= 1;
	  }
	break;
    }

    *strp = endptr;
    *immed = retval;
    return NULL;
}

/* FIXME -- fill in addr struct. */
/* Parse source or destination -- "Addressing modes" in ISA doc.  */
static char *
parse_addr_operand (char **strp, struct operand_t *opnd, enum op_scale_t scale, int pdec_encoding,
		    struct op_offset_tab_t *optab)
{
  char *save_str = *strp;
  int reg, areg, dreg;
  int value;
  int immed;
  struct operand_t temp;

  memset (opnd, 0, sizeof (struct operand_t));

  if (!parse_literal (strp, '#')				/* 000 #<8-bit signed immed>	*/
      && !parse_address (strp, &opnd->value))
    {
      opnd->value &= 0xff;
      return NULL;
    }

  *strp = save_str;
  if (!parse_register (strp, &reg))				/* 001 <reg>			*/
    {
      opnd->value = 0x100;
      insert_bits (&opnd->value, reg, 0, 8);
      return NULL;
    }

  *strp = save_str;
  if (!parse_literal (strp, '(')				/* 1xx (<areg>)			*/
      && !parse_areg (strp, &areg)
      && !parse_literal (strp, ')')
      && (**strp == '\0' || **strp == ','))
    {
      opnd->value = 0x400;
      insert_bits (&opnd->value, areg, 5, 3);
      return NULL;
    }

  *strp = save_str;
  if (!parse_literal (strp, '(')				/* 011 (<areg>,<dreg>)		*/
      && !parse_areg (strp, &areg)
      && !parse_literal (strp, ',')
      && !parse_dreg (strp, &dreg)
      && !parse_literal (strp, ')')
      && (**strp == '\0' || **strp == ','))
    {
      opnd->value = 0x300;
      insert_bits (&opnd->value, areg, 5, 3);
      insert_bits (&opnd->value, dreg, 0, 4);
      return NULL;
    }

  *strp = save_str;
  if (!parse_literal (strp, '(')				/* 010 (<areg>)<ofs>++  M=0	*/
      && !parse_areg (strp, &areg)
      && !parse_literal (strp, ')')
      && !parse_immed (strp, &immed, 4)
      && !parse_literal (strp, '+')
      && !parse_literal (strp, '+')
      && (**strp == '\0' || **strp == ','))
    {
      opnd->value = 0x200;
      immed >>= scale;
      insert_bits (&opnd->value, areg, 5, 3);
      insert_bits (&opnd->value, immed, 0, 4);
      return NULL;
    }

  *strp = save_str;
  /* FIXME -- unsigned value.  */
  if (!parse_offset_operand (strp, &temp, 7, scale, optab)	/* 1xx <ofs>(<areg>)	*/
      && !parse_literal (strp, '(')
      && !parse_areg (strp, &areg)
      && !parse_literal (strp, ')')
      && (**strp == '\0' || **strp == ','))			/* FIXME -- is this valid?  */
    {
      value = temp.value;
      if (pdec_encoding)
	{
	  /* Special encoding for PDEC.  */
	  value = -value;
	}
      value >>= scale;
      opnd->value = 0x400;
      insert_bits (&opnd->value, areg, 5, 3);
      insert_bits (&opnd->value, value & 0x1f, 0, 5);
      insert_bits (&opnd->value, (value >> 5) & 0x3, 8, 2);
      opnd->reloc = temp.reloc;
      opnd->exp = temp.exp;
      return NULL;
    }

  *strp = save_str;
  if (!parse_immed (strp, &value, 4)				/* 010 <ofs>(<areg>)++  M=1	*/
      && !parse_literal (strp, '(')
      && !parse_areg (strp, &areg)
      && !parse_literal (strp, ')')
      && !parse_literal (strp, '+')
      && !parse_literal (strp, '+')
      && (**strp == '\0' || **strp == ','))
    {
      opnd->value = 0x210;
      value >>= scale;
      insert_bits (&opnd->value, areg, 5, 3);
      insert_bits (&opnd->value, value, 0, 4);
      return NULL;
    }

  return _("Invalid address");
}

static const char *
parse_cc (char **strp, int *cc)
{
  struct cc_t *ccop;

  *cc = 0;
  for (ccop = cc_tab; ccop->cond; ccop++)
    {
      if (strncasecmp (ccop->cond, *strp, strlen (ccop->cond)) == 0)
	{
	  *strp += strlen (ccop->cond);
	  *cc = ccop->code;
	  return NULL;
	}
    }
  return NULL;
}

static const char *
parse_sw (char **strp, int *sw)
{
  if ((strncasecmp (*strp, ".s", 2) == 0))
    {
      *strp += 2;
      *sw = 0;
      return NULL;
    }
  if ((strncasecmp (*strp, ".w", 2) == 0))
    {
      *strp += 2;
      *sw = 1;
      return NULL;
    }
  return NULL;
}

static const char *
parse_pred (char **strp, int *pred)
{
  if ((strncasecmp (*strp, ".t", 2) == 0))
    {
      *strp += 2;
      *pred = 1;
      return NULL;
    }
  if ((strncasecmp (*strp, ".f", 2) == 0))
    {
      *strp += 2;
      *pred = 0;
      return NULL;
    }
  return NULL;
}

/* FIXME - Replace 4 with symbolic insn length.  */
static char *
finish_insn (unsigned int value)
{
  char *frag;

#if 0
  printf ("finish_insn (0x%8.8x)\n", value);
#endif
  frag = frag_more (4);
/* FIXME -- is this needed and what does it do?
 * if (result)
 *   result->frag = frag_now;
 */
  /* FIXME -- handle BigEndian/LittleEndian. */
  bfd_put_bits ((bfd_vma) value, frag, 32, 0);

  /* Emit DWARF2 debugging information.  */
  dwarf2_emit_insn (4);
  return frag;
}

static fixS *
add_fixup (char * frag, struct operand_t *operand)
{
  fixS *fixP = NULL;
  int pcrel = (operand->reloc == BFD_RELOC_UBI32_21_PCREL)
	   || (operand->reloc == BFD_RELOC_UBI32_24_PCREL);

  if (operand->reloc != 0)
    fixP = fix_new_exp (frag_now, frag - frag_now->fr_literal, 4, &operand->exp,
			 pcrel, operand->reloc);
  return fixP;
}

static void
put_fmt1a (struct op_table_t *insn)
{
  unsigned int buf = insn->instruction;

  finish_insn (buf);
}

static void
put_fmt1b (struct op_table_t *insn, struct operand_t *sopnd)
{
  int buf = insn->instruction;
  char *frag;

  insert_bits (&buf, sopnd->value, 0, 11);
  if (insn->flags == FLAG_SCSR)
    insert_bits (&buf, 0x100 + REGNO_CSR, 16, 11);
  frag = finish_insn (buf);
  add_fixup (frag, sopnd);
}

static void
put_fmt1c (struct op_table_t *insn, struct operand_t *dopnd)
{
  int buf = insn->instruction;
  char *frag;

  insert_bits (&buf, dopnd->value, 16, 11);
  frag = finish_insn (buf);
  add_fixup (frag, dopnd);
}

static void
put_fmt1d (struct op_table_t *insn, struct operand_t *dopnd, struct operand_t *sopnd)
{
  int buf = insn->instruction;
  char *frag;

  insert_bits (&buf, sopnd->value, 0, 11);
  insert_bits (&buf, dopnd->value, 16, 11);
  frag = finish_insn (buf);
  add_fixup (frag, dopnd);
  add_fixup (frag, sopnd);
}

static void
put_fmt2 (struct op_table_t *insn, struct operand_t *dopnd,
	  struct operand_t *sopnd, int bitno)
{
  int buf = insn->instruction;
  char *frag;

  insert_bits (&buf, dopnd->value, 16, 11);
  insert_bits (&buf, bitno, 11, 5);
  insert_bits (&buf, sopnd->value, 0, 11);
  frag = finish_insn (buf);
  add_fixup (frag, dopnd);
  add_fixup (frag, sopnd);
}

static void
put_fmt3 (struct op_table_t *insn, struct operand_t *dopnd,
	  struct operand_t *sopnd, int sreg)
{
  int buf = insn->instruction;
  char *frag;

  insert_bits (&buf, dopnd->value, 16, 11);
  insert_bits (&buf, sreg, 11, 4);
  insert_bits (&buf, sopnd->value, 0, 11);
  frag = finish_insn (buf);
  add_fixup (frag, dopnd);
  add_fixup (frag, sopnd);
}

static void
put_fmt4 (struct op_table_t *insn, unsigned int dest, int flag, int s_imm,
	  struct operand_t *sopnd)
{
  int buf = insn->instruction;
  char *frag;

  insert_bits (&buf, flag, 26, 1);
  insert_bits (&buf, dest, 16, 4);
  insert_bits (&buf, s_imm, 11, 5);
  insert_bits (&buf, sopnd->value, 0, 11);
  frag = finish_insn (buf);
  add_fixup (frag, sopnd);
}

static void
put_fmt5 (struct op_table_t *insn, struct operand_t *s2opnd,
	  struct operand_t *sopnd)
{
  int buf = insn->instruction;
  char *frag;

  insert_bits (&buf, s2opnd->value, 11, 16);
  insert_bits (&buf, sopnd->value, 0, 11);
  frag = finish_insn (buf);
  add_fixup (frag, s2opnd);
  add_fixup (frag, sopnd);
}

static void
put_fmt6 (struct op_table_t *insn, struct operand_t *dopnd, struct operand_t *sopnd)
{
  int buf = insn->instruction;
  char *frag;

  insert_bits (&buf, sopnd->value, 0, 16);
  insert_bits (&buf, dopnd->value, 16, 11);
  frag = finish_insn (buf);
  add_fixup (frag, dopnd);
  add_fixup (frag, sopnd);
}

static void
put_fmt7 (struct op_table_t *insn, int cc, int sw, int pred,
	  struct operand_t *offset)
{
  int buf = insn->instruction;
  char *frag;

  insert_bits (&buf, cc, 23, 4);
  insert_bits (&buf, pred, 22, 1);
  insert_bits (&buf, sw, 21, 1);
  insert_bits (&buf, offset->value >> 2, 0, 21);
  frag = finish_insn (buf);
  add_fixup (frag, offset);
}

static void
put_fmt8 (struct op_table_t *insn, int areg, struct operand_t *offset)
{
  int buf = insn->instruction;
  char *frag;
  int value = offset->value;

  insert_bits (&buf, areg, 21, 3);
  insert_bits (&buf, value & 0x1fffff, 0, 21);
  insert_bits (&buf, (value >> 21) & 0x7, 24, 3);
  frag = finish_insn (buf);
  add_fixup (frag, offset);
}

static void
put_fmt9 (struct op_table_t *insn, int anreg, int amreg, int offset)
{
  int buf = insn->instruction;

  offset = offset >> insn->scale;

  insert_bits (&buf, anreg, 21, 3);
  insert_bits (&buf, amreg, 5, 3);
  insert_bits (&buf, offset & 0x1f, 0, 5);
  insert_bits (&buf, (offset >> 5) & 0x7, 8, 3);
  insert_bits (&buf, (offset >> 8) & 0x1f, 16, 5);
  insert_bits (&buf, (offset >> 13) & 0x7, 24, 3);
  finish_insn (buf);
}

/* FIXME - handle immediate source. */
static void
put_fmt10 (struct op_table_t *insn, int acc, struct operand_t *sopnd, int s2)
{
  int buf = insn->instruction;
  unsigned int dsp_ctrl = 0;
  char *frag;

  /* Register source.  */
  insert_bits (&buf, 1, 26, 1);
  /* Acc is in fps32 format.  DSP control.  */
  if (acc == 0x12)
    dsp_ctrl |= DSP_CTRL_A;
  insert_bits (&buf, dsp_ctrl, 16, 5);
  insert_bits (&buf, s2, 11, 5);
  insert_bits (&buf, sopnd->value, 0, 11);
  frag = finish_insn (buf);
  add_fixup (frag, sopnd);
}

static void
put_fmt11a (struct op_table_t *insn, int acc, struct operand_t *sopnd, int s2)
{
  int buf = insn->instruction;
  char *frag;

  insert_bits (&buf, acc, 16, 2);
  insert_bits (&buf, s2, 11, 5);
  insert_bits (&buf, sopnd->value, 0, 11);
  frag = finish_insn (buf);
  add_fixup (frag, sopnd);
}

static void
put_fmt11b (struct op_table_t *insn, int acc, struct operand_t *sopnd)
{
  int buf = insn->instruction;
  char *frag;

  insert_bits (&buf, acc >> 1, 17, 1);
  insert_bits (&buf, sopnd->value, 0, 11);
  frag = finish_insn (buf);
  add_fixup (frag, sopnd);
}

static void
put_fmt11c (struct op_table_t *insn, int acc, struct operand_t *sopnd)
{
  int buf = insn->instruction;
  char *frag;

  insert_bits (&buf, acc, 16, 2);
  insert_bits (&buf, sopnd->value, 0, 11);
  frag = finish_insn (buf);
  add_fixup (frag, sopnd);
}

static void
put_fmt11d (struct op_table_t *insn, struct operand_t *sopnd, int s2)
{
  int buf = insn->instruction;
  char *frag;

  insert_bits (&buf, s2, 11, 5);
  insert_bits (&buf, sopnd->value, 0, 11);
  frag = finish_insn (buf);
  add_fixup (frag, sopnd);
}

static void
put_fmt12a (struct op_table_t *insn, int acc, int s1, int s2)
{
  int buf = insn->instruction;

  insert_bits (&buf, (acc >> 1) & 0x1, 17, 1);
  insert_bits (&buf, s2 >> 1, 12, 4);
  insert_bits (&buf, s1 >> 1, 1, 4);
  finish_insn (buf);
}

static void
put_fmt12b (struct op_table_t *insn, int acc, int s1)
{
  int buf = insn->instruction;

  insert_bits (&buf, acc & 0x3, 16, 2);
  insert_bits (&buf, s1 >> 1, 1, 4);
  finish_insn (buf);
}

static void
put_fmt12c (struct op_table_t *insn, int acc, int s1)
{
  int buf = insn->instruction;

  insert_bits (&buf, (acc >> 1) & 0x1, 17, 1);
  insert_bits (&buf, s1 >> 1, 1, 4);
  finish_insn (buf);
}

static void
put_fmt12d (struct op_table_t *insn, int s1, int s2)
{
  int buf = insn->instruction;

  insert_bits (&buf, s2 >> 1, 12, 4);
  insert_bits (&buf, s1 >> 1, 1, 4);
  finish_insn (buf);
}

/* FIXME -- better name */
static void
put_mac (struct op_table_t *insn)
{
  unsigned int buf = insn->instruction;

  finish_insn (buf);
}


void
md_assemble (char *str)
{
  struct op_table_t *insn;
  char *op_start, *op_end, *op;
  int len;
  int immed;
  int an, am, s1, s2, acc, d;
  int cc, sw, pred;
  int offset;
  struct operand_t offset_op;
  struct operand_t dopnd;
  struct operand_t sopnd;
  struct operand_t s2opnd;
  const char *msg = _("unrecognized instruction");

/* FIXME -- not needed? */
  /* Skip leading whitespace.  */
  while (ISSPACE (*str))
    str++;

  /* Find end of op code.  */
  for (op_start = op_end = str;
       *op_end && !is_end_of_line[(unsigned char) *op_end] && *op_end != ' ';
       op_end++)
    ;

  len = op_end - op_start;
  insn = ubi32_lookup_insn (op_start, len, ubi32_version);

  if (!insn)
    {
      as_bad (_("unknown opcode"));
      return;
    }

  /* Skip whitespace.  */
  while (ISSPACE (*op_end))
    op_end++;

  switch (insn->format)
    {
      case FMT_1A:	/* No operands.  */
	    put_fmt1a (insn);
	    msg = NULL;
	break;

      case FMT_1B:	/* One operand - source.  */
	if (!(msg = parse_addr_operand (&op_end, &sopnd, insn->scale, 0,
					op_offset_imm7_s)))
 	  {
	    put_fmt1b (insn, &sopnd);
 	  }
	break;

      case FMT_1C:	/* One operand - destination.  */
	if (!(msg = parse_addr_operand (&op_end, &dopnd, insn->scale, 0,
					op_offset_imm7_d)))
	  {
	    put_fmt1c (insn, &dopnd);
	  }
	break;

      case FMT_1D:	/* Two operands.  */
	if (!(msg = parse_addr_operand (&op_end, &dopnd,
					(insn->flags & FLAG_LEA) ? SZ_4 : insn->scale,
					 0, op_offset_imm7_d))
	    && !(msg = parse_literal (&op_end, ','))
	    && !(msg = parse_addr_operand (&op_end, &sopnd, insn->scale,
					   (insn->flags & FLAG_PDEC) ? 1 : 0,
					   op_offset_imm7_s)))
	  {
	    put_fmt1d (insn, &dopnd, &sopnd);
	  }
	break;

      case FMT_2:
	if (!(msg = parse_addr_operand (&op_end, &dopnd, insn->scale, 0,
					op_offset_imm7_d))
	    && !(msg = parse_literal (&op_end, ','))
	    && !(msg = parse_addr_operand (&op_end, &sopnd, insn->scale, 0,
					   op_offset_imm7_s))
	    && !(msg = parse_literal (&op_end, ','))
	    && !(msg = parse_literal (&op_end, '#'))
	    && !(msg = parse_bitcnt (&op_end, &immed)))
	  {
	    put_fmt2 (insn, &dopnd, &sopnd, immed);
	  }
	break;

      case FMT_3:
	if (!(msg = parse_addr_operand (&op_end, &dopnd, insn->scale, 0,
					op_offset_imm7_d))
	    && !(msg = parse_literal (&op_end, ','))
	    && !(msg = parse_addr_operand (&op_end, &sopnd, insn->scale, 0,
					   op_offset_imm7_s))
	    && !(msg = parse_literal (&op_end, ','))
	    && !(msg = parse_dreg (&op_end, &s2)))
	  {
	    put_fmt3 (insn, &dopnd, &sopnd, s2);
	  }
	break;

      case FMT_4A:
	if (!(msg = parse_dreg (&op_end, &d))
	    && !(msg = parse_literal (&op_end, ','))
	    && !(msg = parse_addr_operand (&op_end, &sopnd, insn->scale, 0,
					   op_offset_imm7_s))
	    && !(msg = parse_literal (&op_end, ',')))
	  {
	    if (!(msg = parse_literal (&op_end, '#'))
		&& !(msg = parse_bitcnt (&op_end, &immed)))
	      {
	        /* immediate value.  */
		put_fmt4 (insn, d, 0, immed, &sopnd);
	      }
	    else if (!(msg = parse_dreg (&op_end, &s2)))
	      {
		/* register value. */
		put_fmt4 (insn, d, 1, s2, &sopnd);
	      }
	  }
	break;

      case FMT_4B:
	if (!(msg = parse_addr_operand (&op_end, &sopnd, insn->scale, 0,
					   op_offset_imm7_s))
	    && !(msg = parse_literal (&op_end, ',')))
	  {
	    if (!(msg = parse_literal (&op_end, '#'))
		&& !(msg = parse_immed (&op_end, &immed, 5)))
	      {
	        /* immediate value.  */
		put_fmt4 (insn, 0, 0, immed, &sopnd);
	      }
	    else if (!(msg = parse_dreg (&op_end, &s2)))
	      {
		/* register value. */
		put_fmt4 (insn, 0, 1, s2, &sopnd);
	      }
	  }
	break;

      case FMT_5:
	if (!(msg = parse_addr_operand (&op_end, &sopnd, insn->scale, 0,
					   op_offset_imm7_s))
	    && !(msg = parse_literal (&op_end, ','))
	    && !(msg = parse_literal (&op_end, '#'))
	    && !(msg = parse_offset_operand (&op_end, &s2opnd, 0, SZ_2, op_offset_imm16)))
	  {
	    put_fmt5 (insn, &s2opnd, &sopnd);
          }
	break;

      case FMT_6:
	if (!(msg = parse_addr_operand (&op_end, &dopnd, insn->scale, 0, op_offset_imm7_d))
	    && !(msg = parse_literal (&op_end, ','))
	    && !(msg = parse_literal (&op_end, '#'))
	    && !(msg = parse_offset_operand (&op_end, &sopnd, 2, SZ_0, op_offset_imm16)))
	  {
	    /* FIXME -- movei scale for dest reg/imm is 4, not 2. */
	    put_fmt6 (insn, &dopnd, &sopnd);
	  }
	break;

      case FMT_7:
	/* JMP - Conditional branch.  */
	op = op_start + 3;
	cc = -1;
	sw = 1;
	pred = 1;
	if (!(msg = parse_cc (&op, &cc)))
	  {
	    msg = parse_sw (&op, &sw);
	    msg = parse_pred (&op, &pred);
	    if ((msg = parse_literal (&op, ' ')) || (cc == -1))
	      {
		msg = _("invalid condition code");
	        break;
	      }
	  }

	if (!(msg = parse_address_operand (&op_end, &offset_op, BFD_RELOC_UBI32_21_PCREL)))
	  {
	    put_fmt7 (insn, cc, sw, pred, &offset_op);
	  }
	break;

      case FMT_8:
	if ((!(msg = parse_areg (&op_end, &an))
	    && !(msg = parse_literal (&op_end, ',')))
	    && (
		(!(msg = parse_literal (&op_end, '#'))	/* Immediate value */
	        && !(msg = parse_offset_operand (&op_end, &offset_op, 24, insn->scale,
					         op_offset_imm25)))
	      ||
		(!(msg = parse_address_operand (&op_end, &offset_op,	/* Address */
						  BFD_RELOC_UBI32_24_PCREL)))))
	  {
	    if (insn->flags & FLAG_MOVEAI)
	      {
		/* If operand has bit 31 set, generate MOVAIH. */
		if (offset_op.value & 0x1000000)
		  insn++;	/* MOVAIH must follow MOVAI.  */
	      }
	    put_fmt8 (insn, an, &offset_op);
	  }
	break;

      case FMT_9:
	if (!(msg = parse_areg (&op_end, &an))
	    && !(msg = parse_literal (&op_end, ','))
	    && !(msg = parse_offset (&op_end, &offset, insn->scale, 16))
	    && !(msg = parse_literal (&op_end, '('))
	    && !(msg = parse_areg (&op_end, &am))
	    && !(msg = parse_literal (&op_end, ')')))
	  {
	    put_fmt9 (insn, an, am, offset);
	  }
	break;

      case FMT_10:
	/* FIXME -- parse immediate src2 value */
	if (!(msg = parse_accreg (&op_end, &acc, ACC64))
	    && !(msg = parse_literal (&op_end, ','))
	    && !(msg = parse_addr_operand (&op_end, &sopnd, insn->scale, 0,
					   op_offset_imm7_s))
	    && !(msg = parse_literal (&op_end, ','))
	    && !(msg = parse_dreg (&op_end, &s2)))
	  {
	    put_fmt10 (insn, acc, &sopnd, s2);
	  }
	break;

      case FMT_11A:
	if (!(msg = parse_accreg (&op_end, &acc, ACC32))
	    && !(msg = parse_literal (&op_end, ','))
	    && !(msg = parse_addr_operand (&op_end, &sopnd, insn->scale, 0,
					   op_offset_imm7_s))
	    && !(msg = parse_literal (&op_end, ','))
	    && !(msg = parse_accdreg (&op_end, &s2, ACC32)))
	  {
	    put_fmt11a (insn, acc, &sopnd, s2);
	  }
	break;

      case FMT_11B:
	if (!(msg = parse_accreg (&op_end, &acc, ACC64))
	    && !(msg = parse_literal (&op_end, ','))
	    && !(msg = parse_addr_operand (&op_end, &sopnd, insn->scale, 0,
					   op_offset_imm7_s)))
	  {
	    put_fmt11b (insn, acc, &sopnd);
	  }
	break;

      case FMT_11C:
	if (!(msg = parse_accreg (&op_end, &acc, ACC32))
	    && !(msg = parse_literal (&op_end, ','))
	    && !(msg = parse_addr_operand (&op_end, &sopnd, insn->scale, 0,
					   op_offset_imm7_s)))
	  {
	    put_fmt11c (insn, acc, &sopnd);
	  }
	break;

      case FMT_11D:
	if (!(msg = parse_addr_operand (&op_end, &sopnd, insn->scale, 0,
					op_offset_imm7_s))
	    && !(msg = parse_literal (&op_end, ','))
	    && !(msg = parse_accdreg (&op_end, &s2, ACC32)))
	  {
	    put_fmt11d (insn, &sopnd, s2);
	  }
	break;

      case FMT_12A:
	if (!(msg = parse_accreg (&op_end, &acc, ACC64))
	    && !(msg = parse_literal (&op_end, ','))
	    && !(msg = parse_accdreg (&op_end, &s1, ACC64))
	    && !(msg = parse_literal (&op_end, ','))
	    && !(msg = parse_accdreg (&op_end, &s2, ACC64)))
	  {
	    put_fmt12a (insn, acc, s1, s2);
	  }
	break;

      case FMT_12B:
	if (!(msg = parse_accreg (&op_end, &acc, ACC32))
	    && !(msg = parse_literal (&op_end, ','))
	    && !(msg = parse_accdreg (&op_end, &s1, ACC64)))
	  {
	    put_fmt12b (insn, acc, s1);
	  }
	break;

      case FMT_12C:
	if (!(msg = parse_accreg (&op_end, &acc, ACC64))
	    && !(msg = parse_literal (&op_end, ','))
	    && !(msg = parse_accdreg (&op_end, &s1, ACC64)))
	  {
	    put_fmt12c (insn, acc, s1);
	  }
	break;

      case FMT_12D:
	if (!(msg = parse_accdreg (&op_end, &s1, ACC64))
	    && !(msg = parse_literal (&op_end, ','))
	    && !(msg = parse_accdreg (&op_end, &s2, ACC64)))
	  {
	    put_fmt12d (insn, s1, s2);
	  }
	break;

      case FMT_MAC:
	if (insn->flags & FLAG_MAC)  /* Output insn only */
	  {
	    put_mac (insn);
	    msg = NULL;
	  }
	break;
    }

  if (msg)
    as_bad ("%s", msg);
}

/* Initialize the DWARF-2 unwind information for this procedure.  */

void
tc_ubi32_frame_initial_instructions (void)
{
  cfi_add_CFA_def_cfa (SP_REGNUM, 0);
}

int
tc_ubi32_regname_to_dw2regnum (char *regname)
{
  struct symbol *sym;
  const char **sym_val;

  sym = hash_find (gdb_reg_hash, regname);
  if (sym)
    {
      sym_val = (const char **) S_GET_VALUE (sym);
      return (sym_val - ubi32_gdb_register_names);
    }

  return -1;
}

/* FIXME -- handle macros. */
/* FIXME -- allow #0 as destination. */
/* FIXME -- bit mask value checks.  */
/* FIXME -- handle %f. -- what is this? */
/* FIXME -- create typedef for address size? */
/* FIXME -- What is "%lo18()?  */
