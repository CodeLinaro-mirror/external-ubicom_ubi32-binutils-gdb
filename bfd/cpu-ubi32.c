/* BFD support for the Ubi32 processor.
   Copyright (C) 2000-2016 Free Software Foundation, Inc.

This file is part of BFD, the Binary File Descriptor library.

This program is free software; you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation; either version 2 of the License, or
(at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program; if not, write to the Free Software
Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA 02111-1307, USA.  */

#include "sysdep.h"
#include "bfd.h"
#include "libbfd.h"

#define UBI32_ARCH(mach, arch_name, mach_name, default, next) \
{									\
  32,				/* bits per word.  */			\
  32,				/* bits per address.  */		\
  8,				/* bits per byte.  */			\
  bfd_arch_ubi32,		/* architecture.  */			\
  mach,				/* machine */				\
  arch_name,			/* architecture name.  */		\
  mach_name,			/* printable mach name.  */		\
  3,				/* section align power.  */		\
  default,			/* the default ?  */			\
  bfd_default_compatible,	/* architecture comparison fn.  */	\
  bfd_default_scan,		/* string to architecture cvt fn.  */	\
  bfd_arch_default_fill,	/* Default fill.  */			\
  next				/* next in list . */			\
}

#ifdef BFD_UBI32_OLD_NAME
static const bfd_arch_info_type ubi32_arch_list[] =
{
  UBI32_ARCH (bfd_mach_ubi32v6, "ubicom32", "ubicom32ver6", FALSE, &ubi32_arch_list[1]),
  UBI32_ARCH (bfd_mach_ubi32v5, "ubicom32", "ubicom32ver5", FALSE, &ubi32_arch_list[2]),
  UBI32_ARCH (bfd_mach_ubi32v4, "ubicom32", "ubicom32ver4", FALSE, &ubi32_arch_list[3]),
  UBI32_ARCH (bfd_mach_ubi32v3, "ubicom32", "ubicom32ver3", FALSE, &ubi32_arch_list[4]),
  UBI32_ARCH (bfd_mach_ubi32,   "ubicom32", "ubicom32",     FALSE, NULL)
};

const bfd_arch_info_type bfd_ubi32_arch =
  UBI32_ARCH (bfd_mach_ubi32v61, "ubi32", "ubicom32ver61", TRUE, &ubi32_arch_list[0]);
#else
static const bfd_arch_info_type ubi32_arch_list[] =
{
  UBI32_ARCH (bfd_mach_ubi32v6, "ubi32", "ubi32v6", FALSE, &ubi32_arch_list[1]),
  UBI32_ARCH (bfd_mach_ubi32v5, "ubi32", "ubi32v5", FALSE, &ubi32_arch_list[2]),
  UBI32_ARCH (bfd_mach_ubi32v4, "ubi32", "ubi32v4", FALSE, &ubi32_arch_list[3]),
  UBI32_ARCH (bfd_mach_ubi32v3, "ubi32", "ubi32v3", FALSE, &ubi32_arch_list[4]),
  UBI32_ARCH (bfd_mach_ubi32,   "ubi32", "ubi32",     FALSE, NULL)
};

const bfd_arch_info_type bfd_ubi32_arch =
  UBI32_ARCH (bfd_mach_ubi32v61, "ubi32", "ubi32v61", TRUE, &ubi32_arch_list[0]);
#endif
