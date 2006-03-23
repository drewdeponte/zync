/*
 * Copyright 2003-2005 Andrew De Ponte
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
 * @file byteorder.cc
 * @brief An implimentation file containing byte order functions for libzmsg.
 *
 * An implimentation file containing definitions of byte order functions for
 * libzmsg.
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "byteorder.h"

/**
 * Get the byte order of the host.
 *
 * Get the byte order of the host machine.
 * @return An integer representing the host byte order.
 * @retval 1 The host byte order is little-endian.
 * @retval 2 The host byte order is big-endian.
 * @retval 0 The host byte order is unkown, or unsigned shorts aren't 2 bytes.
 */
int GetHostByteOrder(void) {
    // Here I use a union to create both a unsigned short integer
    // as well as a unsigned character array within the same memory
    // area so that only sizeof(unsigned short int) is allocated and
    // it can be referred to by either data type.
    union {
        unsigned short int  my_var;
        unsigned char       my_data[sizeof(unsigned short int)];
    } foo;

    // Set the short integer to the big-endian version of some
    // destinguishable value.
    foo.my_var = 0x0102;

    // Check to make sure that unsigned short integers are 2 bytes
    // on this box.
    if (sizeof(unsigned short int) == 2) {
        if ((foo.my_data[0] == 0x01) && (foo.my_data[1] == 0x02)) {
            // We know this is big endian
            return 2;
        } else if ((foo.my_data[0] == 0x02) && (foo.my_data[1] == 0x01)) {
            // We know this is little endian
            return 1;
        } else {
            printf("Unknown byte order.\n");
        }
    } else {
        printf("sizeof(unsigned short int) = %d\n",
	       sizeof(unsigned short int));
    }
   
    // Return a 0 signifying that we are not sure of the byte order.
    return 0;
}

/**
 * Determin if byte order is little-endian.
 *
 * Determin if the host byte order is little-endian.
 * @return An integer representing true (1) or false (0).
 * @retval 1 True, the host byte order IS little-endian.
 * @retval 0 False, the host byte order is NOT little-endian.
 */
int IsLittleEndian(void) {
    if (GetHostByteOrder() == 1)
        return 1;
    else
        return 0;
}

/**
 * Determin if byte order is big-endian.
 *
 * Determin if the host byte order is big-endian.
 * @return An integer representing true (1) or false (0).
 * @retval 1 True, the host byte order IS big-endian.
 * @retval 0 False, the host byte order is NOT big-endian.
 */
int IsBigEndian(void) {
    if (GetHostByteOrder() == 2)
        return 1;
    else
        return 0;
}

/**
 * Swap the byte order.
 *
 * Swap the byte order of the given data from either little-endian to
 * big-endian or from big-endian to little-endian depending on the starting
 * order of the bytes.
 * @param[in] src A pointer to the beginning of the source data.
 * @param[out] dst A pointer to destination buffer to store resulting data.
 * @param[in] len The len, in bytes, of the data to swap the byte order of.
 * @pre The data pointed to by src must be at least len bytes.
 * @pre The buffer pointed to by dst must be at least len bytes.
 * @post The buffer pointed to by dst now contains the data from src in the
 * opposing byte order of when it was passed in.
 * @return An integer representing success (zero) or failure (non-zero).
 * @retval 0 Successfully swapped the byte order of the data.
 * @retval 1 Failed to swap the byte order of the data.
 */
int SwapByteOrder(const void *src, void *dst, int len) {
    unsigned char *pTmpSrc;
    int i;

    if (src == dst) {
	pTmpSrc = (unsigned char *)malloc((size_t)len);
	if (pTmpSrc == NULL)
	    return 1;

	// Here I duplicate the src data so that the src and dst may be
	// pointers to the same memory.
	memcpy(pTmpSrc, src, len);
    } else {
	pTmpSrc = (unsigned char *)src;
    }

    for (i = 0; i < len; i++)
        ((unsigned char *)dst)[i] = pTmpSrc[((len - 1) - i)];

    if (src == dst)
	free((void *)pTmpSrc);

    return 0;
}
