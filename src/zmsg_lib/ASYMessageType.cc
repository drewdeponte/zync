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
 * @file ASYMessageType.cc
 * @brief An implimentation file for the ASYMessageType class.
 *
 * This type was developed to provide an object to represent a message
 * of type ASY.
 */

#include "ASYMessageType.h"

/**
 * Construct a default ASYMessageType object.
 *
 * Construct a default ASYMessageType object with all the basic
 * initialization.
 */
ASYMessageType::ASYMessageType(void) : ZaurusMessageType((void *)"ASY")
{

}

/**
 * Destruct the ASYMessageType object.
 *
 * Destruct the ASYMessageType object by deallocating any dynamically
 * allocated momory.
 */
ASYMessageType::~ASYMessageType(void) {

}

/**
 * Obtain number of Sync IDs of new items.
 *
 * Obtain the number of Sync IDs of new items that are contained in the
 * message content. 
 * @return The number of Sync IDs of new items contained in the message.
 */
unsigned short int ASYMessageType::GetNumNewSyncIDs(void) {
    unsigned char *pCont;
    unsigned short int numSyncIDs;

    pCont = (unsigned char *)GetContent();

    if (IsLittleEndian())
	numSyncIDs = *((unsigned short int *)(pCont + 1));
    else
	SwapByteOrder((const void *)(pCont + 1), (void *)&numSyncIDs,
		      sizeof(unsigned short int));

    return numSyncIDs;
}

/**
 * Obtain number of Sync IDs of modified items.
 *
 * Obtain the number of Sync IDs of modified items that are contained in the
 * message content. 
 * @return The number of Sync IDs of modified items contained in the message.
 */
unsigned short int ASYMessageType::GetNumModSyncIDs(void) {
    unsigned char *pCont;
    unsigned short int numSyncIDs;

    pCont = (unsigned char *)GetContent();

    if (IsLittleEndian()) {
	numSyncIDs = *((unsigned short int *)(pCont + 3 +
					      (GetNumNewSyncIDs() * 4) + 1));
    } else {
	SwapByteOrder((const void *)(pCont + 3 + (GetNumNewSyncIDs() * 4) + 1),
		      (void *)&numSyncIDs, sizeof(unsigned short int));
    }

    return numSyncIDs;
}

/**
 * Obtain number of Sync IDs of deleted items.
 *
 * Obtain the number of Sync IDs of deleted items that are contained in the
 * message content. 
 * @return The number of Sync IDs of deleted items contained in the message.
 */
unsigned short int ASYMessageType::GetNumDelSyncIDs(void) {
    unsigned char *pCont;
    unsigned short int numSyncIDs;

    pCont = (unsigned char *)GetContent();

    if (IsLittleEndian()) {
	numSyncIDs = *((unsigned short int *)(pCont + 3 +
					  (GetNumNewSyncIDs() * 4) +
					  3 +
					  (GetNumModSyncIDs() * 4) +
					  1));
    } else {
	SwapByteOrder((const void *)(pCont + 3 + (GetNumNewSyncIDs() * 4) +
				     3 + (GetNumModSyncIDs() * 4) + 1),
		      (void *)&numSyncIDs, sizeof(unsigned short int));
    }

    return numSyncIDs;
}

/**
 * Obtain a Sync ID from the list of new item Sync IDs.
 *
 * Obtain a new Sync ID from the list of new Sync IDs given its index relative
 * to the list. Note, there is no bounds checking so if someone provides an
 * index beyond the bounds they will receive garbage values. The index value
 * starts at 0 being the first item index.
 * @param index An index, used to identify Sync IDs within the list of new
 * Sync IDs.
 * @return The value of the Sync ID associated with the given index in the
 * list of new Sync IDs.
 */
unsigned long int ASYMessageType::GetNewSyncID(unsigned short int index) {
    unsigned char *pCont;
    unsigned long int syncID;

    pCont = (unsigned char *)GetContent();

    if (IsLittleEndian())
	syncID = *((unsigned long int *)((pCont + 3) + (index * 4)));
    else
	SwapByteOrder((const void *)((pCont + 3) + (index * 4)),
		      (void *)&syncID, sizeof(unsigned long int));

    return syncID;
}

/**
 * Obtain a Sync ID from the list of modified item Sync IDs.
 *
 * Obtain a Sync ID from the list of modified item Sync IDs given its index
 * relative to the the list. Note, there is no bounds checking so if someone
 * provides an index beyond the bounds they will receive garbage values. The
 * index value starts at 0 being the first item index.
 * @param index An index, used to identify Sync IDs within the list of
 * modified Sync IDs.
 * @return The value of the Sync ID associated with the given index in the
 * list of modified Sync IDs.
 */
unsigned long int ASYMessageType::GetModSyncID(unsigned short int index) {
    unsigned char *pCont;
    unsigned long int syncID;

    pCont = (unsigned char *)GetContent();

    if (IsLittleEndian())
	syncID = *((unsigned long int *)((pCont + 3 +
					  (GetNumNewSyncIDs() * 4) +
					  3) +
					 (index * 4)));
    else
	SwapByteOrder((const void *)(pCont + 3 + (GetNumNewSyncIDs() * 4) + 3 +
				     (index * 4)),
		      (void *)&syncID, sizeof(unsigned long int));

    return syncID;
}

/**
 * Obtain a Sync ID from the list of deleted item Sync IDs.
 *
 * Obtain a Sync ID from the list of deleted item Sync IDs given its index
 * relative to the list. Note, there is no bounds checking so if someone
 * provides an index beyond the bounds they will receive garbage values. The
 * index value starts at 0 being the first item index.
 * @param index An index, used to identify Sync IDs within the list of deleted
 * Sync IDs.
 * @return The value of the Sync ID associated with the given index in the
 * list of deleted Sync IDs.
 */
unsigned long int ASYMessageType::GetDelSyncID(unsigned short int index) {
    unsigned char *pCont;
    unsigned long int syncID;

    pCont = (unsigned char *)GetContent();

    if (IsLittleEndian())
	syncID = *((unsigned long int *)((pCont + 3 +
					  (GetNumNewSyncIDs() * 4) +
					  3 + (GetNumModSyncIDs() * 4) + 3) + 
					 (index * 4)));
    else
	SwapByteOrder((const void *)(pCont + 3 + (GetNumNewSyncIDs() * 4) +
				     3 + (GetNumModSyncIDs() * 4) + 3 +
				     (index * 4)),
		      (void *)&syncID, sizeof(unsigned long int));

    return syncID;
}
