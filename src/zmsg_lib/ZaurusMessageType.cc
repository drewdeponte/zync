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
 * @file ZaurusMessageType.cc
 * @brief A implimentation file for the ZaurusMessageType class.
 *
 * This type was developed to provide an object to represent a message
 * originating at the Zaurus.
 */

#include "ZaurusMessageType.h"

/**
 * Construct a default ZaurusMessageType object.
 *
 * Construct a default ZaurusMessageType object with all the basic
 * initialization.
 */
ZaurusMessageType::ZaurusMessageType(void *typeData) : MessageType(typeData) {
    char tmpHeader[] = {0x00, 0x00, 0x00, 0x00, 0x00, 0x96, 0x01, 0x01, 0x00,
			0xff, 0xff, 0xff, 0xff};
    SetHeader(tmpHeader);
}

/**
 * Destruct the ZaurusMessageType object.
 *
 * Destruct the ZaurusMessageType object by deallocating any dynamically
 * allocated momory.
 */
ZaurusMessageType::~ZaurusMessageType(void) {

}
