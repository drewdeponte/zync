/*
 * Copyright 2004-2005 Andrew De Ponte
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
 * @file AIGMessageType.cc
 * @brief A implimentation file for the AIGMessageType class.
 *
 * This type was developed to provide an object to represent a message
 * of type AIG.
 */

#include "AIGMessageType.h"

/**
 * Construct a default AIGMessageType object.
 *
 * Construct a default AIGMessageType object with all the basic
 * initialization.
 */
AIGMessageType::AIGMessageType(void) : ZaurusMessageType((void *)"AIG")
{

}

/**
 * Destruct the AIGMessageType object.
 *
 * Destruct the AIGMessageType object by deallocating any dynamically
 * allocated momory.
 */
AIGMessageType::~AIGMessageType(void) {

}

/**
 * Obtain the size of the Model String.
 *
 * Obtain the size of the Model String in bytes from the message content.
 * @return The size of the Model String in bytes.
 */
unsigned short int AIGMessageType::GetModelSize(void) {
    unsigned short int *pCont;
    unsigned short int modSize;

    pCont = (unsigned short int *)GetContent();

    if (IsLittleEndian())
	modSize = *(pCont);
    else
	SwapByteOrder((const void *)pCont, (void *)&modSize,
		      sizeof(unsigned short int));

    return modSize;
}

/**
 * Obtain the Model String.
 *
 * Obtain the Model String from the message content.
 * @param dest Pointer to destination that the Model c-string should be stored
 * in.
 * @param size The size of the destination buffer in bytes.
 * @return An integer signifying success (zero) or failure (non-zero).
 * @retval 0 Successfully obtained the Model as a c-string.
 * @retval 1 Failed, the buffer is too small to house the Model c-string.
 */
int AIGMessageType::GetModel(char *dest, unsigned short int size) {
    unsigned char *pCont;
    unsigned short int modSize;

    modSize = GetModelSize();

    // Check to make sure that the destination is large enough to hold the
    // model string.
    if (size < (modSize + 1)) {
	return 1;
    }

    pCont = (unsigned char *)GetContent();
    memcpy(dest, (const void *)(pCont + 2), modSize);
    dest[modSize] = '\0';

    return 0;
}

/**
 * Obtain the size of the Language String.
 *
 * Obtain the size of the Language String in bytes.
 * @return The size of the Language String in bytes.
 */
unsigned short int AIGMessageType::GetLanguageSize(void) {
    return 2;
}

/**
 * Obtain the Language String.
 *
 * Obtain the Language String from the message content.
 * @param dest Pointer to destination that the Language  c-string should be
 * stored in.
 * @param size The size of the destination buffer in bytes.
 * @return A integer signifying success (zero) or failure (non-zero).
 * @retval 0 Successfully obtained the Language as a c-string.
 * @retval 1 Failed, the buffer is to small to house the Language c-string.
 */
int AIGMessageType::GetLanguage(char *dest, unsigned short int size) {
    unsigned char *pCont;
    unsigned short int langSize;

    langSize = GetLanguageSize();

    // Check to make sure that the destination is large enough to hold the
    // language string.
    if (size < (langSize + 1)) {
	return 1;
    }

    pCont = (unsigned char *)GetContent();
    memcpy(dest, (const void *)(pCont + 2 + GetModelSize() + 5), langSize);
    dest[langSize] = '\0';

    return 0;
}

/**
 * Obtain the Authentication State.
 *
 * Obtain the Authentication State from the message content.
 * @return The value of the authentication state.
 */
unsigned char AIGMessageType::GetAuthState(void) {
    unsigned char *pCont;

    pCont = (unsigned char *)GetContent();

    return *(pCont + 2 + GetModelSize() + 5 + GetLanguageSize());
}

