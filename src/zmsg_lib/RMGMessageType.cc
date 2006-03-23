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
 * @file RMGMessageType.cc
 * @brief An implimentation file for the RMGMessageType class.
 *
 * This type was developed to provide an object to represent a message
 * of type RMG.
 */

#include "RMGMessageType.h"

/**
 * Construct a default RMGMessageType object.
 *
 * Construct a default RMGMessageType object with all the basic
 * initialization.
 */
RMGMessageType::RMGMessageType(void) : DesktopMessageType((void *)"RMG")
{

}

/**
 * Construct a RMGMessageType object.
 *
 * Construct a RMGMessageType object given type, eliminating the need to
 * set the type of the message at a later time.
 * @param type The synchronization type.
 */
RMGMessageType::RMGMessageType(const unsigned char type)
: DesktopMessageType((void *)"RMG")
{
    SetType(type);
}

/**
 * Destruct the RMGMessageType object.
 *
 * Destruct the RMGMessageType object by deallocating any dynamically
 * allocated momory.
 */
RMGMessageType::~RMGMessageType(void) {

}

/**
 * Set the sync type.
 *
 * Set the synchronization type that should be sent to the Zaurus via this
 * message.
 * @param type The synchronizate type.
 */
void RMGMessageType::SetType(const unsigned char type) {
    unsigned char buff[2];

    buff[0] = 0x01;
    memcpy((void *)(buff + 1), (void *)&type, 1);

    SetContent((void *)buff, 2);
}
