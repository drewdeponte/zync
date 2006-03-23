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
 * @file RGEMessageType.cc
 * @brief A implimentation file for the RGEMessageType class.
 *
 * This type was developed to provide an object to represent a message
 * of type RGE.
 */

#include "RGEMessageType.h"

/**
 * Construct a default RGEMessageType object.
 *
 * Construct a default RGEMessageType object with all the basic
 * initialization.
 */
RGEMessageType::RGEMessageType(const char *path) : 
    DesktopMessageType((void *)"RGE")
{
    SetPath(path);
}

/**
 * Destruct the RGEMessageType object.
 *
 * Destruct the RGEMessageType object by deallocating any dynamically
 * allocated momory.
 */
RGEMessageType::~RGEMessageType(void) {

}

/**
 * Set the request path.
 *
 * Set the path that should be stored within this message object.
 * @param path Pointer to C-String containing the request path.
 */
void RGEMessageType::SetPath(const char *path) {
    unsigned char contBuf[256];
    unsigned short int pathSize;

    pathSize = (unsigned short int)strlen(path);

    // Set the first two bytes of the content to the length of the password.
    memcpy((void *)contBuf, (void *)&pathSize, 2);

    // Copy the password into the following bytes of the content buffer.
    memcpy((void *)&contBuf[2], (void *)path, (size_t)pathSize);

    // Set the content of the RGE message to that which was just created in
    // the content buffer.
    SetContent((void *)contBuf, pathSize + 2);
}
