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
 * @file RTGMessageType.cc
 * @brief An implimentation file for the RTGMessageType class.
 *
 * This type was developed to provide an object to represent a message
 * of type RTG.
 */

#include "RTGMessageType.h"

/**
 * Construct a default RTGMessageType object.
 *
 * Construct a default RTGMessageType object with all the basic
 * initialization.
 */
RTGMessageType::RTGMessageType(void) : DesktopMessageType((void *)"RTG")
{

}

/**
 * Destruct the RTGMessageType object.
 *
 * Destruct the RTGMessageType object by deallocating any dynamically
 * allocated momory.
 */
RTGMessageType::~RTGMessageType(void) {

}
