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
 * @file DesktopMessageType.cc
 * @brief A implimentation file for the DesktopMessageType class.
 *
 * This type was developed to provide an object to represent a message
 * originating at the Desktop.
 */

#include "DesktopMessageType.h"

/**
 * Construct a default DesktopMessageType object.
 *
 * Construct a default DesktopMessageType object with all the basic
 * initialization.
 */
DesktopMessageType::DesktopMessageType(void *typeData) : MessageType(typeData)
{
    char tmpHeader[] = {0x00, 0x00, 0x00, 0x00, 0x00, 0x96, 0x01, 0x01, 0x0c,
			0x00, 0x00, 0x00, 0x00};
    SetHeader(tmpHeader);
}

/**
 * Destruct the DesktopMessageType object.
 *
 * Destruct the DesktopMessageType object by deallocating any dynamically
 * allocated momory.
 */
DesktopMessageType::~DesktopMessageType(void) {

}

int DesktopMessageType::SetContent(void *data, unsigned short int size) {
    MessageType::SetContent(data, size);
    char tmpHeader[] = {0x00, 0x00, 0x00, 0x00, 0x00, 0x96, 0x01, 0x01, 0x0c,
			0x00, 0x00, 0x00, 0x00};
    *((unsigned short int *)(&tmpHeader[9])) = GetContSize();
    SetHeader(tmpHeader);
    return 0;
}
