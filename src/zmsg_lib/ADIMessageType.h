/*
 * Copyright 2004-2005 Andrew De Ponte
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
 * @file ADIMessageType.h
 * @brief A specifications file of a message of type ADI.
 * @author Andrew De Ponte
 *
 * A specifications for a class to represent a message that is of type ADI.
 */

#ifndef ADIMESSAGETYPE_H
#define ADIMESSAGETYPE_H

#include "ZaurusMessageType.h"

/**
 * @class ADIMessageType
 * @brief A type representing messages of type ADI.
 *
 * The ADIMessageType is a class which represents messages of type ADI. It
 * handles all access to data contained in such messages.
 */
class ADIMessageType : public ZaurusMessageType {
 public:
    ADIMessageType(void);
    ~ADIMessageType(void);

    unsigned long int GetNumCards(void);
    unsigned short int GetNumParams(void);
    int GetParamAbrev(char *dest, int destLen, unsigned short int paramOffset);
    unsigned char GetParamTypeID(unsigned short int paramOffset);
    int GetParamDesc(char *dest, int destLen, unsigned short int paramOffset);
};

#endif
