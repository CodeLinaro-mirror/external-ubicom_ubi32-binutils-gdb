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

const bfd_arch_info_type bfd_ubi32v6_arch =
{
  32,				/* bits per word */
  32,				/* bits per address */
  8,				/* bits per byte */
  bfd_arch_ubi32,		/* architecture */
  bfd_mach_ubi32ver6,		/* machine */
  "ubi32",			/* architecture name */
  "ubi32v6",			/* printable name */
  3,				/* section align power */
  TRUE,				/* the default ? */
  bfd_default_compatible,	/* architecture comparison fn */
  bfd_default_scan,		/* string to architecture convert fn */
  bfd_arch_default_fill,	/* Default fill.  */
  NULL				/* next in list */
};

const bfd_arch_info_type bfd_ubi32v61_arch =
{
  32,				/* bits per word */
  32,				/* bits per address */
  8,				/* bits per byte */
  bfd_arch_ubi32,		/* architecture */
  bfd_mach_ubi32ver61,		/* machine */
  "ubi32",			/* architecture name */
  "ubi32ver61",			/* printable name */
  3,				/* section align power */
  TRUE,				/* the default ? */
  bfd_default_compatible,	/* architecture comparison fn */
  bfd_default_scan,		/* string to architecture convert fn */
  bfd_arch_default_fill,	/* Default fill.  */
  &bfd_ubi32v6_arch		/* next in list */
};

const bfd_arch_info_type bfd_ubi32_arch =
{
  32,				/* bits per word */
  32,				/* bits per address */
  8,				/* bits per byte */
  bfd_arch_ubi32,		/* architecture */
  bfd_mach_ubi32,		/* machine */
  "ubi32",			/* architecture name */
  "ubi32",			/* printable name */
  3,				/* section align power */
  TRUE,				/* the default ? */
  bfd_default_compatible,	/* architecture comparison fn */
  bfd_default_scan,		/* string to architecture convert fn */
  bfd_arch_default_fill,	/* Default fill.  */
  &bfd_ubi32v61_arch		/* next in list */
};
