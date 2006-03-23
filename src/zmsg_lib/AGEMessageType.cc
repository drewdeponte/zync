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
 * @file AGEMessageType.cc
 * @brief A implimentation file for the AGEMessageType class.
 *
 * This type was developed to provide an object to represent a message
 * of type AGE.
 */

#include "AGEMessageType.h"

/**
 * Construct a default AGEMessageType object.
 *
 * Construct a default AGEMessageType object with all the basic
 * initialization.
 */
AGEMessageType::AGEMessageType(void) : ZaurusMessageType((void *)"AGE")
{

}

/**
 * Destruct the AGEMessageType object.
 *
 * Destruct the AGEMessageType object by deallocating any dynamically
 * allocated momory.
 */
AGEMessageType::~AGEMessageType(void) {

}

/**
 * Obtain the total length of data.
 *
 * Obtain the length of data contained within all the AGE messages associated
 * with a specific RGE message.
 * @return The number of bytes of data contained in the message.
 */
unsigned long int AGEMessageType::GetTotDataLength(void) {
    unsigned char *pCont;
    unsigned long int *pDataLen;

    pCont = (unsigned char *)GetContent();

    if (IsLittleEndian())
	pDataLen = (unsigned long int *)pCont;
    else
	SwapByteOrder((const void *)pCont, (void *)&pDataLen,
		      sizeof(unsigned long int));

    return *pDataLen;
}

/**
 * Obtain the starting data length.
 *
 * Obtain the data lenght of the first AGE message recieved after sending an
 * RGE message. This function should ONLY be used on the first received AGE
 * message. All subsuquent AGE messages should use the GetDataLength()
 * function.
 * @return Length of data contained in the message, in bytes.
 */
unsigned short int AGEMessageType::GetStartDataLength(void) {
    return (GetContSize() - 4);
}

/**
 * Obtain the data length.
 *
 * Obtain the length of data contained within an AGE message subsuquent to the
 * first AGE message received after sending an RGE message. This function
 * should ONLY be used on subsuquent messages.
 * @return Lenght of data contained within message, in bytes.
 */
unsigned short int AGEMessageType::GetDataLength(void) {
    return GetContSize();
}

/**
 * Obtain the starting data.
 *
 * Obtain the data contained within the first AGE message recieved after
 * sending an RGE message. This function should ONLY be used on the first
 * received AGE message. All subsuquent AGE messages should use the GetData()
 * funciton.
 * @return A pointer to the beginning of the data contained in the message.
 */
void * AGEMessageType::GetStartData(void) {
    unsigned char *pCont;
    unsigned char *pData;

    pCont = (unsigned char *)GetContent();
    pData = (pCont + 4);
    return (void *)pData;
}

/**
 * Obtain the data.
 *
 * Obtain the data contained within an AGE message subsuquent to the first AGE
 * message received after sending an RGE message. This function should ONLY be
 * used on subsuquent messages.
 * @return A pointer to the beginning of the data contained in the message.
 */
void * AGEMessageType::GetData(void) {
    unsigned char *pCont;
    unsigned char *pData;

    pCont = (unsigned char *)GetContent();
    pData = pCont;
    return (void *)pData;
}
