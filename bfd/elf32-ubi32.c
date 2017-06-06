/* Qualcomm Ubi32 specific support for 32-bit ELF
   Copyright (C) 2000-2015 Free Software Foundation, Inc.

   This file is part of BFD, the Binary File Descriptor library.

   This program is free software; you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the Free Software Foundation; either version 3 of the License, or
   (at your option) any later version.

   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.

   You should have received a copy of the GNU General Public License
   along with this program; if not, write to the Free Software
   Foundation, Inc., 51 Franklin Street - Fifth Floor, Boston,
   MA 02110-1301, USA.  */

#include "sysdep.h"
#include "bfd.h"
#include "libbfd.h"
#include "elf-bfd.h"
#include <assert.h>

static reloc_howto_type *
ubi32_elf_reloc_type_lookup (bfd * abfd ATTRIBUTE_UNUSED,
			     bfd_reloc_code_real_type code ATTRIBUTE_UNUSED)
{
  bfd_assert (__FILE__, __LINE__);
  return NULL;
}

static bfd_boolean
ubi32_elf_object_p (bfd *abfd)
{
  flagword  mach = elf_elfheader (abfd)->e_flags & 0xffff;
  bfd_default_set_arch_mach (abfd, bfd_arch_ubi32, mach);
  return TRUE;
}


static reloc_howto_type *
ubi32_elf_reloc_name_lookup (bfd *abfd ATTRIBUTE_UNUSED,
			     const char *r_name ATTRIBUTE_UNUSED)
{
  bfd_assert (__FILE__, __LINE__);
  return NULL;
}

#define TARGET_BIG_SYM	 ubi32_elf32_le_vec
#define TARGET_BIG_NAME  "elf32-bigubi32"

#define TARGET_LITTLE_SYM  ubi32_elf32_be_vec
#define TARGET_LITTLE_NAME  "elf32-littleubi32"

#define ELF_ARCH	 bfd_arch_ubi32
#define ELF_MACHINE_CODE EM_UBI32
#define ELF_MAXPAGESIZE  0x4000

#define bfd_elf32_bfd_reloc_type_lookup		ubi32_elf_reloc_type_lookup
#define bfd_elf32_bfd_reloc_name_lookup		ubi32_elf_reloc_name_lookup

#define elf_backend_object_p		        ubi32_elf_object_p

#include "elf32-target.h"
