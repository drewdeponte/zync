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
 * @file RDIMessageType.cc
 * @brief An implimentation file for the RDIMessageType class.
 *
 * This type was developed to provide an object to represent a message
 * of type RDI.
 */

#include "RDIMessageType.h"

/**
 * Construct a default RDIMessageType object.
 *
 * Construct a default RDIMessageType object with all the basic
 * initialization.
 */
RDIMessageType::RDIMessageType(void) : DesktopMessageType((void *)"RDI")
{
    buff[0] = 0x06;
    buff[1] = 0x07;
}

/**
 * Construct a RDIMessageType object.
 *
 * Construct a RDIMessageType object given type, eliminating the need to
 * set the type of the message at a later time.
 */
RDIMessageType::RDIMessageType(const unsigned char type)
    : DesktopMessageType((void *)"RDI")
{
    buff[0] = 0x06;
    buff[1] = 0x07;
    SetType(type);
}

/**
 * Destruct the RDIMessageType object.
 *
 * Destruct the RDIMessageType object by deallocating any dynamically
 * allocated momory.
 */
RDIMessageType::~RDIMessageType(void) {

}

/**
 * Set the sync type.
 *
 * Set the synchronization type that should be sent to the Zaurus via this
 * message.
 * @param type The synchronizate type.
 */
void RDIMessageType::SetType(const unsigned char type) {
    buff[0] = type;
    SetContent((void *)buff, 2);
}
