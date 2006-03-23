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
 * @file RRLMessageType.cc
 * @brief An implimentation file for the RRLMessageType class.
 *
 * This type was developed to provide an object to represent a message
 * of type RRL.
 */

#include "RRLMessageType.h"

/**
 * Construct a default RRLMessageType object.
 *
 * Construct a default RRLMessageType object with all the basic
 * initialization.
 */
RRLMessageType::RRLMessageType(void) : DesktopMessageType((void *)"RRL")
{

}

/**
 * Construct a RRLMessageType object.
 *
 * Construct a RRLMessageType object given password, eliminating the need to
 * set the password of the message at a later time.
 */
RRLMessageType::RRLMessageType(const char *passwd) :
    DesktopMessageType((void *)"RRL")
{
    SetPassword(passwd);
}

/**
 * Destruct the RRLMessageType object.
 *
 * Destruct the RRLMessageType object by deallocating any dynamically
 * allocated momory.
 */
RRLMessageType::~RRLMessageType(void) {

}

/**
 * Set the password.
 *
 * Set the password that is stored within this message object. If the password
 * is larger than 255 bytes only the first 255 bytes are used.
 * @param passwd Pointer to c-string containing the password.
 */
void RRLMessageType::SetPassword(const char *passwd) {
    unsigned char contBuf[256];

    // Set the first byte of the content to the length of the password.
    contBuf[0] = (unsigned char)strlen(passwd);

    // Copy the password into the following bytes of the contet buffer.
    memcpy((void *)&contBuf[1], (void *)passwd, (size_t)contBuf[0]);

    // Set the content of the RRL message to that which was just created in
    // the content buffer.
    SetContent((void *)contBuf, (unsigned int)contBuf[0] + 1);
}
