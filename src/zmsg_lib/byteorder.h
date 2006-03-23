/*
 * Copyright 2003, 2004, 2005 Andrew De Ponte
 * 
 * This file is part of zsrep.
 * 
 * zsrep is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * any later version.
 * 
 * zsrep is distributed in the hopes that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for details.
 * 
 * You should have received a copy of the GNU General Public License
 * along with zsrep; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA 02111-1307 USA
 */

/**
 * @file byteorder.h
 * @brief A specifications file of byte order functions for the libzmsg.
 *
 * A specifications file containing prototypes for byte order functions
 * created to be used in libzmsg to assist in the process of providing a
 * cross-platform library.
 */

#ifndef BYTEORDER_H
#define BYTEORDER_H

int GetHostByteOrder(void);
int IsLittleEndian(void);
int IsBigEndian(void);
int SwapByteOrder(const void *src, void *dst, int len);

#endif
