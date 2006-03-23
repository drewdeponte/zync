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
 * @file MessageBodyType.h
 * @brief A specifications file of the generic message body type.
 * @author Andrew De Ponte
 *
 * A specifications for a class to represent all message bodies other than the
 * bodies found in the common messages because the common messages don't
 * follow the standard message format.
 */

#ifndef MESSAGEBODYTYPE_H
#define MESSAGEBODYTYPE_H

#include <string.h>
#include <stdlib.h>

#define MSG_TYPE_SIZE 3

/**
 * @class MessageBodyType
 * @brief A type representing the body of a message.
 *
 * The MessageBodyType is a type that represents the body of all messages. It
 * handles storage of the message type and the message content.
 */
class MessageBodyType {
 public:
    MessageBodyType(void);
    ~MessageBodyType(void);
    void SetType(void *msgType);
    void *GetType(void);
    int AllocContent(unsigned short int size);
    void *GetContent(void);
    void FreeContent(void);
 private:
    unsigned char type[3];
    void *pContent;
};

#endif
