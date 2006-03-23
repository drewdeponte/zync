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
 * @file AEXMessageType.cc
 * @brief An implimentation file for the AEXMessageType class.
 *
 * This type was developed to provide an object to represent a message
 * of type AEX.
 */

#include "AEXMessageType.h"

/**
 * Construct a default AEXMessageType object.
 *
 * Construct a default AEXMessageType object with all the basic
 * initialization.
 */
AEXMessageType::AEXMessageType(void) : ZaurusMessageType((void *)"AEX")
{

}

/**
 * Destruct the AEXMessageType object.
 *
 * Destruct the AEXMessageType object by deallocating any dynamically
 * allocated momory.
 */
AEXMessageType::~AEXMessageType(void) {

}
