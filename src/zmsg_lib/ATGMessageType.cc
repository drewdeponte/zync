/*
 * Copyright 2004 Andrew De Ponte
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
 * @file ATGMessageType.cc
 * @brief A implimentation file for the ATGMessageType class.
 *
 * This type was developed to provide an object to represent a message
 * of type ATG.
 */

#include "ATGMessageType.h"

/**
 * Construct a default ATGMessageType object.
 *
 * Construct a default ATGMessageType object with all the basic
 * initialization.
 */
ATGMessageType::ATGMessageType(void) : ZaurusMessageType((void *)"ATG")
{

}

/**
 * Destruct the ATGMessageType object.
 *
 * Destruct the ATGMessageType object by deallocating any dynamically
 * allocated momory.
 */
ATGMessageType::~ATGMessageType(void) {

}

/**
 * Obtain the TimeStamp String.
 *
 * Obtain the TimeStamp String from the message content.
 * @param dest Pointer to destination that the TimeStamp c-string should be
 * stored in. The c-string is in the format of YYYYMMDDhhmmss.
 * @param size The size of the destination buffer in bytes.
 * @return An integer representing success (zero) or failure (non-zero).
 * @retval 0 Successfully obtained the TimeStamp as a c-string.
 * @retval 1 Failed, the buffer is too small to house the TimeStamp c-string.
 */
int ATGMessageType::GetTimestamp(char *dest, unsigned short int size) {
    unsigned char *pCont;
    unsigned short int tsSize;

    tsSize = GetContSize();

    // Check to make sure that the destination is large enough to hold the
    // timestamp c-string.
    if (size < (tsSize + 1)) {
	return 1;
    }

    pCont = (unsigned char *)GetContent();
    memcpy(dest, (const void *)pCont, tsSize);
    dest[tsSize] = '\0';

    return 0;
}
