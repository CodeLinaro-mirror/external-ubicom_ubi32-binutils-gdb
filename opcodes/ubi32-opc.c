/* Qualcomm Ubi32 opcode list
   Copyright (C) Eager Consulting.
   Written by Michael J. Eager <eager@eagercon.com>.

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
   along with this program; see the file COPYING3. If not,
   see <http://www.gnu.org/licenses/>.  */

#include "opcode/ubi32.h"

/* Ubi32 Instructions.  Must be sorted by mnemonic.  */
struct op_table_t op_table[] =
{
  /* mnemonic   format   op sz instr bits  instr mask  flags	  version  */
  {"add.1",	FMT_3  , SZ_1, 0x70008000, 0xf8000000, FLAG_NONE, UBI32_V4|UBI32_V5|UBI32_V6|UBI32_V61},
  {"add.2",	FMT_3  , SZ_2, 0x70000000, 0xf8000000, FLAG_NONE, UBI32_ALL},
  {"add.4",	FMT_3  , SZ_4, 0x78000000, 0xf8000000, FLAG_NONE, UBI32_ALL},
  {"addc",	FMT_3  , SZ_4, 0x80000000, 0xf8000000, FLAG_NONE, UBI32_ALL},
  {"and.1",	FMT_3  , SZ_1, 0x40008000, 0xf8000000, FLAG_NONE, UBI32_V4|UBI32_V5|UBI32_V6|UBI32_V61},
  {"and.2",	FMT_3  , SZ_2, 0x40000000, 0xf8000000, FLAG_NONE, UBI32_ALL},
  {"and.4",	FMT_3  , SZ_4, 0x48000000, 0xf8000000, FLAG_NONE, UBI32_ALL},
  {"asr.1",	FMT_4A , SZ_1, 0x12808000, 0xfbe00000, FLAG_NONE, UBI32_V4|UBI32_V5|UBI32_V6|UBI32_V61},
  {"asr.2",	FMT_4A , SZ_2, 0x12a00000, 0xfbe00000, FLAG_NONE, UBI32_ALL},
  {"asr.4",	FMT_4A , SZ_4, 0x12800000, 0xfbe00000, FLAG_NONE, UBI32_ALL},
  {"bclr",	FMT_2  , SZ_4, 0x28000000, 0xf8000000, FLAG_NONE, UBI32_ALL},
  {"bfextu",	FMT_4A , SZ_4, 0x12c00000, 0xfbe00000, FLAG_NONE, UBI32_ALL},
  {"bfrvrs",	FMT_4A , SZ_4, 0x13000000, 0xfbe00000, FLAG_NONE, UBI32_ALL},
  {"bkpt",	FMT_1B , SZ_0, 0x00003800, 0xf800f800, FLAG_NONE, UBI32_ALL},
  {"bset",	FMT_2  , SZ_4, 0x20000000, 0xf8000000, FLAG_NONE, UBI32_ALL},
  {"btst",	FMT_4B , SZ_4, 0x10c00000, 0xfbe00000, FLAG_NONE, UBI32_ALL},
  {"call",	FMT_8  , SZ_0, 0xd8000000, 0xf8000000, FLAG_NONE, UBI32_ALL},
  {"calli",	FMT_9  , SZ_0, 0xf0000000, 0xf800f800, FLAG_NONE, UBI32_ALL},
  {"calli.b",	FMT_9  , SZ_0, 0xf0000000, 0xf800f800, FLAG_NONE, UBI32_ALL},
  {"calli.b",	FMT_9  , SZ_0, 0xf0001000, 0xf800f800, FLAG_NONE, UBI32_V5|UBI32_V6|UBI32_V61},
  {"cmpi",	FMT_5  , SZ_2, 0xc0000000, 0xf8000000, FLAG_NONE, UBI32_ALL},
  {"crcgen",	FMT_4A , SZ_1, 0x11000000, 0xfbe00000, FLAG_NONE, UBI32_ALL},
  {"ext.1",	FMT_1D , SZ_1, 0x0000b800, 0xf800f800, FLAG_NONE, UBI32_ALL},
  {"ext.2",	FMT_1D , SZ_2, 0x0000a800, 0xf800f800, FLAG_NONE, UBI32_ALL},
  {"fabss",	FMT_11A, SZ_0, 0x19200000, 0xfbe00000, FLAG_NONE, UBI32_V5},
  {"fabsd",	FMT_12C, SZ_0, 0x1b200000, 0xfbe00000, FLAG_NONE, UBI32_V5},
  {"faddd",	FMT_12A, SZ_0, 0x1a000000, 0xfbe00000, FLAG_NONE, UBI32_V5},
  {"fadds",	FMT_11A, SZ_0, 0x18000000, 0xfbe00000, FLAG_NONE, UBI32_V4|UBI32_V5},
  {"fcmpd",	FMT_12D, SZ_0, 0x1b800000, 0xfbe00000, FLAG_NONE, UBI32_V5},
  {"fcmps",	FMT_11D, SZ_0, 0x19800000, 0xfbe00000, FLAG_NONE, UBI32_V5},
  {"fd2i",	FMT_12B, SZ_0, 0x1ac00000, 0xfbe00000, FLAG_NONE, UBI32_V5},
  {"fd2l",	FMT_12C, SZ_0, 0x1b600000, 0xfbe00000, FLAG_NONE, UBI32_V5},
  {"fd2s",	FMT_12B, SZ_0, 0x1aa00000, 0xfbe00000, FLAG_NONE, UBI32_V5},
  {"fdivd",	FMT_12A, SZ_0, 0x1a600000, 0xfbe00000, FLAG_NONE, UBI32_V5},
  {"fdivs",	FMT_11A, SZ_4, 0x18600000, 0xfbe00000, FLAG_NONE, UBI32_V5},
  {"fi2d",	FMT_11B, SZ_4, 0x18800000, 0xfbe00000, FLAG_NONE, UBI32_V5},
  {"fi2s",	FMT_11C, SZ_4, 0x19400000, 0xfbe00000, FLAG_NONE, UBI32_V5},
  {"fl2d",	FMT_12C, SZ_0, 0x1b400000, 0xfbe00000, FLAG_NONE, UBI32_V5},
  {"fl2s",	FMT_12B, SZ_0, 0x1a800000, 0xfbe00000, FLAG_NONE, UBI32_V5},
  {"flush",	FMT_1C , SZ_4, 0x00001000, 0xf800f800, FLAG_NONE, UBI32_V5|UBI32_V6|UBI32_V61},
  {"flushni",	FMT_1C , SZ_4, 0x00008800, 0xf800f800, FLAG_NONE, UBI32_V5|UBI32_V6|UBI32_V61},
  {"fmuld",	FMT_12A, SZ_0, 0x1a400000, 0xfbe00000, FLAG_NONE, UBI32_V5},
  {"fmuls",	FMT_11A, SZ_4, 0x18400000, 0xfbe00000, FLAG_NONE, UBI32_V5},
  {"fnegd",	FMT_12C, SZ_0, 0x1b000000, 0xfbe00000, FLAG_NONE, UBI32_V5},
  {"fnegs",	FMT_11C, SZ_4, 0x19000000, 0xfbe00000, FLAG_NONE, UBI32_V5},
  {"fs2d",	FMT_11B, SZ_4, 0x18a00000, 0xfbe00000, FLAG_NONE, UBI32_V5},
  {"fs2i",	FMT_11C, SZ_4, 0x19600000, 0xfbe00000, FLAG_NONE, UBI32_V5},
  {"fs2l",	FMT_11B, SZ_4, 0x18c00000, 0xfbe00000, FLAG_NONE, UBI32_V5},
  {"fsqrtd",	FMT_12C, SZ_0, 0x1ae00000, 0xfbe00000, FLAG_NONE, UBI32_V5},
  {"fsqrts",	FMT_11C, SZ_4, 0x18e00000, 0xfbe00000, FLAG_NONE, UBI32_V5},
  {"fsubd",	FMT_12A, SZ_0, 0x1a200000, 0xfbe00000, FLAG_NONE, UBI32_V5},
  {"fsubs",	FMT_11A, SZ_4, 0x18200000, 0xfbe00000, FLAG_NONE, UBI32_V5},
  {"inval",	FMT_1C , SZ_4, 0x0000b000, 0xf800f800, FLAG_NONE, UBI32_V5|UBI32_V6|UBI32_V61},
  {"iread",	FMT_1B , SZ_4, 0x00003000, 0xf800f800, FLAG_NONE, UBI32_V2},
  {"iwrite",	FMT_1D , SZ_4, 0x00008000, 0xf800f800, FLAG_NONE, UBI32_V2},
  {"jmp",	FMT_7  , SZ_4, 0xd0000000, 0xf8000000, FLAG_EXT,  UBI32_ALL},
  {"lea.1",	FMT_1D , SZ_1, 0x0000f800, 0xf800f800, FLAG_LEA,  UBI32_ALL},
  {"lea.2",	FMT_1D , SZ_2, 0x0000e800, 0xf800f800, FLAG_LEA,  UBI32_ALL},
  {"lea.4",	FMT_1D , SZ_4, 0x0000e000, 0xf800f800, FLAG_LEA,  UBI32_ALL},
  {"leai",	FMT_9  , SZ_4, 0xf0000800, 0xf800f800, FLAG_NONE, UBI32_V1|UBI32_V5|UBI32_V6|UBI32_V61},
  {"lsl.1",	FMT_4A , SZ_1, 0x11400000, 0xfbe00000, FLAG_NONE, UBI32_V4|UBI32_V5|UBI32_V6|UBI32_V61},
  {"lsl.2",	FMT_4A , SZ_2, 0x12200000, 0xfbe00000, FLAG_NONE, UBI32_ALL},
  {"lsl.4",	FMT_4A , SZ_4, 0x12000000, 0xfbe00000, FLAG_NONE, UBI32_ALL},
  {"lsr.1",	FMT_4A , SZ_1, 0x11600000, 0xfbe00000, FLAG_NONE, UBI32_V4|UBI32_V5|UBI32_V6|UBI32_V61},
  {"lsr.2",	FMT_4A , SZ_2, 0x12600000, 0xfbe00000, FLAG_NONE, UBI32_ALL},
  {"lsr.4",	FMT_4A , SZ_4, 0x12400000, 0xfbe00000, FLAG_NONE, UBI32_ALL},
  {"mac",	FMT_4A , SZ_2, 0x11200000, 0xfbe00000, FLAG_NONE, UBI32_V2},
  {"macf",	FMT_10 , SZ_4, 0x30a00000, 0xfbfa0000, FLAG_NONE, UBI32_V3|UBI32_V4|UBI32_V5},
  {"macf.c",	FMT_10 , SZ_4, 0x30b00000, 0xfbfa0000, FLAG_NONE, UBI32_V3|UBI32_V4|UBI32_V5},
  {"macf.c.t",	FMT_10 , SZ_4, 0x30b80000, 0xfbfa0000, FLAG_NONE, UBI32_V3|UBI32_V4|UBI32_V5},
  {"macf.t",	FMT_10 , SZ_4, 0x30a80000, 0xfbfa0000, FLAG_NONE, UBI32_V3|UBI32_V4|UBI32_V5},
  {"macs",	FMT_10 , SZ_2, 0x30200000, 0xfbfa0000, FLAG_NONE, UBI32_V3|UBI32_V4|UBI32_V5},
  {"macs.4",	FMT_10 , SZ_4, 0x30c00000, 0xfbfa0000, FLAG_NONE, UBI32_V5},
  {"macs.c",	FMT_10 , SZ_2, 0x30300000, 0xfbfa0000, FLAG_NONE, UBI32_V3|UBI32_V4|UBI32_V5},
  {"macs.c.t",	FMT_10 , SZ_2, 0x30380000, 0xfbfa0000, FLAG_NONE, UBI32_V3|UBI32_V4|UBI32_V5},
  {"macs.t",	FMT_10 , SZ_2, 0x30280000, 0xfbfa0000, FLAG_NONE, UBI32_V3|UBI32_V4|UBI32_V5},
  {"macu",	FMT_10 , SZ_2, 0x30600000, 0xfbfa0000, FLAG_NONE, UBI32_V3|UBI32_V4|UBI32_V5},
  {"macu.4",	FMT_10 , SZ_4, 0x31600000, 0xfbfa0000, FLAG_NONE, UBI32_V3|UBI32_V4|UBI32_V5},
  {"macu.c",	FMT_10 , SZ_2, 0x30700000, 0xfbfa0000, FLAG_NONE, UBI32_V3|UBI32_V4|UBI32_V5},
  {"macu.c.t",	FMT_10 , SZ_2, 0x30780000, 0xfbfa0000, FLAG_NONE, UBI32_V3|UBI32_V4|UBI32_V5},
  {"macu.t",	FMT_10 , SZ_2, 0x30680000, 0xfbfa0000, FLAG_NONE, UBI32_V3|UBI32_V4|UBI32_V5},
  {"macus",	FMT_10 , SZ_2, 0x30e00000, 0xfbfa0000, FLAG_NONE, UBI32_V4|UBI32_V5|UBI32_V6|UBI32_V61},
  {"macus.c",	FMT_10 , SZ_2, 0x30f00000, 0xfbfa0000, FLAG_NONE, UBI32_V4|UBI32_V5|UBI32_V6|UBI32_V61},
  {"macus.c.t",	FMT_10 , SZ_2, 0x30f80000, 0xfbfa0000, FLAG_NONE, UBI32_V4|UBI32_V5|UBI32_V6|UBI32_V61},
  {"macus.t",	FMT_10 , SZ_2, 0x30e80000, 0xfbfa0000, FLAG_NONE, UBI32_V4|UBI32_V5|UBI32_V6|UBI32_V61},
  {"madd.2",	FMT_10 , SZ_2, 0x32200000, 0xfbfa0000, FLAG_NONE, UBI32_V3|UBI32_V4|UBI32_V5},
  {"madd.2.c",	FMT_10 , SZ_2, 0x32300000, 0xfbfa0000, FLAG_NONE, UBI32_V3|UBI32_V4|UBI32_V5},
  {"madd.2.c.t",FMT_10 , SZ_2, 0x32380000, 0xfbfa0000, FLAG_NONE, UBI32_V3|UBI32_V4|UBI32_V5},
  {"madd.2.t",	FMT_10 , SZ_2, 0x32280000, 0xfbfa0000, FLAG_NONE, UBI32_V3|UBI32_V4|UBI32_V5},
  {"madd.4",	FMT_10 , SZ_4, 0x32000000, 0xfbfa0000, FLAG_NONE, UBI32_V3|UBI32_V4|UBI32_V5},
  {"madd.4.c",	FMT_10 , SZ_4, 0x32100000, 0xfbfa0000, FLAG_NONE, UBI32_V3|UBI32_V4|UBI32_V5},
  {"merge",	FMT_4A , SZ_4, 0x13800000, 0xfbe00000, FLAG_NONE, UBI32_ALL},
  {"move.1",	FMT_1D , SZ_1, 0x00007800, 0xf800f800, FLAG_NONE, UBI32_ALL},
  {"move.2",	FMT_1D , SZ_2, 0x00006800, 0xf800f800, FLAG_NONE, UBI32_ALL},
  {"move.4",	FMT_1D , SZ_4, 0x00006000, 0xf800f800, FLAG_NONE, UBI32_ALL},
  {"movea",	FMT_1D , SZ_4, 0x00007000, 0xf800f800, FLAG_NONE, UBI32_V3|UBI32_V4|UBI32_V5|UBI32_V6|UBI32_V61},
  {"moveai",	FMT_8  , SZ_0, 0xe0000000, 0xf8000000, FLAG_NONE, UBI32_V1|UBI32_V2|UBI32_V3|UBI32_V4},
  {"moveai",	FMT_8  , SZ_0, 0xe0000000, 0xf8000000, FLAG_MOVEAI, UBI32_V5|UBI32_V6|UBI32_V61},
  {"moveaih",	FMT_8  , SZ_0, 0xe8000000, 0xf8000000, FLAG_NONE, 0},  /* Must follow moveai. */
  {"movei",	FMT_6  , SZ_2, 0xc8000000, 0xf8000000, FLAG_NONE, UBI32_ALL},
  {"moveio.1",	FMT_1D , SZ_1, 0x08001000, 0xf800f800, FLAG_NONE, UBI32_V6|UBI32_V61},
  {"moveio.2",	FMT_1D , SZ_2, 0x08000800, 0xf800f800, FLAG_NONE, UBI32_V6|UBI32_V61},
  {"moveio.4",	FMT_1D , SZ_4, 0x08000000, 0xf800f800, FLAG_NONE, UBI32_V6|UBI32_V61},
  {"moveiop.1",	FMT_1D , SZ_1, 0x08002800, 0xf800f800, FLAG_NONE, UBI32_V61},
  {"moveiop.2",	FMT_1D , SZ_2, 0x08002000, 0xf800f800, FLAG_NONE, UBI32_V61},
  {"moveiop.4",	FMT_1D , SZ_4, 0x08001800, 0xf800f800, FLAG_NONE, UBI32_V61},
  {"msub.2",	FMT_10 , SZ_2, 0x32600000, 0xfbfa0000, FLAG_NONE, UBI32_V3|UBI32_V4|UBI32_V5},
  {"msub.2.c",	FMT_10 , SZ_2, 0x32700000, 0xfbfa0000, FLAG_NONE, UBI32_V3|UBI32_V4|UBI32_V5},
  {"msub.2.c.t",FMT_10 , SZ_2, 0x32780000, 0xfbfa0000, FLAG_NONE, UBI32_V3|UBI32_V4|UBI32_V5},
  {"msub.2.t",	FMT_10 , SZ_2, 0x32680000, 0xfbfa0000, FLAG_NONE, UBI32_V3|UBI32_V4|UBI32_V5},
  {"msub.4",	FMT_10 , SZ_4, 0x32400000, 0xfbfa0000, FLAG_NONE, UBI32_V3|UBI32_V4|UBI32_V5},
  {"msub.4.c",	FMT_10 , SZ_4, 0x32500000, 0xfbfa0000, FLAG_NONE, UBI32_V3|UBI32_V4|UBI32_V5},
  {"msuf",	FMT_10 , SZ_2, 0x31200000, 0xfbfa0000, FLAG_NONE, UBI32_V3|UBI32_V4|UBI32_V5},
  {"msuf.c",	FMT_10 , SZ_2, 0x31300000, 0xfbfa0000, FLAG_NONE, UBI32_V3|UBI32_V4|UBI32_V5},
  {"msuf.c.t",	FMT_10 , SZ_2, 0x31380000, 0xfbfa0000, FLAG_NONE, UBI32_V3|UBI32_V4|UBI32_V5},
  {"msuf.t",	FMT_10 , SZ_2, 0x31280000, 0xfbfa0000, FLAG_NONE, UBI32_V3|UBI32_V4|UBI32_V5},
  {"mulf",	FMT_4A , SZ_2, 0x10a00000, 0xfbe00000, FLAG_NONE, UBI32_V2},
  {"mulf",	FMT_10 , SZ_2, 0x30800000, 0xfbfa0000, FLAG_NONE, UBI32_V3|UBI32_V4|UBI32_V5},
  {"mulf.c",	FMT_10 , SZ_2, 0x30900000, 0xfbfa0000, FLAG_NONE, UBI32_V3|UBI32_V4|UBI32_V5},
  {"mulf.c.t",	FMT_10 , SZ_2, 0x30980000, 0xfbfa0000, FLAG_NONE, UBI32_V3|UBI32_V4|UBI32_V5},
  {"mulf.t",	FMT_10 , SZ_2, 0x30880000, 0xfbfa0000, FLAG_NONE, UBI32_V3|UBI32_V4|UBI32_V5},
  {"muls",	FMT_4A , SZ_2, 0x10200000, 0xfbe00000, FLAG_NONE, UBI32_V2},
  {"muls",	FMT_10 , SZ_2, 0x30000000, 0xfbfa0000, FLAG_NONE, UBI32_V3|UBI32_V4|UBI32_V5},
  {"muls.4",	FMT_10 , SZ_4, 0x31000000, 0xfbfa0000, FLAG_NONE, UBI32_V4|UBI32_V5|UBI32_V6|UBI32_V61},
  {"muls.t",	FMT_10 , SZ_2, 0x30080000, 0xfbfa0000, FLAG_NONE, UBI32_V3|UBI32_V4|UBI32_V5},
  {"mulu",	FMT_4A , SZ_2, 0x10600000, 0xfbe00000, FLAG_NONE, UBI32_V2},
  {"mulu",	FMT_10 , SZ_2, 0x30400000, 0xfbfa0000, FLAG_NONE, UBI32_V3|UBI32_V4|UBI32_V5},
  {"mulu.4",	FMT_10 , SZ_4, 0x31400000, 0xfbfa0000, FLAG_NONE, UBI32_V4|UBI32_V5|UBI32_V6|UBI32_V61},
  {"mulu.t",	FMT_10 , SZ_2, 0x30480000, 0xfbfa0000, FLAG_NONE, UBI32_V3|UBI32_V4|UBI32_V5},
  {"nop",	FMT_MAC, SZ_0, 0xc8000000, 0xffffffff, FLAG_MAC,  UBI32_ALL},
  {"not.2",	FMT_1D , SZ_2, 0x00005800, 0xf800f800, FLAG_NONE, UBI32_ALL},
  {"not.4",	FMT_1D , SZ_4, 0x00005000, 0xf800f800, FLAG_NONE, UBI32_ALL},
  {"or.1",	FMT_3  , SZ_1, 0x50008000, 0xf8000000, FLAG_NONE, UBI32_V4|UBI32_V5|UBI32_V6|UBI32_V61},
  {"or.2",	FMT_3  , SZ_2, 0x50000000, 0xf8000000, FLAG_NONE, UBI32_ALL},
  {"or.4",	FMT_3  , SZ_4, 0x58000000, 0xf8000000, FLAG_NONE, UBI32_ALL},
  {"pdec",	FMT_1D , SZ_4, 0x0000f000, 0xf800f800, FLAG_PDEC, UBI32_ALL},
  {"prefetch",	FMT_1C , SZ_4, 0x00002800, 0xf800f800, FLAG_NONE, UBI32_V5|UBI32_V6|UBI32_V61},
  {"pxadds",	FMT_3  , SZ_2, 0xb0000000, 0xf8000000, FLAG_NONE, UBI32_V4|UBI32_V5|UBI32_V6|UBI32_V61},
  {"pxadds.u",	FMT_3  , SZ_2, 0xb0000000, 0xf8000000, FLAG_NONE, UBI32_V4|UBI32_V5|UBI32_V6|UBI32_V61},
  {"pxblend",	FMT_3  , SZ_4, 0xa0000000, 0xf8000000, FLAG_NONE, UBI32_V4|UBI32_V5|UBI32_V6|UBI32_V61},
  {"pxblend.t",	FMT_3  , SZ_4, 0xa0000000, 0xf8000000, FLAG_NONE, UBI32_V4|UBI32_V5|UBI32_V6|UBI32_V61},
  {"pxcnv",	FMT_1D , SZ_2, 0x0000d000, 0xf800f800, FLAG_NONE, UBI32_V4|UBI32_V5|UBI32_V6|UBI32_V61},
  {"pxcnv.t",	FMT_1D , SZ_2, 0x0000d800, 0xf800f800, FLAG_NONE, UBI32_V4|UBI32_V5|UBI32_V6|UBI32_V61},
  {"pxhi",	FMT_4A , SZ_4, 0x10000000, 0xfbe00000, FLAG_NONE, UBI32_V4|UBI32_V5|UBI32_V6|UBI32_V61},
  {"pxhi.s",	FMT_4A , SZ_4, 0x10400000, 0xfbe00000, FLAG_NONE, UBI32_V4|UBI32_V5|UBI32_V6|UBI32_V61},
  {"pxvi",	FMT_3  , SZ_4, 0xa8000000, 0xf8000000, FLAG_NONE, UBI32_V4|UBI32_V5|UBI32_V6|UBI32_V61},
  {"pxvi.s",	FMT_3  , SZ_4, 0xa8000000, 0xf8000000, FLAG_NONE, UBI32_V4|UBI32_V5|UBI32_V6|UBI32_V61},
  {"ret",	FMT_1B , SZ_4, 0x00002000, 0xf800f800, FLAG_NONE, UBI32_ALL},
  {"setcsr",	FMT_1B , SZ_4, 0x00009000, 0xf800f800, FLAG_SCSR, UBI32_ALL},
  {"shftd",	FMT_4A , SZ_4, 0x13600000, 0xfbe00000, FLAG_NONE, UBI32_ALL},
  {"shmrg.1",	FMT_4A , SZ_1, 0x13e00000, 0xfbe00000, FLAG_NONE, UBI32_ALL},
  {"shmrg.2",	FMT_4A , SZ_2, 0x13c00000, 0xfbe00000, FLAG_NONE, UBI32_ALL},
  {"sub.1",	FMT_3  , SZ_1, 0x88008000, 0xf8000000, FLAG_NONE, UBI32_V4|UBI32_V5|UBI32_V6|UBI32_V61},
  {"sub.2",	FMT_3  , SZ_2, 0x88000000, 0xf8000000, FLAG_NONE, UBI32_ALL},
  {"sub.4",	FMT_3  , SZ_4, 0x90000000, 0xf8000000, FLAG_NONE, UBI32_ALL},
  {"subc",	FMT_3  , SZ_4, 0x98000000, 0xf8000000, FLAG_NONE, UBI32_ALL},
  {"suspend",	FMT_1A , SZ_0, 0x00000800, 0xf800f800, FLAG_NONE, UBI32_ALL},
  {"swapb.2",	FMT_1D , SZ_2, 0x0000c000, 0xf800f800, FLAG_NONE, UBI32_V4|UBI32_V5|UBI32_V6|UBI32_V61},
  {"swapb.4",	FMT_1D , SZ_4, 0x0000c800, 0xf800f800, FLAG_NONE, UBI32_V4|UBI32_V5|UBI32_V6|UBI32_V61},
  {"sync",	FMT_1C , SZ_4, 0x00001800, 0xf800f800, FLAG_NONE, UBI32_V5|UBI32_V6|UBI32_V61},
  {"syscall",	FMT_1C , SZ_4, 0x00004800, 0xf800f800, FLAG_NONE, UBI32_V5|UBI32_V6|UBI32_V61},
  {"sysret",	FMT_1D , SZ_4, 0x00004000, 0xf800f800, FLAG_NONE, UBI32_V5|UBI32_V6|UBI32_V61},
  {"tbclr",	FMT_1D , SZ_4, 0x0000a000, 0xf800f800, FLAG_NONE, UBI32_V5|UBI32_V6|UBI32_V61},
  {"tbrest",	FMT_1D , SZ_4, 0x00008000, 0xf800f800, FLAG_NONE, UBI32_V6|UBI32_V61},
  {"tbset",	FMT_1D , SZ_4, 0x00009800, 0xf800f800, FLAG_NONE, UBI32_V5|UBI32_V6|UBI32_V61},
  {"tbtst",	FMT_1B , SZ_4, 0x00003000, 0xf800f800, FLAG_NONE, UBI32_V6|UBI32_V61},
  {"xor.1",	FMT_3  , SZ_1, 0x60008000, 0xf8000000, FLAG_NONE, UBI32_V4|UBI32_V5|UBI32_V6|UBI32_V61},
  {"xor.2",	FMT_3  , SZ_2, 0x60000000, 0xf8000000, FLAG_NONE, UBI32_ALL},
  {"xor.4",	FMT_3  , SZ_4, 0x68000000, 0xf8000000, FLAG_NONE, UBI32_ALL},
  { NULL }
};

/* Ubi32 registers.
   Sort by RegAddr.  */
struct reg_info_t reg_table[] =
{
  /* Reg Name           RegNo  RegAddr	 R/W/Priv		Version  */
  {"d0",		  0,    0x000,     REG_R | REG_W, DREG,	 UBI32_ALL},
  {"d1",		  1,    0x004,     REG_R | REG_W, DREG,	 UBI32_ALL},
  {"d2",		  2,    0x008,     REG_R | REG_W, DREG,	 UBI32_ALL},
  {"d3",		  3,    0x00c,     REG_R | REG_W, DREG,	 UBI32_ALL},
  {"d4",		  4,    0x010,     REG_R | REG_W, DREG,	 UBI32_ALL},
  {"d5",		  5,    0x014,     REG_R | REG_W, DREG,	 UBI32_ALL},
  {"d6",		  6,    0x018,     REG_R | REG_W, DREG,	 UBI32_ALL},
  {"d7",		  7,    0x01c,     REG_R | REG_W, DREG,	 UBI32_ALL},
  {"d8",		  8,    0x020,     REG_R | REG_W, DREG,	 UBI32_ALL},
  {"d9",		  9,    0x024,     REG_R | REG_W, DREG,	 UBI32_ALL},
  {"d10",		 10,    0x028,     REG_R | REG_W, DREG,	 UBI32_ALL},
  {"d11",		 11,    0x030,     REG_R | REG_W, DREG,	 UBI32_ALL},
  {"d12",		 12,    0x030,     REG_R | REG_W, DREG,	 UBI32_ALL},
  {"d13",		 13,    0x034,     REG_R | REG_W, DREG,	 UBI32_ALL},
  {"d14",		 14,    0x038,     REG_R | REG_W, DREG,	 UBI32_ALL},
  {"d15",		 15,    0x03c,     REG_R | REG_W, DREG,	 UBI32_ALL},
  {"a0",		 32,    0x080,     REG_R | REG_W, AREG,	 UBI32_ALL},
  {"a1",		 33,    0x084,     REG_R | REG_W, AREG,	 UBI32_ALL},
  {"a2",		 34,    0x088,     REG_R | REG_W, AREG,	 UBI32_ALL},
  {"a3",		 35,    0x08c,     REG_R | REG_W, AREG,	 UBI32_ALL},
  {"a4",		 36,    0x090,     REG_R | REG_W, AREG,	 UBI32_ALL},
  {"a5",		 37,    0x094,     REG_R | REG_W, AREG,	 UBI32_ALL},
  {"a6",		 38,    0x098,     REG_R | REG_W, AREG,	 UBI32_ALL},
  {"sp",		 39,    0x09c,     REG_R | REG_W, AREG,	 UBI32_ALL},
  {"a7",		 39,    0x09c,     REG_R | REG_W, AREG,	 UBI32_ALL},
  {"acc0",		 40,    0x0a0,     REG_R | REG_W, ACC64, UBI32_V2|UBI32_V3|UBI32_V4|UBI32_V5|UBI32_V6|UBI32_V61},
  {"acc0_hi",		 40,    0x0a0,     REG_R | REG_W, ACC32, UBI32_V2|UBI32_V3|UBI32_V4|UBI32_V5|UBI32_V6|UBI32_V61},
  {"mac_hi",		 40,    0x0a0,     REG_R | REG_W, ACC32, UBI32_ALL},
  {"acc0_lo",		 41,    0x0a4,     REG_R | REG_W, ACC32, UBI32_V2|UBI32_V3|UBI32_V4|UBI32_V5|UBI32_V6|UBI32_V61},
  {"mac_lo",		 41,    0x0a4,     REG_R | REG_W, ACC32,	UBI32_ALL},
  {"mac_rc16",		 42,    0x0a8,     REG_R | REG_W, NONE,	UBI32_ALL},
  {"source3",		 43,    0x0ac,     REG_R | REG_W, NONE,	UBI32_ALL},
  {"source_3",		 43,    0x0ac,     REG_R | REG_W, NONE,	UBI32_ALL},
  {"inst_cnt",		 44,    0x0b0,     REG_R        , NONE,	UBI32_ALL},
  {"context_cnt",	 44,    0x0b0,     REG_R        , NONE,	UBI32_ALL},
  {"csr",		 45,    0x0b4,     REG_R | REG_W, PRIV,	UBI32_ALL},
  {"rosr",		 46,    0x0b8,     REG_R        , PRIV,	UBI32_ALL},
  {"iread_data",	 47,    0x0bc,     REG_R | REG_W, NONE,	UBI32_ALL},
  {"int_mask0",		 48,    0x0c0,     REG_R | REG_W, PRIV,	UBI32_ALL},
  {"int_mask1",		 49,    0x0c4,     REG_R | REG_W, PRIV,	UBI32_ALL},
  {"int_mask2",		 50,    0x0c8,     REG_R | REG_W, PRIV,	UBI32_V5|UBI32_V6|UBI32_V61},
  {"int_mask3",		 51,    0x0cc,     REG_R | REG_W, PRIV,	UBI32_V5|UBI32_V6|UBI32_V61},
  {"pc",		 52,    0x0d0,     REG_R | REG_W, PRIV,	UBI32_ALL},
  {"trap_cause",	 53,    0x0d4,     REG_R | REG_W, PRIV,	UBI32_V2|UBI32_V3|UBI32_V4|UBI32_V5|UBI32_V6|UBI32_V61},
  {"acc1",		 54,    0x0d8,     REG_R | REG_W, ACC64,	UBI32_V3|UBI32_V4|UBI32_V5|UBI32_V6|UBI32_V61},
  {"acc1_hi",		 54,    0x0d8,     REG_R | REG_W, ACC32,	UBI32_V3|UBI32_V4|UBI32_V5|UBI32_V6|UBI32_V61},
  {"acc1_lo",		 55,    0x0dc,     REG_R | REG_W, ACC32,	UBI32_V3|UBI32_V4|UBI32_V5|UBI32_V6|UBI32_V61},
  {"previous_pc",	 56,    0x0e0,     REG_R        , PRIV,	UBI32_V3|UBI32_V4|UBI32_V5|UBI32_V6|UBI32_V61},
  {"ucsr",		 57,    0x0e4,     REG_R | REG_W, NONE,	UBI32_V5|UBI32_V6|UBI32_V61},
  {"spsr0",		 58,    0x0e8,     0		, NONE,	0}, 
  {"chip_id",		 64,    0x100,     REG_R        , PRIV,	UBI32_ALL},
  {"int_stat0",		 65,    0x104,     REG_R        , PRIV,	UBI32_ALL},
  {"int_stat1",		 66,    0x108,     REG_R        , PRIV,	UBI32_ALL},
  {"int_stat2",		 67,    0x10c,     REG_R        , PRIV,	UBI32_V5|UBI32_V6|UBI32_V61},
  {"int_stat3",		 68,    0x110,     REG_R        , PRIV,	UBI32_V6|UBI32_V61},
  {"int_set0",		 69,    0x114,             REG_W, PRIV,	UBI32_ALL},
  {"int_set1",		 70,    0x118,             REG_W, PRIV,	UBI32_ALL},
  {"int_set2",		 71,    0x11c,             REG_W, PRIV,	UBI32_V5|UBI32_V6|UBI32_V61},
  {"int_set3",		 72,    0x120,             REG_W, PRIV,	UBI32_V6|UBI32_V61},
  {"int_clr0",		 73,    0x124,             REG_W, PRIV,	UBI32_ALL},
  {"int_clr1",		 74,    0x128,             REG_W, PRIV,	UBI32_ALL},
  {"int_clr2",		 75,    0x12c,             REG_W, PRIV,	UBI32_V5|UBI32_V6|UBI32_V61},
  {"int_clr3",		 76,    0x130,             REG_W, PRIV,	UBI32_V6|UBI32_V61},
  {"global_ctrl",	 77,    0x134,     REG_R | REG_W, PRIV,	UBI32_ALL},
  {"mt_active",		 78,    0x138,     REG_R	      , PRIV,	UBI32_ALL},
  {"mt_active_set",	 79,    0x13c,             REG_W, PRIV,	UBI32_ALL},
  {"mt_active_clr",	 80,    0x140,             REG_W, PRIV,	UBI32_ALL},
  {"mt_dbg_active",	 81,    0x144,     REG_R        , PRIV,	UBI32_ALL},
  {"mt_dbg_active_set",	 82,    0x148,             REG_W, PRIV,	UBI32_ALL},
  {"mt_en",		 83,    0x14c,     REG_R | REG_W, PRIV,	UBI32_ALL},
  {"mt_hpri",		 84,    0x150,     REG_R | REG_W, PRIV,	UBI32_ALL},
  {"mt_pri",		 84,    0x150,     REG_R | REG_W, PRIV,	UBI32_ALL},
  {"mt_hrt",		 85,    0x154,     REG_R | REG_W, PRIV,	UBI32_ALL},
  {"mt_sched",		 85,    0x154,     REG_R | REG_W, PRIV,	UBI32_ALL},
  {"mt_break",		 86,    0x158,     REG_R        , PRIV,	UBI32_ALL},
  {"mt_break_clr",	 87,    0x15c,             REG_W, PRIV,	UBI32_ALL},
  {"mt_single_step",	 88,    0x160,     REG_R | REG_W, PRIV,	UBI32_ALL},
  {"mt_min_delay_en",	 89,    0x164,     REG_R | REG_W, PRIV,	UBI32_ALL},
  {"mt_min_del_en",	 89,    0x164,     REG_R | REG_W, PRIV,	UBI32_ALL},
  {"mt_break_set",	 90,    0x168,             REG_W, PRIV,	UBI32_V2|UBI32_V3|UBI32_V4|UBI32_V5|UBI32_V6|UBI32_V61},
  {"perr_addr",		 91,    0x16c,     REG_R        , NONE,	UBI32_V2},
  {"mt_fp_flush",	 91,    0x16c,             REG_W, NONE,	UBI32_V3|UBI32_V4|UBI32_V5|UBI32_V6|UBI32_V61},
  {"dcapt",		 92,    0x170,     REG_R | REG_W, PRIV,	UBI32_ALL},
  {"dcapt_tnum",	 93,    0x174,     REG_R        , NONE,	UBI32_V2},
  {"mt_fp_blocked",	 93,    0x174,     REG_R        , NONE,	UBI32_V5|UBI32_V6|UBI32_V61},
  {"dcapt_pc",		 94,    0x178,     REG_R        , NONE,	UBI32_V2},
  {"mt_fp_blocked_set",	 94,    0x178,             REG_W, NONE,	UBI32_V5|UBI32_V6},
  {"mt_dbg_active_clr",	 95,    0x17c,             REG_W, PRIV,	UBI32_ALL},
  {"scratchpad0",	 96,    0x180,     REG_R | REG_W, PRIV,	UBI32_ALL},
  {"scratchpad1",	 97,    0x184,     REG_R | REG_W, PRIV,	UBI32_ALL},
  {"scratchpad2",	 98,    0x188,     REG_R | REG_W, PRIV,	UBI32_ALL},
  {"scratchpad3",	 99,    0x18c,     REG_R | REG_W, PRIV,	UBI32_ALL},
  {"scratchpad4",	100,    0x190,     REG_R | REG_W, PRIV,	UBI32_V2|UBI32_V3|UBI32_V4|UBI32_V5|UBI32_V6|UBI32_V61},
  {"scratchpad5",	101,    0x194,     REG_R | REG_W, PRIV,	UBI32_V2|UBI32_V3|UBI32_V4|UBI32_V5|UBI32_V6|UBI32_V61},
  {"scratchpad6",	102,    0x198,     REG_R | REG_W, PRIV,	UBI32_V2|UBI32_V3|UBI32_V4|UBI32_V5|UBI32_V6|UBI32_V61},
  {"scratchpad7",	103,    0x19c,     REG_R | REG_W, PRIV,	0},
  {"chip_cfg",		104,    0x1a0,     REG_R | REG_W, PRIV,	UBI32_V2|UBI32_V3|UBI32_V4|UBI32_V5|UBI32_V6|UBI32_V61},
  {"mt_i_blocked",	105,    0x1a4,     REG_R        , PRIV,	UBI32_V2|UBI32_V3|UBI32_V4|UBI32_V5|UBI32_V6|UBI32_V61},
  {"mt_d_blocked",	106,    0x1a8,     REG_R        , PRIV,	UBI32_V2|UBI32_V3|UBI32_V4|UBI32_V5|UBI32_V6|UBI32_V61},
  {"mt_i_blocked_set",	107,    0x1ac,             REG_W, PRIV,	UBI32_V2|UBI32_V3|UBI32_V4|UBI32_V5|UBI32_V6|UBI32_V61},
  {"mt_d_blocked_set",	108,    0x1b0,             REG_W, PRIV,	UBI32_V2|UBI32_V3|UBI32_V4|UBI32_V5|UBI32_V6|UBI32_V61},
  {"mt_blocked_clr",	109,    0x1b4,             REG_W, PRIV,	UBI32_V2|UBI32_V3|UBI32_V4|UBI32_V5|UBI32_V6|UBI32_V61},
  {"mt_trap_en",	110,    0x1b8,     REG_R | REG_W, PRIV,	UBI32_V3|UBI32_V4|UBI32_V5|UBI32_V6|UBI32_V61},
  {"mt_trap",		111,    0x1bc,     REG_R        , PRIV,	UBI32_V3|UBI32_V4|UBI32_V5|UBI32_V6|UBI32_V61},
  {"mt_trap_set",	112,    0x1c0,             REG_W, PRIV,	UBI32_V3|UBI32_V4|UBI32_V5|UBI32_V6|UBI32_V61},
  {"mt_trap_clr",	113,    0x1c4,             REG_W, PRIV,	UBI32_V3|UBI32_V4|UBI32_V5|UBI32_V6|UBI32_V61},
  {"sep",		114,    0x1c8,     REG_R | REG_W, NONE,	UBI32_V5|UBI32_V6|UBI32_V61},
  {"mt_btb_en",		115,    0x1cc,     REG_R | REG_W, NONE,	UBI32_V5|UBI32_V6|UBI32_V61},
  {"btb_ctrl",		116,    0x1d0,     REG_R | REG_W, NONE,	UBI32_V5|UBI32_V6|UBI32_V61},
  {"tnum",		117,    0x1d4,     REG_R        , PRIV,	UBI32_V5|UBI32_V6|UBI32_V61},
  {"i_range0_hi",	128,    0x200,     REG_R | REG_W, PRIV,	UBI32_V2|UBI32_V3|UBI32_V4|UBI32_V5|UBI32_V6|UBI32_V61},
  {"i_range1_hi",	129,    0x204,     REG_R | REG_W, PRIV,	UBI32_V2|UBI32_V3|UBI32_V4|UBI32_V5|UBI32_V6|UBI32_V61},
  {"i_range2_hi",	130,    0x208,     REG_R | REG_W, PRIV,	UBI32_V2|UBI32_V3|UBI32_V4|UBI32_V5|UBI32_V6|UBI32_V61},
  {"i_range3_hi",	131,    0x20c,     REG_R | REG_W, PRIV,	UBI32_V2|UBI32_V3|UBI32_V4|UBI32_V5|UBI32_V6|UBI32_V61},
  {"i_range4_hi",	132,    0x210,     REG_R | REG_W, PRIV,	UBI32_V6|UBI32_V61},
  {"i_range5_hi",	133,    0x214,     REG_R | REG_W, PRIV,	UBI32_V6|UBI32_V61},
  {"i_range6_hi",	134,    0x218,     REG_R | REG_W, PRIV,	UBI32_V6|UBI32_V61},
  {"i_range7_hi",	135,    0x21c,     REG_R | REG_W, PRIV,	UBI32_V6|UBI32_V61},
  {"i_range0_lo",	136,    0x220,     REG_R | REG_W, PRIV,	UBI32_V2|UBI32_V3|UBI32_V4|UBI32_V5|UBI32_V6|UBI32_V61},
  {"i_range1_lo",	137,    0x224,     REG_R | REG_W, PRIV,	UBI32_V2|UBI32_V3|UBI32_V4|UBI32_V5|UBI32_V6|UBI32_V61},
  {"i_range2_lo",	138,    0x228,     REG_R | REG_W, PRIV,	UBI32_V2|UBI32_V3|UBI32_V4|UBI32_V5|UBI32_V6|UBI32_V61},
  {"i_range3_lo",	139,    0x22c,     REG_R | REG_W, PRIV,	UBI32_V2|UBI32_V3|UBI32_V4|UBI32_V5|UBI32_V6|UBI32_V61},
  {"i_range4_lo",	140,    0x230,     REG_R | REG_W, PRIV,	UBI32_V6|UBI32_V61},
  {"i_range5_lo",	141,    0x234,     REG_R | REG_W, PRIV,	UBI32_V6|UBI32_V61},
  {"i_range6_lo",	142,    0x238,     REG_R | REG_W, PRIV,	UBI32_V6|UBI32_V61},
  {"i_range7_lo",	143,    0x23c,     REG_R | REG_W, PRIV,	UBI32_V6|UBI32_V61},
  {"i_range0_en",	144,    0x240,     REG_R | REG_W, PRIV,	UBI32_V2|UBI32_V3|UBI32_V4|UBI32_V5|UBI32_V6|UBI32_V61},
  {"i_range1_en",	145,    0x244,     REG_R | REG_W, PRIV,	UBI32_V2|UBI32_V3|UBI32_V4|UBI32_V5|UBI32_V6|UBI32_V61},
  {"i_range2_en",	146,    0x248,     REG_R | REG_W, PRIV,	UBI32_V2|UBI32_V3|UBI32_V4|UBI32_V5|UBI32_V6|UBI32_V61},
  {"i_range3_en",	147,    0x24c,     REG_R | REG_W, PRIV,	UBI32_V2|UBI32_V3|UBI32_V4|UBI32_V5|UBI32_V6|UBI32_V61},
  {"i_range4_en",	148,    0x250,     REG_R | REG_W, PRIV,	UBI32_V6|UBI32_V61},
  {"i_range5_en",	149,    0x254,     REG_R | REG_W, PRIV,	UBI32_V6|UBI32_V61},
  {"i_range6_en",	150,    0x258,     REG_R | REG_W, PRIV,	UBI32_V6|UBI32_V61},
  {"i_range7_en",	151,    0x25c,     REG_R | REG_W, PRIV,	UBI32_V6|UBI32_V61},
  {"d_range0_hi",	152,    0x260,     REG_R | REG_W, PRIV,	UBI32_V2|UBI32_V3|UBI32_V4|UBI32_V5|UBI32_V6|UBI32_V61},
  {"d_range1_hi",	153,    0x264,     REG_R | REG_W, PRIV,	UBI32_V2|UBI32_V3|UBI32_V4|UBI32_V5|UBI32_V6|UBI32_V61},
  {"d_range2_hi",	154,    0x268,     REG_R | REG_W, PRIV,	UBI32_V2|UBI32_V3|UBI32_V4|UBI32_V5|UBI32_V6|UBI32_V61},
  {"d_range3_hi",	155,    0x26c,     REG_R | REG_W, PRIV,	UBI32_V2|UBI32_V3|UBI32_V4|UBI32_V5|UBI32_V6|UBI32_V61},
  {"d_range4_hi",	156,    0x270,     REG_R | REG_W, PRIV,	UBI32_V2|UBI32_V3|UBI32_V4|UBI32_V5|UBI32_V6|UBI32_V61},
  {"d_range5_hi",	157,    0x274,     REG_R | REG_W, PRIV,	UBI32_V5|UBI32_V6|UBI32_V61},
  {"d_range6_hi",	158,    0x278,     REG_R | REG_W, PRIV,	UBI32_V6|UBI32_V61},
  {"d_range7_hi",	159,    0x27c,     REG_R | REG_W, PRIV,	UBI32_V6|UBI32_V61},
  {"d_range0_lo",	160,    0x280,     REG_R | REG_W, PRIV,	UBI32_V2|UBI32_V3|UBI32_V4|UBI32_V5|UBI32_V6|UBI32_V61},
  {"d_range1_lo",	161,    0x284,     REG_R | REG_W, PRIV,	UBI32_V2|UBI32_V3|UBI32_V4|UBI32_V5|UBI32_V6|UBI32_V61},
  {"d_range2_lo",	162,    0x288,     REG_R | REG_W, PRIV,	UBI32_V2|UBI32_V3|UBI32_V4|UBI32_V5|UBI32_V6|UBI32_V61},
  {"d_range3_lo",	163,    0x28c,     REG_R | REG_W, PRIV,	UBI32_V2|UBI32_V3|UBI32_V4|UBI32_V5|UBI32_V6|UBI32_V61},
  {"d_range4_lo",	164,    0x290,     REG_R | REG_W, PRIV,	UBI32_V2|UBI32_V3|UBI32_V4|UBI32_V5|UBI32_V6|UBI32_V61},
  {"d_range5_lo",	165,    0x294,     REG_R | REG_W, PRIV,	UBI32_V5|UBI32_V6|UBI32_V61},
  {"d_range6_lo",	166,    0x298,     REG_R | REG_W, PRIV,	UBI32_V6|UBI32_V61},
  {"d_range7_lo",	167,    0x29c,     REG_R | REG_W, PRIV,	UBI32_V6|UBI32_V61},
  {"d_range0_en",	168,    0x2a0,     REG_R | REG_W, PRIV,	UBI32_V2|UBI32_V3|UBI32_V4|UBI32_V5|UBI32_V6|UBI32_V61},
  {"d_range1_en",	169,    0x2a4,     REG_R | REG_W, PRIV,	UBI32_V2|UBI32_V3|UBI32_V4|UBI32_V5|UBI32_V6|UBI32_V61},
  {"d_range2_en",	170,    0x2a8,     REG_R | REG_W, PRIV,	UBI32_V2|UBI32_V3|UBI32_V4|UBI32_V5|UBI32_V6|UBI32_V61},
  {"d_range3_en",	171,    0x2ac,     REG_R | REG_W, PRIV,	UBI32_V2|UBI32_V3|UBI32_V4|UBI32_V5|UBI32_V6|UBI32_V61},
  {"d_range4_en",	172,    0x2b0,     REG_R | REG_W, PRIV,	UBI32_V2|UBI32_V3|UBI32_V4|UBI32_V5|UBI32_V6|UBI32_V61},
  {"d_range5_en",	173,    0x2b4,     REG_R | REG_W, PRIV,	UBI32_V5|UBI32_V6|UBI32_V61},
  {"d_range6_en",	174,    0x2b8,     REG_R | REG_W, PRIV,	UBI32_V6|UBI32_V61},
  {"d_range7_en",	175,    0x2bc,     REG_R | REG_W, PRIV,	UBI32_V6|UBI32_V61},
  {"i_range0_user_en",	176,    0x2c0,     REG_R | REG_W, PRIV,	UBI32_V5|UBI32_V6|UBI32_V61},
  {"i_range1_user_en",	177,    0x2c4,     REG_R | REG_W, PRIV,	UBI32_V5|UBI32_V6|UBI32_V61},
  {"i_range2_user_en",	178,    0x2c8,     REG_R | REG_W, PRIV,	UBI32_V5|UBI32_V6|UBI32_V61},
  {"i_range3_user_en",	179,    0x2cc,     REG_R | REG_W, PRIV,	UBI32_V5|UBI32_V6|UBI32_V61},
  {"i_range4_user_en",	180,    0x2d0,     REG_R | REG_W, PRIV,	UBI32_V6|UBI32_V61},
  {"i_range5_user_en",	181,    0x2d4,     REG_R | REG_W, PRIV,	UBI32_V6|UBI32_V61},
  {"i_range6_user_en",	182,    0x2d8,     REG_R | REG_W, PRIV,	UBI32_V6|UBI32_V61},
  {"i_range7_user_en",	183,    0x2dc,     REG_R | REG_W, PRIV,	UBI32_V6|UBI32_V61},
  {"d_range0_user_en",	184,    0x2e0,     REG_R | REG_W, PRIV,	UBI32_V5|UBI32_V6|UBI32_V61},
  {"d_range1_user_en",	185,    0x2e4,     REG_R | REG_W, PRIV,	UBI32_V5|UBI32_V6|UBI32_V61},
  {"d_range2_user_en",	186,    0x2e8,     REG_R | REG_W, PRIV,	UBI32_V5|UBI32_V6|UBI32_V61},
  {"d_range3_user_en",	187,    0x2ec,     REG_R | REG_W, PRIV,	UBI32_V5|UBI32_V6|UBI32_V61},
  {"d_range4_user_en",	188,    0x2f0,     REG_R | REG_W, PRIV,	UBI32_V5|UBI32_V6|UBI32_V61},
  {"d_range5_user_en",	189,    0x2f4,     REG_R | REG_W, PRIV,	UBI32_V5|UBI32_V6|UBI32_V61},
  {"d_range6_user_en",	190,    0x2f8,     REG_R | REG_W, PRIV,	UBI32_V6|UBI32_V61},
  {"d_range7_user_en",	191,    0x2fc,     REG_R | REG_W, PRIV,	UBI32_V6|UBI32_V61},
  { NULL }
};

int reg_table_size = (sizeof (reg_table) / sizeof (struct reg_info_t));

/* Ubi32 JMP instruction condition codes.  */
struct cc_t cc_tab[] =
{
  {"f",  0x0},
  {"cc", 0x1},
  {"lo", 0x1},
  {"cs", 0x2},
  {"hs", 0x2},
  {"eq", 0x3},
  {"ge", 0x4},
  {"gt", 0x5},
  {"hi", 0x6},
  {"le", 0x7},
  {"ls", 0x8},
  {"lt", 0x9},
  {"mi", 0xa},
  {"ne", 0xb},
  {"pl", 0xc},
  {"t",  0xd},
  {"vc", 0xe},
  {"vs", 0xf},
  {NULL, 0}
};

/* Ubi32 GDB register numbering for Ubi32 V6/V6.1 processors.
   This should match GDB ubi32_register_names[] in ubi32-tdep.c  */
const char *ubi32_gdb_register_names[] =
{
  /* General data registers.  */
  "d0", "d1", "d2",  "d3",  "d4",  "d5",  "d6",  "d7",	/*  0- 7 */
  "d8", "d9", "d10", "d11", "d12", "d13", "d14", "d15",	/*  8-15 */
  /* Address registers.  */
  "a0", "a1", "a2",  "a3",  "a4",  "a5",  "a6",  "a7",	/* 16-23 */
  /* Accumulator 0.  */
  "acc0_hi", "acc0_lo", "mac_rc16", "source3",	 	/* 24-27 */
  /* Context & Control.  */
  "inst_cnt", "csr", "rosr", "iread_data",		/* 28-31 */
  /* Interrupt mask registers.  */
  "int_mask0", "int_mask1", "int_mask2", "int_mask3", 	/* 32-35 */
  /* Program counter & trap.  */
  "pc", "trap_cause",					/* 36-37 */
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
  "mt_en", "mt_hpri", "mt_hrt", 			/* 61-63 */
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
  "cfg",						/* 78	 */
  /* Thread status and trap control.  */
  "mt_i_blocked", "mt_d_blocked", 			/* 79-80 */
  "mt_i_blocked_set", "mt_d_blocked_set",		/* 81-82 */
  "mt_blocked_clr", "mt_trap_en",			/* 83-84 */
  "mt_trap", "mt_trap_set", "mt_trap_clr",		/* 85-87 */
  /* Supervisor Call Entry Point.  */
  "sep",						/* 88	 */
  /* Valid instruction address ranges high, low, enable.  */
  "i_range0_hi", "i_range1_hi", "i_range2_hi", "i_range3_hi", /* 89-92 */
  "i_range4_hi", "i_range5_hi", "i_range6_hi", "i_range7_hi", /* 93-96 */
  "i_range0_lo", "i_range1_lo", "i_range2_lo", "i_range3_lo", /* 97-100 */
  "i_range4_lo", "i_range5_lo", "i_range6_lo", "i_range7_lo", /* 101-104 */
  "i_range0_en", "i_range1_en", "i_range2_en", "i_range3_en", /* 105-108 */
  "i_range4_en", "i_range5_en", "i_range6_en", "i_range7_en", /* 109-112 */
  /* Valid data address ranges high, low, enable.  */
  "d_range0_hi", "d_range1_hi", "d_range2_hi", "d_range3_hi", /* 113-116 */
  "d_range4_hi", "d_range5_hi", "d_range6_hi", "d_range7_hi", /* 117-120 */
  "d_range0_lo", "d_range1_lo", "d_range2_lo", "d_range3_lo", /* 121-124 */
  "d_range4_lo", "d_range5_lo", "d_range6_lo", "d_range7_lo", /* 125-128 */
  "d_range0_en", "d_range1_en", "d_range2_en", "d_range3_en", /* 129-132 */
  "d_range4_en", "d_range5_en", "d_range6_en", "d_range7_en", /* 133-136 */
  /* Valid user instruction address ranges enable.  */
  "i_range0_user_en", "i_range1_user_en",
  "i_range2_user_en", "i_range3_user_en",
  "i_range4_user_en", "i_range5_user_en",
  "i_range6_user_en", "i_range7_user_en",		/* 137-144 */
  /* Valid user data address ranges enable.  */
  "d_range0_user_en", "d_range1_user_en",
  "d_range2_user_en", "d_range3_user_en",
  "d_range4_user_en", "d_range5_user_en",
  "d_range6_user_en", "d_range7_user_en",		/* 145-152 */
  /* FP control registers.  */
  NULL
};
