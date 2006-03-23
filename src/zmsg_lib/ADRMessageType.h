/*
 * Copyright 2004, 2005 Andrew De Ponte
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
 * @file ADRMessageType.h
 * @brief A specifications file of a message of type ADR.
 * @author Andrew De Ponte
 *
 * A specifications for a class to represent a message that is of type ADR.
 */

#ifndef ADRMESSAGETYPE_H
#define ADRMESSAGETYPE_H

#include "ZaurusMessageType.h"

#include <string>
#include <time.h>

/**
 * @class ADRMessageType
 * @brief A type representing messages of type ADR.
 *
 * The ADRMessageType is a class which represents messages of type ADR. It
 * handles all access to data contained in such messages. When using this
 * class one should first call the LoadContent function to set the class up
 * and allow for use of the selection of Get functions. Each get function
 * works like a pop function of a que.
 */
class ADRMessageType : public ZaurusMessageType {
 public:
    ADRMessageType(void);
    ~ADRMessageType(void);

    void LoadContent(void);

    unsigned short int GetNumItems(void);
    unsigned short int GetNumParams(void);

    unsigned char GetBit(void);
    time_t GetTime(void);
    unsigned long int GetULong(void);
    std::string GetBarray(void);
    std::string GetUTF8(void);
    unsigned char GetUChar(void);
    unsigned short GetWord(void);

    // Commn Parameter Access Functions - these should no longer be needed to
    // the addition of the GetBit, GetTime, etc functions above however are
    // staying here until such calls used in zync can be replaced. These
    // should never be used in combination with any of the above listed
    // functions.
    unsigned long int GetAttrSize(void);
    unsigned char GetAttribute(void);
    unsigned long int GetCardCreatedTimeSize(void);
    int GetCardCreatedTime(char *dest, unsigned short int size);
    unsigned long int GetCardModifiedTimeSize(void);
    int GetCardModifiedTime(char *dest, unsigned short int size);
    unsigned long int GetSyncIDSize(void);
    unsigned long int GetSyncID(void);

 private:
    int GetData(void *dest, unsigned long int len);
    time_t ConvZDateTime(unsigned char *data, unsigned short int len);
    unsigned char *pCurContPos;
};

#endif
