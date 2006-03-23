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
 * @file RSYMessageType.cc
 * @brief An implimentation file for the RSYMessageType class.
 *
 * This type was developed to provide an object to represent a message
 * of type RSY.
 */

#include "RSYMessageType.h"

/**
 * Construct a default RSYMessageType object.
 *
 * Construct a default RSYMessageType object with all the basic
 * initialization.
 */
RSYMessageType::RSYMessageType(void) : DesktopMessageType((void *)"RSY")
{

}

/**
 * Construct a RSYMessageType object.
 *
 * Construct a RSYMessageType object given type, eliminating the need to
 * set the type of the message at a later time.
 * @param type The synchronization type.
 */
RSYMessageType::RSYMessageType(const unsigned char type) :
    DesktopMessageType((void *)"RSY")
{
    SetType(type);
}

/**
 * Destruct the RSYMessageType object.
 *
 * Destruct the RSYMessageType object by deallocating any dynamically
 * allocated momory.
 */
RSYMessageType::~RSYMessageType(void) {

}

/**
 * Set the sync type.
 *
 * Set the synchronization type that should be sent to the Zaurus via this
 * message.
 * @param type The synchronizate type.
 */
void RSYMessageType::SetType(const unsigned char type) {
    unsigned char buff[2];

    memcpy((void *)buff, (void *)&type, 1);
    buff[1] = 0x07;

    SetContent((void *)buff, 2);
}
