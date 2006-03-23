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
 * @file ItemType.hh
 * @brief A specifications file for an object representing a common item.
 * @author Andrew De Ponte
 *
 * A specifications file for a class existing to represent a common item. This
 * data type was created to provide a common data type between the plugins and
 * the ZaurusSyncer application. It should be used only as a base class for
 * other more specific types of items.
 */

#ifndef ITEMTYPE_H
#define ITEMTYPE_H

#include <time.h>

#include <list>
#include <string>
#include <iostream>

typedef std::list<unsigned long int> SyncIDListType;

/**
 * @class ItemType
 * @brief A type representing a generic item.
 *
 * The ItemType is a class which represents a generic item. It is designed to
 * be used between the ZaurusSync application and its plugins so that there is
 * a common object for generic items. It should be used as a base class for
 * other types of items (Todo, Calendar, Address, etc).
 */
class ItemType {
 public:
    ItemType(void);
    ~ItemType(void);

    void SetAttribute(unsigned char value);
    unsigned char GetAttribute(void) const;

    void SetCreatedTime(time_t epochSecs);
    time_t GetCreatedTime(void) const;

    void SetModifiedTime(time_t epochSecs);
    time_t GetModifiedTime(void) const;

    void SetSyncID(unsigned long int value);
    unsigned long int GetSyncID(void) const;

    void SetAppID(std::string value);
    std::string GetAppID(void) const;

 private:
    unsigned char attribute;
    time_t createdTime;
    time_t modifiedTime;
    unsigned long int syncId;
    std::string appId;
};

#endif
