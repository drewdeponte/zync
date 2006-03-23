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
 * @file ADRMessageType.cc
 * @brief An implimentation file for the ADRMessageType class.
 *
 * This type was developed to provide an object to represent a message
 * of type ADR.
 */

#include "ADRMessageType.h"

#include <iostream>

/**
 * Construct a default ADRMessageType object.
 *
 * Construct a default ADRMessageType object with all the basic
 * initialization.
 */
ADRMessageType::ADRMessageType(void) : ZaurusMessageType((void *)"ADR")
{
    pCurContPos = NULL;
}

/**
 * Destruct the ADRMessageType object.
 *
 * Destruct the ADRMessageType object by deallocating any dynamically
 * allocated momory.
 */
ADRMessageType::~ADRMessageType(void) {

}

/**
 * Load the message Content.
 *
 * Load the content of the message so that the access functions may be used to
 * obtain the data from the content. Note: Due to the fact that the data
 * access functions pop data off the content this function may also be used to
 * reload the content so that the data may be obtained again.
 */
void ADRMessageType::LoadContent(void) {
    pCurContPos = (unsigned char *)GetContent();
    pCurContPos = (pCurContPos + 4);
}

/**
 * Obtain number of Items.
 *
 * Obtain the number of Items contained in the message content.
 * @return The number of Items contained in the message content.
 */
unsigned short int ADRMessageType::GetNumItems(void) {
    unsigned char *pCont;
    unsigned short int numItems;

    pCont = (unsigned char *)GetContent();

    if (IsLittleEndian())
	numItems = *((unsigned short int *)pCont);
    else
	SwapByteOrder((const void *)pCont, (void *)&numItems,
		      sizeof(unsigned short int));

    return numItems;
}

/**
 * Obtain number of Params.
 *
 * Obtain the number of parameters per item.
 * @return The number of parameters per item.
 */
unsigned short int ADRMessageType::GetNumParams(void) {
    unsigned char *pCont;
    unsigned short int numParams;

    pCont = (unsigned char *)GetContent();

    if (IsLittleEndian())
	numParams = *((unsigned short int *)(pCont + 2));
    else
	SwapByteOrder((const void *)(pCont + 2), (void *)&numParams,
		      sizeof(unsigned short int));

    return numParams;
}

/**
 * Get a DATA_ID_BIT.
 *
 * Get a DATA_ID_BIT parameter from the message content.
 * @return The value of the parameter.
 */
unsigned char ADRMessageType::GetBit(void) {
    unsigned char tmp;
    int retval;
    retval = GetData((void *)&tmp, sizeof(unsigned char));
    return tmp;
}

/**
 * Get a DATA_ID_TIME.
 *
 * Get a DATA_ID_TIME parameter from the message content.
 * @return The value of the parameter.
 */
time_t ADRMessageType::GetTime(void) {
    time_t tmp;
    unsigned char buff[5];
    int retval;
    retval = GetData((void *)buff, 5);
    if (retval == 0)
	tmp = 0;
    else
	tmp = ConvZDateTime(buff, retval);

    std::cout << "Converted Time (UTC): ";
    std::cout << asctime(gmtime((const time_t *)&tmp));

    return tmp;
}

/**
 * Get a DATA_ID_ULONG.
 *
 * Get a DATA_ID_ULONG parameter from the message content.
 * @return The value of the parameter.
 */
unsigned long int ADRMessageType::GetULong(void) {
    unsigned long int tmp;
    int retval;
    retval = GetData((void *)&tmp, sizeof(unsigned long int));
    if (IsBigEndian()) {
	unsigned long int tmp1;
	SwapByteOrder((const void *)&tmp, (void *)tmp1,
		      sizeof(unsigned long int));
	tmp = tmp1;
    }
    return tmp;
}

/**
 * Get a DATA_ID_BARRAY.
 *
 * Get a DATA_ID_BARRAY parameter from the message content.
 * @return The value of the parameter.
 */
std::string ADRMessageType::GetBarray(void) {
    std::string tmp;
    int retval;
    char buff[256];
    retval = GetData((void *)buff, 255);
    if (retval >= 0) {
	buff[retval] = '\0';
    } else {
	buff[0] = '\0';
    }
    tmp.assign((const char *)buff);
    return tmp;
}

/**
 * Get a DATA_ID_UTF8.
 *
 * Get a DATA_ID_UTF8 parameter from the message content.
 * @return The value of the parameter.
 */
std::string ADRMessageType::GetUTF8(void) {
    std::string tmp;
    int retval;
    char buff[256];
    retval = GetData((void *)buff, 255);
    if (retval >= 0) {
	buff[retval] = '\0';
    } else {
	buff[0] = '\0';
    }
    tmp.assign((const char *)buff);
    return tmp;
}

/**
 * Get a DATA_ID_UCHAR.
 *
 * Get a DATA_ID_UCHAR parameter from the message content.
 * @return The value of the parameter.
 */
unsigned char ADRMessageType::GetUChar(void) {
    unsigned char tmp;
    int retval;
    retval = GetData((void *)&tmp, sizeof(unsigned char));
    return tmp;
}

/**
 * Get a DATA_ID_WORD.
 *
 * Get a DATA_ID_WORD parameter from the message content.
 * @return The value of the parameter.
 */
unsigned short ADRMessageType::GetWord(void) {
    unsigned short int tmp;
    int retval;
    retval = GetData((void *)&tmp, sizeof(unsigned short int));
    return tmp;
}

/**
 * Obtain Attribute Size.
 *
 * Obtain the Size of the Attribute in bytes.
 * @return The size of the attribute parameter in bytes.
 */
unsigned long int ADRMessageType::GetAttrSize(void) {
    unsigned char *pCont;
    unsigned long int attrSize;

    pCont = (unsigned char *)GetContent();

    attrSize = *((unsigned long int *)(pCont + 2 + 2));

    return attrSize;
}

/**
 * Obtain Attribute.
 *
 * Obtain the value of the Attribute parameter.
 * @return The value of the attribute parameter.
 */
unsigned char ADRMessageType::GetAttribute(void) {
    unsigned char *pCont;
    unsigned char attribute;

    pCont = (unsigned char *)GetContent();

    attribute = *(pCont + 2 + 2 + 4);

    return attribute;
}

/**
 * Obtain Card Created Time Size.
 *
 * Obtain the Size of the Card Created Param in bytes.
 * @return The size of the card created parameter in bytes.
 */
unsigned long int ADRMessageType::GetCardCreatedTimeSize(void) {
    unsigned char *pCont;
    unsigned long int paramSize;

    pCont = (unsigned char *)GetContent();

    paramSize = *((unsigned long int *)(pCont + 8 + GetAttrSize()));

    return paramSize;
}

/**
 * Obtain the Card Created Time.
 *
 * Obtain the value of the Card Created Time parameter in the format described
 * in the ADR Message section dealing with the Card Created Date Time.
 * @param dest The dest buffer to store the Card Created Time in.
 * @param size The size of the dest buffer in bytes.
 * @return An integer representing success (zero) or failure (non-zero).
 * @return 0 Successfully obtained the Card Created Date Time.
 * @return 1 Failed, the buffer is too small to house the card created time.
 */
int ADRMessageType::GetCardCreatedTime(char *dest, unsigned short int size) {
    unsigned char *pCont;

    // Check to see if the buffer is too small.
    if (size < GetCardCreatedTimeSize())
	return 1;

    pCont = (unsigned char *)GetContent();

    memcpy((void *)dest,
	   (void *)(pCont + 8 + GetAttrSize() + 4),
	   GetCardCreatedTimeSize());

    return 0;
}

/**
 * Obtain Card Modified Time Size.
 *
 * Obtain the Size of the Card Modified Param in bytes.
 * @return The size of the card modified parameter in bytes.
 */
unsigned long int ADRMessageType::GetCardModifiedTimeSize(void) {
    unsigned char *pCont;
    unsigned long int paramSize;

    pCont = (unsigned char *)GetContent();

    paramSize = *((unsigned long int *)(pCont + 8 + GetAttrSize() + 4 + \
		      GetCardCreatedTimeSize()));

    return paramSize;
}

/**
 * Obtain the Card Created Time.
 *
 * Obtain the value of the Card Modified Time parameter in the format described
 * in the ADR Message section dealing with the Card Modified Date Time.
 * @param dest The dest buffer to store the Card Modified Time in.
 * @param size The size of the dest buffer in bytes.
 * @return An integer representing success (zero) or failure (non-zero).
 * @return 0 Successfully obtained the Card Modified Date Time.
 * @return 1 Failed, the buffer is too small to house the card modified time.
 */
int ADRMessageType::GetCardModifiedTime(char *dest, unsigned short int size) {
    unsigned char *pCont;

    // Check to see if the buffer is too small.
    if (size < GetCardModifiedTimeSize())
	return 1;

    pCont = (unsigned char *)GetContent();

    memcpy((void *)dest,
	   (void *)(pCont + 8 + GetAttrSize() + 4 + \
		    GetCardCreatedTimeSize() + 4),
	   GetCardModifiedTimeSize());

    return 0;
}

/**
 * Obtain Sync ID Size.
 *
 * Obtain the Size of the Card Synchronization ID in bytes.
 * @return The size of the card sync ID parameter in bytes.
 */
unsigned long int ADRMessageType::GetSyncIDSize(void) {
    unsigned char *pCont;
    unsigned long int paramSize;

    pCont = (unsigned char *)GetContent();

    paramSize = *((unsigned long int *)(pCont + 8 + GetAttrSize() + 4 +
					GetCardCreatedTimeSize() + 4 +
					GetCardModifiedTimeSize()));

    return paramSize;
}

/**
 * Obtain Sync ID.
 *
 * Obtain the Sync ID of che card.
 * @return The sync ID parameter of the card.
 */
unsigned long int ADRMessageType::GetSyncID(void) {
    unsigned char *pCont;
    unsigned long int paramSize;

    pCont = (unsigned char *)GetContent();

    paramSize = *((unsigned long int *)(pCont + 8 + GetAttrSize() + 4 +
					GetCardCreatedTimeSize() + 4 +
					GetCardModifiedTimeSize() + 4));

    return paramSize;
}


/**
 * Get generic data.
 *
 * Obtain generic data from the message. This function works like a que in
 * that once data is obtained using this function the next call will obtain
 * the data following this data. One can think of it as popping data off a
 * que. Given that this is a generic data function it is un-aware of the types
 * being obtained. Hence, it does not support byte order swapping for the
 * obtained data. This means that one should always worry about swapping the
 * byte order of the data after obtaining it.
 * @param dest Pointer to the destination buffer to store the data in.
 * @param len The lenght of the destination buffer in bytes.
 * @return An integer representing success (number of bytes stored) or failure
 * (a negative number).
 * @retval 0 Successfully obtained no data for the item.
 * @retval -1 Failed, due to failure to swap byte order of the item length.
 * @retval -2 Failed, due to destination buffer length being too small.
 */
int ADRMessageType::GetData(void *dest, unsigned long int len) {
    unsigned char *pCont;
    unsigned long int itemLen;

    std::cout << "Entered ADRMessageType::GetData().\n";

    pCont = pCurContPos;

    std::cout << "Set pCont to class scoped pCurContPos.\n";

    // Obtain the length of the following data in bytes.
    if (IsLittleEndian()) {
	std::cout << "Host is little endian.\n";
	itemLen = *((unsigned long int *)pCont);
	std::cout << "Obtained the item length.\n";
    } else {
	std::cout << "Host is big endian.\n";
	if(SwapByteOrder((const void *)pCont, (void *)&itemLen,
			 sizeof(unsigned long int))) {
	    return -1;
	}
	std::cout << "Swapped byte order of itemLen and stored it.\n";
    }

    std::cout << "Obtained lenght of following data.\n";
    std::cout << "LOFD = " << itemLen << ".\n";

    // Check to see if the destination buffer size is large enough to hold the
    // data that is about to be obtained.
    if (len < itemLen) {
	std::cout << "ADRMessageType::GetData() - len(" << len << ") is less" \
	    " than itemLen(" << itemLen << ").\n";
	return -2;
    }

    // Move the pointer to the beginning of this attributes data and copy it
    // to the destination buffer.
    pCont = pCont + sizeof(unsigned long int);
    memcpy(dest, pCont, itemLen);

    std::cout << "Coppied the item data.\n";

    // Increment the content pointer to following the data that was just
    // coppied so that we can use it set the class content pointer allowing
    // the next GetData call to work properly.
    pCont = pCont + itemLen;
    pCurContPos = pCont;

    std::cout << "Adjusted the pointers.\n";
    std::cout << "Exiting ADRMessageType::GetData().\n";

    return itemLen;
}

/**
 * Convert the Zaurus protocol packed time and date (DATA_ID_TIME).
 *
 * Convert the Zaurus protocol packed time and date into the number of seconds
 * since epoch (00:00:00 on January 1, 1970, Coordinate Universal Time
 * (UTC). Since the Zaurus protocol packed time and date is contained within 5
 * bytes, the data buffer should consist of 5 bytes of data and the length
 * should be set to 5 bytes.
 * @param data A pointer to buffer containing the Zaurus packed time and date.
 * @param len Length of the buf containing the Zaurus packed time and date.
 * @return Number of seconds elapsed since epoch, or a negative val in error.
 * @retval -1 Failed, The data pointer points to NULL.
 * @retval -2 Failed, The specified len value is NOT 5 bytes.
 * @retval -3 Failed, Failed in conversion from local time to calendar time.
 */
time_t ADRMessageType::ConvZDateTime(unsigned char *data,
				     unsigned short int len) {
    char *tz;
    struct tm tmpGMTTime;
    time_t secsSinceEpoch;
    unsigned short int year;
    unsigned short int month;
    unsigned short int monthDay;
    unsigned short int hour;
    unsigned short int mins;
    unsigned short int secs;

    // Check to make sure that the data pointer points to data rather than to
    // nothing. If it points to nothing then return in an error.
    if (!data)
	return -1;

    // Check to make sure that the specified length of the provided data is
    // equal to 5. If it is not return in error.
    if (len != 5)
	return -2;

    // Fewww, this was a huge pain in the butt to figure out. I was NOT aware
    // that when bit shifting in C++ it converts your native little endian
    // byte order to big endian byte order before the actual shifting and
    // reverts back to little endian when done with the shifting. The only
    // reason this matters is because we are not starting with values we are
    // starting with bytes of unknown values. Normally, when one is dealing
    // with shifting they do not have to worry about this because they can
    // just think of their value in big endian and shift logically letting the
    // shifting operator handle the actual conversion from little to big and
    // back to little.

    // Here, I do all the proper bit shifting to obtain the proper values. I
    // decided that it was better to do it using bit shifting rather than bit
    // fields because of portability issues with bit fields.
    memcpy((void *)&year, (void *)(data + 3), 2);
    year = year << 4;
    year = year >> 8;

    memcpy((void *)&month, (void *)(data + 2), 2);
    month = month << 4;
    month = month >> 12;

    memcpy((void *)&monthDay, (void *)(data + 2), 2);
    monthDay = monthDay << 8;
    monthDay = monthDay >> 11;

    memcpy((void *)&hour, (void *)(data + 1), 2);
    hour = hour << 5;
    hour = hour >> 11;

    memcpy((void *)&mins, (void *)(data + 1), 2);
    mins = mins << 10;
    mins = mins >> 10;

    memcpy((void *)&secs, (void *)data, 2);
    secs = secs << 8;
    secs = secs >> 10;

    // Here I fill in the tm structure instance with the data from the Zaurus
    // DATA_ID_TIME type so that I can convert the tm struct to the number of
    // seconds since epoch.
    tmpGMTTime.tm_sec = secs;
    tmpGMTTime.tm_min = mins;

    // Now, the date and time are in UTC when the Zaurus sends them
    // over as well as they should be sent to the Zaurus in UTC in most
    // cases. As you may have noticed, I handle this in the suggested portable
    // method described in the GNU extension timegm() man page. Basically, I
    // set the TZ environment variable to UTC before calling mktime() and
    // revert it back when I am done. This allows me to set the hours in the
    // tm struct as they are in UTC.
    tmpGMTTime.tm_hour = hour;

    tmpGMTTime.tm_mday = monthDay;
    tmpGMTTime.tm_mon = month - 1;
    tmpGMTTime.tm_year = year;

    // This may or may not work, I am not sure actually what setting it to
    // negative actually does other than notify mktime() that I have no idea
    // if it is day light savings or not. This may tell mktime() to figure it
    // out on its own (which is what I would like). Theoretically it should be
    // able to determin it based on the other fields, and hopefully it
    // does. Well, crap setting it to -1 didn't seem to work. The dates came
    // out all messed up. The date was a day ahead and still an hour behind
    // what it is on the Zaurus. Hmmm, I think the 2 bits in the time which I
    // thought to be garbage bits are actually bits used for the daylight
    // savings.
    tmpGMTTime.tm_isdst = 0;

    // Here I set the TZ environment variable to UTC or "" which is equivalent
    // to "UTC" and call tzset() to set the correct time zone and daylight
    // savings variables.
    tz = getenv("TZ");
    setenv("TZ", "", 1);
    tzset();

    std::cout << "Set the TZ environment variable to UTC.\n";

    // Given the broken down time make calendar time (seconds since epoch) out
    // of the local broken down time. If the mktime() function fails to do so
    // then return an error.
    secsSinceEpoch = mktime(&tmpGMTTime);

    std::cout << "Obtained Time(UTC): ";
    std::cout << asctime((const struct tm *)&tmpGMTTime);

    // Now, I reset the TZ environment variable back to it's original value if
    // it had one, or unset it if it didn't exist before.
    if (tz)
	setenv("TZ", tz, 1);
    else
	unsetenv("TZ");
    tzset();

    // Note, I do this check after the resetting of the environment
    // variable. This is due to the fact that I don't want to leave the TZ
    // environment variable in UTC even when it fails. I want it to fail in
    // the same state that TZ started in.
    if (secsSinceEpoch == -1) {
	std::cout << "ADRMessageType::ConvZDateTime() - mktime() failed.\n";
	return -3;
    }

    std::cout << "Reset the TZ env variable.\n";
    //std::cout << "ctime LocalTime: " << ctime((const time_t *)&secsSinceEpoch);
    //std::cout << "LocalTime: " << asctime(gmtime((const time_t *)&secsSinceEpoch));

    // Converting to local time in here is not good because some of the values
    // obtained from the Zaurus come over in the Zaurus local time rather than
    // UTC.
    //return mktime(localtime((const time_t *)&secsSinceEpoch));
    return secsSinceEpoch;
}
