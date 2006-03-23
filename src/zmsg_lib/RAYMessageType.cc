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
 * @file RAYMessageType.cc
 * @brief An implimentation file for the RAYMessageType class.
 *
 * This type was developed to provide an object to represent a message
 * of type RAY.
 */

#include "RAYMessageType.h"

/**
 * Construct a default RAYMessageType object.
 *
 * Construct a default RAYMessageType object with all the basic
 * initialization.
 */
RAYMessageType::RAYMessageType(void) : DesktopMessageType((void *)"RAY")
{

}

/**
 * Destruct the RAYMessageType object.
 *
 * Destruct the RAYMessageType object by deallocating any dynamically
 * allocated momory.
 */
RAYMessageType::~RAYMessageType(void) {

}
