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
 * @file ADIMessageType.cc
 * @brief An implimentation file for the ADIMessageType class.
 *
 * This type was developed to provide an object to represent a message
 * of type ADI.
 */

#include "ADIMessageType.h"

/**
 * Construct a default ADIMessageType object.
 *
 * Construct a default ADIMessageType object with all the basic
 * initialization.
 */
ADIMessageType::ADIMessageType(void) : ZaurusMessageType((void *)"ADI")
{

}

/**
 * Destruct the ADIMessageType object.
 *
 * Destruct the ADIMessageType object by deallocating any dynamically
 * allocated momory.
 */
ADIMessageType::~ADIMessageType(void) {

}

/**
 * Obtain the number of cards in the box.
 *
 * Obtain the number of cards in the current type of synchronizations box. A
 * box is an imaginary entity which holds cards. Cards are an imaginary entity
 * which contain all the data for a given item for the type of
 * synchronization.
 * @return The number of cards existing within the current sync type's box.
 */
unsigned long int ADIMessageType::GetNumCards(void) {
    unsigned char *pCont;
    unsigned long int numCards;

    pCont = (unsigned char *)GetContent();

    if (IsLittleEndian())
	numCards = *((unsigned long int *)pCont);
    else
	SwapByteOrder((const void *)pCont, (void *)&numCards,
		      sizeof(unsigned long int));

    return numCards;
}

/**
 * Obtain number of Params.
 *
 * Obtain the number of parameters per card.
 * @return The number of parameters per card.
 */
unsigned short int ADIMessageType::GetNumParams(void) {
    unsigned char *pCont;
    unsigned short int numParams;

    pCont = (unsigned char *)GetContent();

    if (IsLittleEndian())
	numParams = *((unsigned short int *)(pCont + 4));
    else
	SwapByteOrder((const void *)(pCont + 4), (void *)&numParams,
		      sizeof(unsigned short int));

    return numParams;
}

/**
 * Obtain the abreviation of a card parameter.
 *
 * Obtain the abreviation of a card parameter given the offset of the
 * parameter. Acceptable offset values start at 0 and increase depending on
 * the number of parameters associated with cards of the current
 * synchronization type. The destination character array should be atleast
 * five bytes in length.
 * @param dest Pointer to character array to store the abreviation in.
 * @param destLen Length of the destination character array in bytes.
 * @param paramOffset An offset defining which parameter abrev to get.
 * @return An integer representing success (zero) or failure (non-zero).
 * @retval 0 Successfully obtained the parameter abreviation.
 * @retval 1 Failed, destLen is not large enough to store the abreviation.
 * @retval 2 Failed, paramOffset is in an invalid range.
 */
int ADIMessageType::GetParamAbrev(char *dest, int destLen,
				  unsigned short int paramOffset) {
    unsigned char *pCont;

    pCont = (unsigned char *)GetContent();

    // Check to make sure that the destination buffer is large enough to store
    // the abrevitaion in.
    if (destLen < 5) {
	return 1;
    }

    // Check to make sure that the parameter offset is in a valid range.
    if ((paramOffset > (GetNumParams() - 1))) {
	return 2;
    }

    // Copy the proper abreviation into the destination buffer and null
    // terminate it making it a c-string.
    memcpy((void *)dest, (void *)((pCont + 7) + (4 * paramOffset)), 4);
    dest[4] = '\0';

    return 0;
}

/**
 * Get the parameters type ID.
 *
 * Obtain the parameters type ID given a parameter offset. Accpetable values
 * for the offset start at 0 and increment to a maximum based on the number of
 * parameters associated with the card of the current synchronization type.
 * @param paramOffset An offset defining which type ID to obtain.
 * @return zero or negative in failure, positive in success (the type ID).
 * @retval 0x00 Failed, paramOffset is in an invalid range.
 * @retval 0x01 Strings in SJIS or Code Data.
 * @retval 0x02 Image data.
 * @retval 0x03 Hybrid text.
 * @retval 0x04 Time.
 * @retval 0x05 Hours.
 * @retval 0x06 Bit.
 * @retval 0x07 Week/day of the week.
 * @retval 0x08 Unsigned short.
 * @retval 0x09 Binary block.
 * @retval 0x0a Unsigned short GS or Word GS.
 * @retval 0x0b Unsigned char.
 * @retval 0x0c BARRAY.
 * @retval 0x0d Color data.
 * @retval 0x0e SXLS.
 * @retval 0x0f SRTF.
 * @retval 0x11 Strings in UTF-8.
 * @retval 0x12 Unsigned long.
 */
unsigned char ADIMessageType::GetParamTypeID(unsigned short int paramOffset) {
    unsigned char *pCont;

    pCont = (unsigned char *)GetContent();

    // Check to make sure that the paramOffset is a valid offset.
    if ((paramOffset > (GetNumParams() - 1))) {
	return (unsigned char)0;
    }

    pCont = (pCont + 7) + (4 * GetNumParams());

    return *((unsigned char *)(pCont + paramOffset));
}

/**
 * Obtain the description of a card parameter.
 *
 * Obtain the description of a card parameter given the offset of the
 * parameter. Acceptable offset values start at 0 and increase depending on
 * the number of parameters associated with cards of the current
 * synchronization type. The destination character array should large enough
 * to hold the param description.
 * @param dest Pointer to character array to store the description in.
 * @param destLen Length of the destination character array in bytes.
 * @param paramOffset An offset defining which parameter desc to get.
 * @return An integer representing success (zero) or failure (non-zero).
 * @retval 0 Successfully obtained the parameter description.
 * @retval 1 Failed, paramOffset is in an invalid range.
 * @retval 2 Failed, destLen is not large enough to store the description.
 */
int ADIMessageType::GetParamDesc(char *dest, int destLen,
				 unsigned short int paramOffset) {
    unsigned char *pCont;
    unsigned short int lenFol;
    int i;

    pCont = (unsigned char *)GetContent();

    pCont = (pCont + 7) + (4 * GetNumParams()) + GetNumParams();

    // Check to make sure that the parameter offset is in a valid range.
    if ((paramOffset > (GetNumParams() - 1))) {
	return 1;
    }

    for (i = 0; i < paramOffset; i++) {
	lenFol = *((unsigned short int *)pCont);
	pCont = (pCont +  (lenFol + 2));
    }

    lenFol = *((unsigned short int *)pCont);
    pCont = pCont + 2;

    // Check to make sure that the length of the destination is large enough.
    if (destLen <= lenFol) {
	return 2;
    }

    // Copy the proper abreviation into the destination buffer and null
    // terminate it making it a c-string.
    memcpy((void *)dest, (void *)pCont, lenFol);
    dest[lenFol] = '\0';

    return 0;
}
