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
 * @file MessageType.h
 * @brief A specifications file of the generic message type.
 * @author Andrew De Ponte
 *
 * A specifications for a class to represent all messages other than the
 * common messages.
 */

#ifndef MESSAGETYPE_H
#define MESSAGETYPE_H

#include "byteorder.h"

#include "MessageBodyType.h"

#include <limits.h>
#include <string.h>
#include <stdio.h>

#define MSG_HDR_SIZE 13

/**
 * @class MessageType
 * @brief A type representing a base for all message types.
 * 
 * The MessageType is a type that represents a base for all message types. It
 * handles storage of message header, message check sum, message body, and
 * message body size due to the fact that all messages contain such data.
 */
class MessageType {
 public:
    MessageType(void);
    MessageType(void *typeData);
    ~MessageType(void);

    void SetType(void *data);
    int SetContent(void *data, unsigned short int size);

    void *GetHeader(void);
    unsigned short int GetBodySize(void);
    void *GetContent(void);
    void *GetType(void);
    unsigned short int GetCheckSum(void);
    unsigned short int GetContSize(void);
    void SetHeader(void *data);
    void SetBodySize(unsigned short int size);
    void SetCheckSum(unsigned short int sum);
 private:
    char header[13];
    MessageBodyType body;
    unsigned short int body_size;
    unsigned short int check_sum;
    unsigned short int cont_size;
    int type_set_flag;
    int content_set_flag;
};

#endif
