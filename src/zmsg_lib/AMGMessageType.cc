/*
 * Copyright 2004,2005 Andrew De Ponte
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
 * @file AMGMessageType.cc
 * @brief A implimentation file for the AMGMessageType class.
 *
 * This type was developed to provide an object to represent a message
 * of type AMG.
 */

#include "AMGMessageType.h"

#include <stdio.h>

/**
 * Construct a default AMGMessageType object.
 *
 * Construct a default AMGMessageType object with all the basic
 * initialization.
 */
AMGMessageType::AMGMessageType(void) : ZaurusMessageType((void *)"AMG")
{

}

/**
 * Destruct the AMGMessageType object.
 *
 * Destruct the AMGMessageType object by deallocating any dynamically
 * allocated momory.
 */
AMGMessageType::~AMGMessageType(void) {

}

/**
 * Check if content is an empty log.
 *
 * Check if the content of the message is an empty log.
 * @return An int representing a boolean value, true (non-zero), false (zero).
 */
int AMGMessageType::IsEmptyLog(void) {
    if (GetCheckSum() == 213) {
	return 1;
    }

    return 0;
}

/**
 * Check if the Todo synchronization flag has been set.
 *
 * Check if the Todo synchronization requires Slow Sync flag has been
 * set.
 * @return An integer representing true or false.
 * @retval 0 A Slow Sync has already been performed.
 * @retval 1 A Slow Sync has not been performed yet.
 */
int AMGMessageType::IsEmptyTodo(void) {
    unsigned char *pContent = NULL;
    pContent = (unsigned char *)GetContent();
    if (pContent[2] & TODO_ID) {
        return 0;
    } else {
        printf("AMGMessageType:IsEmptyTodo() - Requiers FULL SYNC.\n");
        return 1;
    }
}

/**
 * Check if the Calendar sync flag has been set.
 *
 * Check if the Calendar sync requires Slow Sync flag has been set.
 * @return An integer representing true or false.
 * @retval 0 A Slow Sync has already been performed.
 * @retval 1 A Slow Sync has NOT been performed yet.
 */
int AMGMessageType::IsEmptyCalendar(void) {
    unsigned char *pContent = NULL;
    pContent = (unsigned char *)GetContent();
    if (pContent[2] & CALENDAR_ID) {
        return 0;
    } else {
        return 1;
    }
}

/**
 * Check if the AddressBook sync flag has been set.
 *
 * Check if the AddressBook sync requires Slow Sync flag has been set.
 * @return An integer representing true or false.
 * @retval 0 A Slow Sync has already been performed.
 * @retval 1 A Slow Sync has NOT been performed yet.
 */
int AMGMessageType::IsEmptyAddressBook(void) {
    unsigned char *pContent = NULL;
    pContent = (unsigned char *)GetContent();
    if (pContent[2] & ADDRBOOK_ID) {
        return 0;
    } else {
        return 1;
    }
}
