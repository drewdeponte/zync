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
 * @file AGEMessageType.h
 * @brief A specifications file of a message of type AGE.
 * @author Andrew De Ponte
 *
 * A specifications for a class to represent a message that is of type AGE.
 */

#ifndef AGEMESSAGETYPE_H
#define AGEMESSAGETYPE_H

#include "ZaurusMessageType.h"

/**
 * @class AGEMessageType
 * @brief A type representing messages of type AGE.
 *
 * The AGEMessageType is a class which represents messages of type AGE. It
 * handles all access to data contained in such messages.
 */
class AGEMessageType : public ZaurusMessageType {
 public:
    AGEMessageType(void);
    ~AGEMessageType(void);

    unsigned long int GetTotDataLength(void);
    unsigned short int GetStartDataLength(void);
    unsigned short int GetDataLength(void);
    void *GetStartData(void);
    void *GetData(void);
};

#endif
