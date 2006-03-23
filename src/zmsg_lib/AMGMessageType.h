/*
 * Copyright 2004,2005 Andrew De Ponte
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
 * @file AMGMessageType.h
 * @brief A specifications file of a message of type AMG.
 * @author Andrew De Ponte
 *
 * A specifications for a class to represent a message that is of type AMG.
 */

#ifndef AMGMESSAGETYPE_H
#define AMGMESSAGETYPE_H

#include "ZaurusMessageType.h"

#define TODO_ID 0x01
#define CALENDAR_ID 0x02
#define ADDRBOOK_ID 0x04

/**
 * @class AMGMessageType
 * @brief A type representing messages of type AMG.
 *
 * The AMGMessageType is a class which represents messages of type AMG. It
 * handles all access to data contained in such messages.
 */
class AMGMessageType : public ZaurusMessageType {
 public:
    AMGMessageType(void);
    ~AMGMessageType(void);

    int IsEmptyLog(void);
    int IsEmptyTodo(void);
    int IsEmptyCalendar(void);
    int IsEmptyAddressBook(void);
};

#endif
