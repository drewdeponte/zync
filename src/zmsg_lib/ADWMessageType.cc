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
 * @file ADWMessageType.cc
 * @brief A implimentation file for the ADWMessageType class.
 *
 * This type was developed to provide an object to represent a message
 * of type ADW.
 */

#include "ADWMessageType.h"

/**
 * Construct a default ADWMessageType object.
 *
 * Construct a default ADWMessageType object with all the basic
 * initialization.
 */
ADWMessageType::ADWMessageType(void) : ZaurusMessageType((void *)"ADW")
{

}

/**
 * Destruct the ADWMessageType object.
 *
 * Destruct the ADWMessageType object by deallocating any dynamically
 * allocated momory.
 */
ADWMessageType::~ADWMessageType(void) {

}

/**
 * Obtain the synchronization ID.
 *
 * Obtain the synchronization ID that is stored in the content of message.
 * @return The synchronization ID stored in the message.
 */ 
unsigned long int ADWMessageType::GetSyncID(void) {
    unsigned char *pCont;
    unsigned long int syncId;

    pCont = (unsigned char *)GetContent();

    if (IsLittleEndian())
	syncId = *((unsigned long int *)(pCont + 6));
    else
	SwapByteOrder((const void *)(pCont + 6), (void *)&syncId,
		      sizeof(unsigned long int));

    return syncId;
}
