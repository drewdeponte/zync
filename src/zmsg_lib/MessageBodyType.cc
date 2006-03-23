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

#include <stdio.h>

/**
 * @file MessageBodyType.cc
 * @brief A implimentation file for the MessageBodyType class.
 *
 * This type was developed to provide a representation of the body of
 * messages. It provides means for accessing data that every body of a message
 * contains.
 */

#include "MessageBodyType.h"

/**
 * Constructs a default MessageBodyType object.
 *
 * Constructs a default MessageBodyType object by creating an object that
 * represents the body of a message.
 */
MessageBodyType::MessageBodyType(void) {
    // Set the pointer to the content to NULL since this is the default state.
    pContent = NULL;
}

/**
 * Destructs the MessageBodyType object.
 *
 * Destructs the MessageBodyType object by deallocating any dynamically
 * allocated memory.
 */
MessageBodyType::~MessageBodyType(void) {
    printf("Destructing a MessageBodyType object.\n");
    if (GetContent() != NULL) {
        printf("- Attempting to free the content.\n");
	    FreeContent();
        printf("- Freed the content.\n");
    }
    printf("Destructed a MessageBodyType object.\n");
}

/**
 * Set the message type.
 *
 * Set the type of the message.
 * @param msgType pointer to 3 bytes that are the message type.
 */
void MessageBodyType::SetType(void *msgType) {
    memcpy(type, msgType, MSG_TYPE_SIZE);
}

/**
 * Get the message type.
 *
 * Obtain the type of the message.
 * @return A pointer to the three bytes which combined create the message
 * type.
 */ 
void *MessageBodyType::GetType(void) {
    return type;
}

/**
 * Allocate memory for the content.
 *
 * Allocate memory for the contents of the message body.
 * @param size The size of the message content in bytes.
 * @return A integer representing success (zero) or failure (non-zero).
 * @retval 0 Allocated the memory for the content.
 * @retval -1 Failed to allocate the memory for the content.
 */
int MessageBodyType::AllocContent(unsigned short int size) {
    pContent = malloc(size);

    // If failed to allocate memory return in error.
    if (pContent == NULL)
	return -1;

    // Return 0 in succes.
    return 0;
}

/**
 * Get the message body content.
 *
 * Get the content contained inside the message body.
 * @return A pointer to the message content.
 */
void *MessageBodyType::GetContent(void) {
    return pContent;
}

/**
 * Free the message content.
 *
 * Free the memory allocated for the message content.
 */
void MessageBodyType::FreeContent(void) {
    if (GetContent() != NULL)
	free(pContent);
}
