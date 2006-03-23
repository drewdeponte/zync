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

/**
 * @file MessageType.cc
 * @brief A implimentation file for the MessageType class.
 *
 * This type was developed to provide a base for messages to inherit and grow
 * from. It provides means for accessing data that every message regardless of
 * type contains.
 */

#include "MessageType.h"

/**
 * Construct a default MessageType object.
 *
 * Construct a MessageType object in the default state. That is the content,
 * type, etc. have yet to be set.
 */
MessageType::MessageType(void) {
    body_size = 0;
    check_sum = 0;
    cont_size = 0;
    type_set_flag = 0;
    content_set_flag = 0;
}

/**
 * Construct a MessageType object.
 *
 * Construct a MessageType object given the 3 byte message type.
 * @param data The 3 bytes that are the message header.
 */
MessageType::MessageType(void *data) {
    body_size = 0;
    check_sum = 0;
    cont_size = 0;
    type_set_flag = 0;
    content_set_flag = 0;

    SetType(data);
}

MessageType::~MessageType(void) {
    printf("Destructed a MessageType object.\n");
}

/**
 * Set the message type.
 *
 * Set the type of the message given the data to use as the message type. The
 * data should consist of 3 bytes which are the message type. If less than
 * three bytes of data are given the type will be partially garbage. If the
 * data contains more than 3 bytes only the first three bytes of the data are
 * used as the message type.
 * @param data A pointer to the data to use as the message type.
 */
void MessageType::SetType(void *data) {
    int i;
    unsigned char *type;
    unsigned char *origType;

    // If the type has been set previous to this setting then the check sum
    // has also been set for the type, due to the fact that it is altered
    // inside this function. So, I revert the check sum to its previous state
    // before setting the state again and then allow for setting of the state
    // and recalc of the check sum as would normally occur if the type was not
    // previously set.
    if (type_set_flag) {
	origType = (unsigned char *)body.GetType();
	for (i = 0; i < MSG_TYPE_SIZE; i++) {
	    check_sum = check_sum - origType[i];
	}
    }

    // Set the type within the body sub section.
    body.SetType(data);

    type = (unsigned char *)body.GetType();

    // Calculate the message type portion of the check_sum and set it.
    for (i = 0; i < MSG_TYPE_SIZE; i++) {
	check_sum = check_sum + type[i];
    }

    // Increment the body size of the message by the size of the message type
    // (3). Note: This only happens if the message type has not previously
    // been set. If it was previously set then we are just over writing the
    // value that was there before hence we are not changing the size of the
    // body of the message.
    if (!type_set_flag) {
	body_size = body_size + MSG_TYPE_SIZE;
    }

    // Set the flag to signify that the type has been set.
    type_set_flag = 1;
}

/**
 * Set the message content.
 *
 * Set the message content given the content size and a pointer to the
 * content. If the specified content size is larger then the actual data
 * passed the data will consist of some garbage. If the size is less than the
 * actual size of the data then only the number of bytes specified by the size
 * are set as the content. If the size is zero then it resets the content to a
 * state as if it were never set.
 * @return An integer representing success (zero) or failure (non-zero).
 * @retval 0 Set the message content.
 * @retval 1 Failed due to size being to large.
 * @retval 2 Failed to allocte memory for the content.
 */
int MessageType::SetContent(void *data, unsigned short int size) {
    void *cont;
    unsigned char *buff;
    int i;

    cont = NULL;
    buff = NULL;

    // Check the bounds to make sure that the content is with in the
    // appropriate size. The 3 is subtracted for the the message type while
    // the unsigned short is used as the storage for the body size. Hence,
    // the entire body size is limited to the max of an unsigned short
    // int. Hence, the max content size is the max body size minus the 3 bytes
    // for the message type.
    if (size > (USHRT_MAX - MSG_TYPE_SIZE))
	return 1;

    // Check if the content has previously been set, if so then revert the
    // check sum to a status before the check sum was set. I also revert the
    // body size to a status as if the check sum was not set. This should
    // allow for a calculation and setting of the body size appropriately
    // later.
    if (content_set_flag) {
	buff = (unsigned char *)body.GetContent();
	for (i = 0; i < cont_size; i++) {
	    check_sum = check_sum - buff[i];
	}

	body_size = body_size - cont_size;

	body.FreeContent();

	cont_size = 0;

	content_set_flag = 0;
    }

    // At this point I know that the passed size is between 0 and the maximum
    // content size. If the size is zero then I know that the user is trying
    // to set the content to nothing or rather trying to empty out the content
    // of the message. The data should by standard be set to NULL even though
    // it is NOT used for anything in this case. In this case if the content
    // has previously been set then the code above handled putting the check
    // sum and the body size back to the proper place and I need to do nothing
    // other then return 0 in success and set the content_set_flag to a state
    // of not set. If the content was not set before then I don't want to
    // change either the content size, body size, or the check sum becase they
    // are already in the proper state.
    if (size == 0) {
	return 0;
    }

    // Allocate the memory for the content.
    if (body.AllocContent(size) != 0)
	return 2;

    // Fill the newly allocated memory with the content.
    cont = body.GetContent();
    memcpy(cont, data, size);

    buff = (unsigned char *)cont;

    // Set the content size.
    cont_size = size;
    
    // Set the body size
    body_size = body_size + cont_size;

    // Calculate and set the message content portion of the check sum.
    for (i = 0; i < cont_size; i++) {
	check_sum = check_sum + buff[i];
    }

    content_set_flag = 1;

    // Return in success.
    return 0;
}

/**
 * Get the message header.
 *
 * Obtain the header of the message object. The data should be MSG_HDR_SIZE
 * bytes long.
 * @return A pointer to the message header.
 */
void *MessageType::GetHeader(void) {
    return (void *)header;
}

/**
 * Get the message body size.
 *
 * Obtain the size of the message body.
 * @return The size of the body in bytes.
 */
unsigned short int MessageType::GetBodySize(void) {
    return body_size;
}

/**
 * Get the message content.
 *
 * Get the content contained inside the message.
 * @return A pointer to message content.
 */
void *MessageType::GetContent(void) {
    return body.GetContent();
}

/**
 * Get the message type.
 *
 * Get the type of the message.
 * @return A pointer to the message type.
 */
void *MessageType::GetType(void) {
    return body.GetType();
}

/**
 * Get the message check sum.
 *
 * Get the check sum of the message.
 * @return The value of the message check sum.
 */
unsigned short int MessageType::GetCheckSum(void) {
    return check_sum;
}

/**
 * Get the content size.
 *
 * Obtain the size of the content. If the content of the message was not set
 * then the value returned is not going to be correct.
 * @return The size of the message content in bytes.
 */
unsigned short int MessageType::GetContSize(void) {
    return cont_size;
}

/**
 * Set the message header.
 *
 * Set the message header given a pointer to 13 bytes of dat to set the header
 * to.
 * @param data Pointer to 13 bytes of data to set header to.
 */
void MessageType::SetHeader(void *data) {
    memcpy(header, data, MSG_HDR_SIZE);    
}

/**
 * Set the message body size.
 *
 * Set the message body size.
 * @param size The size in bytes to set the body size to.
 */
void MessageType::SetBodySize(unsigned short int size) {
    body_size = size;
}

/**
 * Set the message check sum.
 *
 * Set the check sum of the message.
 * @param sum The value to set the check sum to.
 */
void MessageType::SetCheckSum(unsigned short int sum) {
    check_sum = sum;
}
