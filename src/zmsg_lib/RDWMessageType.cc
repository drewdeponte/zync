/*
 * Copyright 2004, 2005 Andrew De Ponte
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
 * @file RDWMessageType.cc
 * @brief An implimentation file for the RDWMessageType class.
 *
 * This type was developed to provide an object to represent a message
 * of type RDW.
 */

#include "RDWMessageType.h"

/**
 * Construct a default RDWMessageType object.
 *
 * Construct a default RDWMessageType object with all the basic
 * initialization.
 */
RDWMessageType::RDWMessageType(void) : DesktopMessageType((void *)"RDW")
{
    pBuff = NULL;
    buffSize = 0;
    itemLenSize = 4;
}

/**
 * Destruct the RDWMessageType object.
 *
 * Destruct the RDWMessageType object by deallocating any dynamically
 * allocated momory.
 */
RDWMessageType::~RDWMessageType(void) {
    if (pBuff)
	free(pBuff);
}

/**
 * Initialize as the modification variation.
 *
 * Initialize the RDW message as the variation which is used to modify items
 * which already exist on the Zaurus.
 * @param type The type of synchronization which is being performed.
 * @param itemID The Zaurus based synchronization ID of the item to modify.
 * @return An integer representing success (zero) or failure (non-zero).
 * @retval 0 Successfully initialized the message.
 * @retval 1 Failed to allocate initial buffer.
 * @retval 2 Failed to swap byte order of unkown data.
 * @retval 3 Failed to swap byte order of synchronization ID.
 */
int RDWMessageType::InitAsMod(unsigned char type, unsigned long int itemID) {
    int retval;

    pBuff = (unsigned char *)realloc(pBuff, 23);
    if (pBuff == NULL)
	return 1;

    pBuff[0] = type;
    if (IsBigEndian()) {
	unsigned short int tmp = 1;
	retval = SwapByteOrder((const void *)&tmp, (void *)(pBuff + 1),
			       sizeof(unsigned short int));
	if (retval)
	    return 2;
	retval = SwapByteOrder((const void *)&itemID, (void *)(pBuff + 3),
			       sizeof(unsigned long int));
	if (retval)
	    return 3;
    } else {
	*((unsigned short int *)(pBuff + 1)) = 1;
	*((unsigned long int *)(pBuff + 3)) = itemID;
    }
    memset((void *)(pBuff + 7), 0xff, 16);

    buffSize = 23;

    return 0;
}

/**
 * Initialize as the Obtain Sync ID variation.
 *
 * Initialize the RDW message as the variation which is used to obtain a
 * synchronization ID for items which are going to be added to the Zaurus.
 * @param type The type of synchronization which is being performed.
 * @return An integer representing success (zero) or failure (non-zero).
 * @retval 0 Successfully initialized the message.
 * @retval 1 Failed to allocate initial buffer.
 * @retval 2 Failed to swap byte order of unkown data.
 */
int RDWMessageType::InitAsObt(unsigned char type) {
    int retval;

    pBuff = (unsigned char *)realloc(pBuff, 7);
    if (pBuff == NULL)
	return 1;

    pBuff[0] = type;
    if (IsBigEndian()) {
	unsigned short int tmp = 1;
	retval = SwapByteOrder((const void *)&tmp, (void *)(pBuff + 1),
			       sizeof(unsigned short int));
	if (retval)
	    return 2;
    } else {
	*((unsigned short int *)(pBuff + 1)) = 1;
    }
    memset((void *)(pBuff + 3), 0x00, 4);

    buffSize = 7;

    return 0;
}

/**
 * Initialize as the New Item variation.
 *
 * Initialize the RDW message as the variation which is used to set the data
 * of a new item which is being added to the Zaurus.
 * @param type The type of synchronization which is being performed.
 * @return An integer representing success (zero) or failure (non-zero).
 * @retval 0 Successfully initialized the message.
 * @retval 1 Failed to allocate initial buffer.
 * @retval 2 Failed to swap byte order of unkown data.
 */
int RDWMessageType::InitAsNew(unsigned char type) {
    int retval;
    retval = InitAsObt(type);
    return retval;
}

/**
 * Append a DATA_ID_BIT.
 *
 * Append a DATA_ID_BIT to the message.
 * @param data The data to append to the message.
 * @return An integer representing success (zero) or failure (non-zero).
 * @retval 0 Successfully appended the data.
 * @retval 1 Failed to append the data.
 */
int RDWMessageType::AppendBit(unsigned char data) {
    if (AppendData((const unsigned char *)&data, 1))
	return 1;

    return 0;
}

/**
 * Append a DATA_ID_TIME.
 *
 * Append a DATA_ID_TIME to the message.
 * @param data The data to append to the message.
 * @return An integer representing success (zero) or failure (non-zero).
 * @retval 0 Successfully appended the data.
 * @retval 1 Failed to append the data, due to failure in conversion.
 * @retval 2 Failed to append the data.
 */
int RDWMessageType::AppendTime(time_t data) {
    unsigned short dataLen = 5;
    unsigned char buff[dataLen];

    if (data) {
	if (ConvCalTime(data, buff, dataLen))
	    return 1;
    } else {
	dataLen = 0;
    }

    if (AppendData((const unsigned char *)buff, dataLen))
	return 2;

    return 0;
}

/**
 * Append a DATA_ID_ULONG.
 *
 * Append a DATA_ID_ULONG to the message.
 * @param data The data to append to the message.
 * @return An integer representing success (zero) or failure (non-zero).
 * @retval 0 Successfully appended the data.
 * @retval 1 Failed to append the data, failed to swap data byte order.
 * @retval 2 Failed to append the data.
 */
int RDWMessageType::AppendULong(unsigned long int data) {
    unsigned long int tmp;

    if (IsBigEndian()) {
	if (SwapByteOrder((const void *)data, (void *)&tmp,
			  sizeof(unsigned long int)))
	    return 1;
    } else {
	tmp = data;
    }

    if (AppendData((const unsigned char *)&tmp, sizeof(unsigned long int)))
	return 2;

    return 0;
}

/**
 * Append a DATA_ID_BARRAY.
 *
 * Append a DATA_ID_BARRAY to the message.
 * @param data The data to append to the message.
 * @return An integer representing success (zero) or failure (non-zero).
 * @retval 0 Successfully appended the data.
 * @retval 1 Failed to append the data.
 */
int RDWMessageType::AppendBarray(std::string data) {
    if (AppendData((const unsigned char *)data.c_str(), data.size()))
	return 1;

    return 0;
}

/**
 * Append a DATA_ID_UTF8.
 *
 * Append a DATA_ID_UTF8 to the message.
 * @param data The data to append to the message.
 * @return An integer representing success (zero) or failure (non-zero).
 * @retval 0 Successfully appended the data.
 * @retval 1 Failed to append the data.
 */
int RDWMessageType::AppendUTF8(std::string data) {
    if (AppendData((const unsigned char *)data.c_str(), data.size()))
	return 1;

    return 0;
}

/**
 * Append a DATA_ID_UCHAR.
 *
 * Append a DATA_ID_UCHAR to the message.
 * @param data The data to append to the message.
 * @return An integer representing success (zero) or failure (non-zero).
 * @retval 0 Successfully appended the data.
 * @retval 1 Failed to append the data.
 */
int RDWMessageType::AppendUChar(unsigned char data) {
    if (AppendData((const unsigned char *)&data, 1))
	return 1;

    return 0;
}

/**
 * Append a DATA_ID_WORD.
 *
 * Append a DATA_ID_WORD to the message.
 * @param data The data to append to the message.
 * @return An integer representing success (zero) or failure (non-zero).
 * @retval 0 Successfully appended the data.
 * @retval 1 Failed to append the data, failed to swap data byte order.
 * @retval 2 Failed to append the data.
 */
int RDWMessageType::AppendWord(unsigned short int data) {
    unsigned short int tmp;

    if (IsBigEndian()) {
	if (SwapByteOrder((const void *)&data, (void *)&tmp,
			  sizeof(unsigned short int)))
	    return 1;
    } else {
	tmp = data;
    }

    if (AppendData((const unsigned char *)&tmp, sizeof(unsigned short int)))
	return 2;

    return 0;
}

/**
 * Commit the built content.
 *
 * Commit the content that has been built prior to this point of calling to
 * the message so that inherited functions may be used and so that other
 * utility functions may be used on it. For details on return values meanings
 * refer to the SetContent member function of the DesktopMessageType.
 * @return An integer representing success (zero) or failure (non-zero).
 */
int RDWMessageType::CommitContent(void) {
    int retval;
    retval = SetContent((void *)pBuff, buffSize);
    return retval;
}

/**
 * Append generic data.
 *
 * Append generic data to the message. This function handles the memory
 * realocation as well the formatting for data. This function does NOT handle
 * byte order swapping for the data however because it is un-aware of the
 * types being appended. Hence, one should perform byte order swapping prior
 * to calling this function.
 * @param data Pointer to the data to append to the message.
 * @param len Length, in bytes, of the data to append to the message.
 * @return An integer representing success (zero) or failure (non-zero).
 * @retval 0 Successfully appended the data.
 * @retval 1 Failed, due to failure to reallocate the buffer.
 * @retval 2 Failed, due to failure to swap byte order of item length. The
 * RDWMessageType object is now in a broken state.
 */
int RDWMessageType::AppendData(const unsigned char *data,
			       unsigned long int len) {
    int retval = 0;
    unsigned char *pTempBuff = NULL;
    unsigned char *pItemData = NULL;

    // First things first, I attempt to reallocate the buffer which holds the
    // data for the message content before it has been commited. If it fails
    // then I return in an error. If it succeeds then I append the length of
    // the following data (being handled for byte order) and append the data
    // to the buffer. Note: The data portion is not handled for byte order due
    // to the fact that the type of the data is unknown. Hence, byte order
    // must be handled prior to being passed to this function.
    pTempBuff = (unsigned char *)realloc((void *)pBuff,
					 (buffSize + (len + itemLenSize)));
    if (pTempBuff) {
	pItemData = ((unsigned char *)pTempBuff) + buffSize;

	// Set the item data size, handeling host byte order.
	if (IsBigEndian()) {
	    if (SwapByteOrder((const void *)len, (void *)pItemData,
			      sizeof(unsigned long int)))
		retval = 2;
	} else {
	    *((unsigned long int *)pItemData) = len;
	}

	// Set the actual item data in the message.
	if (len)
	    memcpy((void *)(pItemData + itemLenSize), (const void *)data,
		   (size_t)len);

	// Update the bytes counter for the buffer so that it is correct since
	// I appended the data to it.
	buffSize = buffSize + (len + itemLenSize);

	// Update the buffer pointer of the message to point to the
	// reallocated memory in case realloc had to move the data to find
	// enough memory in sequence.
	pBuff = pTempBuff;	
    } else {
	retval = 1;
    }

    return retval;
}

/**
 * Convert the C calendar time and date (time_t - secs since epoch).
 *
 * Convert the C calendar time and date (time_t - secs since epoch) into the
 * Zaurus protocol packed time and date (DATE_ID_TIME).
 * @param calTime The calTime (secs since epoch) to convert into Zaurus packed
 * time and date.
 * @param dest A pointer to buffer that should contain the Zaurus packed time
 * and date (must be 5 bytes long).
 * @param len Length of the buf that should contain the Zaurus packed time and
 * date.
 * @return An integer representing success (zero) or failure (non-zero).
 * @retval 0 Successfully converted the calendar time to Zaurus packed time
 * and date.
 * @retval 1 Failed due to len NOT being 5 bytes in length.
 * @retval 2 Failed, due to an error in converting calendar time to broken
 * down time.
 */
int RDWMessageType::ConvCalTime(time_t calTime, unsigned char *dest,
				unsigned short int len) {
    struct tm brkDwnTime;
    unsigned short int *tmpPtr;
    unsigned short int month;
    unsigned short int hour;
    unsigned short int min;
    unsigned short int sec;

    // Check to make sure that the len is 5 bytes long.
    if (len != 5)
	return 1;

    // Here I convert the calendar time into broken down time so that I can
    // easily access the different broken down members for storage in the
    // Zaurus protocols packed format.
    if (gmtime_r(&calTime, &brkDwnTime) == NULL) {
	return 2;
    }

    // Here I set all the bytes that are going to be used to store the Zaurus
    // time and date data to zero so that there are no problems with the
    // following inclusive ORs with garbage data.
    memset((void *)dest, 0x00, 5);

    // Here I do some bitwise operations to pack the year and month into the
    // proper bytes for the Zaurus protocols date and time format.

    // Here, I assign a temporary unsigned short pointer to point to the last
    // two bytes of the data array.
    tmpPtr = (unsigned short int *)(dest + 3);
    // Then I set the last to bytes equal to the proper value from the broken
    // down time.
    *tmpPtr = brkDwnTime.tm_year;
    // I then shift the year value four bits to the left (visually in big
    // endian) to make room for the month.
    (*tmpPtr) = (*tmpPtr) << 4;
    // Here, I set unsigned short int variable (month) equal to the proper
    // value to store in the month portion of the packed data. I then take and
    // bitwise inclusive OR the month value and the data that holds the year
    // to get the proper packed data format for the last to bytes of the data.
    month = brkDwnTime.tm_mon + 1;
    (*tmpPtr) = (*tmpPtr) | month;

    // Here I do some bitwise operations to pack hour, min, and month day into
    // the proper bytes for the Zaurus protocol date and time format.

    // Here, I assign a temporary unsigned short pointer to point to the
    // second to last, 2 bytes of the data array.
    tmpPtr = (unsigned short int *)(dest + 1);
    // Then I set the last bytes equal to the proper value from the broken
    // down time for the month day.
    *tmpPtr = brkDwnTime.tm_mday;
    // I then shift the month day value 11 bits to the left (visually in big
    // endian) to make room for the hour and min entries.
    (*tmpPtr) = (*tmpPtr) << 11;
    // Here, I set unsigned short int variable (hour) equal to the proper
    // value to store in the hour portion of the packed data. I then shift it
    // the proper number of bits and do an inclusive bitwise OR to move the
    // bits into the proper location in the data.
    /*
    if (brkDwnTime.tm_hour < 16)
	hour = brkDwnTime.tm_hour + 8;
    else
	hour = brkDwnTime.tm_hour - 16;
    */
    hour = brkDwnTime.tm_hour;
    hour = hour << 6;
    (*tmpPtr) = (*tmpPtr) | hour;
    // Here, I set unsigned short int variable (min) equal to the proper value
    // to store in the min portion of the packed data. I then inclusive OR it
    // into the data so that it is in the proper location in the data.
    min = brkDwnTime.tm_min;
    (*tmpPtr) = (*tmpPtr) | min;

    // Here I do some bitwise operations to pack the secs into the proper byte
    // for the Zaurus protocol date and time format.

    // Here, I assign a temporary unsigned short pointer to point to the first
    // two bytes of the data array.
    tmpPtr = (unsigned short int *)(dest);
    // Then I set unsigned short int variable (sec) equal to the proper value
    // to store in the sec portion of the packed data. I then shift it the
    // proper number of bits and do an inclusive OR to move the bits into the
    // proper location in te data.
    sec = brkDwnTime.tm_sec;
    sec = sec << 2;
    (*tmpPtr) = (*tmpPtr) | sec;

    // At this point all the proper information has been packed into the
    // proper format.

    return 0;
}
