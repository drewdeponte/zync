/*
 * Copyright 2005 Andrew De Ponte
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
 * @file RSSMessageType.cc
 * @brief An implimentation file for the RSSMessageType class.
 *
 * This type was developed to provide an object to represent a message
 * of type RSS.
 */

#include "RSSMessageType.h"

/**
 * Construct a default RSSMessageType object.
 *
 * Construct a default RSSMessageType object with all the basic
 * initialization.
 */
RSSMessageType::RSSMessageType(void) : DesktopMessageType((void *)"RSS")
{

}

/**
 * Construct a RSSMessageType object.
 *
 * Construct a RSSMessageType object given type, eliminating the need to
 * set the type of the message at a later time.
 */
RSSMessageType::RSSMessageType(const unsigned char type)
: DesktopMessageType((void *)"RSS")
{
    SetType(type);
}

/**
 * Destruct the RSSMessageType object.
 *
 * Destruct the RSSMessageType object by deallocating any dynamically
 * allocated momory.
 */
RSSMessageType::~RSSMessageType(void) {

}

/**
 * Set the sync type.
 *
 * Set the synchronization type that should be sent to the Zaurus via this
 * message.
 * @param type The synchronizate type.
 */
void RSSMessageType::SetType(const unsigned char type) {
    unsigned char buff[3];
    buff[0] = 0x01;
    memcpy((void *)(buff + 1), (void *)&type, 1);
    buff[2] = 0x01;
    SetContent((void *)buff, 3);
}
