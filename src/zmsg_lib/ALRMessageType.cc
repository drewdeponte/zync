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
 * @file ALRMessageType.cc
 * @brief A implimentation file for the ALRMessageType class.
 *
 * This type was developed to provide an object to represent a message
 * of type ALR.
 */

#include "ALRMessageType.h"

/**
 * Construct a default ALRMessageType object.
 *
 * Construct a default ALRMessageType object with all the basic
 * initialization.
 */
ALRMessageType::ALRMessageType(void) : ZaurusMessageType((void *)"ALR")
{

}

/**
 * Destruct the ALRMessageType object.
 *
 * Destruct the ALRMessageType object by deallocating any dynamically
 * allocated momory.
 */
ALRMessageType::~ALRMessageType(void) {

}

/**
 * Obtain number of Sync IDs.
 *
 * Obtain the number of Sync IDs contained in the message content.
 * @return The number of Sync IDs contained in the message content.
 */
unsigned short int ALRMessageType::GetNumSyncIDs(void) {
    unsigned char *pCont;
    unsigned short int numSyncIDs;

    pCont = (unsigned char *)GetContent();

    if (IsLittleEndian())
	numSyncIDs = *((unsigned short int *)pCont);
    else
	SwapByteOrder((const void *)pCont, (void *)&numSyncIDs,
		      sizeof(unsigned short int));

    return numSyncIDs;
}

/**
 * Obtain a Sync ID.
 *
 * Obtain a Sync ID from the message given its index relative to the
 * message. Note, there is no bounds checking so if someone provides an index
 * beyond the bounds they will receive garbage values. The index value starts
 * at 0 being the first item index.
 * @param index An index, used to identify Sync IDs within the message.
 * @return The value of the Sync ID associated with the given index.
 */
unsigned long int ALRMessageType::GetSyncID(unsigned short int index) {
    unsigned char *pCont;
    unsigned long int syncID;

    pCont = (unsigned char *)GetContent();

    if (IsLittleEndian())
	syncID = *((unsigned long int *)((pCont + 2) + (index * 4)));
    else
	SwapByteOrder((const void *)((pCont + 2) + (index * 4)),
		      (void *)&syncID, sizeof(unsigned long int));

    return syncID;
}
