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
 * @file RDWMessageType.h
 * @brief A specifications file of a message of type RDW.
 * @author Andrew De Ponte
 *
 * A specifications for a class to represent a message that is of type RDW.
 */

#ifndef RDWMESSAGETYPE_H
#define RDWMESSAGETYPE_H

#include "DesktopMessageType.h"

#include <time.h>

#include <string>

/**
 * @class RDWMessageType
 * @brief A type representing messages of type RDW.
 *
 * The RDWMessageType is a class which represents messages of type RDW. It
 * handles all access to data contained in such messages. In order to properly
 * use this class one must not only construct it but also initialize it as one
 * of the three variations of the message type. This can be done by using any
 * of the member functions whos names begin with InitAs. The message has been
 * constructed and initialized via the above described methods it needs to be
 * build by calling any of the member functions whos names start with Appenend
 * followed by a data type. Finally, after the message has been constructed,
 * initialiized and built it must be commited use the CommitContent member
 * function. Once all that has been done the message is ready te be sent or
 * used with any of the member functions which it has inherited.
 */
class RDWMessageType : public DesktopMessageType {
 public:
    RDWMessageType(void);
    ~RDWMessageType(void);

    int InitAsMod(unsigned char type, unsigned long int itemID);
    int InitAsObt(unsigned char type);
    int InitAsNew(unsigned char type);

    int AppendBit(unsigned char data);
    int AppendTime(time_t data);
    int AppendULong(unsigned long int data);
    int AppendBarray(std::string data);
    int AppendUTF8(std::string data);
    int AppendUChar(unsigned char data);
    int AppendWord(unsigned short int data);

    int CommitContent(void);

 private:
    int AppendData(const unsigned char *data, unsigned long int len);
    int ConvCalTime(time_t calTime, unsigned char *dest,
		    unsigned short int len);


    unsigned char *pBuff;
    unsigned short int buffSize;
    unsigned short int itemLenSize;
};

#endif
