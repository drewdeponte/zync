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
 * @file RDSMessageType.cc
 * @brief An implimentation file for the RDSMessageType class.
 *
 * This type was developed to provide an object to represent a message
 * of type RDS.
 */

#include "RDSMessageType.h"

/**
 * Construct a default RDSMessageType object.
 *
 * Construct a default RDSMessageType object with all the basic
 * initialization.
 */
RDSMessageType::RDSMessageType(void) : DesktopMessageType((void *)"RDS")
{

}

/**
 * Construct a RDSMessageType object.
 *
 * Construct a RDSMessageType object given type, eliminating the need to
 * set the type of the message at a later time.
 */
RDSMessageType::RDSMessageType(const unsigned char type)
: DesktopMessageType((void *)"RDS")
{
    SetType(type);
}

/**
 * Destruct the RDSMessageType object.
 *
 * Destruct the RDSMessageType object by deallocating any dynamically
 * allocated momory.
 */
RDSMessageType::~RDSMessageType(void) {

}

/**
 * Set the sync type.
 *
 * Set the synchronization type that should be sent to the Zaurus via this
 * message.
 * @param type The synchronizate type.
 */
void RDSMessageType::SetType(const unsigned char type) {
    unsigned char buff[4];
    memcpy((void *)buff, (void *)&type, 1);
    buff[1] = 0x07;
    buff[2] = 0x00;
    buff[3] = 0x00;
    SetContent((void *)buff, 4);
}
